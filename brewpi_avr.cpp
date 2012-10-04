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

/*
 * This Atmel Studio 6 project automatically includes all needed Arduino source files, you just have to point it to the right directories.
 * To compile this project on your computer, you will have to set an environment variable to find your local Arduino installation.
 * Set the variable ARDUINO_HOME to point to your local Arduino path, without trailing slash, e.g. 'D:\arduino-1.01'. Instructions on the wiki here:
 * http://wiki.brewpi.com/index.php/Setting_up_the_brewpi-avr_Project
 * 'ArduinoFunctions.cpp' includes all the source files from Arduino that are used. You might have to edit it if you are not using a Leonardo.
 * That is all that is needed! No hassle with makefiles and compiling libraries.
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
