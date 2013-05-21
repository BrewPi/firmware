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

#include "Brewpi.h"
#include "stddef.h"
#include "PiLink.h"

#include "Version.h"
#include "tempControl.h"
#include "Display.h"
#include <stdarg.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <limits.h>
#include <string.h>
#include "JsonKeys.h"
#include "Ticks.h"
#include "Brewpi.h"
#include "EepromManager.h"
#include "EepromFormat.h"
#include "SettingsManager.h"
#if BREWPI_SIMULATE
#include "Simulator.h"
#endif

bool PiLink::firstPair;
// Rename Serial to piStream, to abstract it for later platform independence

#if BREWPI_EMULATE
	class MockSerial : public Stream
	{
		public:
		void print(char c) {}
		void print(const char* c) {}
		void printNewLine() {};
		int read() { return -1; }
		int available() { return -1; }
		void begin(unsigned long) {}
		size_t write(uint8_t w) { return 1; }
		int peek() { return -1; }
		void flush() { };			
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
	if(piStream){ // if Serial connected (on Leonardo)
		piStream.print(tmp);
	}
}

// create a printf like interface to the Arduino Serial function. Format string stored in RAM
void PiLink::print(char *fmt, ... ){
	char tmp[128]; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt );
	vsnprintf(tmp, 128, fmt, args);
	va_end (args);
	if(piStream){
		piStream.print(tmp);
	}
}

void PiLink::printNewLine(){
	piStream.println();
}

void printNibble(uint8_t n)
{
	n &= 0xF;
	piStream.print((char)(n>=10 ? n-10+'A' : n+'0'));
}

void PiLink::receive(void){
	if (piStream.available() > 0){		
		char inByte = piStream.read();
		if (inByte=='\n' || inByte=='\r')		// allow newlines between commands
			return;			
						
		switch(inByte){

#if BREWPI_SIMULATE==1
		case 'y':
			parseJson(HandleSimulatorConfig);
			break;
		case 'Y':
			printSimulatorSettings();
			break;		
#endif						
		case 't': // temperatures requested
			printTemperatures();      
			break;		
		case 'C': // Set default constants
			tempControl.loadDefaultConstants();
			display.printStationaryText(); // reprint stationary text to update to right degree unit
			sendControlConstants(); // update script with new settings
			DEBUG_MSG_1(PSTR("Default constants loaded."));
			break;
		case 'S': // Set default settings
			tempControl.loadDefaultSettings();
			sendControlSettings(); // update script with new settings
			DEBUG_MSG_1(PSTR("Default settings loaded."));
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
			// v - version
			// s - shield type
			// sim: simulator
			print_P(PSTR("N:{ver:\"%S\",shield:\"%S\",sim:%d,board:\"%S\"}\n"), PSTR(VERSION_STRING), PSTR(stringify(BREWPI_STATIC_CONFIG)), BREWPI_SIMULATE, PSTR(BREWPI_BOARD)) ;				
			break;
		case 'l': // Display content requested
			printResponse('L');						
			piStream.print('[');
			char stringBuffer[21];
			for(uint8_t i=0;i<4;i++){
				display.getLine(i, stringBuffer);
				print_P(PSTR("\"%s\""), stringBuffer);
				char close = (i<3) ? ',':']';
				piStream.print(close);
			}							
			printNewLine();						
			break;
		case 'j': // Receive settings as json
			receiveJson();
			break;

#if !BREWPI_SIMULATE		// dynaconfig not needed for simulator

#if BREWPI_EEPROM_HELPER_COMMANDS
		case 'e': // dump contents of eeprom						
			openListResponse('E');
			for (uint16_t i=0; i<1024;) {
				if (i>0) {
					piLink.printNewLine();
					piLink.print(',');
				}
				piLink.print('\"');
				for (uint8_t j=0; j<64; j++) {
					uint8_t d = eepromAccess.readByte(i++);
					printNibble(d>>4);
					printNibble(d);
				}				
				piLink.print('\"');
			}
			closeListResponse();
			break;
#endif
			
		case 'E': // initialize eeprom
			eepromManager.initializeEeprom();
			DEBUG_MSG_2(PSTR("EEPROM initialized"));
			settingsManager.loadSettings();
			break;

		case 'd': // list devices in eeprom order
			openListResponse('d');
			deviceManager.listDevices(piStream);
			closeListResponse();
			break;

		case 'U': // update device		
			printResponse('U');
			deviceManager.parseDeviceDefinition(piStream);
			piLink.printNewLine();
			break;
			
		case 'h': // hardware query
			openListResponse('h');
			deviceManager.enumerateHardware(piStream);
			closeListResponse();
			break;

#if (BREWPI_DEBUG > 0)			
		case 'Z': // zap eeprom
			eepromManager.zapEeprom();
			DEBUG_MSG_1(PSTR("Zapped eeprom."));
			break;
#endif

#endif // !BREWPI_SIMULATE

		default:
			DEBUG_MSG_1(PSTR("Invalid command received by Arduino: %c"), inByte);
		}
	}
}


#define COMPACT_SERIAL BREWPI_SIMULATE
#if COMPACT_SERIAL
	#define JSON_BEER_TEMP  "bt"
	#define JSON_BEER_SET	"bs"
	#define JSON_BEER_ANN	"ba"
	#define JSON_FRIDGE_TEMP "ft"
	#define JSON_FRIDGE_SET  "fs"
	#define JSON_FRIDGE_ANN  "fa"
	#define JSON_STATE		"s"
	#define JSON_TIME		"t"
	#define JSON_ROOM_TEMP  "rt"
	
	fixed7_9 beerTemp = -1, beerSet = -1, fridgeTemp = -1, fridgeSet = -1;
	double roomTemp = -1;
	uint8_t state = 0xFF;
	char* beerAnn; char* fridgeAnn;
	
	typedef char* PChar;
	inline bool changed(uint8_t &a, uint8_t b) { uint8_t c = a; a=b; return b!=c; }
	inline bool changed(fixed7_9 &a, fixed7_9 b) { fixed7_9 c = a; a=b; return b!=c; }
	inline bool changed(double &a, double b) { double c = a; a=b; return b!=c; }
	inline bool changed(PChar &a, PChar b) { PChar c = a; a=b; return b!=c; }
#else
	#define JSON_BEER_TEMP  "BeerTemp"
	#define JSON_BEER_SET	"BeerSet"
	#define JSON_BEER_ANN	"BeerAnn"
	#define JSON_FRIDGE_TEMP "FridgeTemp"
	#define JSON_FRIDGE_SET  "FridgeSet"
	#define JSON_FRIDGE_ANN  "FridgeAnn"
	#define JSON_STATE		"State"
	#define JSON_TIME		"Time"
	#define JSON_ROOM_TEMP  "RoomTemp"
	
	#define changed(a,b)  1
#endif

void PiLink::printTemperaturesJSON(char * beerAnnotation, char * fridgeAnnotation){
	char tempString[9];
	printResponse('T');	
	piLink.print('{');
	firstPair = false;
	if (changed(beerTemp, tempControl.getBeerTemp()))	
		print_P(PSTR("\"" JSON_BEER_TEMP "\":%s,"), tempToString(tempString, tempControl.getBeerTemp(), 2, 9));
	if (changed(beerSet,tempControl.getBeerSetting()))
		print_P(PSTR("\"" JSON_BEER_SET "\":%s,"), tempToString(tempString, tempControl.getBeerSetting(), 2, 9));
	if (changed(beerAnn, beerAnnotation)) {
		print_P(PSTR("\""JSON_BEER_ANN"\":"));
		if(beerAnnotation == 0){
			print_P(PSTR("null,"));
		}
		else{
			print_P(PSTR("\"%s\","), beerAnnotation);
		}
	}
	if (changed(fridgeTemp, tempControl.getFridgeTemp()))
		print_P(PSTR("\""JSON_FRIDGE_TEMP"\":%s,"), tempToString(tempString, tempControl.getFridgeTemp(), 2, 9));
	if (changed(fridgeSet, tempControl.getFridgeSetting()))
		print_P(PSTR("\""JSON_FRIDGE_SET"\":%s,"), tempToString(tempString, tempControl.getFridgeSetting(), 2, 9));
	if (changed(fridgeAnn, fridgeAnnotation)) {
		print_P(PSTR("\""JSON_FRIDGE_ANN"\":"));
		if(fridgeAnnotation == 0){
			print_P(PSTR("null,"));
		}
		else{
			print_P(PSTR("\"%s\","), fridgeAnnotation);	
		}
	}	
	if (tempControl.ambientSensor->isConnected() && changed(roomTemp, tempControl.getRoomTemp()))
		print_P(PSTR("\""JSON_ROOM_TEMP"\":%s,"), tempToString(tempString, tempControl.getRoomTemp(), 2, 9));
	if (changed(state, tempControl.getState()))
		print_P(PSTR("\""JSON_STATE"\":%u,"), tempControl.getState());
	
	print_P(PSTR("\""JSON_TIME"\":%lu}"), ticks.millis()/1000);		
	printNewLine();
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
 	  
void PiLink::printResponse(char type) {
	piStream.print(type);
	piStream.print(':');
	firstPair = true;
}

void PiLink::openListResponse(char type) {
	printResponse(type);
	piStream.print('[');
}

void PiLink::closeListResponse() {
	piStream.print(']');
	printNewLine();
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
	printNewLine();
}


void PiLink::sendJsonClose() {
	piStream.print('}');
	printNewLine();
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


#if OPTIMIZE_JSON_OUTPUT_CC

// where the offset is relative to. This saves having to store a full 16-bit pointer.
// becasue the structs are static, we can only compute an offset relative to the struct (cc,cs,cv etc..)
// rather than offset from tempControl. 
uint8_t* jsonOutputBase;

void PiLink::jsonOutputUint8(const char* key, uint8_t offset) {
	piLink.sendJsonPair(key, *(jsonOutputBase+offset));
}

/**
 * outputs the temperature at the given offset from tempControl.cc.
 * The temperature is assumed to be a fixed7_9 value.
 */
void PiLink::jsonOutputTempToString(const char* key,  uint8_t offset) {
	char buf[12];
	piLink.sendJsonPair(key, tempToString(buf, *((fixed7_9*)(jsonOutputBase+offset)), 1, 12));
}

void PiLink::jsonOutputFixedPointToString(const char* key, uint8_t offset) {
	char buf[12];
	piLink.sendJsonPair(key, fixedPointToString(buf, *((fixed7_9*)(jsonOutputBase+offset)), 3, 12));
}

void PiLink::jsonOutputTempDiffToString(const char* key, uint8_t offset) {
	char buf[12];
	piLink.sendJsonPair(key, tempDiffToString(buf, *((fixed7_9*)(jsonOutputBase+offset)), 3, 12));
}

void PiLink::jsonOutputChar(const char* key, uint8_t offset) {	
	piLink.sendJsonPair(key, *((char*)(jsonOutputBase+offset)));
}

typedef void (*JsonOutputCCHandler)(const char* key, uint8_t offset);

enum JsonOutputIndex {
	JOCC_UINT8=0,
	JOCC_TEMP_FORMAT=1,
	JOCC_FIXED_POINT=2,
	JOCC_TEMP_DIFF=3,
	JOCC_CHAR=4
};

const PiLink::JsonOutputHandler PiLink::JsonOutputHandlers[] = {
	PiLink::jsonOutputUint8,
	PiLink::jsonOutputTempToString,
	PiLink::jsonOutputFixedPointToString,
	PiLink::jsonOutputTempDiffToString,
	PiLink::jsonOutputChar
};

#define JSON_OUTPUT_CC_MAP(name, fn) { JSONKEY_ ## name,  offsetof(ControlConstants, name), fn }
#define JSON_OUTPUT_CV_MAP(name, fn) { JSONKEY_ ## name,  offsetof(ControlVariables, name), fn }
#define JSON_OUTPUT_CS_MAP(name, fn) { JSONKEY_ ## name,  offsetof(ControlSettings, name), fn }

const PiLink::JsonOutput PiLink::jsonOutputCCMap[] = {
	JSON_OUTPUT_CC_MAP(tempFormat, JOCC_CHAR),
	JSON_OUTPUT_CC_MAP(tempSettingMin, JOCC_TEMP_FORMAT),
	JSON_OUTPUT_CC_MAP(tempSettingMax, JOCC_TEMP_FORMAT),

	JSON_OUTPUT_CC_MAP(Kp, JOCC_FIXED_POINT),
	JSON_OUTPUT_CC_MAP(Ki, JOCC_FIXED_POINT),
	JSON_OUTPUT_CC_MAP(Kd, JOCC_FIXED_POINT),

	JSON_OUTPUT_CC_MAP(iMaxError, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(idleRangeHigh, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(idleRangeLow, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(heatingTargetUpper, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(heatingTargetLower, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(coolingTargetUpper, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(coolingTargetLower, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CC_MAP(maxHeatTimeForEstimate, JOCC_TEMP_DIFF),
//	JSON_OUTPUT_CC_MAP(minHeatTimeForEstimate, JOCC_TEMP_DIFF),

	JSON_OUTPUT_CC_MAP(fridgeFastFilter, JOCC_UINT8),
	JSON_OUTPUT_CC_MAP(fridgeSlowFilter, JOCC_UINT8),
	JSON_OUTPUT_CC_MAP(fridgeSlopeFilter, JOCC_UINT8),
	JSON_OUTPUT_CC_MAP(beerFastFilter, JOCC_UINT8),
	JSON_OUTPUT_CC_MAP(beerSlowFilter, JOCC_UINT8),
	JSON_OUTPUT_CC_MAP(beerSlopeFilter, JOCC_UINT8),
	
	JSON_OUTPUT_CC_MAP(lightAsHeater, JOCC_UINT8)
	
};
#endif

// Send control constants as JSON string. Might contain spaces between minus sign and number. Python will have to strip these
void PiLink::sendControlConstants(void){

#if OPTIMIZE_JSON_OUTPUT_CC
	printResponse('C');
	jsonOutputBase = (uint8_t*)&tempControl.cc;
	for (uint8_t i=0; i<sizeof(jsonOutputCCMap)/sizeof(jsonOutputCCMap[0]); i++) {
		JsonOutputHandlers[jsonOutputCCMap[i].handlerOffset](jsonOutputCCMap[i].key,jsonOutputCCMap[i].offset);
	}
	sendJsonClose();

#else
	char tempString[12];
	printResponse('C');	
	sendJsonPair(JSONKEY_tempFormat, tempControl.cc.tempFormat);
	sendJsonPair(JSONKEY_tempSettingMin, tempToString(tempString, tempControl.cc.tempSettingMin, 1, 12));
	sendJsonPair(JSONKEY_tempSettingMax, tempToString(tempString, tempControl.cc.tempSettingMax, 1, 12));
	sendJsonPair(JSONKEY_Kp, fixedPointToString(tempString, tempControl.cc.Kp, 3, 12));
	sendJsonPair(JSONKEY_Ki, fixedPointToString(tempString, tempControl.cc.Ki, 3, 12));
	sendJsonPair(JSONKEY_Kd, fixedPointToString(tempString, tempControl.cc.Kd, 3, 12));
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
	
	sendJsonPair(JSONKEY_lightAsHeater, tempControl.cc.lightAsHeater);
	sendJsonClose();
#endif	
}

#if OPTIMIZE_JSON_OUTPUT_CV
const PiLink::JsonOutput PiLink::jsonOutputCVMap[] = {
	JSON_OUTPUT_CV_MAP(beerDiff, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CV_MAP(diffIntegral, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CV_MAP(beerSlope, JOCC_TEMP_DIFF),
	JSON_OUTPUT_CV_MAP(p, JOCC_FIXED_POINT),
	JSON_OUTPUT_CV_MAP(i, JOCC_FIXED_POINT),
	JSON_OUTPUT_CV_MAP(d, JOCC_FIXED_POINT),
	JSON_OUTPUT_CV_MAP(estimatedPeak, JOCC_TEMP_FORMAT),
	JSON_OUTPUT_CV_MAP(negPeakEstimate, JOCC_TEMP_FORMAT),
	JSON_OUTPUT_CV_MAP(posPeakEstimate, JOCC_TEMP_FORMAT),
	JSON_OUTPUT_CV_MAP(negPeak, JOCC_TEMP_FORMAT),
	JSON_OUTPUT_CV_MAP(posPeak, JOCC_TEMP_FORMAT)	
};
#endif


// Send all control variables. Useful for debugging and choosing parameters
void PiLink::sendControlVariables(void){
	printResponse('V');	
#if OPTIMIZE_JSON_OUTPUT_CV
	jsonOutputBase = (uint8_t*)&tempControl.cv;
	for (uint8_t i=0; i<sizeof(jsonOutputCVMap)/sizeof(jsonOutputCVMap[0]); i++) {
		JsonOutputHandlers[jsonOutputCVMap[i].handlerOffset](jsonOutputCVMap[i].key,jsonOutputCVMap[i].offset);
	}

#else
	char tempString[12];
	sendJsonPair(JSONKEY_beerDiff, tempDiffToString(tempString, tempControl.cv.beerDiff, 3, 12));
	sendJsonPair(JSONKEY_diffIntegral, tempDiffToString(tempString, tempControl.cv.diffIntegral, 3, 12));
	sendJsonPair(JSONKEY_beerSlope, tempDiffToString(tempString, tempControl.cv.beerSlope, 3, 12));
	sendJsonPair(JSONKEY_p, fixedPointToString(tempString, tempControl.cv.p, 3, 12));
	sendJsonPair(JSONKEY_i, fixedPointToString(tempString, tempControl.cv.i, 3, 12));
	sendJsonPair(JSONKEY_d, fixedPointToString(tempString, tempControl.cv.d, 3, 12));
	sendJsonPair(JSONKEY_estimatedPeak, tempToString(tempString, tempControl.cv.estimatedPeak, 3, 12));
	sendJsonPair(JSONKEY_negPeakEstimate, tempToString(tempString, tempControl.cv.negPeakEstimate, 3, 12));
	sendJsonPair(JSONKEY_posPeakEstimate, tempToString(tempString, tempControl.cv.posPeakEstimate, 3, 12));
	sendJsonPair(JSONKEY_negPeak, tempToString(tempString, tempControl.cv.negPeak, 3, 12));
	sendJsonPair(JSONKEY_posPeak, tempToString(tempString, tempControl.cv.posPeak, 3, 12));
#endif	
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

int readNext()
{
	uint8_t retries = 0;
	while (piStream.available()==0) {
		_delay_us(100);
		retries++;
		if(retries >= 10){
			return -1;
		}
	}
	return piStream.read();		
}
/**
 * Parses a token from the piStream.
 * \return true if a token was parsed
 */
bool parseJsonToken(char* val) {
	uint8_t index = 0;
	val[0] = 0;
	bool result = true;
	for(;;) // get value
	{
		int character = readNext();
		if (index==29 || character == '}' || character==-1) {
			result = false;
			break;
		}
		if(character == ',' || character==':')  // end of value
			break;		
		if(character == ' ' || character == '"'){
			; // skip spaces and apostrophes
		}		
		else
			val[index++] = character;
	}
	val[index]=0; // null terminate string
	return result;	
}

void PiLink::parseJson(ParseJsonCallback fn, void* data) 
{
	char key[30];
	char val[30];
	bool next = true;
	// read first open brace
	int c = readNext();		
	if (c!='{')
	{
		DEBUG_MSG_1(PSTR("Expected { got %c"), c);
		return;
	}
	do {
		next = parseJsonToken(key) && parseJsonToken(val);
		if (val[0] && key[0])
			fn(key, val, data);
	} while (next);
}

void PiLink::receiveJson(void){

	parseJson(&processJsonPair, NULL);
	
	eepromManager.storeTempConstantsAndSettings();
				
#if !BREWPI_SIMULATE	// this is quite an overhead and not needed for the simulator
	sendControlSettings();	// update script with new settings
	sendControlConstants();
#endif
	return;
}

#if 0 
// mdma - I'll finish this later, should be able to reduce size of this function, this is presently our largest function
struct JsonPaserConvert {
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

void PiLink::processJsonPair(const char * key, const char * val, void* pv){
	DEBUG_MSG_1(PSTR("Received new setting: %s = %s"), key, val);
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
	else if(strcmp_P(key,JSONKEY_Kp) == 0){ tempControl.cc.Kp = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_Ki) == 0){ tempControl.cc.Ki = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_Kd) == 0){ tempControl.cc.Kd = stringToFixedPoint(val); }
	else if(strcmp_P(key,JSONKEY_iMaxError) == 0){ tempControl.cc.iMaxError = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_idleRangeHigh) == 0){ tempControl.cc.idleRangeHigh = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_idleRangeLow) == 0){ tempControl.cc.idleRangeLow = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_heatingTargetUpper) == 0){ tempControl.cc.heatingTargetUpper = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_heatingTargetLower) == 0){ tempControl.cc.heatingTargetLower = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_coolingTargetUpper) == 0){ tempControl.cc.coolingTargetUpper = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_coolingTargetLower) == 0){ tempControl.cc.coolingTargetLower = stringToTempDiff(val); }
	else if(strcmp_P(key,JSONKEY_maxHeatTimeForEstimate) == 0){ tempControl.cc.maxHeatTimeForEstimate = atol(val); }
	else if(strcmp_P(key,JSONKEY_maxCoolTimeForEstimate) == 0){ tempControl.cc.maxCoolTimeForEstimate = atol(val); }
	else if(strcmp_P(key,JSONKEY_maxCoolTimeForEstimate) == 0){ tempControl.cc.maxCoolTimeForEstimate = atol(val); }

	// Receive the b value for the filter
	else if(strcmp_P(key,JSONKEY_fridgeFastFilter) == 0){ 
		tempControl.cc.fridgeFastFilter = atol(val);
		tempControl.fridgeSensor->setFastFilterCoefficients(tempControl.cc.fridgeFastFilter);
	}
	else if(strcmp_P(key,JSONKEY_fridgeSlowFilter) == 0){
		tempControl.cc.fridgeSlowFilter = atol(val);
		tempControl.fridgeSensor->setSlowFilterCoefficients(tempControl.cc.fridgeSlowFilter);
	}
	else if(strcmp_P(key,JSONKEY_fridgeSlopeFilter) == 0){
		tempControl.cc.fridgeSlopeFilter = atol(val);
		tempControl.fridgeSensor->setSlopeFilterCoefficients(tempControl.cc.fridgeSlopeFilter);
	}
	else if(strcmp_P(key,JSONKEY_beerFastFilter) == 0){
		tempControl.cc.beerFastFilter = atol(val);
		tempControl.beerSensor->setFastFilterCoefficients(tempControl.cc.beerFastFilter);

	}
	else if(strcmp_P(key,JSONKEY_beerSlowFilter) == 0){
		tempControl.cc.beerSlowFilter = atol(val);
		tempControl.beerSensor->setSlowFilterCoefficients(tempControl.cc.beerSlowFilter);
	}
	else if(strcmp_P(key,JSONKEY_beerSlopeFilter) == 0){
		tempControl.cc.beerSlopeFilter = atol(val);
		tempControl.beerSensor->setSlopeFilterCoefficients(tempControl.cc.beerSlopeFilter);
	}
	else if(strcmp_P(key,JSONKEY_lightAsHeater) == 0){
		tempControl.cc.lightAsHeater = atol(val)!=0;
	}
	else{
		DEBUG_MSG_1(PSTR("Could not process setting"));
	}
}


	
