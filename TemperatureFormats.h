/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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

#include "Brewpi.h"
#include <inttypes.h>
#include <limits.h>
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

#define INVALID_TEMP -32768		
#define MAX_TEMP INT_MAX
#define MIN_TEMP INT_MIN+1

/* Temperature expressed as an integer. */
typedef int8_t temp_int;
typedef fixed7_9 temperature;
typedef fixed23_9 long_temperature;
typedef fixed7_25 temperature_precise;

#define TEMP_FIXED_POINT_BITS (9)
#define TEMP_FIXED_POINT_SCALE (1<<TEMP_FIXED_POINT_BITS)
#define TEMP_FIXED_POINT_MASK (TEMP_FIXED_POINT_SCALE-1)
#define TEMP_PRECISE_EXTRA_FRACTION_BITS 16

inline int8_t tempToInt(temperature val) { return int8_t(val>>TEMP_FIXED_POINT_BITS); }
inline int16_t longTempToInt(long_temperature val) { return int16_t(val>>TEMP_FIXED_POINT_BITS); }
inline temperature intToTemp(int8_t val) { return temperature(val)<<TEMP_FIXED_POINT_BITS; }
inline temperature doubleToTemp(double temp) { return temp*TEMP_FIXED_POINT_SCALE>=MAX_TEMP ? MAX_TEMP : temp*TEMP_FIXED_POINT_SCALE<=MIN_TEMP ? MIN_TEMP : temperature(temp*TEMP_FIXED_POINT_SCALE); }

inline long_temperature intToLongTemp(int16_t val) { return long_temperature(val)<<9; }

inline temperature tempPreciseToRegular(temperature_precise val) { return val>>TEMP_PRECISE_EXTRA_FRACTION_BITS; }
inline temperature_precise tempRegularToPrecise(temperature val) { return temperature_precise(val)<<TEMP_PRECISE_EXTRA_FRACTION_BITS; }

char * tempToString(char s[9], long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
temperature stringToTemp(const char * string);

char * tempDiffToString(char s[9], long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
temperature stringToTempDiff(const char * string);

char * fixedPointToString(char s[9], long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
char * fixedPointToString(char s[9], temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
long_temperature stringToFixedPoint(const char * numberString);

int fixedToTenths(long_temperature temperature);
temperature tenthsToFixed(int temperature);

temperature constrainTemp(long_temperature val, temperature lower, temperature upper);

temperature constrainTemp16(long_temperature val);

temperature multiplyTemperatureLong(temperature a, long_temperature b);
temperature multiplyTemperature(temperature a, temperature b);

	

#define OPTIMIZE_TEMPERATURE_FORMATS 1 && OPTIMIZE_GLOBAL
