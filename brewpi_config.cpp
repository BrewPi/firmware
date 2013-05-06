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
#include "brewpi_avr.h"
#include "Ticks.h"
#include "Display.h"
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
#include "Sensor.h"
#include "FastDigitalPin.h"
#include "OneWireActuator.h"

#if BREWPI_SIMULATE
#include "simulator.h"

Simulator simulator;
#endif

#include "SettingsManager.h"

// global class objects static and defined in class cpp and h files

// instantiate and configure the sensors, actuators and controllers we want to use


void setup(void);
void loop (void);

/* Configure the counter and delay timer. The actual type of these will vary depending upon the environment.
 * They are non-virtual to keep code size minimal, so typedefs and preprocessing are used to select the actual compile-time type used. */
TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);

DisplayType realDisplay;
DisplayType DISPLAY_REF display = realDisplay;


void setup()
{
	piLink.init();

	DEBUG_MSG(PSTR("started"));
	SettingsManager::loadSettings();
	
#if BREWPI_SIMULATE
	simulator.step();
#endif	
	display.init();
	display.printStationaryText();
	display.printState();
		
	rotaryEncoder.init();
	
#if BREWPI_BUZZER	
	buzzer.init();
	buzzer.beep(2, 500);
#endif	

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
		lastUpdate = ticks.millis();
			
		tempControl.updateTemperatures();
		tempControl.detectPeaks();
		tempControl.updatePID();
		tempControl.updateState();
		tempControl.updateOutputs();

#if MULTICHAMBER
		chamberManager.switchChamber(prev);
#endif		

#if BREWPI_MENU && !BREWPI_SIMULATE		// disable rotary encoder since this stalls output
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

void SettingsManager::loadSettings()
{
	DEBUG_MSG(PSTR("loading settings"));
	eepromManager.init();

	// for multichamber, set number of chambers to 1
	tempControl.loadDefaultSettings();
	tempControl.loadDefaultConstants();
	deviceManager.loadDefaultDevices();
			
	if (!eepromManager.applySettings())
		piLink.debugMessage(PSTR("EEPROM Settings not available. Starting in safe mode."));
}