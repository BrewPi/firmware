/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
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


#pragma once

#include "DataStream.h"

void readPlatformEndianMaskedBytes(void* data, uint8_t size, DataIn& in, DataIn& mask);

void writePlatformEndianBytes(void* data, uint8_t size, DataOut& out);


inline uint8_t readMaskedByte(DataIn& in, DataIn& mask)
{
	return in.next() & mask.next();
}
