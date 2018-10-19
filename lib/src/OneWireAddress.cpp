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
#include <cstdint>

char
toHex(uint8_t b)
{
    return ((b > 9) ? b - 10 + 'A' : b + '0');
}

std::string
OneWireAddress::toString() const
{
    auto retv = std::string(16, '0');
    auto strIt = retv.begin();
    auto const addr = asUint8ptr();
    for (auto bytePtr = addr; bytePtr < addr + 8; ++bytePtr) {
        *strIt++ = toHex(((*bytePtr >> 4) & 0x0f));
        *strIt++ = toHex((*bytePtr) & 0x0f);
    }
    return retv;
}
