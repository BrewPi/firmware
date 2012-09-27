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

#include <Arduino.h>
#include <limits.h>

#include "Display.h"
#include "Menu.h"
#include "TempControl.h"
#include "temperatureFormats.h"
#include "pins.h"

Display display;

void Display::init(void){
	content[0][20]='\0';
	content[1][20]='\0';
	content[2][20]='\0';
	content[3][20]='\0';
	stateOnDisplay = 0xFF; // set to unknown state to force update
	
	oled.init(DISP_RS, DISP_RW, DISP_EN, DISP_D4, DISP_D5, DISP_D6, DISP_D7); // initialize OLED with these pins
	oled.begin(20, 4);
	oled.clear();
}

//print all temperatures on the LCD
void Display::printAllTemperatures(void){
	printBeerTemp();
	printBeerSet();
	printFridgeTemp();
	printFridgeSet();
}

void Display::printBeerTemp(void){
	oled.setCursor(6,1);
	if(tempControl.beerSensor.isConnected()){
		printTemperature(tempControl.getBeerTemp());
	}
	else{
		oled.print_P(PSTR(" --.-"));
	}
}

void Display::printBeerSet(void){
	oled.setCursor(12,1);
	fixed7_9 beerSet = tempControl.getBeerSetting();
	if(beerSet == INT_MIN){ // beer setting is not active
		oled.print_P(PSTR(" --.-"));	
	}
	else{
		printTemperature(beerSet);	
	}		
}

void Display::printFridgeTemp(void){	
	oled.setCursor(6,2);
	if(tempControl.fridgeSensor.isConnected()){
		printTemperature(tempControl.getFridgeTemp());
	}
	else{
		oled.print_P(PSTR(" --.-"));
	}
}

void Display::printFridgeSet(void){	
	oled.setCursor(12,2);
	fixed7_9 fridgeSet = tempControl.getFridgeSetting();
	if(fridgeSet == INT_MIN){ // beer setting is not active
		oled.print_P(PSTR(" --.-"));	
	}
	else{
		printTemperature(fridgeSet);
	}		
}

void Display::printTemperature(fixed7_9 temp){
	char tempString[9];
	tempToString(tempString, temp, 1 , 9);
	for(int i = 0; i<(5-strlen(tempString));i++){
		oled.write(' ');
	}
	oled.print(tempString);
}

//print the stationary text on the oled.
void Display::printStationaryText(void){
	oled.setCursor(0,0);
	oled.print_P(PSTR("Mode   "));

	oled.setCursor(0,1);
	oled.print_P(PSTR("Beer   "));
	
	oled.setCursor(0,2);
	oled.print_P(PSTR("Fridge ")); 
	
	oled.setCursor(18,1);
	printDegreeUnit();
	oled.setCursor(18,2);
	printDegreeUnit();
}

//print degree sign + C
void Display::printDegreeUnit(void){
	oled.write(0b11011111);
	oled.write(tempControl.cc.tempFormat);	
}

// print mode on the right location on the first line, after "Mode   "
void Display::printMode(void){
	oled.setCursor(7,0);
	switch(tempControl.getMode()){
		case MODE_FRIDGE_CONSTANT:
			oled.print_P(PSTR("Fridge Const."));
			break;
		case MODE_BEER_CONSTANT:
			oled.print_P(PSTR("Beer Constant"));
			break;
		case MODE_BEER_PROFILE:
			oled.print_P(PSTR("Beer Profile "));
			break;
		case MODE_OFF:
			oled.print_P(PSTR("Off          "));
			break;
		default:
			oled.print_P(PSTR("Invalid mode "));
			break;
	}
}

// print the current state on the last line of the lcd
void Display::printState(void){
	uint8_t state = tempControl.getState();
	if(state != stateOnDisplay){ //only print static text when state has changed
		oled.setCursor(0,3); 
		// Reprint state and clear rest of the line
		switch (tempControl.getState()){
			case IDLE:
				oled.print_P(PSTR("Idle for            "));	
			break;
			case STARTUP:
				oled.print_P(PSTR("Starting up...      "));
			break;
			case COOLING:
				oled.print_P(PSTR("Cooling for         "));
			break;
			case HEATING:
				oled.print_P(PSTR("Heating for         "));
			break;
			case DOOR_OPEN:
				oled.print_P(PSTR("Door open           "));
			break;
			case STATE_OFF:
				oled.print_P(PSTR("Temp. control OFF   "));
			break;
			default:
				oled.print_P(PSTR("Unknown status!     "));
			break;
		}
		stateOnDisplay = state;
	}
	switch(state){
		case IDLE:
			oled.setCursor(9,3);
			oled.print(min(tempControl.timeSinceCooling(), tempControl.timeSinceHeating())/1000);
			oled.print_P(PSTR(" s"));
		break;
		case COOLING:
		case HEATING:
			oled.setCursor(12,3);
			oled.print(tempControl.timeSinceIdle()/1000);
			oled.print_P(PSTR(" s"));
		break;
    }
}

void Display::readContent(void){
	oled.setCursor(0,0);
	for(uint8_t i =0;i<20;i++){
		content[0][i] = oled.readChar();
		if(content[0][i] == 0b11011111){
			content[0][i] = 0xB0; // correct degree sign
		}
	}
	for(uint8_t i =0;i<20;i++){
		content[2][i] = oled.readChar();
		if(content[2][i] == 0b11011111){
			content[2][i] = 0xB0; // correct degree sign
		}
	}
	oled.setCursor(0,1);
	for(uint8_t i =0;i<20;i++){
		content[1][i] = oled.readChar();
		if(content[1][i] == 0b11011111){
			content[1][i] = 0xB0; // correct degree sign
		}
	}
	for(uint8_t i =0;i<20;i++){
		content[3][i] = oled.readChar();
		if(content[3][i] == 0b11011111){
			content[3][i] = 0xB0; // correct degree sign
		}
	}
}