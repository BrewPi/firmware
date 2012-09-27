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
	cs.mode=MODE_BEER_CONSTANT;
	state=STARTUP;
	beerSensor.init();
	fridgeSensor.init();
	updateTemperatures();
	reset();
}

void TempControl::reset(void){
	fixed7_9 temp = getBeerTemp();
	if(cs.beerSetting < temp){
		cv.Kp=cc.KpCool;
		cv.Kd=cc.KdCool;
	}
	else{
		cv.Kp=cc.KpHeat;
		cv.Kd=cc.KdHeat;
	}
	doPosPeakDetect=false;
	doNegPeakDetect=false;
}

void TempControl::updateTemperatures(void){
	beerSensor.update();
	fridgeSensor.update();
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
			if(abs(cv.beerDiff) < cc.iMaxError && cv.beerSlope <= cc.iMaxSlope && cv.beerSlope >= cc.iMinSlope ){
				//difference is smaller than iMaxError degree and slope is almost horizontal 
				if(abs(cv.beerDiff) > 26){ // difference is more than 0.05 degree Celsius
					cv.diffIntegral = cv.diffIntegral + cv.beerDiff;
				}
			}
			else{
				// decrease integral by 1/8 when not close to end value to prevent integrator windup
				cv.diffIntegral = cv.diffIntegral-(cv.diffIntegral>>3);
			
			}
		}			
		if(cv.beerDiff<0){ // slowly go to cool parameters
			// shift left instead of right or limited precision will cause a limit value 512/(2^8) under the target value.
			cv.Kp = ((((fixed23_9) cv.Kp)<<8) - cv.Kp + cc.KpCool)>>8;
			cv.Kd = ((((fixed23_9) cv.Kd)<<8) - cv.Kd + cc.KdCool)>>8;
		}
		else{ // slowly go to heat parameters
			cv.Kp = ((((fixed23_9) cv.Kp)<<8) - cv.Kp + cc.KpHeat)>>8;
			cv.Kd = ((((fixed23_9) cv.Kd)<<8) - cv.Kd + cc.KdHeat)>>8;
		}
		
		// calculate PID parts. Use fixed23_9 to prevent overflow
		cv.p = ((fixed23_9) cv.Kp * (fixed23_9) cv.beerDiff)>>9;
		cv.i = ((fixed23_9) cc.Ki * cv.diffIntegral)>>9;
		cv.d = ((fixed23_9) cv.Kd * (fixed23_9) cv.beerSlope)>>9;
				
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

	switch(state)
	{
		case STARTUP:
		case IDLE:
		case STATE_OFF:
		{
			lastIdleTime=millis();
			if(	((timeSinceCooling() > 900000UL || doNegPeakDetect==false) && (timeSinceHeating() > 600000UL || doPosPeakDetect==false)) ||
					state==STARTUP) //if cooling is 15 min ago and heating 10, or I just started
			{
				if(fridgeSensor.readFastFiltered() > (cs.fridgeSetting+cc.idleRangeHigh) ){
					if(cs.mode!=MODE_FRIDGE_CONSTANT){
						if(beerSensor.readFastFiltered()>cs.beerSetting+26){ // only start cooling when beer is too warm (0.05 degree idle space)
							state=COOLING;
							return;
						}		  
					}
					else{
						state=COOLING;
						return;
					}
				}
				else if(fridgeSensor.readFastFiltered() < (cs.fridgeSetting+cc.idleRangeLow)){
					if(cs.mode!=MODE_FRIDGE_CONSTANT){
						if(beerSensor.readFastFiltered()<cs.beerSetting-26){ // only start heating when beer is too cold (0.05 degree idle space)
							state=HEATING;
							return;
						}
					}
					else{
						state=HEATING;
						return;
					}
				}
			}
			if(timeSinceCooling()>1800000UL){ //30 minutes
				doNegPeakDetect=false;  //peak would be from drifting in idle, not from cooling
			}
			if(timeSinceHeating()>900000UL){ //20 minutes
				doPosPeakDetect=false;  //peak would be from drifting in idle, not from heating
			}
		}			
		break; 
		case COOLING:
		{
			doNegPeakDetect=true;
			lastCoolTime = millis();
			int coolTime = min(cc.maxCoolTimeForEstimate, timeSinceIdle()/1000); // cool time in seconds
			fixed7_9 estimatedOvershoot = ((fixed23_9) cs.coolEstimator * coolTime)/3600; // overshoot estimator is in overshoot per hour
			cv.estimatedPeak = fridgeSensor.readFastFiltered() - estimatedOvershoot;
			if(cv.estimatedPeak <= cs.fridgeSetting + COOLING_TARGET){
				cv.negPeakSetting = cs.fridgeSetting; // remember temperature when I switch to Idle, to adjust estimator later
				state=IDLE;
				return;
			}
		}		
		break;
		case HEATING:
		{
			doPosPeakDetect=true;
			lastHeatTime=millis();
			int heatTime = min(cc.maxHeatTimeForEstimate, timeSinceIdle()/1000	); // heat time in seconds
			fixed7_9 estimatedOvershoot = ((fixed23_9) cs.heatEstimator * heatTime)/3600; // overshoot estimator is in overshoot per hour
			cv.estimatedPeak = fridgeSensor.readFastFiltered() + estimatedOvershoot;
			if(cv.estimatedPeak >= cs.fridgeSetting + HEATING_TARGET){
				cv.posPeakSetting=cs.fridgeSetting; // remember temperature when I switch to Idle, to adjust estimator later
				state=IDLE;
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
	switch (state)
	{
		case IDLE:
		case STARTUP:
			digitalWrite(coolingPin, LOW);
			digitalWrite(heatingPin, LOW);
			break;
		case COOLING:
			digitalWrite(coolingPin, HIGH);
			digitalWrite(heatingPin, LOW);
			break;
		case HEATING:
		case DOOR_OPEN:
			digitalWrite(coolingPin, LOW);
			digitalWrite(heatingPin, HIGH);
			break;
		default:
			digitalWrite(coolingPin, LOW);
			digitalWrite(heatingPin, LOW);
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
			if(posPeak>cv.posPeakSetting+cc.heatingTargetUpper){
				//should not happen, estimated overshoot was too low, so adjust overshoot estimator
				fixed7_9 error = posPeak-(cv.posPeakSetting+cc.heatingTargetUpper);
				increaseEstimator(&cs.heatEstimator, error);
			}
			if(posPeak<cv.posPeakSetting+cc.heatingTargetLower){
				//should not happen, estimated overshoot was too high, so adjust overshoot estimator
				fixed7_9 error = posPeak-(cv.posPeakSetting+cc.heatingTargetLower); // will be negative
				decreaseEstimator(&cs.heatEstimator, error);
			}
			piLink.debugMessage(PSTR("Positive peak detected."));
			detected = true;
		}
		else if(timeSinceHeating() > 580000UL && timeSinceCooling() > 880000UL && fridgeSensor.readFastFiltered() < (cv.posPeakSetting+cc.heatingTargetLower)){
			// heating is almost 10 minutes ago, cooling is almost 15 minutes ago, but still no peak
			// This is the heat, then drift up too slow (but in the right direction).
			// estimator is too high
			posPeak=fridgeSensor.readFastFiltered();
			fixed7_9 error = posPeak-(cv.posPeakSetting+cc.heatingTargetLower); // will be negative
			decreaseEstimator(&cs.heatEstimator, error);
			
			piLink.debugMessage(PSTR("Drifting up after heating too short."));
			detected = true;
		}
		if(detected){
			char tempString1[9]; char tempString2[9]; char tempString3[9];
			piLink.debugMessage(PSTR("Peak: %s Estimated: %s. New estimator: %s"),
				tempToString(tempString1, posPeak, 3, 9),
				tempToString(tempString2, cv.posPeakSetting, 3, 9),
				fixedPointToString(tempString3, cs.heatEstimator, 3, 9));
			doPosPeakDetect=false;
			cv.posPeak = posPeak;
		}			
	}		
	if(doNegPeakDetect && state!=COOLING){
		fixed7_9 negPeak = fridgeSensor.detectNegPeak();
		bool detected = false;
		if(negPeak != INT_MIN){
			// negative peak detected
			if(negPeak<cv.negPeakSetting+cc.coolingTargetLower){
				//should not happen, estimated overshoot was too low, so adjust overshoot estimator
				fixed7_9 error = negPeak-(cv.negPeakSetting+cc.coolingTargetLower); //positive value
				increaseEstimator(&cs.coolEstimator, error);
			}
			if(negPeak>cv.negPeakSetting+cc.coolingTargetUpper){
				fixed7_9 error = negPeak-(cv.negPeakSetting+cc.coolingTargetLower); //negative value
				decreaseEstimator(&cs.coolEstimator, error);
			}
			piLink.debugMessage(PSTR("Negative peak detected."));
			detected = true;
		}
		else if(timeSinceHeating() > 580000UL && timeSinceCooling() > 880000UL && fridgeSensor.readFastFiltered() > (cv.negPeakSetting+cc.coolingTargetUpper)){
			// Heating is almost 10 minutes ago, cooling is almost 15 minutes ago, but still no peak
			// This is the cooling, then drift down too slow (but in the right direction).
			// estimator is too high
			fixed7_9 error = negPeak-(cv.negPeakSetting+cc.coolingTargetLower); //negative value
			decreaseEstimator(&cs.coolEstimator, error);
			piLink.debugMessage(PSTR("Drifting down after cooling too short."));
			detected = true;
		}
		if(detected){
			char tempString1[9]; char tempString2[9]; char tempString3[9];
			piLink.debugMessage(PSTR("Peak: %s. Estimated: %s. New estimator: %s"),
				tempToString(tempString1, negPeak, 3, 9),
				tempToString(tempString2, cv.negPeakSetting, 3, 9),
				fixedPointToString(tempString3, cs.coolEstimator, 3, 9));
			doNegPeakDetect=false;
				cv.negPeak = negPeak;
		}			
	}		
}

void TempControl::increaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed23_9 factor = 614 + (error>>5); // 1.2 + 3.1% of error
	factor = constrain(factor, 614, 768); // limit between 1.2 and 1.5
	*estimator = ((fixed23_9) *estimator * factor)>>9;
	storeSettings();
}

void TempControl::decreaseEstimator(fixed7_9 * estimator, fixed7_9 error){
	fixed23_9 factor = 410 + (error>>5); // 0.8 + 3.1% of error
	factor = constrain(factor, 256, 410); // limit between 0.5 and 0.8
	*estimator = ((fixed23_9) *estimator * factor)>>9;
	storeSettings();
}

unsigned long TempControl::timeSinceCooling(void){
	unsigned long currentTime = millis();
	unsigned long timeSinceLastOn;
	if(currentTime>=lastCoolTime){
		timeSinceLastOn = currentTime - lastCoolTime;
	}
	else{
		// millis() overflow has occured
		timeSinceLastOn = (currentTime + 1440000) - (lastCoolTime +1440000); // add a day to both for calculation
	}
	return timeSinceLastOn;
}

unsigned long TempControl::timeSinceHeating(void){
	unsigned long currentTime = millis();
	unsigned long timeSinceLastOn;
	if(currentTime>=lastHeatTime){
		timeSinceLastOn = currentTime - lastHeatTime;
	}
	else{
		// millis() overflow has occured
		timeSinceLastOn = (currentTime + 1440000) - (lastHeatTime +1440000); // add a day to both for calculation
	}
	return timeSinceLastOn;
}


unsigned long TempControl::timeSinceIdle(void){
	unsigned long currentTime = millis();
	unsigned long timeSinceLastOn;
	if(currentTime>=lastIdleTime){
		timeSinceLastOn = currentTime - lastIdleTime;
	}
	else{
		// millis() overflow has occured
		timeSinceLastOn = (currentTime + 1440000) - (lastIdleTime +1440000); // add a day to both for calculation
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
}

void TempControl::loadDefaultSettings(void){
	cs.mode = MODE_BEER_CONSTANT;
	cs.beerSetting = 20<<9;;
	cs.fridgeSetting = 20<<9;
	cs.heatEstimator=16; // 0.2*2^9
	cs.coolEstimator=5<<9;
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
	cc.tempSettingMin = 4*512;	// + 4 deg Celsius

	// control defines, also in fixed point format (7 int bits, 9 frac bits), so multiplied by 2^9=512
	cc.KpHeat	= 7680;		// +15
	cc.KpCool	= 7680;		// +15
	cc.Ki		= 154;		// +0.3
	cc.KdCool	= -1536;	// -3
	cc.KdHeat	= -1536;	// -3
	cc.iMaxSlope = 26;		// 0.05 deg/hour
	cc.iMinSlope = -51;		// 0.1 deg/hour
	cc.iMaxError = 256;  // 0.5 deg

	// Stay Idle when temperature is in this range
	cc.idleRangeHigh = 256;	// +0.5 deg Celsius
	cc.idleRangeLow = -256;	// -0.5 deg Celsius

	// when peak falls between these limits, its good.
	cc.heatingTargetUpper = 102;	// +0.2 deg Celsius
	cc.heatingTargetLower = -51;	// -0.1 deg Celsius
	cc.coolingTargetUpper = 51;		// +0.1 deg Celsius
	cc.coolingTargetLower = -102;	// -0.2 deg Celsius

	cc.fridgeFastFilter = SETTLING_TIME_25_SAMPLES;
	fridgeSensor.setFastFilterCoefficients(cc.fridgeFastFilter);
	cc.fridgeSlowFilter = SETTLING_TIME_200_SAMPLES;
	fridgeSensor.setSlowFilterCoefficients(cc.fridgeSlowFilter);
	cc.fridgeSlopeFilter = SETTLING_TIME_100_SAMPLES;
	fridgeSensor.setSlopeFilterCoefficients(cc.fridgeSlopeFilter);
	cc.beerFastFilter = SETTLING_TIME_50_SAMPLES;
	beerSensor.setFastFilterCoefficients(cc.beerFastFilter);
	cc.beerSlowFilter = SETTLING_TIME_400_SAMPLES;
	beerSensor.setSlowFilterCoefficients(cc.beerSlowFilter);
	cc.beerSlopeFilter = SETTLING_TIME_50_SAMPLES;
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
	cs.mode = newMode;
	if(newMode==MODE_BEER_PROFILE || MODE_OFF){
		// set temperatures to undefined until temperatures have been received from RPi
		cs.beerSetting = INT_MIN;
		cs.fridgeSetting = INT_MIN;
	}
	storeSettings();
}

char TempControl::getMode(void){
	return cs.mode;
}

void TempControl::setState(unsigned char newState){
	state = newState;		
}

unsigned char TempControl::getState(void){
	return state;
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
	if(abs(storedBeerSetting = newTemp) > 128){ // more than half a degree C difference with EEPROM
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