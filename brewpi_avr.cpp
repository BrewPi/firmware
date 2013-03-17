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
#include <Arduino.h>
#include "Ticks.h"
#include "LcdDisplay.h"
#include "TempControl.h"
#include "PiLink.h"
#include "Menu.h"
#include "pins.h"
#include "RotaryEncoder.h"
#include "Buzzer.h"
#include "chamber.h"
#include "TempSensor.h"
#include "MockTempSensor.h"
#include "OneWireTempSensor.h"
#include "Ticks.h"
#include "brewpi_avr.h"
#include "config.h"
#include "Sensor.h"

// global class objects static and defined in class cpp and h files

// instantiate and configure the sensors, actuators and controllers we want to use


void setup(void);
void loop (void);

TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);


DisplayType realDisplay;
Display DISPLAY_REF display = realDisplay;

#if BREWPI_EMULATE
MockTempSensor directFridgeSensor(10,10);
MockTempSensor directBeerSensor(5,5);
#else
OneWireTempSensor directFridgeSensor(fridgeSensorPin);
OneWireTempSensor directBeerSensor(beerSensorPin);
#endif
MockTempSensor directFridgeSensor2(10,10);
MockTempSensor directBeerSensor2(5,5);

TempSensor fridgeSensor(directFridgeSensor);
TempSensor beerSensor(directBeerSensor);
TempSensor fridgeSensor2(directFridgeSensor2);
TempSensor beerSensor2(directBeerSensor2);

ValueActuator heat1, heat2;
ValueActuator cool1, cool2;
ValueActuator light;
ValueSensor<boolean> door((boolean)false);

Chamber c1(fridgeSensor, beerSensor, cool1, heat1, light, door);
Chamber c2(fridgeSensor2, beerSensor2, cool2, heat2, light, door);

Chamber* chambers[] = {
	&c1, &c2
};

ChamberManager chamberManager(chambers, sizeof(chambers)/sizeof(chambers[0]));

void setup()
{
	Serial.begin(57600);

	beerSensor2.init();
	beerSensor.init();
		
	DEBUG_MSG(PSTR("started"));

	chamberManager.init();
	
	for (chamber_id i=0; i<chamberManager.chamberCount(); i++) {
		chamberManager.initChamber(i);
		piLink.printFridgeAnnotation(PSTR("Arduino restarted. Chamber %d ready!"), i+1);
	}
	
	chamberManager.switchChamber(0);
	chamberManager.switchChamber(1); // todo - debugging only
		
	wait.millis(2000); // give LCD time to power up
	display.init();
	display.printStationaryText();
	display.printState();
		
	rotaryEncoder.init();
	
	buzzer.init();
	//buzzer.beep(2, 500);

	DEBUG_MSG(PSTR("init complete"));
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
	static unsigned long lastUpdate = 0;
	static chamber_id nextChamber = 0;
	
	if(ticks.millis() - lastUpdate > (1000/chamberManager.chamberCount())) { //update settings every second
		nextChamber = ++nextChamber % chamberManager.chamberCount();
		DEBUG_MSG(PSTR("loop chamber %d"), nextChamber);

		chamber_id prev = chamberManager.switchChamber(nextChamber);
		
		lastUpdate=ticks.millis();
		
		DEBUG_MSG(PSTR("update TC"));
		tempControl.updateTemperatures();		
		DEBUG_MSG(PSTR("update TC peaks"));
		tempControl.detectPeaks();
		DEBUG_MSG(PSTR("update TC pid"));
		tempControl.updatePID();
		DEBUG_MSG(PSTR("update TC state"));
		tempControl.updateState();
		DEBUG_MSG(PSTR("update TC outputs"));
		tempControl.updateOutputs();
		
		DEBUG_MSG(PSTR("switch chamber"));
		chamberManager.switchChamber(prev);
				
				#if 0
		if(rotaryEncoder.pushed()){
			rotaryEncoder.resetPushed();
			menu.pickSettingToChange();	
		}
		#endif
		
		if (prev==nextChamber)
		{
			DEBUG_MSG(PSTR("update display"));
			display.printState();
			display.printAllTemperatures();
			display.printMode();			
		}
	}	
	//listen for incoming serial connections while waiting top update
	piLink.receive();
}

// catch bad interrupts here when debugging
ISR(BADISR_vect){
	DEBUG_MSG(PSTR("*** BASISR_vect ***"));
}

