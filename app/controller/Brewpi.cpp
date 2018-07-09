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
#include "Control.h"
#include "PiLink.h"
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

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP(System.enableFeature(FEATURE_RESET_INFO));

void setup()
{
    boardInit();
    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);

    bool resetEeprom = platform_init();

    piLink.init(); // enables Serial, WiFi is enabled later

    eepromManager.init();
    if (resetEeprom) {
        eepromManager.initializeEeprom();
    }
    ui.init();

    logDebug("started");

    ui.showStartupPage();

    while (ui.inStartup()) {
        ui.ticks();
    }

    // initialize OneWire
    if (!primaryOneWireBus.init()) {
        logError(ERROR_ONEWIRE_INIT_FAILED);
    }

    settingsManager.loadSettings();

    logDebug("init complete");
}

void brewpiLoop(void)
{
    static unsigned long lastUpdate = -1000; // init at -1000 to update immediately

    if(ticks.millis() > lastUpdate + 1000) { //update settings every second
        lastUpdate = ticks.millis();
        control.update();
        ui.update();
    }

    control.fastUpdate(); // update actuators as often as possible for PWM

    ui.ticks();

    //listen for incoming serial and wifi connections while waiting to update
    piLink.receive();
}

void loop() {
    brewpiLoop();
}

