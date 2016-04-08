/*
 * Copyright 2015 Matthew McGowan.
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

#include "Static.h"
#include <cstdint>
#include <cstddef>

#if !CONTROLBOX_STATIC

typedef uint16_t eptr_t;
#define INVALID_EPTR (0)

struct EepromAccess
{
	virtual uint8_t readByte(eptr_t offset) const=0;
	virtual void writeByte(eptr_t offset, uint8_t value)=0;
	virtual void readBlock(void* target, eptr_t offset, uint16_t size) const=0;
	virtual void writeBlock(eptr_t target, const void* source, uint16_t size)=0;

	virtual size_t length() const=0;
};

#else

#include "EepromAccessImpl.h"

extern EepromAccess eepromAccess;


#endif


