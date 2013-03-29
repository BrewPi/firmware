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

#include "temperatureFormats.h"


class PiLink{
	public:
	
	// There can only be one PiLink object, so functions are static
	static void init(void);
	static void receive(void);
	
	static void printFridgeAnnotation(const char * annotation, ...);
	static void printBeerAnnotation(const char * annotation, ...);
	static void debugMessage(const char * message, ...);
	static void debugMessageDirect(const char * message, ...);
	
	private:
	static void printTemperatures(void);
	
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
	
	static void processJsonPair(char * key, char * val); // process one pair
	
	/* Prints the name part of a json name/value pair. The name must exist in PROGMEM */
	static void printJsonName(const char * name);
	static void printJsonSeparator();
	static void sendJsonClose();
	static void updateInputs();
	private:
	static bool firstPair;
	
};

extern PiLink piLink;


#define DEBUG_MSG(...) piLink.debugMessageDirect(__VA_ARGS__);


#endif /* PILINK_H_ */