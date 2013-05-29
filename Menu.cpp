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

#include "Brewpi.h"
#include "BrewpiStrings.h"

#if BREWPI_MENU

#include "Menu.h"

#include <limits.h>
#include "Pins.h"
#include "Display.h"
#include "TempControl.h"
#include "TemperatureFormats.h"
#include "RotaryEncoder.h"
#include "PiLink.h"
#include "Ticks.h"

Menu menu;

void Menu::pickSettingToChange(){
	// ensure beer temp is displayed
	uint8_t flags = display.getDisplayFlags();
	display.setDisplayFlags(flags &= ~(LCD_FLAG_ALTERNATE_ROOM|LCD_FLAG_DISPLAY_ROOM));
	pickSettingToChangeLoop();
	display.setDisplayFlags(flags);
}

void Menu::pickSettingToChangeLoop(void){
	
	rotaryEncoder.setRange(0, 0, 2); // mode setting, beer temp, fridge temp
	unsigned long timer = ticks.millis();
	uint8_t blinkTimer = 0;
	while((ticks.millis()) - timer < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			timer=ticks.millis();
			blinkTimer = 0;		
		}
		if(blinkTimer == 0){
			// print all text again for blinking
			display.printStationaryText();		
		}
		if(blinkTimer == 128){ // blink one of the options by overwriting it with spaces
			display.printAt_P(0, rotaryEncoder.read(), STR_SPACES_END-6);
		}
		if( rotaryEncoder.pushed() ){
			rotaryEncoder.resetPushed();
			switch(rotaryEncoder.read()){
				case 0:
					pickMode();
					return;
				case 1:
					// switch to beer constant, because beer setting will be set through display
					tempControl.setMode(MODE_BEER_CONSTANT);
					display.printMode();
					pickBeerSetting();
					return;
				case 2:
					// switch to fridge constant, because fridge setting will be set through display
					tempControl.setMode(MODE_FRIDGE_CONSTANT);
					display.printMode();
					pickFridgeSetting();
					return;
			}
		}
		
		blinkTimer++;
		wait.millis(3); // delay for blinking
	}
}

void Menu::initRotaryWithTemp(fixed7_9 oldSetting){
	fixed7_9 startVal;
	if(oldSetting == INT_MIN){ // previous temperature was not defined, start at 20C
		startVal = 20*512;
	}
	else{
		startVal = oldSetting;
	}
	rotaryEncoder.setRange(fixedToTenths(startVal), fixedToTenths(tempControl.cc.tempSettingMin), fixedToTenths(tempControl.cc.tempSettingMax));
}

void Menu::pickMode(void){
	display.printStationaryText(); // restore original text after blinking 'Mode'
	char oldSetting = tempControl.getMode();
	uint8_t startValue=0;
	switch(oldSetting){
		case 'b':
			startValue = 0;
			break;
		case 'f':
			startValue = 1;
			break;
		case 'p':
			startValue = 2;
			break;
	}
	rotaryEncoder.setRange(startValue, 0, 3); // toggle between beer constant, beer profile, fridge constant
	const char lookup[] = {'b', 'f', 'p', 'o'};
	uint8_t blinkTimer = 0;
	unsigned long timer = ticks.millis();
	while((ticks.millis()) - timer < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			timer=ticks.millis();
			blinkTimer = 0;
			
			tempControl.setMode(lookup[rotaryEncoder.read()]);
			display.printMode();
			if( rotaryEncoder.pushed() ){
				rotaryEncoder.resetPushed();
				
				if(tempControl.getMode() ==  MODE_BEER_CONSTANT){
					menu.pickBeerSetting();
				}
				else if(tempControl.getMode() == MODE_FRIDGE_CONSTANT){
					menu.pickFridgeSetting();
				}
				else if(tempControl.getMode() == MODE_BEER_PROFILE){
					piLink.printBeerAnnotation(PSTR("Changed to profile mode in menu."));
				}
				else if(tempControl.getMode() == MODE_OFF){
					piLink.printBeerAnnotation(PSTR("Temp control turned off in menu."));
				}						
				return;
			}
		}
		else{
			if(blinkTimer == 0){
				display.printMode();
			}
			if(blinkTimer == 128){
				display.printAt_P(7, 0, STR_SPACES_END-13);
			}				
			blinkTimer++;
			wait.millis(3); // delay for blinking
		}
	}
	// Time Out. Restore original setting
	tempControl.setMode(oldSetting);
}

#define OPT_TEMP_SETTING 1

#if OPT_TEMP_SETTING
typedef void (* PrintAnnotation)(const char * annotation, ...);
typedef void (* DisplayUpdate)(void);
typedef fixed7_9 (* ReadTemp)();
typedef void (* WriteTemp)(fixed7_9);

void pickTempSetting(ReadTemp readTemp, WriteTemp writeTemp, const char* tempName, DisplayUpdate update, PrintAnnotation printAnnoation, int row) {
	display.printStationaryText(); // restore original text after blinking
	fixed7_9 oldSetting = readTemp();
	menu.initRotaryWithTemp(oldSetting);
	
	uint8_t blinkTimer = 0;
	unsigned long timer = ticks.millis();
	while((ticks.millis()) - timer < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			timer=ticks.millis();
			blinkTimer = 0;
			
			writeTemp(tenthsToFixed(rotaryEncoder.read()));
			update();
			
			if( rotaryEncoder.pushed() ){
				rotaryEncoder.resetPushed();
				char tempString[9];
				printAnnoation(PSTR("%S temp set to %s in Menu."), tempName, tempToString(tempString,readTemp(),1,9));
				return;
			}
		}
		else{
			if(blinkTimer == 0){
				update();
			}
			if(blinkTimer == 128){
				display.printAt_P(12, row, STR_SPACES_END-5);
			}
			blinkTimer++;
			wait.millis(3); // delay for blinking
		}
	}
	// Time Out. Restore original setting
	writeTemp(oldSetting);
	
}

void Menu::pickFridgeSetting(void){
	pickTempSetting(tempControl.getFridgeSetting, tempControl.setFridgeTemp, PSTR("Fridge"), display.printFridgeSet, piLink.printFridgeAnnotation, 2);
}

void Menu::pickBeerSetting(void){
	pickTempSetting(tempControl.getBeerSetting, tempControl.setBeerTemp, PSTR("Beer"), display.printBeerSet, piLink.printBeerAnnotation, 1);
}


#else

void Menu::pickBeerSetting(void){
	display.printStationaryText(); // restore original text after blinking
	fixed7_9 oldSetting = tempControl.getBeerSetting();
	initRotaryWithTemp(oldSetting);
	
	uint8_t blinkTimer = 0;
	unsigned long timer = ticks.millis();
	while((ticks.millis()) - timer < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			timer=ticks.millis();
			blinkTimer = 0;
			
			tempControl.setBeerTemp(tenthsToFixed(rotaryEncoder.read()));			
			display.printBeerSet();
			if( rotaryEncoder.pushed() ){
				rotaryEncoder.resetPushed();
				char tempString[9];
				piLink.printBeerAnnotation(PSTR("Beer temp set to %s in Menu."), tempToString(tempString,tempControl.getBeerSetting(),1,9));
				return;
			}
		}
		else{
			if(blinkTimer == 0){
				display.printBeerSet();
			}
			if(blinkTimer == 128){
				display.printAt(12,1, STR_NULL, 5);
			}				
			blinkTimer++;
			wait.millis(3); // delay for blinking
		}
	}
	// Time Out. Restore original setting
	tempControl.setBeerTemp(oldSetting);
}

void Menu::pickFridgeSetting(void){
	display.printStationaryText(); // restore original text after blinking
	fixed7_9 oldSetting = tempControl.getFridgeSetting();
	initRotaryWithTemp(oldSetting);
	
	uint8_t blinkTimer = 0;
	unsigned long timer = ticks.millis();
	while((ticks.millis()) - timer < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			timer=ticks.millis();
			blinkTimer = 0;
			
			tempControl.setFridgeTemp(tenthsToFixed(rotaryEncoder.read()));			
			display.printFridgeSet();
			if( rotaryEncoder.pushed() ){
				rotaryEncoder.resetPushed();
				char tempString[9];
				piLink.printFridgeAnnotation(PSTR("Fridge temp set to %s in Menu."), tempToString(tempString,tempControl.getFridgeSetting(),1,9));
				return;
			}
		}
		else{
			if(blinkTimer == 0){
				display.printFridgeSet();
			}
			if(blinkTimer == 128){
				display.printAt(12, 2, PSTR("     "));
			}				
			blinkTimer++;
			wait.millis(3); // delay for blinking
		}
	}
	// Time Out. Restore original setting
	tempControl.setFridgeTemp(oldSetting);
}
#endif

#endif