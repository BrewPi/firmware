#pragma once

#include "Brewpi.h"
#include "DisplayBase.h"
#include "SpiLcd.h"


class LcdDisplay DISPLAY_SUPERCLASS
{
	public:
	// initializes the lcd display
	DISPLAY_METHOD void init(void);
	
	DISPLAY_METHOD void printAll() {
		printStationaryText();
		printState();
		printAllTemperatures();
		printMode();
	}
	
	// print all temperatures on the LCD
	DISPLAY_METHOD void printAllTemperatures(void);
	
	// print the stationary text on the lcd.
	DISPLAY_METHOD void printStationaryText(void);

	// print mode on the right location on the first line, after Mode:
	DISPLAY_METHOD void printMode(void);

	DISPLAY_METHOD void setDisplayFlags(uint8_t flags);
	DISPLAY_METHOD uint8_t getDisplayFlags(){ return flags; };

	// print beer temperature at the right place on the display
	DISPLAY_METHOD void printBeerTemp(void);

	// print beer temperature setting at the right place on the display
	DISPLAY_METHOD void printBeerSet(void);

	// print fridge temperature at the right place on the display
	DISPLAY_METHOD void printFridgeTemp(void);

	// print fridge temperature setting at the right place on the display
	DISPLAY_METHOD void printFridgeSet(void);

	// print the current state on the last line of the LCD
	DISPLAY_METHOD void printState(void);

	DISPLAY_METHOD void getLine(uint8_t lineNumber, char * buffer) { lcd.getLine(lineNumber, buffer); }

	DISPLAY_METHOD void printAt(uint8_t x, uint8_t y, const char* text) { lcd.setCursor(x, y); 	lcd.print_P(text); }
	
	DISPLAY_METHOD void setBufferOnly(bool bufferOnly) {
		lcd.setBufferOnly(bufferOnly);
	}
	
	DISPLAY_METHOD void resetBacklightTimer() { }
	DISPLAY_METHOD void updateBacklight() { }
	
	private:
	// print a temperature
	DISPLAY_METHOD void printTemperature(fixed7_9 temp);

	// print degree sign + C/F
	DISPLAY_METHOD void printDegreeUnit(uint8_t x, uint8_t y);
		
	private:
	DISPLAY_FIELD SpiLcd lcd;
	DISPLAY_FIELD uint8_t stateOnDisplay;
	DISPLAY_FIELD uint8_t flags;
		
};


