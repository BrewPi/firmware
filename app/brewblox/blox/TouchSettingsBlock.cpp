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

#include "TouchSettingsBlock.h"
#include "d4d.hpp"
#include <cstring>

cbox::CboxError
TouchSettingsBlock::streamTo(cbox::DataOut& out) const
{
    blox_TouchSettings message = blox_TouchSettings_init_zero;

    auto calib = D4D_TCH_GetCalibration();

    message.calibrated = blox_TouchSettings_Calibrated(calib.ScreenCalibrated);
    message.x_offset = calib.TouchScreenXoffset;
    message.y_offset = calib.TouchScreenYoffset;
    message.x_bits_per_pixel_x16 = calib.TouchScreenXBitsPerPixelx16;
    message.y_bits_per_pixel_x16 = calib.TouchScreenYBitsPerPixelx16;

    return streamProtoTo(out, &message, blox_TouchSettings_fields, blox_TouchSettings_size);
}

cbox::CboxError
TouchSettingsBlock::streamFrom(cbox::DataIn& in)
{
    blox_TouchSettings message = blox_TouchSettings_init_zero;
    cbox::CboxError result = streamProtoFrom(in, &message, blox_TouchSettings_fields, blox_TouchSettings_size);
    if (result == cbox::CboxError::OK) {
        auto calib = D4D_GetTouchScreenCalibration();
        if (message.calibrated == blox_TouchSettings_Calibrated_NEW || calib.ScreenCalibrated == 0) {
            calib.ScreenCalibrated = 1;
            calib.TouchScreenXoffset = message.x_offset;
            calib.TouchScreenYoffset = message.y_offset;
            calib.TouchScreenXBitsPerPixelx16 = message.x_bits_per_pixel_x16;
            calib.TouchScreenYBitsPerPixelx16 = message.y_bits_per_pixel_x16;
            D4D_TCH_SetCalibration(calib);
        }
    }
    return result;
};

cbox::CboxError
TouchSettingsBlock::streamPersistedTo(cbox::DataOut& out) const
{
    return streamTo(out);
}