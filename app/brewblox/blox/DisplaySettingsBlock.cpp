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

#include "DisplaySettingsBlock.h"
#include "deviceid_hal.h"
#include <cstring>

cbox::CboxError
DisplaySettingsBlock::streamTo(cbox::DataOut& out) const
{
    return streamProtoTo(out, &m_settings, blox_DisplaySettings_fields, blox_DisplaySettings_size);
}

cbox::CboxError
DisplaySettingsBlock::streamFrom(cbox::DataIn& in)
{
    blox_DisplaySettings msg = blox_DisplaySettings_init_zero;
    cbox::CboxError result = streamProtoFrom(in, &msg, blox_DisplaySettings_fields, blox_DisplaySettings_size);

    if (result == cbox::CboxError::OK) {
        m_settings = msg;
        m_newSettingsReceived = true;
    }
    return result;
};

cbox::CboxError
DisplaySettingsBlock::streamPersistedTo(cbox::DataOut& out) const
{
    return streamTo(out);
}

// use global static settings, because we only have one display
blox_DisplaySettings DisplaySettingsBlock::m_settings = blox_DisplaySettings_init_zero;
bool DisplaySettingsBlock::m_newSettingsReceived = false;