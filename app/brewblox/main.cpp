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

#include "AppTicks.h"
#include "Board.h"
#include "BrewBlox.h"
#include "Buzzer.h"
#include "MDNS.h"
#include "application.h" // particle stuff
#include "cbox/Object.h"
#include "d4d.hpp"
#include "display/screens/startup_screen.h"
#include "spark_wiring_timer.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP(System.enableFeature(FEATURE_RESET_INFO));

auto mdns = MDNS();
auto mdns_started = bool(false);
#if PLATFORM_ID == PLATFORM_GCC
auto httpserver = TCPServer(8380); // listen on 8380 to serve a simple page with instructions
#else
auto httpserver = TCPServer(80); // listen on 80 to serve a simple page with instructions
#endif

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

#define str(x) #x
#define xstr(x) "x"

void
displayTick()
{
    static ticks_millis_t lastTick = -40;
    auto now = ticks.millis();
    if (now > lastTick + 40) {
        lastTick = now;
        D4D_TimeTickPut();
    }
}

void
setup()
{
    // Install a signal handler
#if PLATFORM_ID == PLATFORM_GCC
    std::signal(SIGINT, signal_handler);
#endif

    boardInit();
    Buzzer.beep(2, 100);
    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);
    WiFi.setListenTimeout(30);
    brewbloxBox().loadObjectsFromStorage(); // init box and load stored objects
    System.on(setup_update, watchdogCheckin);

    bool success = mdns.setHostname(System.deviceID());
    success = success && mdns.addService("tcp", "http", 80, System.deviceID());
    success = success && mdns.addService("tcp", "brewblox", 8332, System.deviceID());
    if (success) {
        auto hw = String("Spark ");
        switch (getSparkVersion()) {
        case SparkVersion::V1:
            hw += "1";
            break;
        case SparkVersion::V2:
            hw += "2";
            break;
        case SparkVersion::V3:
            hw += "3";
            break;
        }
        mdns.addTXTEntry("VERSION", "0.1.0");
        mdns.addTXTEntry("ID", System.deviceID());
        mdns.addTXTEntry("PLATFORM", xstr(PLATFORM_ID));
        mdns.addTXTEntry("HW", hw);
    }

    D4D_Init(NULL);
    StartupScreen::activate();
    displayTick();
}

void
loop()
{
    if (!WiFi.ready() || WiFi.listening()) {
        if (!WiFi.connecting()) {
            WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
#if PLATFORM_ID != PLATFORM_GCC
            Particle.connect();
#endif
        }
    } else {
        if (!mdns_started) {
            mdns_started = mdns.begin(true);
        } else {
            mdns.processQueries();
        }
        TCPClient client = httpserver.available();
        if (client) {
            while (client.read() != -1) {
            }

            client.write("HTTP/1.1 200 Ok\n\n<html><body>Your BrewBlox Spark is online but it does not run it's own web server.\n"
                         "Please install a BrewBlox server to connect to it using the BrewBlox protocol.</body></html>\n\n");
            client.flush();
            delay(5);
            client.stop();
        }
    }

    if (!WiFi.listening()) {
        brewbloxBox().hexCommunicate();
    }

    updateBrewbloxBox();

    // update display
    D4D_CheckTouchScreen();
    D4D_Poll();
    D4D_FlushOutput();
    displayTick();

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
