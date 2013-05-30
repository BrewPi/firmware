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
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
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
	if(!beerSensor->isConnected() && (cs.mode == MODE_BEER_CONSTANT || cs.mode == MODE_FRIDGE_CONSTANT)){
		beerSensor->init(); // try to restart the sensor when controlling beer temperature
	}
	fridgeSensor->update();
	if(!fridgeSensor->isConnected()){
		fridgeSensor->init(); // always try to restart the fridge sensor
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
	if(cs.mode == MODE_BEER_CONSTANT || cs.mode == MODE_BEER_PROFILE){
		if(cs.beerSetting == INT_MIN){
			// beer setting is not updated yet
			// set fridge to unknown too
			cs.fridgeSetting = INT_MIN;
			return;
		}
		
		// fridge setting is calculated with PID algorithm. Beer temperature error is input to PID
		cv.beerDiff =  cs.beerSetting - beerSensor->readSlowFiltered();
		cv.beerSlope = beerSensor->readSlope();
		fixed7_9 fridgeFastFiltered = fridgeSensor->readFastFiltered();
			
		if(integralUpdateCounter++ == 60){
			integralUpdateCounter = 0;
			
			fixed7_9 integratorUpdate = cv.beerDiff;
			if(abs(integratorUpdate) < cc.iMaxError){
				//difference is smaller than iMaxError, check 4 conditions to see if integrator should be active
				bool updateSign = (integratorUpdate > 0); // 1 = positive, 0 = negative
				bool integratorSign = (cv.diffIntegral > 0);		
				
				// Actuator is not saturated. Update integrator
				if(updateSign == integratorSign){
					// beerDiff and integrator have same sign. Integrator would be increased.
					
					// set update to zero when timeSinceIdle > 30 min. Actuator is probably saturated
					integratorUpdate = (timeSinceIdle() > 1800u) ? 0 : integratorUpdate;
					
					// If actuator is already at max increasing actuator will only cause integrator windup.
					integratorUpdate = (cs.fridgeSetting == cc.tempSettingMax) ? 0 : integratorUpdate;
					integratorUpdate = (cs.fridgeSetting == cc.tempSettingMin) ? 0 : integratorUpdate;										
					
					// cooling and fridge temp is more than 2 degrees from setting, actuator is saturated.
					integratorUpdate = (!updateSign && (fridgeFastFiltered > (cs.fridgeSetting +1024))) ? 0 : integratorUpdate;
					
					// heating and fridge temp is more than 2 degrees from setting, actuator is saturated.
					integratorUpdate = (updateSign && (fridgeFastFiltered < (cs.fridgeSetting -1024))) ? 0 : integratorUpdate;
									
				}
				else{
					// integrator action is decreased. Decrease faster than increase.
					integratorUpdate = integratorUpdate*4;
				}
				
			}
			else{
				// decrease integral by 1/8 when not close to end value to prevent integrator windup
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
		cs.fridgeSetting = constrain(constrainTemp16(newFridgeSetting), cc.tempSettingMin, cc.tempSettingMax);
	}
	else{
		// FridgeTemperature is set manually, use INT_MIN to indicate
		cs.beerSetting = INT_MIN;
	}
}

void TempControl::updateState(void){
	//update state
	if(door->sense()){
		if(state!=DOOR_OPEN){
			// todo - use place holder annotation strings, and replace with full message in the python script. 
			piLink.printFridgeAnnotation(PSTR("Fridge door opened"));
		}
		state=DOOR_OPEN;
		return;
	}
	if(cs.mode == MODE_OFF){
		state = STATE_OFF;
		return;
	}
	if(cs.fridgeSetting == INT_MIN){
		// Do nothing when fridge setting is undefined
		state = IDLE;
		return;
	}
	
	if(!fridgeSensor->isConnected() || (!beerSensor->isConnected() && (cs.mode == MODE_BEER_CONSTANT || cs.mode == MODE_BEER_PROFILE))){
		state = IDLE; // stay idle when one of the sensors is disconnected
		return;
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
			resetWaitTime();
			if(fridgeFast > (cs.fridgeSetting+cc.idleRangeHigh) ){  // fridge temperature is too high			
				tempControl.updateWaitTime(MIN_SWITCH_TIME, sinceHeating);			
				if(cs.mode==MODE_FRIDGE_CONSTANT){
					tempControl.updateWaitTime(MIN_COOL_OFF_TIME_FRIDGE_CONSTANT, sinceCooling);
				}
				else{
					if(beerFast<cs.beerSetting){ // only start cooling when beer is too warm
						state = IDLE; // beer is already colder than setting, stay in or go to idle
						break;
					}
					tempControl.updateWaitTime(MIN_COOL_OFF_TIME, sinceCooling);
				}
				if(getWaitTime() > 0){
					state = WAITING_TO_COOL;
				}
				else{
					state = COOLING;	
				}
			}
			else if(fridgeFast < (cs.fridgeSetting+cc.idleRangeLow)){  // fridge temperature is too low
				tempControl.updateWaitTime(MIN_SWITCH_TIME, sinceCooling);
				tempControl.updateWaitTime(MIN_HEAT_OFF_TIME, sinceHeating);
				if(cs.mode!=MODE_FRIDGE_CONSTANT){
					if(beerFast > cs.beerSetting){ // only start heating when beer is too cold
						state = IDLE;  // beer is already warmer than setting, stay in or go to idle
						break;
					}
				}
				if(getWaitTime() > 0){
					state = WAITING_TO_HEAT;
				}
				else{
					state = HEATING;
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
			if(cv.estimatedPeak <= cs.fridgeSetting){
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
			if(cv.estimatedPeak >= cs.fridgeSetting){
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
		{
			if(!door->sense()){ 
				piLink.printFridgeAnnotation(PSTR("Fridge door closed"));
				state=IDLE;
			}
		}
		break;
	}			
}

void TempControl::updateEstimatedPeak(uint16_t timeLimit, fixed7_9 estimator, uint16_t sinceIdle)
{
	uint16_t activeTime = min(timeLimit, sinceIdle); // heat time in seconds
	fixed7_9 estimatedOvershoot = ((fixed23_9) estimator * activeTime)/3600; // overshoot estimator is in overshoot per hour
	if(state==COOLING || state==COOLING_MIN_TIME){
		estimatedOvershoot = -estimatedOvershoot; // when cooling subtract overshoot from fridge temperature
	}
	cv.estimatedPeak = fridgeSensor->readFastFiltered() + estimatedOvershoot;		
}

void TempControl::updateOutputs(void) {
	if (cs.mode==MODE_TEST)
		return;
		
	cameraLight.update();
	cooler->setActive(state==COOLING || state==COOLING_MIN_TIME);		
	heater->setActive(!cc.lightAsHeater && (state==HEATING || state==HEATING_MIN_TIME));	
	light->setActive(state==DOOR_OPEN || (cc.lightAsHeater && (state==HEATING || state==HEATING_MIN_TIME)) || cameraLightState.isActive());	
	fan->setActive(state==HEATING || state==COOLING || state==HEATING_MIN_TIME || state==COOLING_MIN_TIME);
}


void TempControl::detectPeaks(void){  
	//detect peaks in fridge temperature to tune overshoot estimators
	if(doPosPeakDetect && state!=HEATING){
		bool detected = false;
		fixed7_9 posPeak = fridgeSensor->detectPosPeak();
		fixed7_9 error = posPeak-cv.posPeakEstimate;
		if(posPeak != INT_MIN){
			// positive peak detected
			if(error > cc.heatingTargetUpper){ // positive error, peak was higher than estimate
				// estimated overshoot was too low, so adjust overshoot estimator
				increaseEstimator(&(cs.heatEstimator), error);
			}
			if(error < cc.heatingTargetLower){ // negative error, peak was lower than estimate
				// estimated overshoot was too high, so adjust overshoot estimator
				decreaseEstimator(&(cs.heatEstimator), error);
			}
			ESTIMATOR_MSG("Positive peak detected.");
			detected = true;
		}
		else if(timeSinceHeating() + 10 > HEAT_PEAK_DETECT_TIME && 
				fridgeSensor->readFastFiltered() < (cv.posPeakEstimate+cc.heatingTargetLower)){
			// Idle period almost reaches maximum allowed time for peak detection
			// This is the heat, then drift up too slow (but in the right direction).
			// estimator is too high
			posPeak=fridgeSensor->readFastFiltered();
			decreaseEstimator(&(cs.heatEstimator), error);			
			ESTIMATOR_MSG("Drifting up after heating too short.");
			detected = true;
		}
		if(detected){
			#if BREWPI_ESTIMATOR_MESSAGES 
			char tempString1[9]; char tempString2[9]; char tempString3[9];
			ESTIMATOR_MSG("Peak: %s Estimated: %s. New estimator: %s",
				tempToString(tempString1, posPeak, 3, 9),
				tempToString(tempString2, cv.posPeakEstimate, 3, 9),
				fixedPointToString(tempString3, cs.heatEstimator, 3, 9));
			#endif
			doPosPeakDetect=false;
			cv.posPeak = posPeak;
		}
		if(timeSinceHeating() > HEAT_PEAK_DETECT_TIME){
			doPosPeakDetect = false;
		}
	}			
	if(doNegPeakDetect && state!=COOLING){
		bool detected = false;
		fixed7_9 negPeak = fridgeSensor->detectNegPeak();
		fixed7_9 error = negPeak-cv.negPeakEstimate;
		if(negPeak != INT_MIN){
			// negative peak detected
			if(error < cc.coolingTargetLower){ // negative error, overshoot was higher than estimate
				// estimated overshoot was too low, so adjust overshoot estimator
				increaseEstimator(&(cs.coolEstimator), error);
			}
			if(error > cc.coolingTargetUpper){ // positive error, overshoot was lower than estimate
				// estimated overshoot was too high, so adjust overshoot estimator
				decreaseEstimator(&(cs.coolEstimator), error);
			}
			ESTIMATOR_MSG("Negative peak detected.");
			detected = true;
		}
		else if(timeSinceCooling() + 10 > COOL_PEAK_DETECT_TIME && 
				fridgeSensor->readFastFiltered() > (cv.negPeakEstimate+cc.coolingTargetUpper)){
			// Idle period almost reaches maximum allowed time for peak detection
			// This is the cooling, then drift down too slow (but in the right direction).
			// estimator is too high
			decreaseEstimator(&(cs.coolEstimator), error);
			ESTIMATOR_MSG("Drifting down after cooling too short.");
			detected = true;
		}
		if(detected){
			#if BREWPI_ESTIMATOR_MESSAGES
			char tempString1[9]; char tempString2[9]; char tempString3[9];
			ESTIMATOR_MSG("Peak: %s. Estimated: %s. New estimator: %s",
				tempToString(tempString1, negPeak, 3, 9),
				tempToString(tempString2, cv.negPeakEstimate, 3, 9),
				fixedPointToString(tempString3, cs.coolEstimator, 3, 9));
			#endif				
			doNegPeakDetect=false;
			cv.negPeak = negPeak;
		}
		if(timeSinceCooling() > COOL_PEAK_DETECT_TIME){
			doNegPeakDetect = false;
		}			
	}		
}

// Increase estimator at least 20%, max 50%s
void TempControl::increaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed23_9 factor = 614 + constrainTemp(abs(error)>>5, 0, 154); // 1.2 + 3.1% of error, limit between 1.2 and 1.5
	fixed23_9 newEstimator = (fixed23_9) *estimator * factor;
	byte max = byte((INT_MAX*512L)>>24);
	byte upper = byte(newEstimator>>24);
	*estimator = upper>max ? INT_MAX : newEstimator>>8; // shift back to normal precision	
	eepromManager.storeTempSettings();
}

// Decrease estimator at least 16.7% (1/1.2), max 33.3% (1/1.5)
void TempControl::decreaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed23_9 factor = 426 - constrainTemp(abs(error)>>5, 0, 85); // 0.833 - 3.1% of error, limit between 0.667 and 0.833
	fixed23_9 newEstimator = (fixed23_9) *estimator * factor;
	*estimator = newEstimator>>8; // shift back to normal precision
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
	cs.beerSetting = 20<<9;;
	cs.fridgeSetting = 20<<9;
	cs.heatEstimator = 102; // 0.2*2^9
	cs.coolEstimator=5<<9;
}

void TempControl::storeConstants(eptr_t offset){	
	eepromAccess.writeBlock(offset, (void *) &cc, sizeof(ControlConstants));	
}

void TempControl::loadConstants(eptr_t offset){
	eepromAccess.readBlock((void *) &cc, offset, sizeof(ControlConstants));
	constantsChanged();	
}

// write new settings to EEPROM to be able to reload them after a reset
// The update functions only write to EEPROM if the value has changed
void TempControl::storeSettings(eptr_t offset){
	eepromAccess.writeBlock(offset, (void *) &cs, sizeof(ControlSettings));
	storedBeerSetting = cs.beerSetting;		
}

void TempControl::loadSettings(eptr_t offset){
	eepromAccess.readBlock((void *) &cs, offset, sizeof(ControlSettings));	
	logDeveloper("loaded settings");
	setMode(cs.mode, true);		// force the mode update
}


void TempControl::loadDefaultConstants(void){
	cc.tempFormat = 'C';
	// maximum history to take into account, in seconds
	cc.maxHeatTimeForEstimate = 600;
	cc.maxCoolTimeForEstimate = 1200;

	// Limits of fridge temperature setting
	cc.tempSettingMax = 30*512;	// +30 deg Celsius
	cc.tempSettingMin = 1*512;	// +1 deg Celsius

	// control defines, also in fixed point format (7 int bits, 9 frac bits), so multiplied by 2^9=512
	cc.Kp	= 10240;	// +20
	cc.Ki		= 307;		// +0.6
	cc.Kd	= -1536;	// -3
	cc.iMaxError = 256;  // 0.5 deg

	// Stay Idle when temperature is in this range
	cc.idleRangeHigh = 512;	// +1 deg Celsius
	cc.idleRangeLow = -512;	// -1 deg Celsius

	// when peak falls between these limits, its good.
	cc.heatingTargetUpper = 154;	// +0.3 deg Celsius
	cc.heatingTargetLower = -102;	// -0.2 deg Celsius
	cc.coolingTargetUpper = 102;	// +0.2 deg Celsius
	cc.coolingTargetLower = -154;	// -0.3 deg Celsius

	cc.lightAsHeater = 0;
	cc.rotaryHalfSteps = 0;
	
	// Set filter coefficients. This is the b value. See FilterFixed.h for delay times.
	// The delay time is 3.33 * 2^b * number of cascades
	cc.fridgeFastFilter = 1u;
	cc.fridgeSlowFilter = 4u;
	cc.fridgeSlopeFilter = 3u;
	cc.beerFastFilter = 3u;
	cc.beerSlowFilter = 5u;
	cc.beerSlopeFilter = 4u;
	
	cc.lightAsHeater = 0;
	constantsChanged();
}

void TempControl::constantsChanged()
{
	fridgeSensor->setFastFilterCoefficients(cc.fridgeFastFilter);
	fridgeSensor->setSlowFilterCoefficients(cc.fridgeSlowFilter);
	fridgeSensor->setSlopeFilterCoefficients(cc.fridgeSlopeFilter);
	beerSensor->setFastFilterCoefficients(cc.beerFastFilter);
	beerSensor->setSlowFilterCoefficients(cc.beerSlowFilter);
	beerSensor->setSlopeFilterCoefficients(cc.beerSlopeFilter);		
}

void TempControl::setMode(char newMode, bool force){
	logDeveloper("TempControl::setMode from %c to %c", cs.mode, newMode);
	
	if(newMode != cs.mode){
		state = IDLE;
		force = true;
	}
	if (force) {
		cs.mode = newMode;
		if(newMode==MODE_BEER_PROFILE || newMode == MODE_OFF){
			// set temperatures to undefined until temperatures have been received from RPi
			cs.beerSetting = INT_MIN;
			cs.fridgeSetting = INT_MIN;
		}
		eepromManager.storeTempSettings();
	}
}

fixed7_9 TempControl::getBeerTemp(void){
	if(beerSensor->isConnected()){
		return beerSensor->readFastFiltered();	
	}
	else{
		return INT_MIN;
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
		return INT_MIN;
	}
}

fixed7_9 TempControl::getFridgeSetting(void){
	return cs.fridgeSetting;	
}

void TempControl::setBeerTemp(fixed7_9 newTemp){
	int oldBeerSetting = cs.beerSetting;
	cs.beerSetting= newTemp;
	if(abs(oldBeerSetting - newTemp) > 128){ // more than half a degree C difference with old setting
		reset(); // reset controller
	}
	updatePID();
	updateState();
	if(abs(storedBeerSetting - newTemp) > 128){ // more than half a degree C difference with EEPROM
		// Do not store settings every time, because EEPROM has limited number of write cycles.
		// If Raspberry Pi is connected, it will update the settings anyway. This is just a safety feature.
		eepromManager.storeTempSettings();
	}		
}

void TempControl::setFridgeTemp(fixed7_9 newTemp){
	cs.fridgeSetting = newTemp;
	reset(); // reset peak detection and PID
	updatePID();
	updateState();	
}