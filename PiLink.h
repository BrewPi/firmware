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

class TempControl;
class PiLink{
	public:
	//PiLink(TempControl& control) : tempControl(control) { }
	// There can only be one PiLink object, so functions are static
	void receive(void);
	
	void print(char *fmt, ...); // use when format string is stored in RAM
	void print_P(const char *fmt, ...); // use when format string is stored in PROGMEM with PSTR("string")
	
	void printTemperatures(void);
	void printBeerAnnotation(const char * annotation, ...);
	void printFridgeAnnotation(const char * annotation, ...);
	void debugMessage(const char * message, ...);
	
	void sendControlSettings(void);
	void receiveControlConstants(void);
	void sendControlConstants(void);
	void sendControlVariables(void);
	
	void receiveJson(void); // receive settings as JSON key:value pairs
	
	
	private:
	void printTemperaturesJSON(char * beerAnnotation, char * fridgeAnnotation);
	void sendJsonPair(const char * name, char * val); // send one JSON pair with a string value as name:val,
	void sendJsonPair(const char * name, char val); // send one JSON pair with a char value as name:val,
	void sendJsonPair(const char * name, uint16_t val); // send one JSON pair with a uint16_t value as name:val,
	void processJsonPair(char * key, char * val); // process one pair
	
	//TempControl& tempControl;
};

extern PiLink piLink;

#endif /* PILINK_H_ */