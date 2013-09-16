/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later v7ersion.
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
#include "BrewpiStrings.h"
#include <limits.h>

#include "Display.h"
#include "DisplayLcd.h"
#include "Menu.h"
#include "TempControl.h"
#include "TemperatureFormats.h"
#include "Pins.h"


uint8_t LcdDisplay::stateOnDisplay;
uint8_t LcdDisplay::flags;
LcdDriver LcdDisplay::lcd;

// Constant strings used multiple times
static const char STR_Beer_[] PROGMEM = "Beer ";
static const char STR_Fridge_[] PROGMEM = "Fridge ";
static const char STR_Const_[] PROGMEM = "Const.";
static const char STR_for[] PROGMEM = "for";
static const char STR_Waiting_to_[] PROGMEM = "Waiting to ";
static const char STR_Cooling_[] PROGMEM = "Cooling ";
static const char STR_Heating_[] PROGMEM = "Heating ";
static const char STR_min_time[] PROGMEM = "min time";
static const char STR_empty_string[] PROGMEM = "";

void LcdDisplay::init(void){
	stateOnDisplay = 0xFF; // set to unknown state to force update
	flags = LCD_FLAG_ALTERNATE_ROOM;
	lcd.init(); // initialize LCD
	lcd.begin(20, 4);
	lcd.clear();
}

//print all temperatures on the LCD
void LcdDisplay::printAllTemperatures(void){
	// alternate between beer and room temp
	if (flags & LCD_FLAG_ALTERNATE_ROOM) {
		bool displayRoom = ((ticks.seconds()&0x08)==0) && !BREWPI_SIMULATE && tempControl.ambientSensor->isConnected();
		if (displayRoom ^ ((flags & LCD_FLAG_DISPLAY_ROOM)!=0)) {	// transition
			flags = displayRoom ? flags | LCD_FLAG_DISPLAY_ROOM : flags & ~LCD_FLAG_DISPLAY_ROOM;
			printStationaryText();
		}
	}
	
	printBeerTemp();
	printBeerSet();
	printFridgeTemp();
	printFridgeSet();
}

void LcdDisplay::setDisplayFlags(uint8_t newFlags) {
	flags = newFlags;
	printStationaryText();
	printAllTemperatures();
}



void LcdDisplay::printBeerTemp(void){
	printTemperatureAt(6, 1, tempControl.getBeerTemp());
}

void LcdDisplay::printBeerSet(void){
	fixed7_9 beerSet = tempControl.getBeerSetting();	
	printTemperatureAt(12, 1, beerSet);	
}

void LcdDisplay::printFridgeTemp(void){	
	printTemperatureAt(6,2, flags & LCD_FLAG_DISPLAY_ROOM ?
		tempControl.ambientSensor->read() :
		tempControl.getFridgeTemp());
}

void LcdDisplay::printFridgeSet(void){	
	fixed7_9 fridgeSet = tempControl.getFridgeSetting();	
	if(flags & LCD_FLAG_DISPLAY_ROOM) // beer setting is not active
		fridgeSet = INT_MIN;
	printTemperatureAt(12, 2, fridgeSet);	
}

void LcdDisplay::printTemperatureAt(uint8_t x, uint8_t y, fixed7_9 temp){
	lcd.setCursor(x,y);
	printTemperature(temp);
}


void LcdDisplay::printTemperature(fixed7_9 temp){
	if (temp==INT_MIN)
	{
		lcd.print_P(PSTR(" --.-"));
		return;
	}
	char tempString[9];
	tempToString(tempString, temp, 1 , 9);
	int8_t spacesToWrite = 5 - (int8_t) strlen(tempString); 
	for(int8_t i = 0; i < spacesToWrite ;i++){
		lcd.write(' ');
	}
	lcd.print(tempString);
}

//print the stationary text on the lcd.
void LcdDisplay::printStationaryText(void){
	printAt_P(0, 0, PSTR("Mode"));
	printAt_P(0, 1, STR_Beer_);
	printAt_P(0, 2, (flags & LCD_FLAG_DISPLAY_ROOM) ?  PSTR("Room  ") : STR_Fridge_); 
	printDegreeUnit(18, 1);
	printDegreeUnit(18, 2);
}

//print degree sign + temp unit
void LcdDisplay::printDegreeUnit(uint8_t x, uint8_t y){
	lcd.setCursor(x,y);
	lcd.write(0b11011111);
	lcd.write(tempControl.cc.tempFormat);	
}

void LcdDisplay::printAt_P(uint8_t x, uint8_t y, const char* text){
	lcd.setCursor(x, y);
	lcd.print_P(text);
}


void LcdDisplay::printAt(uint8_t x, uint8_t y, char* text){
	lcd.setCursor(x, y);
	lcd.print(text);
}

// print mode on the right location on the first line, after "Mode   "
void LcdDisplay::printMode(void){
	lcd.setCursor(7,0);
	// Factoring prints out of switch has negative effect on code size in this function
	switch(tempControl.getMode()){
		case MODE_FRIDGE_CONSTANT:
			lcd.print_P(STR_Fridge_);
			lcd.print_P(STR_Const_);
			break;
		case MODE_BEER_CONSTANT:
			lcd.print_P(STR_Beer_);
			lcd.print_P(STR_Const_);
			break;
		case MODE_BEER_PROFILE:
			lcd.print_P(STR_Beer_);
			lcd.print_P(PSTR("Profile"));
			break;
		case MODE_OFF:
			lcd.print_P(PSTR("Off"));
			break;
		case MODE_TEST:
			lcd.print_P(PSTR("** Testing **"));
			break;
		default:
			lcd.print_P(PSTR("Invalid mode"));
			break;
	}
	lcd.printSpacesToRestOfLine();
}

// print the current state on the last line of the lcd
void LcdDisplay::printState(void){
	uint16_t time;
	uint8_t state = tempControl.getDisplayState();
	uint8_t counterPrintPos=0;
	if(state != stateOnDisplay){ //only print static text when state has changed
		stateOnDisplay = state;
		// Reprint state and clear rest of the line
		const char * part1 = STR_empty_string;
		const char * part2 = STR_empty_string;
		switch (state){
			case IDLE:
				part1 = PSTR("Idle ");
				part2 = STR_for;
				break;
			case WAITING_TO_COOL:
				part1 = STR_Waiting_to_;
				part2 = PSTR("cool");
				break;
			case WAITING_TO_HEAT:
				part1 = STR_Waiting_to_;
				part2 = PSTR("heat");
				break;
			case WAITING_FOR_PEAK_DETECT:
				part1 = PSTR("Awaiting peak detect");
				break;
			case COOLING:
				part1 = STR_Cooling_;
				part2 = STR_for;
				break;
			case HEATING:
				part1 = STR_Heating_;
				part2 = STR_for;
				break;
			case COOLING_MIN_TIME:
				part1 = STR_Cooling_;
				part2 = STR_min_time;
				break;
			case HEATING_MIN_TIME:
				part1 = STR_Heating_;
				part2 = STR_min_time;
				break;
			case DOOR_OPEN:
				part1 = PSTR("Door open");
				break;
			case STATE_OFF:
				part1 = PSTR("Temp. control OFF");
				break;
			default:
				part1 = PSTR("Unknown status!");
				break;
		}
		printAt_P(0, 3, part1);
		lcd.print_P(part2);		
		lcd.printSpacesToRestOfLine();
	}
	uint16_t sinceIdleTime = tempControl.timeSinceIdle();
	if(state==IDLE){
		counterPrintPos = 9;
		time = 	min(tempControl.timeSinceCooling(), tempControl.timeSinceHeating());
	}
	else if(state==COOLING || state==HEATING){
		counterPrintPos = 12;
		time = sinceIdleTime;
	}
	else if(state==COOLING_MIN_TIME){
		counterPrintPos = 17;
		time = MIN_COOL_ON_TIME-sinceIdleTime;
	}
	
	else if(state==HEATING_MIN_TIME){
		counterPrintPos = 17;
		time = MIN_HEAT_ON_TIME-sinceIdleTime;
	}
	else if(state == WAITING_TO_COOL || state == WAITING_TO_HEAT){
		counterPrintPos = 16;
		time = tempControl.getWaitTime();
	}
	if(counterPrintPos > 0){
		char timeString[10];
		sprintf_P(timeString,STR_FMT_U, (unsigned int) time); // cheaper than itoa, because it overlaps with vsnprintf
		printAt(counterPrintPos, 3, timeString);
		lcd.printSpacesToRestOfLine(); // overwrite previous numbers that had more digits
	}
}