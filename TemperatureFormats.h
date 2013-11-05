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
// all temperature are stored as fixed point integers

// The internal fixed point format has 9 bits (512 steps) per degree. The range is -16 to 112C, an offset of -48C
// The communication over serial is in C or F and it is always converted to the internal fixed point format in C.
// From C to fixed point temp: T = (C-48)*512 = (C-48)<<9 = C<<9 - 48<<9 = C<<9 - 24576
// From F to fixed point temp: ((F-32)*5/9-48) * 512 = (F*5/9)<<9 - 33678
// From fixed point temp to C: C = T/512 + 48 = (T+24576)/512 = (T+24576)>>9
// From fixed point temp to F: F = (T/512 + 48)*9/5 + 32 = (T+24576)*9/5/512 + 32 = (T+33678)*9/5/512

// Offsets when converting to the internal format:
#define C_OFFSET -24576l
#define F_OFFSET -33678l

// The interface to the Raspberry Pi uses decimal notation, like 21.3.
// Depending on the EEPROM setting cc.tempFormat, this will be interpreted as Celsius or Fahrenheit

// just for clarity, typedefs are used instead of normal integers.
// Addition and shifting can be done normally. When two fixed points values are multiplied, you have shift the result
typedef int16_t fixed7_9; // fixed7_9 uses 7 signed int bits and 9 fraction bits
typedef int32_t fixed23_9; // fixed23_9 uses 23 signed int bits and 9 fraction bits. Used when results can overflow
typedef int32_t fixed7_25; // fixed7_25 uses 7 signed int bits and 25 fraction bits. Used when extra precision is needed
typedef int16_t fixed12_4;	// 1 sign bit, 11 integer bits, and 4 fraction bits - encoding returned by DS18B20 sensors.
typedef int8_t fixed4_4;	// fixed4_4 uses 1-sign bit, 3 int bits and 4 fraction bits. Corresponds with precision of DS18B20 sensors

#define INVALID_TEMP -32768		

inline int8_t asIntFixed7_9(fixed7_9 val) { return val>>9; }

char * tempToString(char * s, fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed7_9 stringToTemp(const char * string);

char * tempDiffToString(char * s, fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed7_9 stringToTempDiff(const char * string);

char * fixedPointToString(char * s, fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
char * fixedPointToString(char * s, fixed7_9 rawValue, uint8_t numDecimals, uint8_t maxLength);
fixed23_9 stringToFixedPoint(const char * numberString);

int fixedToTenths(fixed23_9 temperature);
fixed7_9 tenthsToFixed(int temperature);


fixed7_9 constrainTemp(fixed23_9 val, fixed7_9 lower, fixed7_9 upper);
fixed7_9 constrainTemp16(fixed23_9 val);

#define OPTIMIZE_TEMPERATURE_FORMATS 1 && OPTIMIZE_GLOBAL
