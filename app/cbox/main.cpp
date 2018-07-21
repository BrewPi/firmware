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


#include "../../controlbox/src/lib/Object.h"
#include "EepromTypes.h"
#include "EepromAccessImpl.h"

#include "Ticks.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "PersistChangeValue.h"
#include "Commands.h"
#include "Platform.h"
//#include "MDNS.h"

// todo - add a system object that describes the application version
// from this, the protocol of all objects can be determined by the client.

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

//MDNS mdns;

void setup()
{
    Serial.begin(57600);
    eepromAccess.init();
    cbox::controlbox_setup(0);
    platform_init();

    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);
    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
    Particle.connect();
}



void loop()
{
//
	cbox::controlbox_loop();
/*
	if(!mdns_started && WiFi.ready() && WiFi.RSSI() < 0){

		String id = System.deviceID();
		bool success = mdns.setHostname(id)
		  && mdns.addService("tcp", "brewpi", 8332, id)
		  && mdns.begin();
		if (!success) {
			Comms::dataOut().writeAnnotation(mdns.getStatus().c_str());
		}
	}
	if(mdns_started){
		mdns.processQueries();
	}
*/
}

