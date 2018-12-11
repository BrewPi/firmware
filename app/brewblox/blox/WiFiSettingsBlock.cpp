/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of Controlbox
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "WiFiSettingsBlock.h"
#include "d4d.hpp"
#include "deviceid_hal.h"
#include "spark_wiring_wifi.h"
#include <cstring>

using namespace spark;

cbox::CboxError
WiFiSettingsBlock::streamTo(cbox::DataOut& out) const
{
    blox_WiFiSettings message = blox_WiFiSettings_init_zero;

    strncpy(message.ssid, WiFi.SSID(), 33);

    IPAddress ip = WiFi.localIP();
    message.ip[0] = ip[0];
    message.ip[1] = ip[1];
    message.ip[2] = ip[2];
    message.ip[3] = ip[3];

    message.signal = WiFi.RSSI();

    return streamProtoTo(out, &message, blox_WiFiSettings_fields, blox_WiFiSettings_size);
}

cbox::CboxError
WiFiSettingsBlock::streamFrom(cbox::DataIn& in)
{
    blox_WiFiSettings message = blox_WiFiSettings_init_zero;
    cbox::CboxError result = streamProtoFrom(in, &message, blox_WiFiSettings_fields, blox_WiFiSettings_size);
    if (result == cbox::CboxError::OK) {
        if (message.password[0] != 0) {
            // new wifi credentials received
            WiFi.setCredentials(message.ssid, message.password, message.security, message.cipher);
        }
    }
    return result;
};

cbox::CboxError
WiFiSettingsBlock::streamPersistedTo(cbox::DataOut& out) const
{
    return cbox::CboxError::OK;
}