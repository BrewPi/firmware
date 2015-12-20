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

/* old implementation */

void parseBytes(uint8_t* data, const char* s, uint8_t len);
void printBytes(const uint8_t* data, uint8_t len, char* buf);
typedef uint8_t DeviceAddress[8];

/* new implementation ? */

class OneWireAddress{
public:
    OneWireAddress(){}

    void parse(const char* s);
    void print(char* buf, uint8_t len);

    uint8_t &operator[](uint8_t i){
        return address[i];
    }

    operator uint8_t *(){ // for compatibility with OneWire classes that take a uint8_t *
        return address;
    }

private:
    uint8_t address[8];
};
