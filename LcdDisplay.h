/*
 * LcdDisplay.h
 *
 * Created: 15/03/2013 09:21:49
 *  Author: mat
 */ 


#ifndef LCDDISPLAY_H_
#define LCDDISPLAY_H_

#include "Display.h"


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
	
	// print a temperature
	DISPLAY_METHOD void printTemperature(fixed7_9 temp);

	// print the stationary text on the lcd.
	DISPLAY_METHOD void printStationaryText(void);

	// print degree sign + C/F
	DISPLAY_METHOD void printDegreeUnit(void);

	// print mode on the right location on the first line, after Mode:
	DISPLAY_METHOD void printMode(void);

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

	DISPLAY_METHOD void printAt(int x, int y, const char* text) { lcd.setCursor(x, y); lcd.print(text); }
	
	DISPLAY_METHOD void setBufferOnly(bool bufferOnly) {
		lcd.setBufferOnly(bufferOnly);
	}
	
	DISPLAY_METHOD void resetBacklightTimer() { }
	DISPLAY_METHOD void updateBacklight() { }
	
	private:
	DISPLAY_METHOD void printUndefinedTemperature(void);
	
	private:
	DISPLAY_FIELD SpiLcd lcd;
	DISPLAY_FIELD uint8_t stateOnDisplay;
		
};




#endif /* LCDDISPLAY_H_ */