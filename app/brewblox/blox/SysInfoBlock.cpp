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

#include "SysInfoBlock.h"
#include "application.h"
#include "d4d.hpp"
#include "deviceid_hal.h"
#include <cstring>

cbox::CboxError
SysInfoBlock::streamTo(cbox::DataOut& out) const
{
    blox_SysInfo message = blox_SysInfo_init_zero;

    HAL_device_ID(static_cast<uint8_t*>(&message.deviceId[0]), 12);

    strncpy(message.wifi.ssid, WiFi.SSID(), 33);

    IPAddress ip = WiFi.localIP();
    message.wifi.ip[0] = ip[0];
    message.wifi.ip[1] = ip[1];
    message.wifi.ip[2] = ip[2];
    message.wifi.ip[3] = ip[3];

    message.wifi.signal = WiFi.RSSI();

    return streamProtoTo(out, &message, blox_SysInfo_fields, blox_SysInfo_size);
}

cbox::CboxError
SysInfoBlock::streamFrom(cbox::DataIn& in)
{
    blox_SysInfo message = blox_SysInfo_init_zero;
    cbox::CboxError result = streamProtoFrom(in, &message, blox_SysInfo_fields, blox_SysInfo_size);
    if (result == cbox::CboxError::OK) {
        if (message.wifi.password[0] != 0) {
            // new wifi credentials received
            WiFi.setCredentials(message.wifi.ssid, message.wifi.password, message.wifi.security, message.wifi.cipher);
        }
        auto calib = D4D_GetTouchScreenCalibration();
        if (message.touch.calibrated == blox_SysInfo_Touch_Calibrated_NEW || calib.ScreenCalibrated == 0) {
            calib.ScreenCalibrated = 1;
            calib.TouchScreenXoffset = message.touch.x_offset;
            calib.TouchScreenYoffset = message.touch.y_offset;
            calib.TouchScreenXBitsPerPixelx16 = message.touch.x_bits_per_pixel_x16;
            calib.TouchScreenYBitsPerPixelx16 = message.touch.y_bits_per_pixel_x16;
            D4D_TCH_SetCalibration(calib);
        }
    }
    return result;
};

cbox::CboxError
SysInfoBlock::streamPersistedTo(cbox::DataOut& out) const
{
    // only persist touch calibration. WiFi settings are stored by particle system layer
    blox_SysInfo message = blox_SysInfo_init_zero;

    auto calib = D4D_TCH_GetCalibration();

    message.touch.calibrated = blox_SysInfo_Touch_Calibrated(calib.ScreenCalibrated);
    message.touch.x_offset = calib.TouchScreenXoffset;
    message.touch.y_offset = calib.TouchScreenYoffset;
    message.touch.x_bits_per_pixel_x16 = calib.TouchScreenXBitsPerPixelx16;
    message.touch.y_bits_per_pixel_x16 = calib.TouchScreenYBitsPerPixelx16;

    return streamProtoTo(out, &message, blox_SysInfo_fields, blox_SysInfo_size);
}