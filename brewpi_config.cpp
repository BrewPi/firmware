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
#include "ExternalTempSensor.h"
#include "Ticks.h"
#include "brewpi_avr.h"
#include "config.h"
#include "Sensor.h"
#include "FastDigitalPin.h"
#include "OneWireActuator.h"

#if BREWPI_SIMULATE
#include "simulator.h"

Simulator simulator;
#endif

// global class objects static and defined in class cpp and h files

// instantiate and configure the sensors, actuators and controllers we want to use


void setup(void);
void loop (void);

/* Configure the counter and delay timer. The actual type of these will vary depending upon the environment.
 * They are non-virtual to keep code size minimal, so typedefs and preprocessing are used to select the actual compile-time type used. */
TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);

DisplayType realDisplay;
Display DISPLAY_REF display = realDisplay;

#if BREWPI_EMULATE	// use in-memory/emulated devices
	MockTempSensor directFridgeSensor(10,10);
	MockTempSensor directBeerSensor(5,5);
#elif BREWPI_SIMULATE
	ExternalTempSensor directFridgeSensor(true);
	ExternalTempSensor directBeerSensor(true);
#else  // non emulation - use real hardware devices
	OneWireTempSensor directFridgeSensor(fridgeSensorPin);
	OneWireTempSensor directBeerSensor(beerSensorPin);
#endif

#if BREWPI_EMULATE || BREWPI_SIMULATE
	ValueActuator heater;
	ValueActuator cooler;
	ValueSensor<bool> doorSensor((bool)false);	
#else	
	DigitalPinActuator<heatingPin, SHIELD_INVERT> heater;
	DigitalPinActuator<coolingPin, SHIELD_INVERT> cooler;	
	ValueSensor<bool> doorSensor((bool)false);	
	//DigitalPinSensor<doorPin, SHIELD_INVERT, USE_INTERNAL_PULL_UP_RESISTORS> doorSensor;	
#endif
	
#if LIGHT_AS_HEATER
Actuator& light = heater;
#else
ValueActuator lightOn;	// eventually map the light to a real pin
Actuator& light = lightOn;
#endif

TempSensor fridgeSensor(directFridgeSensor);
TempSensor beerSensor(directBeerSensor);

#if MULTICHAMBER
MockTempSensor directFridgeSensor2(10,10);
MockTempSensor directBeerSensor2(5,5);
#if 1
TempSensor fridgeSensor2(directFridgeSensor2);
TempSensor beerSensor2(directBeerSensor2);
#if 1 || BREWPI_EMULATE		// use emulator for now until we get multi-define one wire
	ValueActuator heat2;
	ValueActuator cool2;		
#endif
#endif
Chamber c1(fridgeSensor, beerSensor, cooler, heater, light, door);
Chamber c2(fridgeSensor2, beerSensor2, cool2, heat2, light, door);

Chamber* chambers[] = {
	&c1
	, &c2
};

ChamberManager chamberManager(chambers, sizeof(chambers)/sizeof(chambers[0]));
#endif

void setup()
{
	piLink.init();

	DEBUG_MSG(PSTR("started"));
	DEBUG_MSG(PSTR("Door at %x %d"), &doorSensor, doorSensor.sense());

#if MULTICHAMBER
	chamberManager.init();	
	for (chamber_id i=0; i<chamberManager.chamberCount(); i++) {
		chamberManager.initChamber(i);
		piLink.printFridgeAnnotation(PSTR("Arduino restarted. Chamber %d ready!"), i+1);
	}
	chamberManager.switchChamber(0);
#else
	tempControl.beerSensor = &beerSensor;
	tempControl.fridgeSensor = &fridgeSensor;
	tempControl.cooler = &cooler;
	tempControl.heater = &heater;
	tempControl.door = &doorSensor;
	tempControl.light = &light;
	DEBUG_MSG(PSTR("TC Door at %x %d"), tempControl.door, tempControl.door->sense());

	tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM
	DEBUG_MSG(PSTR("TC Door at %x %d"), tempControl.door, tempControl.door->sense());
	tempControl.init();
	DEBUG_MSG(PSTR("TC Door at %x %d"), tempControl.door, tempControl.door->sense());
	tempControl.updatePID();
	DEBUG_MSG(PSTR("TC Door at %x %d"), tempControl.door, tempControl.door->sense());
	tempControl.updateState();	
	DEBUG_MSG(PSTR("TC Door at %x %d"), tempControl.door, tempControl.door->sense());
		
#endif
	
#if BREWPI_SIMULATE
	simulator.step();
#endif	
	display.init();
	display.printStationaryText();
	display.printState();
		
	rotaryEncoder.init();
	
	//buzzer.init();
	//buzzer.beep(2, 500);

	DEBUG_MSG(PSTR("init complete"));
}

#if BREWPI_SIMULATE
static fixed7_9 funFactor = 0;	// paused 
static unsigned long lastUpdate = 0;
uint8_t printTempInterval = 5;

void setRunFactor(fixed7_9 factor)
{
	funFactor = factor>>9;		// for now whole values only
	lastUpdate = ::millis();
}	

void updateSimulationTicks() 
{	
#if BREWPI_EMULATE
	ticks.incMillis(1000);
#else	
	if (funFactor) {		
		unsigned long now = ::millis();
		int interval = 1000/funFactor;
		if (interval>0) {
			if ((now-lastUpdate)>=uint16_t(interval)) {
				lastUpdate += interval;
				ticks.incMillis(1000);
			}			
		}			
		else
		{
			lastUpdate = now;
			ticks.incMillis(1000);						
		}
	}
#endif	
}


#endif

void loop(void)
{
	static unsigned long lastUpdate = 0;
	
#if BREWPI_SIMULATE 
	// only needed if we want the arduino to be self running. Useful for manual testing, but not so much with an 
	// external driver. 
	updateSimulationTicks();	
#endif	
	
#if MULTICHAMBER	
	// update period is 1000ms / num chambers, so still 1000ms update for all chambers. 
	static chamber_id nextChamber = 0;
	
	if(ticks.millis() - lastUpdate >= (1000/chamberManager.chamberCount())) { //update settings every second
		nextChamber = (nextChamber+1) % chamberManager.chamberCount();
		DEBUG_MSG(PSTR("loop chamber %d"), nextChamber);
		chamber_id prev = chamberManager.switchChamber(nextChamber);
#else
	if(ticks.millis() - lastUpdate >= (1000)) { //update settings every second
#endif		
		lastUpdate+=1000;
			
		tempControl.updateTemperatures();		
		tempControl.detectPeaks();
		tempControl.updatePID();
		tempControl.updateState();
		tempControl.updateOutputs();

#if MULTICHAMBER
		chamberManager.switchChamber(prev);
#endif		

#if !BREWPI_SIMULATE		// disable rotary encoder since this stalls output
		if(rotaryEncoder.pushed()){
			rotaryEncoder.resetPushed();
			menu.pickSettingToChange();	
		}
#endif

#if BREWPI_SIMULATE && !BREWPI_EMULATE			// simulation on actual hardware
		static byte updateCount = 0;
		if (printTempInterval && (++updateCount%printTempInterval)==0) {
			piLink.printTemperatures();
			updateCount = 0;
		}

		static unsigned long lastDisplayUpdate = 0;  // update the display every second
		if ((::millis()-lastDisplayUpdate)>=1000 && (lastDisplayUpdate+=1000))
#elif MULTI_CHAMBER
		if (prev==nextChamber)
#endif		
		{
			// update the lcd for the chamber being displayed
			DEBUG_MSG(PSTR("update display"));
			display.printState();
			display.printAllTemperatures();
			display.printMode();
			display.updateBacklight();
		}
		
#if BREWPI_SIMULATE
		simulator.step();
#else
		piLink.printTemperatures();
#endif		
	}	
#if BREWPI_SIMULATE && !BREWPI_EMULATE
	static unsigned long lastCheckSerial = 0;
	if ((::millis()-lastCheckSerial)>=1000 && (lastCheckSerial=::millis()>0))	// only listen if 1s passed since last time
#endif	
	//listen for incoming serial connections while waiting to update
	piLink.receive();

}

