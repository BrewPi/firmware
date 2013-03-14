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

#include "PiLink.h"


#include "tempControl.h"
#include "Display.h"
#include <stdarg.h>
#include <avr/pgmspace.h>
#include <limits.h>
#include <string.h>
#include "jsonKeys.h"

// create a printf like interface to the Arduino Serial function. Format string stored in PROGMEM
void PiLink::print_P(const char *fmt, ... ){
	char tmp[128]; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt );
	vsnprintf_P(tmp, 128, fmt, args);
	va_end (args);
	Serial.print(tmp);
}

// create a printf like interface to the Arduino Serial function. Format string stored in RAM
void PiLink::print(char *fmt, ... ){
	char tmp[128]; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt );
	vsnprintf(tmp, 128, fmt, args);
	va_end (args);
	Serial.print(tmp);
}

void PiLink::receive(void){
	if (Serial.available() > 0){
		char inByte = Serial.read();
		switch(inByte){
		case 't': // temperatures requested
			printTemperatures();      
			break;
		case 'C': // Set default constants
			tempControl.loadDefaultConstants();
			display.printStationaryText(); // reprint stationary text to update to right degree unit
			sendControlConstants(); // update script with new settings
			debugMessage(PSTR("Default constants loaded."));
			break;
		case 'S': // Set default settings
			tempControl.loadDefaultSettings();
			sendControlSettings(); // update script with new settings
			debugMessage(PSTR("Default settings loaded."));
			break;
		case 's': // Control settings requested
			sendControlSettings();
			break;
		case 'c': // Control constants requested
			sendControlConstants();
			break;
		case 'v': // Control variables requested
			sendControlVariables();
			break;	
		case 'l': // Display content requested
			print_P(PSTR("L:"));
			char stringBuffer[21];
			for(uint8_t i=0;i<4;i++){
				display.lcd.getLine(i, stringBuffer);
				print_P(PSTR("%s<BR>"), stringBuffer);
			}				
			print_P(PSTR("\n"));
			break;
		case 'j': // Receive settings as json
			receiveJson();
			break;
		default:
			debugMessage(PSTR("Invalid command received by Arduino: %c"), inByte);
		}
		//Serial.flush(); Messages can be back to back. Flush should not be necessary.
		// Functions should not read more than what is meant for that function.
	}
}

void PiLink::printTemperaturesJSON(char * beerAnnotation, char * fridgeAnnotation){
	char tempString[9];
	print_P(PSTR("T:{\"BeerTemp\":%s,"), tempToString(tempString, tempControl.getBeerTemp(), 2, 9));
	print_P(PSTR("\"BeerSet\":%s,"), tempToString(tempString, tempControl.getBeerSetting(), 2, 9));
	print_P(PSTR("\"BeerAnn\":"));
	if(beerAnnotation == 0){
		print_P(PSTR("null,"));
	}
	else{
		print_P(PSTR("\"%s\","), beerAnnotation);
	}
	print_P(PSTR("\"FridgeTemp\":%s,"), tempToString(tempString, tempControl.getFridgeTemp(), 2, 9));
	print_P(PSTR("\"FridgeSet\":%s,"), tempToString(tempString, tempControl.getFridgeSetting(), 2, 9));
	print_P(PSTR("\"FridgeAnn\":"));
	if(fridgeAnnotation == 0){
		print_P(PSTR("null}\n"));
	}
	else{
		print_P(PSTR("\"%s\"}\n"), fridgeAnnotation);	
	}
}

void PiLink::printTemperatures(void){
	// print all temperatures with empty annotations
	printTemperaturesJSON(0, 0);
}

void PiLink::printBeerAnnotation(const char * annotation, ...){
	char tempString[128]; // resulting string limited to 128 chars
	va_list args;	
	// Using print_P for the Annotation fails. Arguments are not passed correctly. Use Serial directly as a work around.
	va_start (args, annotation );
	vsnprintf_P(tempString, 128, annotation, args);
	va_end (args);
	printTemperaturesJSON(tempString, 0);
	va_end (args);
}

void PiLink::printFridgeAnnotation(const char * annotation, ...){
	char tempString[128]; // resulting string limited to 128 chars
	va_list args;	
	// Using print_P for the Annotation fails. Arguments are not passed correctly. Use Serial directly as a work around.
	va_start (args, annotation );
	vsnprintf_P(tempString, 128, annotation, args);
	va_end (args);
	printTemperaturesJSON(0, tempString);
	va_end (args);
}	  
 	  
void PiLink::debugMessage(const char * message, ...){
	char tempString[128]; // resulting string limited to 128 chars
	va_list args;
	
	//print 'D:' as prefix
	print_P(PSTR("D:"));
	
	// Using print_P for the Annotation fails. Arguments are not passed correctly. Use Serial directly as a work around.
	va_start (args, message );
	vsnprintf_P(tempString, 128, message, args);
	va_end (args);
	Serial.print(tempString);

	print_P(PSTR("\n")); // print newline
}

// Send settings as JSON string
void PiLink::sendControlSettings(void){
	char tempString[12];
	print_P(PSTR("S:{"));
	sendJsonPair(jsonKeys.mode, tempControl.cs.mode);
	sendJsonPair(jsonKeys.beerSetting, tempToString(tempString, tempControl.cs.beerSetting, 2, 12));
	sendJsonPair(jsonKeys.fridgeSetting, tempToString(tempString, tempControl.cs.fridgeSetting, 2, 12));
	sendJsonPair(jsonKeys.heatEstimator, fixedPointToString(tempString, tempControl.cs.heatEstimator, 3, 12));
	// last one 'manually' to have no trailing comma
	print_P(PSTR("\"%s\":%s}\n"), jsonKeys.coolEstimator, fixedPointToString(tempString, tempControl.cs.coolEstimator, 3, 12));
}

// Send control constants as JSON string. Might contain spaces between minus sign and number. Python will have to strip these
void PiLink::sendControlConstants(void){
	char tempString[12];
	print_P(PSTR("C:{"));
	sendJsonPair(jsonKeys.tempFormat, tempControl.cc.tempFormat);
	sendJsonPair(jsonKeys.tempSettingMin, tempToString(tempString, tempControl.cc.tempSettingMin, 1, 12));
	sendJsonPair(jsonKeys.tempSettingMax, tempToString(tempString, tempControl.cc.tempSettingMax, 1, 12));
	sendJsonPair(jsonKeys.KpHeat, fixedPointToString(tempString, tempControl.cc.KpHeat, 3, 12));
	sendJsonPair(jsonKeys.KpCool, fixedPointToString(tempString, tempControl.cc.KpCool, 3, 12));
	sendJsonPair(jsonKeys.Ki, fixedPointToString(tempString, tempControl.cc.Ki, 3, 12));
	sendJsonPair(jsonKeys.KdCool, fixedPointToString(tempString, tempControl.cc.KdCool, 3, 12));
	sendJsonPair(jsonKeys.KdHeat, fixedPointToString(tempString, tempControl.cc.KdHeat, 3, 12));
	sendJsonPair(jsonKeys.iMaxError, tempDiffToString(tempString, tempControl.cc.iMaxError, 3, 12));
	sendJsonPair(jsonKeys.iMaxSlope, tempDiffToString(tempString, tempControl.cc.iMaxSlope, 3, 12));
	sendJsonPair(jsonKeys.iMinSlope, tempDiffToString(tempString, tempControl.cc.iMinSlope, 3, 12));
	sendJsonPair(jsonKeys.idleRangeHigh, tempDiffToString(tempString, tempControl.cc.idleRangeHigh, 3, 12));
	sendJsonPair(jsonKeys.idleRangeLow, tempDiffToString(tempString, tempControl.cc.idleRangeLow, 3, 12));
	sendJsonPair(jsonKeys.heatingTargetUpper, tempDiffToString(tempString, tempControl.cc.heatingTargetUpper, 3, 12));
	sendJsonPair(jsonKeys.heatingTargetLower, tempDiffToString(tempString, tempControl.cc.heatingTargetLower, 3, 12));
	sendJsonPair(jsonKeys.coolingTargetUpper, tempDiffToString(tempString, tempControl.cc.coolingTargetUpper, 3, 12));
	sendJsonPair(jsonKeys.coolingTargetLower, tempDiffToString(tempString, tempControl.cc.coolingTargetLower, 3, 12));
	sendJsonPair(jsonKeys.maxHeatTimeForEstimate, tempControl.cc.maxHeatTimeForEstimate);
	sendJsonPair(jsonKeys.maxCoolTimeForEstimate, tempControl.cc.maxCoolTimeForEstimate);

	sendJsonPair(jsonKeys.fridgeFastFilter, tempControl.cc.fridgeFastFilter);
	sendJsonPair(jsonKeys.fridgeSlowFilter, tempControl.cc.fridgeSlowFilter);
	sendJsonPair(jsonKeys.fridgeSlopeFilter, tempControl.cc.fridgeSlopeFilter);
	sendJsonPair(jsonKeys.beerFastFilter, tempControl.cc.beerFastFilter);
	sendJsonPair(jsonKeys.beerSlowFilter, tempControl.cc.beerSlowFilter);
	// last one 'manually' to have no trailing comma
	print_P(PSTR("\"%s\":%u}\n"), jsonKeys.beerSlopeFilter, tempControl.cc.beerSlopeFilter);
}

// Send all control variables. Useful for debugging and choosing parameters
void PiLink::sendControlVariables(void){
	char tempString[12];
	print_P(PSTR("V:{"));
	sendJsonPair(jsonKeys.beerDiff, tempDiffToString(tempString, tempControl.cv.beerDiff, 3, 12));
	sendJsonPair(jsonKeys.diffIntegral, tempDiffToString(tempString, tempControl.cv.diffIntegral, 3, 12));
	sendJsonPair(jsonKeys.beerSlope, tempDiffToString(tempString, tempControl.cv.beerSlope, 3, 12));
	sendJsonPair(jsonKeys.p, fixedPointToString(tempString, tempControl.cv.p, 3, 12));
	sendJsonPair(jsonKeys.i, fixedPointToString(tempString, tempControl.cv.i, 3, 12));
	sendJsonPair(jsonKeys.d, fixedPointToString(tempString, tempControl.cv.d, 3, 12));
	sendJsonPair(jsonKeys.Kp, fixedPointToString(tempString, tempControl.cv.Kp, 3, 12));
	sendJsonPair(jsonKeys.Kd, fixedPointToString(tempString, tempControl.cv.Kd, 3, 12));
	sendJsonPair(jsonKeys.estimatedPeak, tempToString(tempString, tempControl.cv.estimatedPeak, 3, 12));
	sendJsonPair(jsonKeys.negPeakSetting, tempToString(tempString, tempControl.cv.negPeakSetting, 3, 12));
	sendJsonPair(jsonKeys.posPeakSetting, tempToString(tempString, tempControl.cv.posPeakSetting, 3, 12));
	sendJsonPair(jsonKeys.negPeak, tempToString(tempString, tempControl.cv.negPeak, 3, 12));
	print_P(PSTR("\"%s\":%s}\n"), jsonKeys.posPeak, tempToString(tempString, tempControl.cv.posPeak, 3, 12));
}

void PiLink::sendJsonPair(const char * name, char * val){
	print_P(PSTR("\"%s\":%s,"), name, val);	
}

void PiLink::sendJsonPair(const char * name, char val){
	print_P(PSTR("\"%s\":\"%c\","), name, val);
}

void PiLink::sendJsonPair(const char * name, uint16_t val){
	print_P(PSTR("\"%s\":%u,"), name, val);
}

void PiLink::sendJsonPair(const char * name, uint8_t val){
	print_P(PSTR("\"%s\":%u,"), name, val);
}

void PiLink::receiveJson(void){
	char key[30];
	char val[30];
	uint8_t index=0;
	char character=0;
	delay(1);
	while(Serial.available() > 0){ // outer while loop can process multiple pairs
		index=0;
		while(Serial.available() > 0) // get key
		{
			delay(1);
			character = Serial.read();
			if(character == ':'){		
				// value comes now
				break;
			}
			else if(character == ' ' || character == '{' || character == '"'){
				;
			}
			else{
				key[index++] = character;
			}
			if(index>=29)
			{
				return; // value was too long, don't process anything
			}
		}
		key[index]=0; // null terminate string
		index = 0;
		while(Serial.available() > 0) // get value
		{
			delay(1);
			character = Serial.read();
			if(character == ',' || character == '}'){
				// end of value
				break;
			}
			else if(character == ' ' || character == '"'){
				; // skip spaces and apostrophes
			}
			else{
				val[index++] = character;
			}
			if(index>=29)
			{
				return; // value was too long, don't process anything
			}
		}
		val[index]=0; // null terminate string
		processJsonPair(key,val);
		
		if(character == '}'){
			// this was the last pair.
			tempControl.storeSettings(); // store new settings to EEPROM
			tempControl.storeConstants();
			sendControlSettings(); // update script with new settings
			sendControlConstants();
			return;
		}
	}
}

void PiLink::processJsonPair(char * key, char * val){
	debugMessage(PSTR("Received new setting: %s = %s"), key, val);
	if(strcmp(key,jsonKeys.mode) == 0){
		tempControl.setMode(val[0]);
		piLink.printFridgeAnnotation(PSTR("Mode set to %c in web interface"), val[0]);
	}
	else if(strcmp(key,jsonKeys.beerSetting) == 0){ 
		fixed7_9 newTemp = stringToTemp(val);
		if(tempControl.cs.mode == 'p'){
			if(abs(newTemp-tempControl.cs.beerSetting) > 100){ // this excludes gradual updates under 0.2 degrees
				printBeerAnnotation(PSTR("Beer temp set to %s by temperature profile."), val);
			}
		}
		else{
			printBeerAnnotation(PSTR("Beer temp set to %s in web interface."), val);
		}
		tempControl.cs.beerSetting = newTemp;
	}
	else if(strcmp(key,jsonKeys.fridgeSetting) == 0){
		fixed7_9 newTemp = stringToTemp(val);
		if(tempControl.cs.mode == 'f'){
			printFridgeAnnotation(PSTR("Fridge temp set to %s in web interface."), val);
		}
		tempControl.cs.fridgeSetting = newTemp;
	}
	else if(strcmp(key,jsonKeys.heatEstimator) == 0){ tempControl.cs.heatEstimator = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.coolEstimator) == 0){ tempControl.cs.coolEstimator = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.tempFormat) == 0){
		tempControl.cc.tempFormat = val[0];
		display.printStationaryText(); // reprint stationary text to update to right degree unit
	}
	else if(strcmp(key,jsonKeys.tempSettingMin) == 0){ tempControl.cc.tempSettingMin = stringToTemp(val); }
	else if(strcmp(key,jsonKeys.tempSettingMax) == 0){ tempControl.cc.tempSettingMax = stringToTemp(val); }
	else if(strcmp(key,jsonKeys.KpHeat) == 0){ tempControl.cc.KpHeat = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.KpCool) == 0){ tempControl.cc.KpCool = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.Ki) == 0){ tempControl.cc.Ki = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.KdCool) == 0){ tempControl.cc.KdCool = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.KdHeat) == 0){ tempControl.cc.KdHeat = stringToFixedPoint(val); }
	else if(strcmp(key,jsonKeys.iMaxError) == 0){ tempControl.cc.iMaxError = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.iMaxSlope) == 0){ tempControl.cc.iMaxSlope = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.iMinSlope) == 0){ tempControl.cc.iMinSlope = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.idleRangeHigh) == 0){ tempControl.cc.idleRangeHigh = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.idleRangeLow) == 0){ tempControl.cc.idleRangeLow = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.heatingTargetUpper) == 0){ tempControl.cc.heatingTargetUpper = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.heatingTargetLower) == 0){ tempControl.cc.heatingTargetLower = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.coolingTargetUpper) == 0){ tempControl.cc.coolingTargetUpper = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.coolingTargetLower) == 0){ tempControl.cc.coolingTargetLower = stringToTempDiff(val); }
	else if(strcmp(key,jsonKeys.maxHeatTimeForEstimate) == 0){ tempControl.cc.maxHeatTimeForEstimate = strtoul(val, NULL, 10); }
	else if(strcmp(key,jsonKeys.maxCoolTimeForEstimate) == 0){ tempControl.cc.maxCoolTimeForEstimate = strtoul(val, NULL, 10); }
	else if(strcmp(key,jsonKeys.maxCoolTimeForEstimate) == 0){ tempControl.cc.maxCoolTimeForEstimate = strtoul(val, NULL, 10); }
		
	// Receive the b value for the filter
	else if(strcmp(key,jsonKeys.fridgeFastFilter) == 0){ 
		tempControl.cc.fridgeFastFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor.setFastFilterCoefficients(tempControl.cc.fridgeFastFilter);
	}
	else if(strcmp(key,jsonKeys.fridgeSlowFilter) == 0){
		tempControl.cc.fridgeSlowFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor.setSlowFilterCoefficients(tempControl.cc.fridgeSlowFilter);
	}
	else if(strcmp(key,jsonKeys.fridgeSlopeFilter) == 0){
		tempControl.cc.fridgeSlopeFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor.setSlopeFilterCoefficients(tempControl.cc.fridgeSlopeFilter);
	}
	else if(strcmp(key,jsonKeys.beerFastFilter) == 0){
		tempControl.cc.beerFastFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor.setFastFilterCoefficients(tempControl.cc.beerFastFilter);
	
	}
	else if(strcmp(key,jsonKeys.beerSlowFilter) == 0){
		tempControl.cc.beerSlowFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor.setSlowFilterCoefficients(tempControl.cc.beerSlowFilter);
	}
	else if(strcmp(key,jsonKeys.beerSlopeFilter) == 0){
		tempControl.cc.beerSlopeFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor.setSlopeFilterCoefficients(tempControl.cc.beerSlopeFilter);
	}
	else{
		debugMessage(PSTR("Could not process setting"));
	}
}