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

/* This Atmel Studio 6 project automatically includes all needed Arduino source files, you just have to point it to the right directories.
 * To compile this project on your computer, go the the project properties and change the following options:
 * AVR/GNU C++ Compiler --> Directories. Change the two directories to point to your local Arduino installation. 
 * For the second directory, choose the Arduino variant that you are using.
 * You might have to edit 'ArduinoFunctions.cpp' if you are not using a Leonardo.
		D:\arduino-1.0.1\hardware\arduino\cores\arduino
		D:\arduino-1.0.1\hardware\arduino\variants\leonardo
 * That is all that is needed! No hassle with makefiles.
 */

#include <arduino.h>

#include "Display.h"
#include "TempControl.h"
#include "PiLink.h"
#include "Menu.h"
#include "pins.h"
#include "RotaryEncoder.h"

// global class opbjects static and defined in class cpp and h files

void setup(void);
void loop (void);

void setup()
{
	
	Serial.begin(57600);
	
	pinMode(coolingPin, OUTPUT);
	pinMode(heatingPin, OUTPUT);
	
	#if(USE_INTERNAL_PULL_UP_RESISTORS)
		pinMode(doorPin, INPUT_PULLUP);
	#else
		pinMode(doorPin, INPUT);
	#endif
	
	tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM
	
	delay(2000); // give LCD time to power up
	
	tempControl.init();
	
	display.init();
	display.printStationaryText();
	display.printState();
	
	
	tempControl.updatePID();
	tempControl.updateState();
	
	rotaryEncoder.init();
	
	piLink.printFridgeAnnotation(PSTR("Arduino restarted!"));
}

void main() __attribute__ ((noreturn)); // tell the compiler main doesn't return.

void main(void)
{
	init();

	#if defined(USBCON)
	USBDevice.attach();
	#endif
	
	setup();
	
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
}

void loop(void)
{
	static unsigned long lastUpdate;
	if(millis() - lastUpdate > 1000){ //update settings every second
		lastUpdate=millis();
		
		tempControl.updateTemperatures();		
		tempControl.detectPeaks();
		tempControl.updatePID();
		tempControl.updateState();
		tempControl.updateOutputs();
		
		display.printState();
		display.printAllTemperatures();
		display.printMode();
	}
	if(rotaryEncoder.pushed()){
		rotaryEncoder.resetPushed();
		menu.pickSettingToChange();		
	}
	
	//listen for incoming serial connections while waiting top update
	piLink.receive();
}

/* catch bad interrupts here when debugging
ISR(BADISR_vect){
	while(1){
		;
	}
}
*/
