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

#include "Brewpi.h"
#include "Optimize.h"
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
typedef int16_t fixed12_4;	// 1 sign bit, 11 integer bits, and 4 fraction bits - encoding returned by DS18B20 sensors.
typedef int8_t fixed4_4;	// fixed4_4 uses 1-sign bit, 3 int bits and 4 fraction bits. Corresponds with precision of DS18B20 sensors

inline int8_t asIntFixed7_9(fixed7_9 val) { return val>>9; }

char * tempToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed7_9 stringToTemp(const char * string);

char * tempDiffToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed7_9 stringToTempDiff(const char * string);

char * fixedPointToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
char * fixedPointToString(char s[9], fixed7_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed23_9 stringToFixedPoint(const char * numberString);

int fixedToTenths(fixed23_9 temperature);
fixed7_9 tenthsToFixed(int temperature);

fixed7_9 constrainTemp(fixed23_9 val, fixed7_9 lower, fixed7_9 upper);



fixed7_9 constrainTemp16(fixed23_9 val);
	

#define OPTIMIZE_TEMPERATURE_FORMATS 1 && OPTIMIZE_GLOBAL

#endif /* TEMPERATUREFORMATS_H_ */