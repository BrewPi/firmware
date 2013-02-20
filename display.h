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

#ifndef C_lcd_H_
#define C_lcd_H_

#include "pins.h"
#include "SpiLcd.h"
#include "temperatureFormats.h"

class Display{
	public:
	Display(){};
	~Display(){};
		
	// initializes the lcd display
	virtual void init(void)=0;
			
	// print all temperatures on the LCD
	virtual void printAllTemperatures(void)=0;
	
	// print a temperature
	virtual void printTemperature(fixed7_9 temp)=0;

	// print the stationary text on the lcd.
	virtual void printStationaryText(void)=0;

	// print degree sign + C/F
	virtual void printDegreeUnit(void)=0;

	// print mode on the right location on the first line, after Mode:
	virtual void printMode(void)=0;

	// print beer temperature at the right place on the display
	virtual void printBeerTemp(void)=0;

	// print beer temperature setting at the right place on the display
	virtual void printBeerSet(void)=0;

	// print fridge temperature at the right place on the display
	virtual void printFridgeTemp(void)=0;

	// print fridge temperature setting at the right place on the display
	virtual void printFridgeSet(void)=0;

	// print the current state on the last line of the LCD
	virtual void printState(void)=0;

	virtual void printAt(int x, int y, const char* text)=0;
	
	virtual void getLine(uint8_t lineNumber, char * buffer)=0;
};

class LcdDisplay : public Display
{
	public:
	// initializes the lcd display
	virtual void init(void);
	
	// print all temperatures on the LCD
	virtual void printAllTemperatures(void);
	
	// print a temperature
	virtual void printTemperature(fixed7_9 temp);

	// print the stationary text on the lcd.
	virtual void printStationaryText(void);

	// print degree sign + C/F
	virtual void printDegreeUnit(void);

	// print mode on the right location on the first line, after Mode:
	virtual void printMode(void);

	// print beer temperature at the right place on the display
	virtual void printBeerTemp(void);

	// print beer temperature setting at the right place on the display
	virtual void printBeerSet(void);

	// print fridge temperature at the right place on the display
	virtual void printFridgeTemp(void);

	// print fridge temperature setting at the right place on the display
	virtual void printFridgeSet(void);

	// print the current state on the last line of the LCD
	virtual void printState(void);

	// print the static text of a menu page
	virtual void printMenu(int menuPage);

	virtual void readContent(void);

	virtual void getLine(uint8_t lineNumber, char * buffer);
	
	virtual void printAt(int x, int y, const char* text);
	
	private:
	SpiLcd lcd;
	uint8_t stateOnDisplay;
};

class NullDisplay : public Display
{
public:	
	// initializes the lcd display
	virtual void init(void){}
	
	// print all temperatures on the LCD
	virtual void printAllTemperatures(void){}
	
	// print a temperature
	virtual void printTemperature(fixed7_9 temp){}

	// print the stationary text on the lcd.
	virtual void printStationaryText(void){}

	// print degree sign + C/F
	virtual void printDegreeUnit(void){}

	// print mode on the right location on the first line, after Mode:
	virtual void printMode(void){}

	// print beer temperature at the right place on the display
	virtual void printBeerTemp(void){}

	// print beer temperature setting at the right place on the display
	virtual void printBeerSet(void){}

	// print fridge temperature at the right place on the display
	virtual void printFridgeTemp(void){}

	// print fridge temperature setting at the right place on the display
	virtual void printFridgeSet(void){}

	// print the current state on the last line of the LCD
	virtual void printState(void){}
		
	virtual void getLine(uint8_t lineNumber, char * buffer) {
		for (int i=0; i<20; i++)
			buffer[i] = ' ';
		buffer[20] = 0;
	}
		
	virtual void printAt(int x, int y, const char* text) {}

};
	
extern Display& display;

#endif /* C_lcd_H_ */