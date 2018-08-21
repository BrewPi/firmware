/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 * Copyright 2015 Matthew McGowan
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


#pragma once

#include <stdint.h>

class OneWireAddress{
public:
    OneWireAddress() : address(0){};
    OneWireAddress(uint64_t addr) : address(addr){};
    ~OneWireAddress() = default;

    void parse(const char* s);
    void print(char* buf, uint8_t len);

    operator uint8_t *() { // for compatibility with OneWire classes that take a uint8_t *
        return reinterpret_cast<uint8_t *>(&address);
    }

    operator uint64_t() const { // for compatibility with OneWire classes that take a uint8_t *
        return address;
    }

private:
    uint64_t address;
};
