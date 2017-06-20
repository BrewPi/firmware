/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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

#include "Platform.h"
#include "Brewpi.h"
#include "Ticks.h"
#include "Display.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensor.h"
#include "TempSensorMock.h"
#include "TempSensorExternal.h"
#include "ActuatorMocks.h"
#include "Sensor.h"
#include "SettingsManager.h"
#include "UI.h"

#if BREWPI_SIMULATE
	#include "Simulator.h"
#endif

// global class objects static and defined in class cpp and h files

// instantiate and configure the sensors, actuators and controllers we want to use


void setup(void);
void loop (void);

/* Configure the counter and delay timer. The actual type of these will vary depending upon the environment.
 * They are non-virtual to keep code size minimal, so typedefs and preprocessing are used to select the actual compile-time type used. */
TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);

UI ui;

SYSTEM_MODE(MANUAL);

void setup()
{
    boardInit();
    bool resetEeprom = platform_init();
    eepromManager.init();
    if (resetEeprom) {
        eepromManager.initializeEeprom();
    }
	ui.init();

    uint32_t start = ticks.millis();
    uint32_t delay = ui.showStartupPage();

    piLink.init();
    // flush any waiting input.
    // Linux can put garbage in the serial input buffer during connect
    piLink.flushInput();

    logDebug("started");

    while (ticks.millis()-start <= delay) {
        ui.ticks();
    }
    
    // initialize OneWire
    if (!primaryOneWireBus.init()) {
        logError(ERROR_ONEWIRE_INIT_FAILED);
    }

#if BREWPI_SIMULATE
	simulator.step();
	// initialize the filters with the assigned initial temp value
	//tempControl.beerSensor->init();
	//tempControl.fridgeSensor->init();
#endif	
    settingsManager.loadSettings();

    control.update();

    ui.showControllerPage();
    			
	logDebug("init complete");
}

void brewpiLoop(void)
{
	static unsigned long lastUpdate = -1000; // init at -1000 to update immediately
    ui.ticks();
        
    if(!ui.inStartup() && (ticks.millis() - lastUpdate >= (1000))) { //update settings every second
		lastUpdate = ticks.millis();
		control.update();
        ui.update();
    }

    control.fastUpdate(); // update actuators as often as possible for PWM

    //listen for incoming serial connections while waiting to update
    piLink.receive();
}

void loop() {
	#if BREWPI_SIMULATE
	simulateLoop();
	#else
	brewpiLoop();
	#endif
}

