/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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


#ifndef TEMPERATUREFORMATS_H_
#define TEMPERATUREFORMATS_H_

#include <inttypes.h>
// all temperature are stored as fixed point integers:
// 7 bits for the integer part: -64 to 63
// 9 bits for the fraction part
// To convert an integer to the fixed point format, multiply by 512.

// The interface to the Raspberry Pi uses decimal notation, like 21.3.
// This can be in Celsius or Fahrenheit, this is defined in the project settings. TODO!

// just for clarity, typedefs are used instead of normal integers.
// Addition and shifting can be done normally. When two fixed points values are multiplied, you have shift the result
typedef int16_t fixed7_9; // fixed7_9 uses 7 signed int bits and 9 fraction bits
typedef int32_t fixed23_9; // fixed23_9 uses 23 signed int bits and 9 fraction bits. Used when results can overflow
typedef int32_t fixed7_25; // fixed7_25 uses 7 signed int bits and 25 fraction bits. Used when extra precision is needed

char * tempToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed7_9 stringToTemp(char * string);

char * tempDiffToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed7_9 stringToTempDiff(char * string);

char * fixedPointToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed23_9 stringToFixedPoint(char * numberString);

int fixedToTenths(fixed23_9 temperature);
fixed7_9 tenthsToFixed(int temperature);

#endif /* TEMPERATUREFORMATS_H_ */