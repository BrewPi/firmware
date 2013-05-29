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

#define MENU_OPTIMIZE 1

#define MENU_TIMEOUT 10u

void Menu::pickSettingToChange(){
	// ensure beer temp is displayed
	uint8_t flags = display.getDisplayFlags();
	display.setDisplayFlags(flags &= ~(LCD_FLAG_ALTERNATE_ROOM|LCD_FLAG_DISPLAY_ROOM));
	pickSettingToChangeLoop();
	display.setDisplayFlags(flags);
}

#if MENU_OPTIMIZE 
/**
 * @return {@code true} if a value was selected. {@code false} on timeout.
 */
bool blinkLoop(
	void (*changed)(),	// called to update the value
	void (*show)(),		// called to show the current value
	void (*hide)(),	// called to blank out the current value
	void (*pushed)())	// handle selection
{	
	uint16_t lastChangeTime = ticks.seconds();
	uint8_t blinkTimer = 0;
	
	while(ticks.timeSince(lastChangeTime) < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			lastChangeTime = ticks.seconds();
			blinkTimer = 0;
			changed();
		}
		if (blinkTimer==0)
			show();
		else if (blinkTimer==128)
			hide();
			
		if (rotaryEncoder.pushed()) {
			rotaryEncoder.resetPushed();
			show();
			pushed();
			return true;
		}
		
		blinkTimer++;
		wait.millis(3); // delay for blinking		
	}
	return false;
}

void clearSettingText() {
	display.printAt_P(0, rotaryEncoder.read(), PSTR("      "));
}

void settingChanged() {} // no -op - the only change is to update the display which happens already

void settingSelected() {
	switch(rotaryEncoder.read()){
		case 0:
			menu.pickMode();
			return;
		case 1:
			// switch to beer constant, because beer setting will be set through display
			tempControl.setMode(MODE_BEER_CONSTANT);
			display.printMode();
			menu.pickBeerSetting();
			return;
		case 2:
			// switch to fridge constant, because fridge setting will be set through display
			tempControl.setMode(MODE_FRIDGE_CONSTANT);
			display.printMode();
			menu.pickFridgeSetting();
			return;
	}	
}

void Menu::pickSettingToChangeLoop(void) {
	rotaryEncoder.setRange(0, 0, 2); // mode setting, beer temp, fridge temp
	blinkLoop(
		settingChanged,
		display.printStationaryText,
		clearSettingText,
		settingSelected
	);
}

void changedMode() {
	const char lookup[] = {'b', 'f', 'p', 'o'};
	tempControl.setMode(lookup[rotaryEncoder.read()]);
}

void clearMode() {
	display.printAt_P(7, 0, PSTR("            "));
}

void selectMode() {
	char mode = tempControl.getMode();
	if(mode ==  MODE_BEER_CONSTANT){
		menu.pickBeerSetting();
	}
	else if(mode == MODE_FRIDGE_CONSTANT){
		menu.pickFridgeSetting();
	}
	else if(mode == MODE_BEER_PROFILE){
		piLink.printBeerAnnotation(PSTR("Changed to profile mode in menu."));
	}
	else if(mode == MODE_OFF){
		piLink.printBeerAnnotation(PSTR("Temp control turned off in menu."));
	}	
}

void Menu::pickMode(void) {	
	char oldSetting = tempControl.getMode();
	uint8_t startValue=0;
	const char* LOOKUP = "bfpo";
	startValue = indexOf(LOOKUP, oldSetting);
	rotaryEncoder.setRange(startValue, 0, 3); // toggle between beer constant, beer profile, fridge constant
	
	if (!blinkLoop(changedMode, display.printMode, clearMode, selectMode)) 
		tempControl.setMode(oldSetting);		
}


#else
void Menu::pickSettingToChangeLoop(void){
	
	rotaryEncoder.setRange(0, 0, 2); // mode setting, beer temp, fridge temp
	uint16_t lastChangeTime = ticks.seconds();
	uint8_t blinkTimer = 0;
	while(ticks.timeSince(lastChangeTime) < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			lastChangeTime = ticks.seconds();
			blinkTimer = 0;		
		}
		if(blinkTimer == 0){
			// print all text again for blinking
			display.printStationaryText();		
		}
		if(blinkTimer == 128){ // blink one of the options by overwriting it with spaces
			display.printAt_P(0, rotaryEncoder.read(), PSTR("      "));
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

const char* LOOKUP = "bfpo";

void Menu::pickMode(void){
	display.printStationaryText(); // restore original text after blinking 'Mode'
	char oldSetting = tempControl.getMode();
	uint8_t startValue=0;
	startValue = indexOf(LOOKUP, oldSetting);
	rotaryEncoder.setRange(startValue, 0, 3); // toggle between beer constant, beer profile, fridge constant
	const char lookup[] = {'b', 'f', 'p', 'o'};
	uint8_t blinkTimer = 0;		
	uint16_t lastChangeTime = ticks.seconds();
	while(ticks.timeSince(lastChangeTime) < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			lastChangeTime = ticks.seconds();
			blinkTimer = 0;
			
			tempControl.setMode(lookup[rotaryEncoder.read()]);
			display.printMode();
			
			if( rotaryEncoder.pushed() ){
				rotaryEncoder.resetPushed();
				char mode = tempControl.getMode();
				if(mode ==  MODE_BEER_CONSTANT){
					menu.pickBeerSetting();
				}
				else if(mode == MODE_FRIDGE_CONSTANT){
					menu.pickFridgeSetting();
				}
				else if(mode == MODE_BEER_PROFILE){
					piLink.printBeerAnnotation(PSTR("Changed to profile mode in menu."));
				}
				else if(mode == MODE_OFF){
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
				clearMode();
			}				
			blinkTimer++;
			wait.millis(3); // delay for blinking
		}
	}
	// Time Out. Restore original setting
	tempControl.setMode(oldSetting);
}

#endif

// tried optimizing this to use blinkLoop but the size becomes larger

typedef void (* PrintAnnotation)(const char * annotation, ...);
typedef void (* DisplayUpdate)(void);
typedef fixed7_9 (* ReadTemp)();
typedef void (* WriteTemp)(fixed7_9);

void pickTempSetting(ReadTemp readTemp, WriteTemp writeTemp, const char* tempName, DisplayUpdate update, PrintAnnotation printAnnoation, int row) {
	
	fixed7_9 oldSetting = readTemp();
	fixed7_9 startVal = oldSetting;
	if(oldSetting == INT_MIN)	 // previous temperature was not defined, start at 20C
		startVal = 20*512;
	
	rotaryEncoder.setRange(fixedToTenths(startVal), fixedToTenths(tempControl.cc.tempSettingMin), fixedToTenths(tempControl.cc.tempSettingMax));

	uint8_t blinkTimer = 0;
	uint16_t lastChangeTime = ticks.seconds();
	while(ticks.timeSince(lastChangeTime) < MENU_TIMEOUT){ // time out at 10 seconds
		if(rotaryEncoder.changed()){
			lastChangeTime = ticks.seconds();
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
				display.printAt_P(12, row, PSTR("     "));
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


#endif