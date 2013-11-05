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

#include "Pins.h"
#include <limits.h>

#include "TemperatureFormats.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensor.h"
#include "Ticks.h"
#include "TempSensorMock.h"
#include "EepromManager.h"
#include "TempSensorDisconnected.h"
#include "RotaryEncoder.h"

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
uint8_t TempControl::state;
bool TempControl::doPosPeakDetect;
bool TempControl::doNegPeakDetect;
bool TempControl::doorOpen;
	
	// keep track of beer setting stored in EEPROM
fixed7_9 TempControl::storedBeerSetting;
	
	// Timers
uint16_t TempControl::lastIdleTime;
uint16_t TempControl::lastHeatTime;
uint16_t TempControl::lastCoolTime;
uint16_t TempControl::waitTime;
#endif

void TempControl::init(void){
	state=IDLE;		
	cs.mode = MODE_OFF;
	
	cameraLight.setActive(false);
		
	if (tempControl.beerSensor==NULL)
		tempControl.beerSensor = new TempSensor(TEMP_SENSOR_TYPE_BEER, &defaultTempSensor);
	if (tempControl.fridgeSensor==NULL)
		tempControl.fridgeSensor = new TempSensor(TEMP_SENSOR_TYPE_FRIDGE, &defaultTempSensor);
		
	beerSensor->init();
	fridgeSensor->init();
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

void TempControl::updateTemperatures(void){
	beerSensor->update();
	if(!beerSensor->isConnected() && tempControl.modeIsBeer()){
		beerSensor->init(); // try to restart the sensor when controlling beer temperature
	}
	fridgeSensor->update();
	if(!fridgeSensor->isConnected()){
		fridgeSensor->init(); // always try to restart the fridge sensor
	}
	
	// Read ambient sensor to keep the value up to date. If no sensor is connected, this does nothing.
	// This prevents a delay in serial response because the value is not up to date.
	if(ambientSensor->read() == TEMP_SENSOR_DISCONNECTED){
		ambientSensor->init(); // try to reconnect a disconnected, but installed sensor
	}
}


fixed7_9 multiplyFixeda7_9b23_9(fixed7_9 a, fixed23_9 b)
{
	return constrainTemp16(((fixed23_9) a * b)>>9);
}

fixed7_9 multiplyFixed7_9(fixed7_9 a, fixed7_9 b) 
{	
	return constrainTemp16(((fixed23_9) a * (fixed23_9) b)>>9);	
}


void TempControl::updatePID(void){
	static unsigned char integralUpdateCounter = 0;
	if(tempControl.modeIsBeer()){
		if(cs.beerSetting == INVALID_TEMP){
			// beer setting is not updated yet
			// set fridge to unknown too
			cs.fridgeSetting = INVALID_TEMP;
			return;
		}
		
		// fridge setting is calculated with PID algorithm. Beer temperature error is input to PID
		cv.beerDiff =  cs.beerSetting - beerSensor->readSlowFiltered();
		cv.beerSlope = beerSensor->readSlope();
		fixed7_9 fridgeFastFiltered = fridgeSensor->readFastFiltered();
			
		if(integralUpdateCounter++ == 60){
			integralUpdateCounter = 0;
			
			fixed7_9 integratorUpdate = cv.beerDiff;
			
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
		
		// calculate PID parts. Use fixed23_9 to prevent overflow
		cv.p = multiplyFixed7_9(cc.Kp, cv.beerDiff);
		cv.i = multiplyFixeda7_9b23_9(cc.Ki, cv.diffIntegral);
		cv.d = multiplyFixed7_9(cc.Kd, cv.beerSlope);
		fixed23_9 newFridgeSetting = cs.beerSetting;
		newFridgeSetting += cv.p;
		newFridgeSetting += cv.i;
		newFridgeSetting += cv.d;		

		// constrain so fridge setting is max pidMax from beer setting
		newFridgeSetting = constrain(constrainTemp16(newFridgeSetting), cs.beerSetting - cc.pidMax, cs.beerSetting + cc.pidMax);
		// constrain within absolute limits
		cs.fridgeSetting = constrain(constrainTemp16(newFridgeSetting), cc.tempSettingMin, cc.tempSettingMax);
	}
	else if(cs.mode == MODE_FRIDGE_CONSTANT){
		// FridgeTemperature is set manually, use INT_MIN to indicate beer temp is not active
		cs.beerSetting = INVALID_TEMP;
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
	// stay idle when one of the required sensors is disconnected, or the fridge setting is INT_MIN
	if( cs.fridgeSetting == INT_MIN || 
		!fridgeSensor->isConnected() || 
		(!beerSensor->isConnected() && tempControl.modeIsBeer())){
		state = IDLE;
		stayIdle = true;
	}
	
	uint16_t sinceIdle = timeSinceIdle();
	uint16_t sinceCooling = timeSinceCooling();
	uint16_t sinceHeating = timeSinceHeating();
	fixed7_9 fridgeFast = fridgeSensor->readFastFiltered();
	fixed7_9 beerFast = beerSensor->readFastFiltered();
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
				if(doNegPeakDetect == true || doPosPeakDetect == true){
					// If peak detect is not finished, but the fridge wants to switch to heat/cool
					// Wait for peak detection and display 'Await peak detect' on display
					state = WAITING_FOR_PEAK_DETECT;
					break;
				}
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
	}			
}

void TempControl::updateEstimatedPeak(uint16_t timeLimit, fixed7_9 estimator, uint16_t sinceIdle)
{
	uint16_t activeTime = min(timeLimit, sinceIdle); // heat or cool time in seconds
	fixed7_9 estimatedOvershoot = ((fixed23_9) estimator * activeTime)/3600; // overshoot estimator is in overshoot per hour
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
	heater->setActive(!cc.lightAsHeater && heating);	
	light->setActive(isDoorOpen() || (cc.lightAsHeater && heating) || cameraLightState.isActive());	
	fan->setActive(heating || cooling);
}


void TempControl::detectPeaks(void){  
	//detect peaks in fridge temperature to tune overshoot estimators
	LOG_ID_TYPE detected = 0;
	fixed7_9 peak, estimate, error, oldEstimator, newEstimator;
	
	if(doPosPeakDetect && !stateIsHeating()){
		peak = fridgeSensor->detectPosPeak();
		estimate = cv.posPeakEstimate;
		error = peak-estimate;
		oldEstimator = cs.heatEstimator;
		if(peak != INT_MIN){
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
		if(timeSinceHeating() > HEAT_PEAK_DETECT_TIME){
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
		if(peak != INT_MIN){
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
void TempControl::increaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed7_9 factor = 614 + constrainTemp(abs(error)>>5, 0, 154); // 1.2 + 3.1% of error, limit between 1.2 and 1.5
	*estimator = multiplyFixed7_9(factor, *estimator);
	eepromManager.storeTempSettings();
}

// Decrease estimator at least 16.7% (1/1.2), max 33.3% (1/1.5)
void TempControl::decreaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed7_9 factor = 426 - constrainTemp(abs(error)>>5, 0, 85); // 0.833 - 3.1% of error, limit between 0.667 and 0.833
	*estimator = multiplyFixed7_9(factor, *estimator);
	eepromManager.storeTempSettings();
}

uint16_t TempControl::timeSinceCooling(void){
	return ticks.timeSince(lastCoolTime);
}

uint16_t TempControl::timeSinceHeating(void){
	return ticks.timeSince(lastHeatTime);
}

uint16_t TempControl::timeSinceIdle(void){
	return ticks.timeSince(lastIdleTime);
}

void TempControl::loadDefaultSettings(){
#if BREWPI_EMULATE
	setMode(MODE_BEER_CONSTANT);
#else	
	setMode(MODE_OFF);
#endif	
	cs.beerSetting = 20 * 512 + C_OFFSET;
	cs.fridgeSetting = 20 * 512 + C_OFFSET;
	cs.heatEstimator = 102; // 0.2*2^9
	cs.coolEstimator = 5 * 512;
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
		if(newMode==MODE_BEER_PROFILE || newMode == MODE_OFF){
			// set temperatures to undefined until temperatures have been received from RPi
			cs.beerSetting = INVALID_TEMP;
			cs.fridgeSetting = INVALID_TEMP;
		}
		eepromManager.storeTempSettings();
	}
}

fixed7_9 TempControl::getBeerTemp(void){
	if(beerSensor->isConnected()){
		return beerSensor->readFastFiltered();	
	}
	else{
		return INVALID_TEMP;
	}
}

fixed7_9 TempControl::getBeerSetting(void){
	return cs.beerSetting;	
}

fixed7_9 TempControl::getFridgeTemp(void){
	if(fridgeSensor->isConnected()){
		return fridgeSensor->readFastFiltered();		
	}
	else{
		return INVALID_TEMP;
	}
}

fixed7_9 TempControl::getFridgeSetting(void){
	return cs.fridgeSetting;	
}

void TempControl::setBeerTemp(fixed7_9 newTemp){
	fixed7_9 oldBeerSetting = cs.beerSetting;
	cs.beerSetting= newTemp;
	if(abs(oldBeerSetting - newTemp) > 256){ // more than half degree C difference with old setting
		reset(); // reset controller
	}
	updatePID();
	updateState();
	if(cs.mode != MODE_BEER_PROFILE || abs(storedBeerSetting - newTemp) > 128){
		// more than 1/4 degree C difference with EEPROM
		// Do not store settings every time in profile mode, because EEPROM has limited number of write cycles.
		// A temperature ramp would cause a lot of writes
		// If Raspberry Pi is connected, it will update the settings anyway. This is just a safety feature.
		eepromManager.storeTempSettings();
	}		
}

void TempControl::setFridgeTemp(fixed7_9 newTemp){
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
	/* tempSettingMin */ 1*512 + C_OFFSET,	// +1 deg Celsius
	/* tempSettingMax */ 30*512 + C_OFFSET,	// +30 deg Celsius
	/* pidMax */ 10*512,	// +/- 10 deg Celsius
	
	// control defines, also in fixed point format (7 int bits, 9 frac bits), so multiplied by 2^9=512
	/* Kp	*/ 2560,	// +5
	/* Ki	*/ 128,		// +0.25
	/* Kd	*/ -768,	// -1.5
	/* iMaxError */ 256,  // 0.5 deg

	// Stay Idle when fridge temperature is in this range
	/* idleRangeHigh */ 512,	// +1 deg Celsius
	/* idleRangeLow */ -512,	// -1 deg Celsius

	// when peak falls between these limits, its good.
	/* heatingTargetUpper */ 154,	// +0.3 deg Celsius
	/* heatingTargetLower */ -102,	// -0.2 deg Celsius
	/* coolingTargetUpper */ 102,	// +0.2 deg Celsius
	/* coolingTargetLower */ -154,	// -0.3 deg Celsius

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
};
