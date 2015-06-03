/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Brewpi.h"
#include "TemperatureFormats.h"
#include "Pins.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensor.h"
#include "Ticks.h"
#include "TempSensorMock.h"
#include "EepromManager.h"
#include "TempSensorDisconnected.h"
#include "ModeControl.h"
#include "fixstl.h"

TempControl tempControl;

#if TEMP_CONTROL_STATIC

extern ValueSensor<bool> defaultSensor;
extern ValueActuator defaultActuator;
extern DisconnectedTempSensor defaultTempSensor;

// These sensors are switched out to implement multi-chamber.
TempSensor* TempControl::beerSensor;
TempSensor* TempControl::fridgeSensor;
BasicTempSensor* TempControl::ambientSensor = &defaultTempSensor;


Actuator* TempControl::heater = &defaultActuator;
Actuator* TempControl::cooler = &defaultActuator;
Actuator* TempControl::light = &defaultActuator;
Actuator* TempControl::fan = &defaultActuator;

ValueActuator cameraLightState;		
AutoOffActuator TempControl::cameraLight(600, &cameraLightState);	// timeout 10 min
Sensor<bool>* TempControl::door = &defaultSensor;
	
// Control parameters
ControlConstants TempControl::cc;
ControlSettings TempControl::cs;
ControlVariables TempControl::cv;
	
	// State variables
states TempControl::state;
bool TempControl::doPosPeakDetect;
bool TempControl::doNegPeakDetect;
bool TempControl::doorOpen;
	
	// keep track of beer setting stored in EEPROM
temperature TempControl::storedBeerSetting;
	
	// Timers
tcduration_t TempControl::lastIdleTime;
tcduration_t TempControl::lastHeatTime;
tcduration_t TempControl::lastCoolTime;
tcduration_t TempControl::waitTime;
#endif

void TempControl::init(void){
	state=IDLE;		
	cs.mode = MODE_OFF;
	
	cameraLight.setActive(false);
	
	// this is for cases where the device manager hasn't configured beer/fridge sensor.	
	if (beerSensor==NULL) {
		beerSensor = new TempSensor(TEMP_SENSOR_TYPE_BEER, &defaultTempSensor);
		beerSensor->init();
	}
		
	if (fridgeSensor==NULL) {
		fridgeSensor = new TempSensor(TEMP_SENSOR_TYPE_FRIDGE, &defaultTempSensor);
		fridgeSensor->init();
	}
	
	updateTemperatures();
	reset();
	
	// Do not allow heating/cooling directly after reset.
	// A failing script + CRON + Arduino uno (which resets on serial connect) could damage the compressor
	// For test purposes, set these to -3600 to eliminate waiting after reset
	lastHeatTime = 0;
	lastCoolTime = 0;
}

void TempControl::reset(void){
	doPosPeakDetect=false;
	doNegPeakDetect=false;
}

void updateSensor(TempSensor* sensor) {
	sensor->update();
	if(!sensor->isConnected()) {
		sensor->init();
	}		
}

void TempControl::updateTemperatures(void){
	
	updateSensor(beerSensor);
	updateSensor(fridgeSensor);
	
	// Read ambient sensor to keep the value up to date. If no sensor is connected, this does nothing.
	// This prevents a delay in serial response because the value is not up to date.
	if(ambientSensor->read() == TEMP_SENSOR_DISCONNECTED){
		ambientSensor->init(); // try to reconnect a disconnected, but installed sensor
	}
}

void TempControl::updatePID(void){
	static unsigned char integralUpdateCounter = 0;
	if(tempControl.modeIsBeer()){
		if(isDisabledOrInvalid(cs.beerSetting)){
			// beer setting is not updated yet
			// set fridge to unknown too
			cs.fridgeSetting = DISABLED_TEMP;
			return;
		}
		
		// fridge setting is calculated with PID algorithm. Beer temperature error is input to PID
		cv.beerDiff =  cs.beerSetting - beerSensor->readSlowFiltered();
		cv.beerSlope = beerSensor->readSlope();
		temperature fridgeFastFiltered = fridgeSensor->readFastFiltered();
			
		if(integralUpdateCounter++ == 60){
			integralUpdateCounter = 0;
			
			temperature integratorUpdate = cv.beerDiff;
			
			// Only update integrator in IDLE, because thats when the fridge temp has reached the fridge setting.
			// If the beer temp is still not correct, the fridge setting is too low/high and integrator action is needed.
			if(state != IDLE){
				integratorUpdate = 0;
			}
			else if(abs(integratorUpdate) < cc.iMaxError){
				// difference is smaller than iMaxError				
				// check additional conditions to see if integrator should be active to prevent windup
				bool updateSign = (integratorUpdate > 0); // 1 = positive, 0 = negative
				bool integratorSign = (cv.diffIntegral > 0);		
				
				if(updateSign == integratorSign){
					// beerDiff and integrator have same sign. Integrator would be increased.
					
					// If actuator is already at max increasing actuator will only cause integrator windup.
					integratorUpdate = (cs.fridgeSetting >= cc.tempSettingMax) ? 0 : integratorUpdate;
					integratorUpdate = (cs.fridgeSetting <= cc.tempSettingMin) ? 0 : integratorUpdate;
					integratorUpdate = ((cs.fridgeSetting - cs.beerSetting) >= cc.pidMax) ? 0 : integratorUpdate;
					integratorUpdate = ((cs.beerSetting - cs.fridgeSetting) >= cc.pidMax) ? 0 : integratorUpdate;
										
					// cooling and fridge temp is more than 2 degrees from setting, actuator is saturated.
					integratorUpdate = (!updateSign && (fridgeFastFiltered > (cs.fridgeSetting +1024))) ? 0 : integratorUpdate;
					
					// heating and fridge temp is more than 2 degrees from setting, actuator is saturated.
					integratorUpdate = (updateSign && (fridgeFastFiltered < (cs.fridgeSetting -1024))) ? 0 : integratorUpdate;
				}
				else{
					// integrator action is decreased. Decrease faster than increase.
					integratorUpdate = integratorUpdate*2;
				}	
			}
			else{
				// decrease integral by 1/8 when far from the end value to reset the integrator
				integratorUpdate = -(cv.diffIntegral >> 3);		
			}
			cv.diffIntegral = cv.diffIntegral + integratorUpdate;
		}			
		
		// calculate PID parts. Use long_temperature to prevent overflow
		cv.p = multiplyFactorTemperatureDiff(cc.Kp, cv.beerDiff);
		cv.i = multiplyFactorTemperatureDiffLong(cc.Ki, cv.diffIntegral);
		cv.d = multiplyFactorTemperatureDiff(cc.Kd, cv.beerSlope);
		long_temperature newFridgeSetting = cs.beerSetting;
		newFridgeSetting += cv.p;
		newFridgeSetting += cv.i;
		newFridgeSetting += cv.d;
		
		// constrain to tempSettingMin or beerSetting - pidMAx, whichever is lower.
		temperature lowerBound = (cs.beerSetting <= cc.tempSettingMin + cc.pidMax) ? cc.tempSettingMin : cs.beerSetting - cc.pidMax;
		// constrain to tempSettingMax or beerSetting + pidMAx, whichever is higher.
		temperature upperBound = (cs.beerSetting >= cc.tempSettingMax - cc.pidMax) ? cc.tempSettingMax : cs.beerSetting + cc.pidMax;
		
		cs.fridgeSetting = constrain(constrainTemp16(newFridgeSetting), lowerBound, upperBound);
	}
	else if(cs.mode == MODE_FRIDGE_CONSTANT){
		// FridgeTemperature is set manually, disable beer setpoint
		cs.beerSetting = DISABLED_TEMP;
	}
}

void TempControl::updateState(void){
	//update state
	bool stayIdle = false;
	bool newDoorOpen = door->sense();
		
	if(newDoorOpen!=doorOpen) {
		doorOpen = newDoorOpen;
		piLink.printFridgeAnnotation(PSTR("Fridge door %S"), doorOpen ? PSTR("opened") : PSTR("closed"));
	}

	if(cs.mode == MODE_OFF){
		state = STATE_OFF;
		stayIdle = true;
	}
	// stay idle when one of the required sensors is disconnected, or the fridge setting is INVALID_TEMP
	if(isDisabledOrInvalid(cs.fridgeSetting) || 
		!fridgeSensor->isConnected() || 
		(!beerSensor->isConnected() && tempControl.modeIsBeer())){
		state = IDLE;
		stayIdle = true;
	}
	
	tcduration_t sinceIdle = timeSinceIdle();
	tcduration_t sinceCooling = timeSinceCooling();
	tcduration_t sinceHeating = timeSinceHeating();
	temperature fridgeFast = fridgeSensor->readFastFiltered();
	temperature beerFast = beerSensor->readFastFiltered();
	ticks_seconds_t secs = ticks.seconds();
	switch(state)
	{
		case IDLE:
		case STATE_OFF:
		case WAITING_TO_COOL:
		case WAITING_TO_HEAT:
		case WAITING_FOR_PEAK_DETECT:
		{
			lastIdleTime=secs;		
			// set waitTime to zero. It will be set to the maximum required waitTime below when wait is in effect.
			if(stayIdle){
				break;
			}
			resetWaitTime();
			if(fridgeFast > (cs.fridgeSetting+cc.idleRangeHigh) ){  // fridge temperature is too high			
				tempControl.updateWaitTime(MIN_SWITCH_TIME, sinceHeating);			
				if(cs.mode==MODE_FRIDGE_CONSTANT){
					tempControl.updateWaitTime(MIN_COOL_OFF_TIME_FRIDGE_CONSTANT, sinceCooling);
				}
				else{
					if(beerFast < (cs.beerSetting + 16) ){ // If beer is already under target, stay/go to idle. 1/2 sensor bit idle zone
						state = IDLE; // beer is already colder than setting, stay in or go to idle
						break;
					}
					tempControl.updateWaitTime(MIN_COOL_OFF_TIME, sinceCooling);
				}
				if(tempControl.cooler != &defaultActuator){
					if(getWaitTime() > 0){
						state = WAITING_TO_COOL;
					}
					else{
						state = COOLING;	
					}
				}
			}
			else if(fridgeFast < (cs.fridgeSetting+cc.idleRangeLow)){  // fridge temperature is too low
				tempControl.updateWaitTime(MIN_SWITCH_TIME, sinceCooling);
				tempControl.updateWaitTime(MIN_HEAT_OFF_TIME, sinceHeating);
				if(cs.mode!=MODE_FRIDGE_CONSTANT){
					if(beerFast > (cs.beerSetting - 16)){ // If beer is already over target, stay/go to idle. 1/2 sensor bit idle zone
						state = IDLE;  // beer is already warmer than setting, stay in or go to idle
						break;
					}
				}
				if(tempControl.heater != &defaultActuator || (cc.lightAsHeater && (tempControl.light != &defaultActuator))){
					if(getWaitTime() > 0){
						state = WAITING_TO_HEAT;
					}
					else{
						state = HEATING;
					}
				}
			}
			else{
				state = IDLE; // within IDLE range, always go to IDLE
				break;
			}
			if(state == HEATING || state == COOLING){	
					// If peak detect is not finished, but the fridge wants to switch to heat/cool
				// Wait for peak detection and show on display
				if(doNegPeakDetect == true){
					tempControl.updateWaitTime(COOL_PEAK_DETECT_TIME, sinceCooling);
                                }
				else if(doPosPeakDetect == true){
					tempControl.updateWaitTime(HEAT_PEAK_DETECT_TIME, sinceHeating);
				}
				else{
					break; // peak detect has finished
				}
					state = WAITING_FOR_PEAK_DETECT;
				}
			}
		break; 
		case COOLING:
		case COOLING_MIN_TIME:
		{
			doNegPeakDetect=true;
			lastCoolTime = secs;
			updateEstimatedPeak(cc.maxCoolTimeForEstimate, cs.coolEstimator, sinceIdle);
			state = COOLING; // set to cooling here, so the display of COOLING/COOLING_MIN_TIME is correct
			
			// stop cooling when estimated fridge temp peak lands on target or if beer is already too cold (1/2 sensor bit idle zone)
			if(cv.estimatedPeak <= cs.fridgeSetting || (cs.mode != MODE_FRIDGE_CONSTANT && beerFast < (cs.beerSetting - 16))){
				if(sinceIdle > MIN_COOL_ON_TIME){
					cv.negPeakEstimate = cv.estimatedPeak; // remember estimated peak when I switch to IDLE, to adjust estimator later
					state=IDLE;
					break;
				}
				else{
					state = COOLING_MIN_TIME;
					break;
				}				
			}
		}
		break;
		case HEATING:
		case HEATING_MIN_TIME:
		{
			doPosPeakDetect=true;
			lastHeatTime=secs;
			updateEstimatedPeak(cc.maxHeatTimeForEstimate, cs.heatEstimator, sinceIdle);
			state = HEATING; // reset to heating here, so the display of HEATING/HEATING_MIN_TIME is correct
			
			// stop heating when estimated fridge temp peak lands on target or if beer is already too warm (1/2 sensor bit idle zone)
			if(cv.estimatedPeak >= cs.fridgeSetting || (cs.mode != MODE_FRIDGE_CONSTANT && beerFast > (cs.beerSetting + 16))){
				if(sinceIdle > MIN_HEAT_ON_TIME){
					cv.posPeakEstimate=cv.estimatedPeak; // remember estimated peak when I switch to IDLE, to adjust estimator later
					state=IDLE;
					break;
				}
				else{
					state = HEATING_MIN_TIME;
					break;
				}
			}
		}
		break;
		case DOOR_OPEN:
		break; // do nothing
	}			
}

void TempControl::updateEstimatedPeak(uint16_t timeLimit, temperature estimator, uint16_t sinceIdle)
{
	uint16_t activeTime = std::min(timeLimit, sinceIdle); // heat or cool time in seconds
	temperature estimatedOvershoot = ((long_temperature) estimator * activeTime)/3600; // overshoot estimator is in overshoot per hour
	if(stateIsCooling()){
		estimatedOvershoot = -estimatedOvershoot; // when cooling subtract overshoot from fridge temperature
	}
	cv.estimatedPeak = fridgeSensor->readFastFiltered() + estimatedOvershoot;		
}

void TempControl::updateOutputs(void) {
	if (cs.mode==MODE_TEST)
		return;
		
	cameraLight.update();
	bool heating = stateIsHeating();
	bool cooling = stateIsCooling();
	cooler->setActive(cooling);		
	heater->setActive(heating);	
	light->setActive(isDoorOpen() || (cc.lightAsHeater && heating) || cameraLightState.isActive());	
	fan->setActive(heating || cooling);
}


void TempControl::detectPeaks(void){  
	//detect peaks in fridge temperature to tune overshoot estimators
	LOG_ID_TYPE detected = 0;
	temperature peak, estimate, error, oldEstimator, newEstimator;
	
	if(doPosPeakDetect && !stateIsHeating()){
		peak = fridgeSensor->detectPosPeak();
		estimate = cv.posPeakEstimate;
		error = peak-estimate;
		oldEstimator = cs.heatEstimator;
		if(peak != INVALID_TEMP){
			// positive peak detected
			if(error > cc.heatingTargetUpper){
				// Peak temperature was higher than the estimate.
				// Overshoot was higher than expected
				// Increase estimator to increase the estimated overshoot
				increaseEstimator(&(cs.heatEstimator), error);
			}
			if(error < cc.heatingTargetLower){
				// Peak temperature was lower than the estimate.
				// Overshoot was lower than expected
				// Decrease estimator to decrease the estimated overshoot
				decreaseEstimator(&(cs.heatEstimator), error);
			}
			detected = INFO_POSITIVE_PEAK;
		}
		else if(timeSinceHeating() > HEAT_PEAK_DETECT_TIME){
			if(fridgeSensor->readFastFiltered() < (cv.posPeakEstimate+cc.heatingTargetLower)){
				// Idle period almost reaches maximum allowed time for peak detection
				// This is the heat, then drift up too slow (but in the right direction).
				// estimator is too high
				peak=fridgeSensor->readFastFiltered();
				decreaseEstimator(&(cs.heatEstimator), error);			
				detected = INFO_POSITIVE_DRIFT;
			}
			else{
				// maximum time for peak estimation reached
				doPosPeakDetect = false;	
			}
		}
		if(detected){
			newEstimator = cs.heatEstimator;	
			cv.posPeak = peak;
			doPosPeakDetect = false;
		}
	}			
	else if(doNegPeakDetect && !stateIsCooling()){
		peak = fridgeSensor->detectNegPeak();
		estimate = cv.negPeakEstimate;
		error = peak-estimate;
		oldEstimator = cs.coolEstimator;
		if(peak != INVALID_TEMP){
			// negative peak detected
			if(error < cc.coolingTargetLower){
				// Peak temperature was lower than the estimate.
				// Overshoot was higher than expected
				// Increase estimator to increase the estimated overshoot
				increaseEstimator(&(cs.coolEstimator), error);
			}
			if(error > cc.coolingTargetUpper){
				// Peak temperature was higher than the estimate.
				// Overshoot was lower than expected
				// Decrease estimator to decrease the estimated overshoot
				decreaseEstimator(&(cs.coolEstimator), error);

			}
			detected = INFO_NEGATIVE_PEAK;
		}
		else if(timeSinceCooling() > COOL_PEAK_DETECT_TIME){
			if(fridgeSensor->readFastFiltered() > (cv.negPeakEstimate+cc.coolingTargetUpper)){
				// Idle period almost reaches maximum allowed time for peak detection
				// This is the cooling, then drift down too slow (but in the right direction).
				// estimator is too high
				peak = fridgeSensor->readFastFiltered();
				decreaseEstimator(&(cs.coolEstimator), error);
				detected = INFO_NEGATIVE_DRIFT;
			}
			else{
				// maximum time for peak estimation reached
				doNegPeakDetect=false;
			}
		}
		if(detected){
			newEstimator = cs.coolEstimator;
			cv.negPeak = peak;
			doNegPeakDetect=false;
		}
	}
	if(detected){
		// send out log message for type of peak detected
		logInfoTempTempFixedFixed(detected, peak, estimate, oldEstimator, newEstimator);
	}
}

// Increase estimator at least 20%, max 50%s
void TempControl::increaseEstimator(temperature * estimator, temperature error){
	temperature factor = 614 + constrainTemp(abs(error)>>5, 0, 154); // 1.2 + 3.1% of error, limit between 1.2 and 1.5
	*estimator = multiplyFactorTemperatureDiff(factor, *estimator);
	if(*estimator < 25){
		*estimator = intToTempDiff(5)/100; // make estimator at least 0.05
	}
	eepromManager.storeTempSettings();
}

// Decrease estimator at least 16.7% (1/1.2), max 33.3% (1/1.5)
void TempControl::decreaseEstimator(temperature * estimator, temperature error){
	temperature factor = 426 - constrainTemp(abs(error)>>5, 0, 85); // 0.833 - 3.1% of error, limit between 0.667 and 0.833
	*estimator = multiplyFactorTemperatureDiff(factor, *estimator);
	eepromManager.storeTempSettings();
}

tcduration_t TempControl::timeSinceCooling(void){
	return ticks.timeSince(lastCoolTime);
}

tcduration_t TempControl::timeSinceHeating(void){
	return ticks.timeSince(lastHeatTime);
}

tcduration_t TempControl::timeSinceIdle(void){
	return ticks.timeSince(lastIdleTime);
}

void TempControl::loadDefaultSettings(){
#if BREWPI_EMULATE
	setMode(MODE_BEER_CONSTANT);
#else	
	setMode(MODE_OFF);
#endif	
	cs.beerSetting = DISABLED_TEMP; // start with no temp settings
	cs.fridgeSetting = DISABLED_TEMP;
	cs.heatEstimator = intToTempDiff(2)/10; // 0.2
	cs.coolEstimator=intToTempDiff(5);
}

void TempControl::storeConstants(eptr_t offset){	
	eepromAccess.writeBlock(offset, (void *) &cc, sizeof(ControlConstants));
}

void TempControl::loadConstants(eptr_t offset){
	eepromAccess.readBlock((void *) &cc, offset, sizeof(ControlConstants));
	initFilters();	
}

// write new settings to EEPROM to be able to reload them after a reset
// The update functions only write to EEPROM if the value has changed
void TempControl::storeSettings(eptr_t offset){
	eepromAccess.writeBlock(offset, (void *) &cs, sizeof(ControlSettings));
	storedBeerSetting = cs.beerSetting;		
}

void TempControl::loadSettings(eptr_t offset){
	eepromAccess.readBlock((void *) &cs, offset, sizeof(ControlSettings));	
	logDebug("loaded settings");
	storedBeerSetting = cs.beerSetting;
	setMode(cs.mode, true);		// force the mode update
}

void TempControl::loadDefaultConstants(void){
	memcpy_P((void*) &tempControl.cc, (void*) &tempControl.ccDefaults, sizeof(ControlConstants));
	initFilters();
}

void TempControl::initFilters()
{
	fridgeSensor->setFastFilterCoefficients(cc.fridgeFastFilter);
	fridgeSensor->setSlowFilterCoefficients(cc.fridgeSlowFilter);
	fridgeSensor->setSlopeFilterCoefficients(cc.fridgeSlopeFilter);
	beerSensor->setFastFilterCoefficients(cc.beerFastFilter);
	beerSensor->setSlowFilterCoefficients(cc.beerSlowFilter);
	beerSensor->setSlopeFilterCoefficients(cc.beerSlopeFilter);		
}

void TempControl::setMode(char newMode, bool force){
	logDebug("TempControl::setMode from %c to %c", cs.mode, newMode);
	
	if(newMode != cs.mode || state == WAITING_TO_HEAT || state == WAITING_TO_COOL || state == WAITING_FOR_PEAK_DETECT){
		state = IDLE;
		force = true;
	}
	if (force) {
		cs.mode = newMode;
		if(newMode == MODE_OFF){
			cs.beerSetting = DISABLED_TEMP;
			cs.fridgeSetting = DISABLED_TEMP;
		}
		eepromManager.storeTempSettings();
	}
}

temperature TempControl::getBeerTemp(void){
	if(beerSensor->isConnected()){
		return beerSensor->readFastFiltered();	
	}
	else{
		return INVALID_TEMP;
	}
}

temperature TempControl::getBeerSetting(void){
	return cs.beerSetting;	
}

temperature TempControl::getFridgeTemp(void){
	if(fridgeSensor->isConnected()){
		return fridgeSensor->readFastFiltered();		
	}
	else{
		return INVALID_TEMP;
	}
}

temperature TempControl::getFridgeSetting(void){
	return cs.fridgeSetting;	
}

void TempControl::setBeerTemp(temperature newTemp){
	temperature oldBeerSetting = cs.beerSetting;
	cs.beerSetting= newTemp;
	if(abs(oldBeerSetting - newTemp) > intToTempDiff(1)/2){ // more than half degree C difference with old setting
		reset(); // reset controller
	}
	updatePID();
	updateState();
	if(cs.mode != MODE_BEER_PROFILE || abs(storedBeerSetting - newTemp) > intToTempDiff(1)/4){
		// more than 1/4 degree C difference with EEPROM
		// Do not store settings every time in profile mode, because EEPROM has limited number of write cycles.
		// A temperature ramp would cause a lot of writes
		// If Raspberry Pi is connected, it will update the settings anyway. This is just a safety feature.
		eepromManager.storeTempSettings();
	}		
}

void TempControl::setFridgeTemp(temperature newTemp){
	cs.fridgeSetting = newTemp;
	reset(); // reset peak detection and PID
	updatePID();
	updateState();	
	eepromManager.storeTempSettings();
}

bool TempControl::stateIsCooling(void){
	return (state==COOLING || state==COOLING_MIN_TIME);
}
bool TempControl::stateIsHeating(void){
	return (state==HEATING || state==HEATING_MIN_TIME);
}

const ControlConstants TempControl::ccDefaults PROGMEM =
{
	// Do Not change the order of these initializations!
	/* tempFormat */ 'C',
	/* tempSettingMin */ intToTemp(1),	// +1 deg Celsius
	/* tempSettingMax */ intToTemp(30),	// +30 deg Celsius
	
	// control defines, also in fixed point format (7 int bits, 9 frac bits), so multiplied by 2^9=512
	/* Kp	*/ doubleToTempDiff(5.0),	// +5
	/* Ki	*/ doubleToTempDiff(0.25), // +0.25
	/* Kd	*/ doubleToTempDiff(1.5),	// -1.5
	/* iMaxError */ doubleToTempDiff(0.5),  // 0.5 deg

	// Stay Idle when fridge temperature is in this range
	/* idleRangeHigh */ intToTempDiff(1),	// +1 deg Celsius
	/* idleRangeLow */ intToTempDiff(-1),	// -1 deg Celsius

	// when peak falls between these limits, its good.
	/* heatingTargetUpper */ doubleToTempDiff(0.3),	// +0.3 deg Celsius
	/* heatingTargetLower */ doubleToTempDiff(-0.2),	// -0.2 deg Celsius
	/* coolingTargetUpper */ doubleToTempDiff(0.2),	// +0.2 deg Celsius
	/* coolingTargetLower */ doubleToTempDiff(-0.3),	// -0.3 deg Celsius

	// maximum history to take into account, in seconds
	/* maxHeatTimeForEstimate */ 600,
	/* maxCoolTimeForEstimate */ 1200,

	// Set filter coefficients. This is the b value. See FilterFixed.h for delay times.
	// The delay time is 3.33 * 2^b * number of cascades
	/* fridgeFastFilter */ 1u,
	/* fridgeSlowFilter */ 4u,
	/* fridgeSlopeFilter */ 3u,
	/* beerFastFilter */ 3u,
	/* beerSlowFilter */ 4u,
	/* beerSlopeFilter */ 4u,
	
	/* lightAsHeater */ 0,
	/* rotaryHalfSteps */ 0,

	/* pidMax */ intToTempDiff(10),	// +/- 10 deg Celsius
};

control_mode_t ModeControl_GetMode()
{
    return tempControl.getMode();
}

control_mode_t ModeControl_SetMode(control_mode_t mode)
{
    control_mode_t prev = tempControl.getMode();
    tempControl.setMode(mode, true);
    return prev;
}

