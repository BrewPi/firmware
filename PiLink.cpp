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

#include "version.h"
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

bool PiLink::firstPair;
// Rename Serial to piStream, to abstract it for later platform independence

#ifdef TEST		// write to a stream to capture output
	extern Stream& SerialCapture;
	#define piStream SerialCapture
#elif BREWPI_EMULATE
	class MockSerial
	{
		public:
	void print(char c) {}
		void print(const char* c) {}
		char read() { return '\0'; }
		uint8_t available() { return 0; }
	};

	static MockSerial mockSerial;
	#define piStream mockSerial

#else
	#define piStream Serial
#endif

void PiLink::init(void){
	piStream.begin(57600);	
}

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
	if (piStream.available() > 0){
		
		char inByte = piStream.read();
#if MULTICHAMBER
		char arg = piStream.read();
		if (arg=='\n') {}
			switch (inByte) {
				case 'x': // request chamber info
				printChamberInfo();
				return;
				case 'n':
				print_P(PSTR("N:\"%S\"\n"), PSTR(VERSION_STRING));
				break;
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

#if BREWPI_SIMULATE
		case 'u':
			updateInputs();
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
		case 'n':
			print_P(PSTR("N:\"%S\"\n"), PSTR(VERSION_STRING));	
			break;
		case 'l': // Display content requested
			printResponse('L');

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
			piStream.print('\n');			
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
		//piStream.flush(); Messages can be back to back. Flush should not be necessary.
		// Functions should not read more than what is meant for that function.
	}
}


#if MULTICHAMBER
void PiLink::printChamberInfo()
{
	printResponse('X');
	print('{');
	sendJsonPair(PSTR("chambers"), chamberManager.chamberCount());
	sendJsonPair(PSTR("current"), chamberManager.currentChamber());
	print('}');
	print('\n');
}
#endif

void PiLink::printTemperaturesJSON(char * beerAnnotation, char * fridgeAnnotation){
	char tempString[9];
	printResponse('T');
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
		print_P(PSTR("null,"));
	}
	else{
		print_P(PSTR("\"%s\","), fridgeAnnotation);	
	}
#if BREWPI_SIMULATE
	print_P(PSTR("\"t\":%lu,"), ticks.millis());
#endif	
	print_P(PSTR("\"State\":%u}\n"), tempControl.getState());
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
	printResponse('D');
	
	// Using print_P for the Annotation fails. Arguments are not passed correctly. Use Serial directly as a work around.
	va_start (args, message );
	vsnprintf_P(tempString, 128, message, args);
	va_end (args);
	piStream.print(tempString);
	piStream.print('\n'); // print newline
}

void PiLink::printResponse(char type) {
	piStream.print(type);
#if MULTICHAMBER
	print(chamberManager.currentChamber()+'1');
#endif
	piStream.print(':');
	firstPair = true;
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
	piStream.print('\n');
}

// Send settings as JSON string
void PiLink::sendControlSettings(void){
	char tempString[12];
	printResponse('S');
	ControlSettings& cs = tempControl.cs;
	sendJsonPair(JSONKEY_mode, cs.mode);
	sendJsonPair(JSONKEY_beerSetting, tempToString(tempString, cs.beerSetting, 2, 12));
	sendJsonPair(JSONKEY_fridgeSetting, tempToString(tempString, cs.fridgeSetting, 2, 12));
	sendJsonPair(JSONKEY_heatEstimator, fixedPointToString(tempString, cs.heatEstimator, 3, 12));
	sendJsonPair(JSONKEY_coolEstimator, fixedPointToString(tempString, cs.coolEstimator, 3, 12));	
	sendJsonClose();
}

// Send control constants as JSON string. Might contain spaces between minus sign and number. Python will have to strip these
void PiLink::sendControlConstants(void){
	char tempString[12];
	printResponse('C');	
	sendJsonPair(JSONKEY_tempFormat, tempControl.cc.tempFormat);
	sendJsonPair(JSONKEY_tempSettingMin, tempToString(tempString, tempControl.cc.tempSettingMin, 1, 12));
	sendJsonPair(JSONKEY_tempSettingMax, tempToString(tempString, tempControl.cc.tempSettingMax, 1, 12));
	sendJsonPair(JSONKEY_KpHeat, fixedPointToString(tempString, tempControl.cc.KpHeat, 3, 12));
	sendJsonPair(JSONKEY_KpCool, fixedPointToString(tempString, tempControl.cc.KpCool, 3, 12));
	sendJsonPair(JSONKEY_Ki, fixedPointToString(tempString, tempControl.cc.Ki, 3, 12));
	sendJsonPair(JSONKEY_KdCool, fixedPointToString(tempString, tempControl.cc.KdCool, 3, 12));
	sendJsonPair(JSONKEY_KdHeat, fixedPointToString(tempString, tempControl.cc.KdHeat, 3, 12));
	sendJsonPair(JSONKEY_iMaxError, tempDiffToString(tempString, tempControl.cc.iMaxError, 3, 12));
	
	sendJsonPair(JSONKEY_idleRangeHigh, tempDiffToString(tempString, tempControl.cc.idleRangeHigh, 3, 12));
	sendJsonPair(JSONKEY_idleRangeLow, tempDiffToString(tempString, tempControl.cc.idleRangeLow, 3, 12));
	sendJsonPair(JSONKEY_heatingTargetUpper, tempDiffToString(tempString, tempControl.cc.heatingTargetUpper, 3, 12));
	sendJsonPair(JSONKEY_heatingTargetLower, tempDiffToString(tempString, tempControl.cc.heatingTargetLower, 3, 12));
	sendJsonPair(JSONKEY_coolingTargetUpper, tempDiffToString(tempString, tempControl.cc.coolingTargetUpper, 3, 12));
	sendJsonPair(JSONKEY_coolingTargetLower, tempDiffToString(tempString, tempControl.cc.coolingTargetLower, 3, 12));
	sendJsonPair(JSONKEY_maxHeatTimeForEstimate, tempControl.cc.maxHeatTimeForEstimate);
	sendJsonPair(JSONKEY_maxCoolTimeForEstimate, tempControl.cc.maxCoolTimeForEstimate);

	sendJsonPair(JSONKEY_fridgeFastFilter, tempControl.cc.fridgeFastFilter);
	sendJsonPair(JSONKEY_fridgeSlowFilter, tempControl.cc.fridgeSlowFilter);
	sendJsonPair(JSONKEY_fridgeSlopeFilter, tempControl.cc.fridgeSlopeFilter);
	sendJsonPair(JSONKEY_beerFastFilter, tempControl.cc.beerFastFilter);
	sendJsonPair(JSONKEY_beerSlowFilter, tempControl.cc.beerSlowFilter);
	sendJsonPair(JSONKEY_beerSlopeFilter, tempControl.cc.beerSlopeFilter);
	sendJsonClose();
}

#if 0
struct PairOffset {
	const char* key; uint8_t valueOffset;
};

// todo - add compiletime check that offset is <256

#define PAIR_OFFSET_CV_7_9(name) { &jsonKeys_ ## name [0],  offsetof(ControlVariables, name) }

static PROGMEM const PairOffset offsets_cv_7_9 [] = {
	PAIR_OFFSET_CV_7_9(Kp),
	PAIR_OFFSET_CV_7_9(Kd),
	PAIR_OFFSET_CV_7_9(estimatedPeak),
	PAIR_OFFSET_CV_7_9(negPeakSetting),
	PAIR_OFFSET_CV_7_9(posPeakSetting),
	PAIR_OFFSET_CV_7_9(posPeakSetting),
	PAIR_OFFSET_CV_7_9(negPeak),
	PAIR_OFFSET_CV_7_9(posPeak),
};
#endif

// Send all control variables. Useful for debugging and choosing parameters
void PiLink::sendControlVariables(void){
	char tempString[12];
	printResponse('V');	
	sendJsonPair(JSONKEY_beerDiff, tempDiffToString(tempString, tempControl.cv.beerDiff, 3, 12));
	sendJsonPair(JSONKEY_diffIntegral, tempDiffToString(tempString, tempControl.cv.diffIntegral, 3, 12));
	sendJsonPair(JSONKEY_beerSlope, tempDiffToString(tempString, tempControl.cv.beerSlope, 3, 12));
	sendJsonPair(JSONKEY_p, fixedPointToString(tempString, tempControl.cv.p, 3, 12));
	sendJsonPair(JSONKEY_i, fixedPointToString(tempString, tempControl.cv.i, 3, 12));
	sendJsonPair(JSONKEY_d, fixedPointToString(tempString, tempControl.cv.d, 3, 12));
	sendJsonPair(JSONKEY_Kp, fixedPointToString(tempString, tempControl.cv.Kp, 3, 12));
	sendJsonPair(JSONKEY_Kd, fixedPointToString(tempString, tempControl.cv.Kd, 3, 12));
	sendJsonPair(JSONKEY_estimatedPeak, tempToString(tempString, tempControl.cv.estimatedPeak, 3, 12));
	sendJsonPair(JSONKEY_negPeakSetting, tempToString(tempString, tempControl.cv.negPeakSetting, 3, 12));
	sendJsonPair(JSONKEY_posPeakSetting, tempToString(tempString, tempControl.cv.posPeakSetting, 3, 12));
	sendJsonPair(JSONKEY_negPeak, tempToString(tempString, tempControl.cv.negPeak, 3, 12));
	sendJsonPair(JSONKEY_posPeak, tempToString(tempString, tempControl.cv.posPeak, 3, 12));
	sendJsonClose();
}

void PiLink::printJsonName(const char * name)
{
	printJsonSeparator();
	piStream.print('"');
	print_P(name);
	piStream.print('"');
	piStream.print(':');
}

inline void PiLink::printJsonSeparator() {
	piStream.print(firstPair ? '{' : ',');	
	firstPair = false;
}

void PiLink::sendJsonPair(const char * name, const char * val){
	printJsonName(name);
	piStream.print(val);
}

void PiLink::sendJsonPair(const char * name, char val){
	printJsonName(name);
	piStream.print('"');
	piStream.print(val);
	piStream.print('"');
}

void PiLink::sendJsonPair(const char * name, uint16_t val){
	printJsonName(name);
	print_P(PSTR("\"%u\""), val);
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
#if 0

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

static const PROGMEM JsonConvert jsonConverters[] = {
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
#endif

void PiLink::processJsonPair(char * key, char * val){
	debugMessage(PSTR("Received new setting: %s = %s"), key, val);
	if(strcmp_P(key,JSONKEY_mode) == 0){
		tempControl.setMode(val[0]);
		piLink.printFridgeAnnotation(PSTR("Mode set to %c in web interface"), val[0]);
	}
	else if(strcmp_P(key,JSONKEY_beerSetting) == 0){
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
else if(strcmp_P(key,JSONKEY_fridgeSetting) == 0){
	fixed7_9 newTemp = stringToTemp(val);
	if(tempControl.cs.mode == 'f'){
		printFridgeAnnotation(PSTR("Fridge temp set to %s in web interface."), val);
	}
	tempControl.cs.fridgeSetting = newTemp;
}
	else if(strcmp_P(key,JSONKEY_heatEstimator) == 0){ tempControl.cs.heatEstimator = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_coolEstimator) == 0){ tempControl.cs.coolEstimator = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_tempFormat) == 0){
		tempControl.cc.tempFormat = val[0];
		display.printStationaryText(); // reprint stationary text to update to right degree unit
	}
	else if(strcmp_P(key,JSONKEY_tempSettingMin) == 0){ tempControl.cc.tempSettingMin = stringToTemp(val); }
	else if(strcmp_P(key,JSONKEY_tempSettingMax) == 0){ tempControl.cc.tempSettingMax = stringToTemp(val); }
	else if(strcmp_P(key,JSONKEY_KpHeat) == 0){ tempControl.cc.KpHeat = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_KpCool) == 0){ tempControl.cc.KpCool = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_Ki) == 0){ tempControl.cc.Ki = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_KdCool) == 0){ tempControl.cc.KdCool = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_KdHeat) == 0){ tempControl.cc.KdHeat = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_iMaxError) == 0){ tempControl.cc.iMaxError = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_idleRangeHigh) == 0){ tempControl.cc.idleRangeHigh = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_idleRangeLow) == 0){ tempControl.cc.idleRangeLow = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_heatingTargetUpper) == 0){ tempControl.cc.heatingTargetUpper = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_heatingTargetLower) == 0){ tempControl.cc.heatingTargetLower = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_coolingTargetUpper) == 0){ tempControl.cc.coolingTargetUpper = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_coolingTargetLower) == 0){ tempControl.cc.coolingTargetLower = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_maxHeatTimeForEstimate) == 0){ tempControl.cc.maxHeatTimeForEstimate = strtoul(val, NULL, 10); }
	else if(strcmp_P(key,JSONKEY_maxCoolTimeForEstimate) == 0){ tempControl.cc.maxCoolTimeForEstimate = strtoul(val, NULL, 10); }
	else if(strcmp_P(key,JSONKEY_maxCoolTimeForEstimate) == 0){ tempControl.cc.maxCoolTimeForEstimate = strtoul(val, NULL, 10); }

	// Receive the b value for the filter
	else if(strcmp_P(key,JSONKEY_fridgeFastFilter) == 0){ 
		tempControl.cc.fridgeFastFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor->setFastFilterCoefficients(tempControl.cc.fridgeFastFilter);
	}
	else if(strcmp_P(key,JSONKEY_fridgeSlowFilter) == 0){
		tempControl.cc.fridgeSlowFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor->setSlowFilterCoefficients(tempControl.cc.fridgeSlowFilter);
	}
	else if(strcmp_P(key,JSONKEY_fridgeSlopeFilter) == 0){
		tempControl.cc.fridgeSlopeFilter = strtoul(val, NULL, 10);
		tempControl.fridgeSensor->setSlopeFilterCoefficients(tempControl.cc.fridgeSlopeFilter);
	}
	else if(strcmp_P(key,JSONKEY_beerFastFilter) == 0){
		tempControl.cc.beerFastFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor->setFastFilterCoefficients(tempControl.cc.beerFastFilter);

	}
	else if(strcmp_P(key,JSONKEY_beerSlowFilter) == 0){
		tempControl.cc.beerSlowFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor->setSlowFilterCoefficients(tempControl.cc.beerSlowFilter);
	}
	else if(strcmp_P(key,JSONKEY_beerSlopeFilter) == 0){
		tempControl.cc.beerSlopeFilter = strtoul(val, NULL, 10);
		tempControl.beerSensor->setSlopeFilterCoefficients(tempControl.cc.beerSlopeFilter);
	}
	else{
		debugMessage(PSTR("Could not process setting"));
	}
}


#if BREWPI_SIMULATE==1
#include "ExternalTempSensor.h"

void setTemp(TempSensor* sensor, const char* val) {
	
	ExternalTempSensor& externalSensor = (ExternalTempSensor&)sensor->basicTempSensor();
	if (val==NULL || *val==0)
		externalSensor.setConnected(false);
	else
	{
		fixed7_9 newTemp = stringToTemp(val);
		externalSensor.setValue(newTemp);
		externalSensor.setConnected(true);
	}	
}

void setSwitch(SwitchSensor* sensor, bool newSetting) {
	ValueSensor<bool>* externalSensor = (ValueSensor<bool>*) sensor;
	externalSensor->setValue(newSetting);
}

void setTicks(ExternalTicks& externalTicks, const char* val, int multiplier=1000) {		
	
	if (val==NULL || *val==0) {
		externalTicks.incMillis(1000);
	}
	else {
		unsigned long newTicks = strtoul(val, NULL, 10)*multiplier;
		if (*val=='=')
			externalTicks.setMillis(newTicks);
		else
			externalTicks.incMillis(newTicks);
	}
	
	DEBUG_MSG(PSTR("New ticks %lu"), externalTicks.millis());
}

extern void setRunFactor(fixed7_9 factor);

#endif

void PiLink::updateInputs()
{
#if BREWPI_SIMULATE==1
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
		DEBUG_MSG(PSTR("name %s val %s"), key, val);
		if (strcmp_P(key, PSTR("BeerTemp"))==0) {
			DEBUG_MSG(PSTR("setting beer temp %s"), val);
			setTemp(tempControl.beerSensor, val);
		}
		else if (strcmp_P(key, PSTR("FridgeTemp"))==0) {
			DEBUG_MSG(PSTR("setting fridge temp %s"), val);
			setTemp(tempControl.fridgeSensor, val);
		}
		else if (strcmp_P(key, PSTR("DoorState"))==0) {		// 0 for closed, anything else for open
			DEBUG_MSG(PSTR("setting door state to %s"), val);
			setSwitch(tempControl.door, strcmp(val, "0")!=0);
		}
		else if (strcmp_P(key, PSTR("ms"))==0) {
			DEBUG_MSG(PSTR("setting ticks to %s"), val);
			setTicks(ticks, val);
		}		
		else if (strcmp_P(key, PSTR("s"))==0) {
			DEBUG_MSG(PSTR("setting seconds to %s"), val);
			setTicks(ticks, val, 1000);
		}
		else if (strcmp_P(key, PSTR("r"))==0) {
			DEBUG_MSG(PSTR("setting run factor to %s"), val);
			setRunFactor(stringToFixedPoint(val));
		}
	}
	printTemperatures();	
#endif	
}

	
