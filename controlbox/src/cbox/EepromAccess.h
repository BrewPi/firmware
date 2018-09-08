/*
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of Controlbox.
 * 
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Controlbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdint>
#include <cstring>

class EepromAccess {
public:
    EepromAccess() = default;
    virtual ~EepromAccess() = default;

    virtual uint8_t readByte(uint16_t offset) const = 0;
    virtual void writeByte(uint16_t offset, uint8_t value) = 0;
    virtual void readBlock(void* target, uint16_t offset, uint16_t size) const = 0;
    virtual void writeBlock(uint16_t target, const void* source, uint16_t size) = 0;
    virtual uint16_t length() const = 0;
    virtual void clear() = 0;

    template <typename T>
    T& get(const uint16_t& idx, T& t) const
    {
        readBlock(&t, idx, sizeof(T));
        return t;
    }

    template <typename T>
    const T& put(const uint16_t& idx, const T& t)
    {
        writeBlock(idx, &t, sizeof(T));
        return t;
    }
};
