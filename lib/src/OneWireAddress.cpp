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


/* Old implementation, for backwards compatibility */

#include "OneWireAddress.h"
#include <stdint.h>

void parseBytes(uint8_t *    data,
                const char * s,
                uint8_t      len)
{
    char c;

    while ((c = *s++)){
        uint8_t d = ((c >= 'A') ? c - 'A' + 10 : c - '0') << 4;

        c       = *s++;
        d       |= ((c >= 'A') ? c - 'A' + 10 : c - '0');
        *data++ = d;
    }
}

void printBytes(const uint8_t * data,
                uint8_t         len,
                char *          buf)    // prints 8-bit data in hex
{
    for (int i = 0; i < len; i++){
        uint8_t b = (data[i] >> 4) & 0x0f;

        *buf++ = ((b > 9) ? b - 10 + 'A' : b + '0');
        b      = data[i] & 0x0f;
        *buf++ = ((b > 9) ? b - 10 + 'A' : b + '0');
    }

    *buf = 0;
}


/* New implementation */

uint8_t fromHex(char c){
    return (c >= 'A') ? c - 'A' + 10 : c - '0';
}

char toHex(uint8_t b){
    return ((b > 9) ? b - 10 + 'A' : b + '0');
}

void OneWireAddress::parse(const char * s)
{
    char c;

    for(int8_t i = 0; i<8; i++){
        c = *s++;
        if(!c){
            break; // encountered \0
        }
        uint8_t d = fromHex(c) << 4;
        c       = *s++;
        if(!c){
            break; // encountered \0
        }
        d       |= fromHex(c);
        address[i] = d;
    }
}

void OneWireAddress::print(char * buf, uint8_t len)    // prints 8-bit data in hex
{
    for (int i = 0; i < 8 && i*2 < len; i++){
        uint8_t b = (address[i] >> 4) & 0x0f;

        *buf++ = toHex(b);
        b      = address[i] & 0x0f;
        *buf++ = toHex(b);
    }

    *buf = 0;
}
