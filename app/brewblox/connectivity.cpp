/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewBlox.
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

#include "connectivity.h"
#include "application.h"

void
printWiFiIp(char dest[16])
{
    IPAddress ip = WiFi.localIP();
    snprintf(dest, 16, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

int8_t
wifiSignal()
{
    return WiFi.RSSI();
}

bool
serialConnected()
{
    return Serial.isConnected();
}

bool
setWifiCredentials(const char* ssid, const char* password, uint8_t security, uint8_t cipher)
{
    return WiFi.setCredentials(ssid, password, security, cipher);
};