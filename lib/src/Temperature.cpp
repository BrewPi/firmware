/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#include "Temperature.h"

TempFormat tempFormat = TempFormat::Celsius;

void
tempDiff_to_chars(const temp_t& t, char* buf, uint8_t len, uint8_t decimals)
{
    fp12_t val = t;
    if (tempFormat == Fahrenheit) {
        val = (t * 9) / 5;
    }
    to_chars_dec(val, buf, len, decimals);
}

void
temp_to_chars(const temp_t& t, char* buf, uint8_t len, uint8_t decimals)
{
    fp12_t val = t;
    if (tempFormat == Fahrenheit) {
        val += (t * 9) / 5 + fp12_t(32);
    }
    to_chars_dec(val, buf, len, decimals);
}