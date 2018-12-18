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
#include "connectivity.h"
#include "d4d.hpp"
#include "deviceid_hal.h"
#include <cstring>

cbox::CboxError
WiFiSettingsBlock::streamTo(cbox::DataOut& out) const
{
    blox_WiFiSettings message = blox_WiFiSettings_init_zero;

    printWiFiIp(message.ip);

    message.signal = wifiSignal();

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
            setWifiCredentials(message.ssid, message.password, message.security, message.cipher);
        }
    }
    return result;
};

cbox::CboxError
WiFiSettingsBlock::streamPersistedTo(cbox::DataOut& out) const
{
    return cbox::CboxError::OK;
}