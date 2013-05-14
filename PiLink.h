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

#ifndef PILINK_H_
#define PILINK_H_

#include "brewpi_avr.h"
#include "temperatureFormats.h"
#include "DeviceManager.h"

#ifndef OPTIMIZE_JSON_OUTPUT
#define OPTIMIZE_JSON_OUTPUT 1 && OPTIMIZE_GLOBAL
#endif

#ifndef OPTIMIZE_JSON_OUTPUT_CC
#define OPTIMIZE_JSON_OUTPUT_CC OPTIMIZE_JSON_OUTPUT
#endif
#define OPTIMIZE_JSON_OUTPUT_CV 1 && OPTIMIZE_JSON_OUTPUT

#ifndef OPTIMIZE_JSON_OUTPUT_CV
#define OPTIMIZE_JSON_OUTPUT_CV OPTIMIZE_JSON_OUTPUT
#endif

#ifndef OPTIMIZE_JSON_OUTPUT_CS
#define OPTIMIZE_JSON_OUTPUT_CS OPTIMIZE_JSON_OUTPUT
#endif

class DeviceConfig;


class PiLink{
	public:
	
	// There can only be one PiLink object, so functions are static
	static void init(void);
	static void receive(void);
	
	static void printFridgeAnnotation(const char * annotation, ...);	
	static void printBeerAnnotation(const char * annotation, ...);

	static void debugMessage(const char * message, ...);
	static void debugMessageDirect(const char * message, ...);
	static void printTemperatures(void);
	
	typedef void (*ParseJsonCallback)(const char* key, const char* val, void* data);

	static void parseJson(ParseJsonCallback fn, void* data=NULL);
	
	private:
	
	static void sendControlSettings(void);
	static void receiveControlConstants(void);
	static void sendControlConstants(void);
	static void sendControlVariables(void);
	
	static void receiveJson(void); // receive settings as JSON key:value pairs
	
	static void print(char *fmt, ...); // use when format string is stored in RAM
	static void print(char c) { Serial.print(c); }
	static void print_P(const char *fmt, ...); // use when format string is stored in PROGMEM with PSTR("string")
	static void printChamberCount();
	
	private:
	static void printResponse(char responseChar);
	static void printChamberInfo();
	
	static void printTemperaturesJSON(char * beerAnnotation, char * fridgeAnnotation);
	static void sendJsonPair(const char * name, const char * val); // send one JSON pair with a string value as name:val,
	static void sendJsonPair(const char * name, char val); // send one JSON pair with a char value as name:val,
	static void sendJsonPair(const char * name, uint16_t val); // send one JSON pair with a uint16_t value as name:val,
	static void sendJsonPair(const char * name, uint8_t val); // send one JSON pair with a uint8_t value as name:val,
	
	static void processJsonPair(const char * key, const char * val, void* pv); // process one pair
	
	/* Prints the name part of a json name/value pair. The name must exist in PROGMEM */
	static void printJsonName(const char * name);
	static void printJsonSeparator();
	static void sendJsonClose();
	
	static void openListResponse(char type);
	static void closeListResponse();

#if OPTIMIZE_JSON_OUTPUT
	struct JsonOutput {
		const char* key;			// JSON key
		uint8_t offset;			// offset into TempControl class
		uint8_t handlerOffset;		// handler index
	};
	typedef void (*JsonOutputHandler)(const char* key, uint8_t offset);

	// handler functions for JSON output
	static void jsonOutputUint8(const char* key, uint8_t offset);
	static void jsonOutputTempToString(const char* key, uint8_t offset);
	static void jsonOutputFixedPointToString(const char* key, uint8_t offset);
	static void jsonOutputTempDiffToString(const char* key, uint8_t offset);
	static void jsonOutputChar(const char* key, uint8_t offset);
	static const JsonOutputHandler JsonOutputHandlers[];		
#if OPTIMIZE_JSON_OUTPUT_CC	
	static const JsonOutput jsonOutputCCMap[];
#endif	
#if OPTIMIZE_JSON_OUTPUT_CV
	static const JsonOutput jsonOutputCVMap[];
#endif
	
#endif	
	
	
#if BREWPI_SIMULATE	
	static void updateInputs();
	public:
	static void printSimulatorSettings();
	private:
	static void sendJsonPair(const char* name, double val);
	static void printDouble(double val);
#endif	
	private:
	static bool firstPair;
	friend class DeviceManager;
	friend class PiLinkTest;
};

extern PiLink piLink;

#ifndef BREWPI_DEBUG
#define BREWPI_DEBUG 0
#endif

#if BREWPI_DEBUG
#define DEBUG_MSG(...) piLink.debugMessageDirect(__VA_ARGS__);
#else
#define DEBUG_MSG(...)
#endif


#endif /* PILINK_H_ */