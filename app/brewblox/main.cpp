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

#include "Board.h"
#include "BrewBlox.h"
#include "application.h" // particle stuff
#include "cbox/Object.h"
//#include "MDNS.h"

// todo - add a system object that describes the application version
// from this, the protocol of all objects can be determined by the client.

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

//MDNS mdns;

#if PLATFORM_ID == PLATFORM_GCC
#include <csignal>
void
signal_handler(int signal)
{
    exit(signal);
}
#endif

#if PLATFORM_THREADING
ApplicationWatchdog appWatchdog(60000, System.reset);
inline void
watchdogCheckin()
{
    appWatchdog.checkin();
}
#else
// define dummy watchdog checkin for when the watchdog is not available
inline void
watchdogCheckin()
{
}
#endif

void
setup()
{
    //Serial.begin(57600);
    // Install a signal handler
#if PLATFORM_ID == PLATFORM_GCC
    std::signal(SIGINT, signal_handler);
#endif

    boardInit();
    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);
    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
    Particle.connect();
    brewbloxBox(); // init box
    System.on(setup_update, watchdogCheckin);
}

void
loop()
{

    brewbloxBox().hexCommunicate();
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
    watchdogCheckin();
}

void
handleReset(bool exitFlag)
{
    if (exitFlag) {
#if PLATFORM_ID == PLATFORM_GCC
        exit(0);
#else
        System.reset();
#endif
    }
}
