/*
 * Copyright 2017 BrewPi
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


#include "OneWireBusCBox.h"
#include "EepromTypes.h"
#include "EepromAccessImpl.h"

#include "Values.h"
#include "Ticks.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "PersistChangeValue.h"
#include "Commands.h"
#include "Platform.h"
#include "MDNS.h"

// todo - add a system object that describes the application version
// from this, the protocol of all objects can be determined by the client.

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

MDNS mdns;

void setup()
{
    Serial.begin(9600);
    eepromAccess.init();
    controlbox_setup(0);
    platform_init();

    WiFi.on();
    if (WiFi.hasCredentials()) {
        WiFi.connect();
        waitFor(WiFi.ready, 30*1000);

        if (WiFi.ready()) {
            String id = System.deviceID();
            bool success = mdns.setHostname(id)
              && mdns.addService("tcp", "brewpi", 8332, id)
              && mdns.begin();
            if (!success) {
                Comms::dataOut().writeAnnotation(mdns.getStatus().c_str());
            }
        }
    }
}



void loop()
{
    controlbox_loop();
    mdns.processQueries();
}

