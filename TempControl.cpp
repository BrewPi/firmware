/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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

#include <Arduino.h>

#include "pins.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <limits.h>

#include "temperatureFormats.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensor.h"
#include "Ticks.h"


TempControl tempControl;

// Declare static variables
TempSensor TempControl::beerSensor(beerSensorPin);
TempSensor TempControl::fridgeSensor(fridgeSensorPin);
	
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
unsigned long TempControl::lastIdleTime;
unsigned long TempControl::lastHeatTime;
unsigned long TempControl::lastCoolTime;

void TempControl::init(void){
	state=STARTUP;
	beerSensor.init();
	fridgeSensor.init();
	updateTemperatures();
	reset();
}

void TempControl::reset(void){
	doPosPeakDetect=false;
	doNegPeakDetect=false;
}

void TempControl::updateTemperatures(void){
	beerSensor.update();
	if(!beerSensor.isConnected() && (cs.mode == MODE_BEER_CONSTANT || cs.mode == MODE_FRIDGE_CONSTANT)){
		beerSensor.init(); // try to restart the sensor when controlling beer temperature
	}
	fridgeSensor.update();
	if(!fridgeSensor.isConnected()){
		fridgeSensor.init(); // always try to restart the fridge sensor
	}
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
		cv.beerDiff =  cs.beerSetting - beerSensor.readSlowFiltered();
		cv.beerSlope = beerSensor.readSlope();
		if(integralUpdateCounter++ == 60){
			integralUpdateCounter = 0;
			if(abs(cv.beerDiff) < cc.iMaxError){
				//difference is smaller than iMaxError, check 4 conditions to see if integrator should be active
				if(timeSinceIdle() > 1800){
					// more than 30 minutes since idle, actuator is probably saturated. Do not increase integrator.
				}			
				else if(cv.beerDiff < 0 && (cs.fridgeSetting +1024) < fridgeSensor.readFastFiltered()){
					// cooling and fridge temp is more than 2 degrees from setting, actuator is saturated.
				}
				else if(cv.beerDiff > 0 && (cs.fridgeSetting -1024) > fridgeSensor.readFastFiltered()){
					// heating and fridge temp is more than 2 degrees from setting, actuator is saturated.
				}					
				else{
					// Actuator is not saturated. Update integrator
					cv.diffIntegral = cv.diffIntegral + cv.beerDiff;
				}
			}
			else{
				// decrease integral by 1/8 when not close to end value to prevent integrator windup
				cv.diffIntegral = cv.diffIntegral-(cv.diffIntegral>>3);
			
			}
		}			
				
		// calculate PID parts. Use fixed23_9 to prevent overflow
		cv.p = ((fixed23_9) cc.Kp * (fixed23_9) cv.beerDiff)>>9;
		cv.i = ((fixed23_9) cc.Ki * cv.diffIntegral)>>9;
		cv.d = ((fixed23_9) cc.Kd * (fixed23_9) cv.beerSlope)>>9;
				
		cs.fridgeSetting = constrain(cs.beerSetting + cv.p + cv.i + cv.d, cc.tempSettingMin, cc.tempSettingMax);
	}
	else{
		// FridgeTemperature is set manually, use INT_MIN to indicate
		cs.beerSetting = INT_MIN;
	}
}

void TempControl::updateState(void){
	//update state
	if(digitalRead(doorPin) == LOW){
		if(state!=DOOR_OPEN){
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
		// Don nothing when fridge setting is undefined
		state = IDLE;
		return;
	}
	
	if(!fridgeSensor.isConnected() || (!beerSensor.isConnected() && (cs.mode == MODE_BEER_CONSTANT || cs.mode == MODE_BEER_PROFILE))){
		state = IDLE; // stay idle when one of the sensors is disconnected
		return;
	}

	switch(state)
	{
		case STARTUP:
		case IDLE:
		case STATE_OFF:
		{
			lastIdleTime=ticks.seconds();
			if(doNegPeakDetect == true || doPosPeakDetect == true){
				// Wait for peaks before starting to heat or cool again
				return;
			}				
			if(fridgeSensor.readFastFiltered() > (cs.fridgeSetting+cc.idleRangeHigh) ){ // fridge temperature is too high
				if(cs.mode==MODE_FRIDGE_CONSTANT){
					if((timeSinceCooling() > MIN_COOL_OFF_TIME_FRIDGE_CONSTANT && timeSinceHeating() > MIN_SWITCH_TIME) || state == STARTUP){
						state=COOLING;
					}
					return;
				}
				else{
					if(beerSensor.readFastFiltered()<cs.beerSetting){ // only start cooling when beer is too warm
						return; // beer is already colder than setting, stay in IDLE.
					}
					if((timeSinceCooling() > MIN_COOL_OFF_TIME && timeSinceHeating() > MIN_SWITCH_TIME) || state == STARTUP){
						state=COOLING;
					}
					return;
				}
			}
			else if(fridgeSensor.readFastFiltered() < (cs.fridgeSetting+cc.idleRangeLow)){ // fridge temperature is too low
				if(beerSensor.readFastFiltered()>cs.beerSetting){ // only start heating when beer is too cold
					return; // beer is already warmer than setting, stay in IDLE
				}
				if((timeSinceCooling() > MIN_SWITCH_TIME && timeSinceHeating() > MIN_HEAT_OFF_TIME) || state == STARTUP){
					state=HEATING;
					return;
				}					
			}
		}			
		break; 
		case COOLING:
		{
			doNegPeakDetect=true;
			lastCoolTime = ticks.seconds();
			int coolTime = min(cc.maxCoolTimeForEstimate, timeSinceIdle()); // cool time in seconds
			fixed7_9 estimatedOvershoot = ((fixed23_9) cs.coolEstimator * coolTime)/3600; // overshoot estimator is in overshoot per hour
			cv.estimatedPeak = fridgeSensor.readFastFiltered() - estimatedOvershoot;
			if(cv.estimatedPeak <= cs.fridgeSetting){
				if(timeSinceIdle() > MIN_COOL_ON_TIME){
					cv.negPeakEstimate = cv.estimatedPeak; // remember estimated peak when I switch to IDLE, to adjust estimator later
					state=IDLE;
				}					
				return;
			}
		}		
		break;
		case HEATING:
		{
			doPosPeakDetect=true;
			lastHeatTime=ticks.seconds();
			int heatTime = min(cc.maxHeatTimeForEstimate, timeSinceIdle()); // heat time in seconds
			fixed7_9 estimatedOvershoot = ((fixed23_9) cs.heatEstimator * heatTime)/3600; // overshoot estimator is in overshoot per hour
			cv.estimatedPeak = fridgeSensor.readFastFiltered() + estimatedOvershoot;
			if(cv.estimatedPeak >= cs.fridgeSetting){
				if(timeSinceIdle() > MIN_HEAT_ON_TIME){
					cv.posPeakEstimate=cv.estimatedPeak; // remember estimated peak when I switch to IDLE, to adjust estimator later
					state=IDLE;
				}
				return;
			}
		}
		break;
		case DOOR_OPEN:
		{
			if(digitalRead(doorPin) == HIGH){ 
				piLink.printFridgeAnnotation(PSTR("Fridge door closed"));
				state=IDLE;
				return;
			}
		}
		break;
	}			
}

void TempControl::updateOutputs(void){
	// Outputs are inverted on the shield by the mosfets!
	switch (state)
	{
		case IDLE:
		case STARTUP:
			digitalWrite(coolingPin, HIGH);
			digitalWrite(heatingPin, HIGH);
			break;
		case COOLING:
			digitalWrite(coolingPin, LOW);
			digitalWrite(heatingPin, HIGH);
			break;
		case HEATING:
			digitalWrite(coolingPin, HIGH);
			digitalWrite(heatingPin, LOW);
			break;
		case DOOR_OPEN:
			if(LIGHT_AS_HEATER){
				digitalWrite(coolingPin, HIGH);
				digitalWrite(heatingPin, LOW);
			}
			else{
				digitalWrite(coolingPin, HIGH);
				digitalWrite(heatingPin, HIGH);
			}			
			break;
		default:
			digitalWrite(coolingPin, HIGH);
			digitalWrite(heatingPin, HIGH);
			break;
	}
}

void TempControl::detectPeaks(void){  
	//detect peaks in fridge temperature to tune overshoot estimators
	if(doPosPeakDetect && state!=HEATING){
		bool detected = false;
		fixed7_9 posPeak = fridgeSensor.detectPosPeak();
		if(posPeak != INT_MIN){
			// maximum detected
			if(posPeak>cv.posPeakEstimate+cc.heatingTargetUpper){
				//should not happen, estimated overshoot was too low, so adjust overshoot estimator
				fixed7_9 error = posPeak-(cv.posPeakEstimate+cc.heatingTargetUpper);
				increaseEstimator(&(cs.heatEstimator), error);
			}
			if(posPeak<cv.posPeakEstimate+cc.heatingTargetLower){
				//should not happen, estimated overshoot was too high, so adjust overshoot estimator
				fixed7_9 error = posPeak-(cv.posPeakEstimate+cc.heatingTargetLower); // will be negative
				decreaseEstimator(&(cs.heatEstimator), error);
			}
			piLink.debugMessage(PSTR("Positive peak detected."));
			detected = true;
		}
		else if(timeSinceHeating() + 10 > HEAT_PEAK_DETECT_TIME && fridgeSensor.readFastFiltered() < (cv.posPeakEstimate+cc.heatingTargetLower)){
			// Idle period almost reaches maximum allowed time for peak detection
			// This is the heat, then drift up too slow (but in the right direction).
			// estimator is too high
			posPeak=fridgeSensor.readFastFiltered();
			fixed7_9 error = posPeak-(cv.posPeakEstimate+cc.heatingTargetLower); // will be negative
			decreaseEstimator(&(cs.heatEstimator), error);
			
			piLink.debugMessage(PSTR("Drifting up after heating too short."));
			detected = true;
		}
		if(detected){
			char tempString1[9]; char tempString2[9]; char tempString3[9];
			piLink.debugMessage(PSTR("Peak: %s Estimated: %s. New estimator: %s"),
				tempToString(tempString1, posPeak, 3, 9),
				tempToString(tempString2, cv.posPeakEstimate, 3, 9),
				fixedPointToString(tempString3, cs.heatEstimator, 3, 9));
			doPosPeakDetect=false;
			cv.posPeak = posPeak;
		}
		if(timeSinceHeating() > HEAT_PEAK_DETECT_TIME){
			doPosPeakDetect = false;
		}
	}		
	if(doNegPeakDetect && state!=COOLING){
		fixed7_9 negPeak = fridgeSensor.detectNegPeak();
		bool detected = false;
		if(negPeak != INT_MIN){
			// negative peak detected
			if(negPeak<cv.negPeakEstimate+cc.coolingTargetLower){
				//should not happen, estimated overshoot was too low, so adjust overshoot estimator
				fixed7_9 error = negPeak-(cv.negPeakEstimate+cc.coolingTargetLower); //positive value
				increaseEstimator(&(cs.coolEstimator), error);
			}
			if(negPeak>cv.negPeakEstimate+cc.coolingTargetUpper){
				fixed7_9 error = negPeak-(cv.negPeakEstimate+cc.coolingTargetLower); //negative value
				decreaseEstimator(&(cs.coolEstimator), error);
			}
			piLink.debugMessage(PSTR("Negative peak detected."));
			detected = true;
		}
		else if(timeSinceCooling() + 10 > COOL_PEAK_DETECT_TIME && fridgeSensor.readFastFiltered() > (cv.negPeakEstimate+cc.coolingTargetUpper)){
			// Idle period almost reaches maximum allowed time for peak detection
			// This is the cooling, then drift down too slow (but in the right direction).
			// estimator is too high
			fixed7_9 error = negPeak-(cv.negPeakEstimate+cc.coolingTargetLower); //negative value
			decreaseEstimator(&(cs.coolEstimator), error);
			piLink.debugMessage(PSTR("Drifting down after cooling too short."));
			detected = true;
		}
		if(detected){
			char tempString1[9]; char tempString2[9]; char tempString3[9];
			piLink.debugMessage(PSTR("Peak: %s. Estimated: %s. New estimator: %s"),
				tempToString(tempString1, negPeak, 3, 9),
				tempToString(tempString2, cv.negPeakEstimate, 3, 9),
				fixedPointToString(tempString3, cs.coolEstimator, 3, 9));
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
	fixed23_9 factor = 614 + constrain(error>>5, 0, 154); // 1.2 + 3.1% of error, limit between 1.2 and 1.5
	fixed23_9 newEstimator = (fixed23_9) *estimator * factor;
	*estimator = (newEstimator >= INT_MAX*512L) ? INT_MAX : newEstimator>>9; // shift back to normal precision
	storeSettings();
}

// Decrease estimator at least 16.7% (1/1.2), max 33.3% (1/1.5)
void TempControl::decreaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed23_9 factor = 426 + constrain(error>>5, -85, 0); // 0.833 + 3.1% of error, limit between 0.667 and 0.833
	fixed23_9 newEstimator = (fixed23_9) *estimator * factor;
	*estimator = newEstimator>>9; // shift back to normal precision
	storeSettings();
}

uint16_t TempControl::timeSinceCooling(void){
	uint16_t currentTime = ticks.seconds();
	uint16_t timeSinceLastOn;
	if(currentTime>=lastCoolTime){
		timeSinceLastOn = currentTime - lastCoolTime;
	}
	else{
		// overflow has occured
		timeSinceLastOn = (currentTime + 1440) - (lastCoolTime +1440); // add a day to both for calculation
	}
	return timeSinceLastOn;
}

uint16_t TempControl::timeSinceHeating(void){
	uint16_t currentTime = ticks.seconds();
	uint16_t timeSinceLastOn;
	if(currentTime>=lastHeatTime){
		timeSinceLastOn = currentTime - lastHeatTime;
	}
	else{
		// overflow has occured
		timeSinceLastOn = (currentTime + 1440) - (lastHeatTime +1440); // add a day to both for calculation
	}
	return timeSinceLastOn;
}

uint16_t TempControl::timeSinceIdle(void){
	uint16_t currentTime = ticks.seconds();
	uint16_t timeSinceLastOn;
	if(currentTime>=lastIdleTime){
		timeSinceLastOn = currentTime - lastIdleTime;
	}
	else{
		// overflow has occured
		timeSinceLastOn = (currentTime + 1440) - (lastIdleTime +1440); // add a day to both for calculation
	}
	return timeSinceLastOn;
}

// write new settings to EEPROM to be able to reload them after a reset
// The update functions only write to EEPROM if the value has changed
void TempControl::storeSettings(void){
	eeprom_update_block((void *) &cs, (void *) EEPROM_CONTROL_SETTINGS_ADDRESS, sizeof(ControlSettings));		
	storedBeerSetting = cs.beerSetting;
}

void TempControl::loadSettings(void){
	eeprom_read_block((void *) &cs, (void *) EEPROM_CONTROL_SETTINGS_ADDRESS, sizeof(ControlSettings));
	fridgeSensor.setFastFilterCoefficients(cc.fridgeFastFilter);
	fridgeSensor.setSlowFilterCoefficients(cc.fridgeSlowFilter);
	fridgeSensor.setSlopeFilterCoefficients(cc.fridgeSlopeFilter);
	beerSensor.setFastFilterCoefficients(cc.beerFastFilter);
	beerSensor.setSlowFilterCoefficients(cc.beerSlowFilter);
	beerSensor.setSlopeFilterCoefficients(cc.beerSlopeFilter);
}

void TempControl::loadDefaultSettings(void){
	cs.mode = MODE_BEER_CONSTANT;
	cs.beerSetting = 20<<9;;
	cs.fridgeSetting = 20<<9;
	cs.heatEstimator = 102; // 0.2*2^9
	cs.coolEstimator = 5<<9;
	storeSettings();
}

void TempControl::storeConstants(void){
	eeprom_update_block((void *) &cc, (void *) EEPROM_CONTROL_CONSTANTS_ADDRESS, sizeof(ControlConstants));
}

void TempControl::loadConstants(void){
	eeprom_read_block((void *) &cc, (void *) EEPROM_CONTROL_CONSTANTS_ADDRESS, sizeof(ControlConstants));
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
	cc.Ki	= 307;		// +0.6
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

	// Set filter coefficients. This is the b value. See FixedFilter.h for delay times.
	// The delay time is 3.33 * 2^b * number of cascades
	cc.fridgeFastFilter = 1u;
	fridgeSensor.setFastFilterCoefficients(cc.fridgeFastFilter);
	cc.fridgeSlowFilter = 4u;
	fridgeSensor.setSlowFilterCoefficients(cc.fridgeSlowFilter);
	cc.fridgeSlopeFilter = 3u;
	fridgeSensor.setSlopeFilterCoefficients(cc.fridgeSlopeFilter);
	cc.beerFastFilter = 3u;
	beerSensor.setFastFilterCoefficients(cc.beerFastFilter);
	cc.beerSlowFilter = 5u;
	beerSensor.setSlowFilterCoefficients(cc.beerSlowFilter);
	cc.beerSlopeFilter = 4u;
	beerSensor.setSlopeFilterCoefficients(cc.beerSlopeFilter);
	storeConstants();
}

void TempControl::loadSettingsAndConstants(void){
	if(eeprom_read_byte((unsigned char *) EEPROM_IS_INITIALIZED_ADDRESS) != 1){
		// EEPROM is not initialized, use default settings
		loadDefaultSettings();
		loadDefaultConstants();
		eeprom_write_byte((unsigned char *) EEPROM_IS_INITIALIZED_ADDRESS, 1);
		storeSettings();
		storeConstants();
	}
	else{
		loadSettings();
		loadConstants();
	}
}

void TempControl::setMode(char newMode){
	if(newMode != cs.mode){
		state = IDLE;
		cs.mode = newMode;
		if(newMode==MODE_BEER_PROFILE || newMode == MODE_OFF){
			// set temperatures to undefined until temperatures have been received from RPi
			cs.beerSetting = INT_MIN;
			cs.fridgeSetting = INT_MIN;
		}
		storeSettings();
	}
}

fixed7_9 TempControl::getBeerTemp(void){
	return beerSensor.readFastFiltered();
}

fixed7_9 TempControl::getBeerSetting(void){
	return cs.beerSetting;	
}

fixed7_9 TempControl::getFridgeTemp(void){
	return fridgeSensor.readFastFiltered();	
}

fixed7_9 TempControl::getFridgeSetting(void){
	return cs.fridgeSetting;	
}

void TempControl::setBeerTemp(int newTemp){
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
		storeSettings();
	}		
}

void TempControl::setFridgeTemp(int newTemp){
	cs.fridgeSetting = newTemp;
	reset(); // reset peak detection and PID
	updatePID();
	updateState();
	storeSettings();
}