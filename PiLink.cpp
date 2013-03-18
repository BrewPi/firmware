/*
 * Copyright 2012 BrewPi/Elco Jacobs.
 * Various changes Copyright 2013 Matthew McGowan.
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
#include "stddef.h"
#include "PiLink.h"


#include "tempControl.h"
#include "Display.h"
#include <stdarg.h>
#include <avr/pgmspace.h>
#include <limits.h>
#include <string.h>
#include "jsonKeys.h"
#include "chamber.h"
#include "Ticks.h"
#include "brewpi_avr.h"

bool PiLink::firstPair = false;

#define strcmp_json strcmp_P
// settings


const PROGMEM char jsonKeys_mode[] = {"mode"};
#define FIRST_JSON_KEY jsonKeys_mode
const PROGMEM char jsonKeys_beerSetting [] = {"beerSetting"};
const PROGMEM char jsonKeys_fridgeSetting [] = {"fridgeSetting"};
const PROGMEM char jsonKeys_heatEstimator [] = {"heatEstimator"};
const PROGMEM char jsonKeys_coolEstimator [] = {"coolEstimator"};
	// constants
const PROGMEM char jsonKeys_tempFormat [] = {"tempFormat"};
const PROGMEM char jsonKeys_tempSettingMin [] = {"tempSettingMin"};
const PROGMEM char jsonKeys_tempSettingMax [] = {"tempSettingMax"};
const PROGMEM char jsonKeys_pidMax [] = {"pidMax"};
const PROGMEM char jsonKeys_KpHeat [] = {"KpHeat"};
const PROGMEM char jsonKeys_KpCool [] = {"KpCool"};
const PROGMEM char jsonKeys_Ki [] = {"Ki"};
const PROGMEM char jsonKeys_KdCool  [] = {"KdCool"};
const PROGMEM char jsonKeys_KdHeat [] = {"KdHeat"};
const PROGMEM char jsonKeys_iMaxSlope  [] = {"iMaxSlope"};
const PROGMEM char jsonKeys_iMinSlope  [] = {"iMinSlope"};
const PROGMEM char jsonKeys_iMaxError  [] = {"iMaxError"};
const PROGMEM char jsonKeys_idleRangeHigh [] = {"idleRangeHigh"};
const PROGMEM char jsonKeys_idleRangeLow  [] = {"idleRangeLow"};
const PROGMEM char jsonKeys_heatingTargetUpper  [] = {"heatingTargetUpper"};
const PROGMEM char jsonKeys_heatingTargetLower [] = {"heatingTargetLower"};
const PROGMEM char jsonKeys_coolingTargetUpper  [] = {"coolingTargetUpper"};
const PROGMEM char jsonKeys_coolingTargetLower [] = {"coolingTargetLower"};
const PROGMEM char jsonKeys_maxHeatTimeForEstimate  [] = {"maxHeatTimeForEstimate"};
const PROGMEM char jsonKeys_maxCoolTimeForEstimate  [] = {"maxCoolTimeForEstimate"};
const PROGMEM char jsonKeys_pidFilter  [] = {"pidFilter"};
const PROGMEM char jsonKeys_fridgeFastFilter  [] = {"fridgeFastFilter"};
const PROGMEM char jsonKeys_fridgeSlowFilter [] = {"fridgeSlowFilter"};
const PROGMEM char jsonKeys_fridgeSlopeFilter [] = {"fridgeSlopeFilter"};
const PROGMEM char jsonKeys_beerFastFilter [] = {"beerFastFilter"};
const PROGMEM char jsonKeys_beerSlowFilter [] = {"beerSlowFilter"};
const PROGMEM char jsonKeys_beerSlopeFilter [] = {"beerSlopeFilter"};
// variables
const PROGMEM char jsonKeys_beerDiff  [] = {"beerDiff"};
const PROGMEM char jsonKeys_diffIntegral  [] = {"diffIntegral"};
const PROGMEM char jsonKeys_beerSlope  [] = {"beerSlope"};
const PROGMEM char jsonKeys_p  [] = {"p"};
const PROGMEM char jsonKeys_i  [] = {"i"};
const PROGMEM char jsonKeys_d  [] = {"d"};
const PROGMEM char jsonKeys_Kp  [] = {"Kp"};
const PROGMEM char jsonKeys_Kd  [] = {"Kd"};
const PROGMEM char jsonKeys_estimatedPeak  [] = {"estimatedPeak"};
const PROGMEM char jsonKeys_negPeakSetting  [] = {"negPeakSetting"};
const PROGMEM char jsonKeys_posPeakSetting  [] = {"posPeakSetting"};
const PROGMEM char jsonKeys_negPeak  [] = {"negPeak"};
const PROGMEM char jsonKeys_posPeak  [] = {"posPeak"};

class MockSerial
{
public:
	void print(char c) {}
	void print(const char* c) {}
	char read() { return '\0'; }
	uint8_t available() { return 0; }
};

#if BREWPI_EMULATE	
	static MockSerial mockSerial;
	#define piStream mockSerial
#else
	#define piStream Serial
#endif

// create a printf like interface to the Arduino Serial function. Format string stored in PROGMEM
void PiLink::print_P(const char *fmt, ... ){
	char tmp[128]; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt );
	vsnprintf_P(tmp, 128, fmt, args);
	va_end (args);
	piStream.print(tmp);
}

// create a printf like interface to the Arduino Serial function. Format string stored in RAM
void PiLink::print(char *fmt, ... ){
	char tmp[128]; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt );
	vsnprintf(tmp, 128, fmt, args);
	va_end (args);
	piStream.print(tmp);
}

void PiLink::receive(void){
	if (piStream.available() > 1){
		
		char inByte = piStream.read();
#if MULTICHAMBER		
		char arg = piStream.read();
		if (arg=='\n') {}
			switch (inByte) {
				case 'x': // request chamber info
				printChamberInfo();
				return;
		}
		
		chamber_id chamber = arg-'1';		
		chamber_id prev = chamberManager.currentChamber();
		if (chamber>=0 && chamber<=9)
			prev = chamberManager.switchChamber(chamber);
#else				
		if (inByte=='\n')		// allow newlines between commands
			return;			
#endif
			
		switch(inByte){
#if MULTICHAMBER
		case 'X': // switch chamber
			prev = chamber;		// don't switch back to the previous chamber
			printChamberInfo();
			break;
#endif			
					
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
			printChamberResponse('L');

#if MULTICHAMBER			
			display.setBufferOnly(true);
			display.printAll();
#endif			
						
			char stringBuffer[21];
			for(uint8_t i=0;i<4;i++){
				display.getLine(i, stringBuffer);
				print_P(PSTR("%s<BR>"), stringBuffer);
			}							
#if MULTICHAMBER		
			display.setBufferOnly(false);
#endif			
			print_P(PSTR("\n"));
			break;
		case 'j': // Receive settings as json
			receiveJson();
			break;
		default:
			debugMessage(PSTR("Invalid command received by Arduino: %c"), inByte);
		}

#if MULTICHAMBER
		chamberManager.switchChamber(prev);		
#endif		
		//Serial.flush(); Messages can be back to back. Flush should not be necessary.
		// Functions should not read more than what is meant for that function.
	}
}

#if MULTICHAMBER
void PiLink::printChamberInfo()
{
	printChamberResponse('X');
	print('{');
	sendJsonPair(PSTR("chambers"), chamberManager.chamberCount());
	sendJsonPair(PSTR("current"), chamberManager.currentChamber());
	print('}');
	print('\n');
}
#endif

void PiLink::printChamberResponse(char responseChar)
{
	print(responseChar);
#if MULTICHAMBER	
	print(chamberManager.currentChamber()+'1');	
#endif	
	print(':');
}

void PiLink::printTemperaturesJSON(char * beerAnnotation, char * fridgeAnnotation){
	char tempString[9];
	printChamberResponse('T');
	print_P(PSTR("{\"BeerTemp\":%s,"), tempToString(tempString, tempControl.getBeerTemp(), 2, 9));
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
	printChamberResponse('D');
	
	// Using print_P for the Annotation fails. Arguments are not passed correctly. Use Serial directly as a work around.
	va_start (args, message );
	vsnprintf_P(tempString, 128, message, args);
	va_end (args);
	piStream.print(tempString);

	print_P(PSTR("\n")); // print newline
}

void PiLink::debugMessageDirect(const char * message, ...){
	char tempString[128]; // resulting string limited to 128 chars
	va_list args;
	// Using print_P for the Annotation fails. Arguments are not passed correctly. Use  directly as a work around.
	va_start (args, message );
	vsnprintf_P(tempString, 128, message, args);
	va_end (args);
	piStream.print(tempString);

	print('\n'); // print newline
}

void PiLink::sendJsonClose() {
	piStream.print('}');
}

// Send settings as JSON string
void PiLink::sendControlSettings(void){
	char tempString[12];
	printChamberResponse('S');
	ControlSettings& cs = tempControl.cs;
	firstPair = true;
	sendJsonPair(jsonKeys_mode, tempControl.cs.mode);
	sendJsonPair(jsonKeys_beerSetting, tempToString(tempString, cs.beerSetting, 2, 12));
	sendJsonPair(jsonKeys_fridgeSetting, tempToString(tempString, cs.fridgeSetting, 2, 12));
	sendJsonPair(jsonKeys_heatEstimator, fixedPointToString(tempString, cs.heatEstimator, 3, 12));	
	sendJsonPair(jsonKeys_coolEstimator, fixedPointToString(tempString, cs.coolEstimator, 3, 12));		
	sendJsonClose();
}

// Send control constants as JSON string. Might contain spaces between minus sign and number. Python will have to strip these
void PiLink::sendControlConstants(void){
	char tempString[12];
	printChamberResponse('C');
	firstPair = true;	
	sendJsonPair(jsonKeys_tempFormat, tempControl.cc.tempFormat);
	sendJsonPair(jsonKeys_tempSettingMin, tempToString(tempString, tempControl.cc.tempSettingMin, 1, 12));
	sendJsonPair(jsonKeys_tempSettingMax, tempToString(tempString, tempControl.cc.tempSettingMax, 1, 12));
	sendJsonPair(jsonKeys_KpHeat, fixedPointToString(tempString, tempControl.cc.KpHeat, 3, 12));
	sendJsonPair(jsonKeys_KpCool, fixedPointToString(tempString, tempControl.cc.KpCool, 3, 12));
	sendJsonPair(jsonKeys_Ki, fixedPointToString(tempString, tempControl.cc.Ki, 3, 12));
	sendJsonPair(jsonKeys_KdCool, fixedPointToString(tempString, tempControl.cc.KdCool, 3, 12));
	sendJsonPair(jsonKeys_KdHeat, fixedPointToString(tempString, tempControl.cc.KdHeat, 3, 12));
	sendJsonPair(jsonKeys_iMaxError, tempDiffToString(tempString, tempControl.cc.iMaxError, 3, 12));
	sendJsonPair(jsonKeys_iMaxSlope, tempDiffToString(tempString, tempControl.cc.iMaxSlope, 3, 12));
	sendJsonPair(jsonKeys_iMinSlope, tempDiffToString(tempString, tempControl.cc.iMinSlope, 3, 12));
	sendJsonPair(jsonKeys_idleRangeHigh, tempDiffToString(tempString, tempControl.cc.idleRangeHigh, 3, 12));
	sendJsonPair(jsonKeys_idleRangeLow, tempDiffToString(tempString, tempControl.cc.idleRangeLow, 3, 12));
	sendJsonPair(jsonKeys_heatingTargetUpper, tempDiffToString(tempString, tempControl.cc.heatingTargetUpper, 3, 12));
	sendJsonPair(jsonKeys_heatingTargetLower, tempDiffToString(tempString, tempControl.cc.heatingTargetLower, 3, 12));
	sendJsonPair(jsonKeys_coolingTargetUpper, tempDiffToString(tempString, tempControl.cc.coolingTargetUpper, 3, 12));
	sendJsonPair(jsonKeys_coolingTargetLower, tempDiffToString(tempString, tempControl.cc.coolingTargetLower, 3, 12));
	sendJsonPair(jsonKeys_maxHeatTimeForEstimate, tempControl.cc.maxHeatTimeForEstimate);
	sendJsonPair(jsonKeys_maxCoolTimeForEstimate, tempControl.cc.maxCoolTimeForEstimate);

	sendJsonPair(jsonKeys_fridgeFastFilter, tempControl.cc.fridgeFastFilter);
	sendJsonPair(jsonKeys_fridgeSlowFilter, tempControl.cc.fridgeSlowFilter);
	sendJsonPair(jsonKeys_fridgeSlopeFilter, tempControl.cc.fridgeSlopeFilter);
	sendJsonPair(jsonKeys_beerFastFilter, tempControl.cc.beerFastFilter);
	sendJsonPair(jsonKeys_beerSlowFilter, tempControl.cc.beerSlowFilter);	
	sendJsonPair(jsonKeys_beerSlopeFilter, tempControl.cc.beerSlopeFilter);		
	sendJsonClose();
}

struct PairOffset {
	const char* key; uint8_t valueOffset;
};

// todo - add compiletime check that offset is <256

#define PAIR_OFFSET_CV_7_9(name) { &jsonKeys_ ## name [0],  offsetof(ControlVariables, name) }

static const PairOffset offsets_cv_7_9 [] = {
	PAIR_OFFSET_CV_7_9(Kp),
	PAIR_OFFSET_CV_7_9(Kd),
	PAIR_OFFSET_CV_7_9(estimatedPeak),
	PAIR_OFFSET_CV_7_9(negPeakSetting),
	PAIR_OFFSET_CV_7_9(posPeakSetting),
	PAIR_OFFSET_CV_7_9(posPeakSetting),
	PAIR_OFFSET_CV_7_9(negPeak),
	PAIR_OFFSET_CV_7_9(posPeak),
};

// Send all control variables. Useful for debugging and choosing parameters
void PiLink::sendControlVariables(void){
	char tempString[12];
	printChamberResponse('V');
	firstPair = true;	
	ControlVariables& cv = tempControl.cv;
	sendJsonPair(jsonKeys_beerDiff, tempDiffToString(tempString, cv.beerDiff, 3, 12));
	sendJsonPair(jsonKeys_diffIntegral, tempDiffToString(tempString, cv.diffIntegral, 3, 12));
	sendJsonPair(jsonKeys_beerSlope, tempDiffToString(tempString, cv.beerSlope, 3, 12));

	sendJsonPair(jsonKeys_p, fixedPointToString(tempString, cv.p, 3, 12));
	sendJsonPair(jsonKeys_i, fixedPointToString(tempString, cv.i, 3, 12));
	sendJsonPair(jsonKeys_d, fixedPointToString(tempString, cv.d, 3, 12));
		
	for (uint8_t i=0; i<sizeof(offsets_cv_7_9)/sizeof(offsets_cv_7_9[0]); i++) {
		sendJsonPair(offsets_cv_7_9[i].key, 
			tempToString(tempString, *(fixed7_9*)(((void*)&cv)+offsets_cv_7_9[i].valueOffset), 3, 12));			
	}

	sendJsonClose();
}

void PiLink::printJsonName(const char * name)
{
	piStream.print('"');
	print_P(name);
	piStream.print('"');
	piStream.print(':');		
}

void PiLink::printJsonSeparator() {
	if (!firstPair)
		piStream.print(',');
	else {
		piStream.print('{');
	}
}

void PiLink::sendJsonPair(const char * name, const char * val){
	printJsonSeparator();
	printJsonName(name);
	piStream.print(val);
}

void PiLink::sendJsonPair(const char * name, char val){
	printJsonName(name);
	piStream.print('"');
	piStream.print(val);
	piStream.print('"');	
	printJsonSeparator();
}

void PiLink::sendJsonPair(const char * name, uint16_t val){
	printJsonName(name);
	print_P(PSTR("\"%u\""), val);
	printJsonSeparator();
}

void PiLink::sendJsonPair(const char * name, uint8_t val) {
	sendJsonPair(name, (uint16_t)val);
}

void PiLink::receiveJson(void){
	char key[30];
	char val[30];
	uint8_t index=0;
	char character=0;
	wait.millis(1);
	while(piStream.available() > 0){ // outer while loop can process multiple pairs
		index=0;
		while(piStream.available() > 0) // get key
		{
			wait.millis(1);
			character = piStream.read();
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
		while(piStream.available() > 0) // get value
		{
			wait.millis(1);
			character = piStream.read();
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

struct JsonConvert {
	const char* key; 
	uint8_t offset; 
	uint8_t fnType;
	};

#define stringToTempVal 1
#define stringToFixedPointVal 2
#define stringToTempDiffVal 3
#define stringToLongVal 4

#define JSON_CONVERT(name, fn) { &jsonKeys_ ## name[0], offsetof(ControlConstants, name), fn }

static JsonConvert jsonConverters[] = {
	JSON_CONVERT(tempSettingMin, stringToTempVal),
	JSON_CONVERT(tempSettingMax, stringToTempVal),
	JSON_CONVERT(KpHeat, stringToFixedPointVal),
	JSON_CONVERT(KpCool, stringToFixedPointVal),
	JSON_CONVERT(Ki, stringToFixedPointVal),
	JSON_CONVERT(KdCool, stringToFixedPointVal),
	JSON_CONVERT(KdHeat, stringToFixedPointVal),
	JSON_CONVERT(iMaxError, stringToTempDiffVal),
	JSON_CONVERT(iMaxSlope, stringToTempDiffVal),
	JSON_CONVERT(iMinSlope, stringToTempDiffVal),
	JSON_CONVERT(idleRangeHigh, stringToTempDiffVal),
	JSON_CONVERT(idleRangeLow, stringToTempDiffVal),
	JSON_CONVERT(heatingTargetUpper, stringToTempDiffVal),
	JSON_CONVERT(heatingTargetLower, stringToTempDiffVal),
	JSON_CONVERT(coolingTargetUpper, stringToTempDiffVal),
	JSON_CONVERT(coolingTargetLower, stringToTempDiffVal),
	JSON_CONVERT(maxHeatTimeForEstimate, stringToLongVal),
	JSON_CONVERT(maxCoolTimeForEstimate, stringToLongVal),
};	

void PiLink::processJsonPair(char * key, char * val){

	debugMessage(PSTR("Received new setting: %s = %s"), key, val);
	const char* msg = ("%s temp setting changed to %s %s.");
	const char* webui = "in web interface";	
	
	// saves a ca 300 hundred bytes by taking these references up front when temp control is non-static
	// static, the compiler optimizes by linking the address directly, so this has no affect.
	ControlConstants& cc = tempControl.cc;
	ControlSettings& cs = tempControl.cs;
		
	for (uint8_t i=0; i<sizeof(jsonConverters)/sizeof(jsonConverters[0]); i++) {
		if (strcmp_json(key, jsonConverters[i].key)==0) 
			switch (jsonConverters[i].fnType) {
				case stringToTempVal:
					*(fixed7_9*)(((void*)&cc)+jsonConverters[i].offset) = stringToTemp(val);
					return;
				case stringToFixedPointVal:
					*(fixed23_9*)(((void*)&cc)+jsonConverters[i].offset) = stringToFixedPoint(val);
					return;			
				case stringToTempDiffVal:
					*(fixed7_9*)(((void*)&cc)+jsonConverters[i].offset) = stringToTempDiff(val);
					return;
				case stringToLongVal:
					*(uint16_t*)(((void*)&cc)+jsonConverters[i].offset) = strtoul(val, NULL, 10);
					return;
			}
	}
	
	if(strcmp_json(key,jsonKeys_mode) == 0){
		tempControl.setMode(val[0]);
		piLink.printFridgeAnnotation(PSTR("Mode set to %c in web interface"), val[0]);
	}
	else if(strcmp_json(key,jsonKeys_beerSetting) == 0){ 
		fixed7_9 newTemp = stringToTemp(val);
		if(cs.mode == 'p'){
			if(abs(newTemp-cs.beerSetting) > 100){ // this excludes gradual updates under 0.2 degrees
				printBeerAnnotation(msg, "Beer", val, "by profile");
			}
		}
		else{
			printBeerAnnotation(msg, "Beer", val, webui);
		}
		cs.beerSetting = newTemp;
	}
	else if(strcmp_json(key,jsonKeys_fridgeSetting) == 0){
		fixed7_9 newTemp = stringToTemp(val);
		if(cs.mode == 'f'){
			printFridgeAnnotation(msg, "Fridge", val, webui);
		}
		cs.fridgeSetting = newTemp;
	}		
	else if(strcmp_json(key,jsonKeys_heatEstimator) == 0){ cs.heatEstimator = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_coolEstimator) == 0){ cs.coolEstimator = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_tempFormat) == 0){
		cc.tempFormat = val[0];
		display.printStationaryText(); // reprint stationary text to update to right degree unit
	}
#if 0
	else if(strcmp_json(key,jsonKeys_tempSettingMin) == 0){ cc.tempSettingMin = stringToTemp(val); }
	else if(strcmp_json(key,jsonKeys_tempSettingMax) == 0){ cc.tempSettingMax = stringToTemp(val); }
	else if(strcmp_json(key,jsonKeys_KpHeat) == 0){ cc.KpHeat = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_KpCool) == 0){ cc.KpCool = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_Ki) == 0){ cc.Ki = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_KdCool) == 0){ cc.KdCool = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_KdHeat) == 0){ cc.KdHeat = stringToFixedPoint(val); }
	else if(strcmp_json(key,jsonKeys_iMaxError) == 0){ cc.iMaxError = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_iMaxSlope) == 0){ cc.iMaxSlope = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_iMinSlope) == 0){ cc.iMinSlope = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_idleRangeHigh) == 0){ cc.idleRangeHigh = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_idleRangeLow) == 0){ cc.idleRangeLow = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_heatingTargetUpper) == 0){ cc.heatingTargetUpper = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_heatingTargetLower) == 0){ cc.heatingTargetLower = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_coolingTargetUpper) == 0){ cc.coolingTargetUpper = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_coolingTargetLower) == 0){ cc.coolingTargetLower = stringToTempDiff(val); }
	else if(strcmp_json(key,jsonKeys_maxHeatTimeForEstimate) == 0){ cc.maxHeatTimeForEstimate = strtoul(val, NULL, 10); }
	else if(strcmp_json(key,jsonKeys_maxCoolTimeForEstimate) == 0){ cc.maxCoolTimeForEstimate = strtoul(val, NULL, 10); }
#endif

	// Receive the b value for the filter
	else if(strcmp_json(key,jsonKeys_fridgeFastFilter) == 0){ 
		cc.fridgeFastFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor->setFastFilterCoefficients(cc.fridgeFastFilter);
	}
	else if(strcmp_json(key,jsonKeys_fridgeSlowFilter) == 0){
		cc.fridgeSlowFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor->setSlowFilterCoefficients(cc.fridgeSlowFilter);
	}
	else if(strcmp_json(key,jsonKeys_fridgeSlopeFilter) == 0){
		cc.fridgeSlopeFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor->setSlopeFilterCoefficients(cc.fridgeSlopeFilter);
	}
	else if(strcmp_json(key,jsonKeys_beerFastFilter) == 0){
		cc.beerFastFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor->setFastFilterCoefficients(cc.beerFastFilter);	
	}
	else if(strcmp_json(key,jsonKeys_beerSlowFilter) == 0){
		cc.beerSlowFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor->setSlowFilterCoefficients(cc.beerSlowFilter);
	}
	else if(strcmp_json(key,jsonKeys_beerSlopeFilter) == 0){
		cc.beerSlopeFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor->setSlopeFilterCoefficients(cc.beerSlopeFilter);
	}
	else{
		debugMessage(PSTR("Could not process setting"));
	}

}