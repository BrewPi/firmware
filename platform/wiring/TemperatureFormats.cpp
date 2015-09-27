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

#if 0

#include "Brewpi.h"
#include "TemperatureFormats.h"
#include "Platform.h"
#include <string.h>
#include <stdarg.h>
#include "str_functions.h"

// See header file for details about the temp format used.

// result can have maximum length of : sign + 3 digits integer part + point + 3 digits fraction part + '\0' = 9 bytes;
// only 1, 2 or 3 decimals allowed.
// returns pointer to the string
// long_temperature is used to prevent overflow

char * tempToString(char * s, temp_t rawValue, uint8_t numDecimals, uint8_t maxLength) {
    if (rawValue.isDisabledOrInvalid()) {
        strcpy_P(s, PSTR("null"));
        return s;
    }
    rawValue = convertFromInternalTemp(rawValue);
    return rawValue.toString(s, Value, numDecimals, maxLength);
}

char * tempDiffToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength) {
    rawValue = convertFromInternalTempDiff(rawValue);
    return fixedPointToString(s, rawValue, numDecimals, maxLength);
}

char * fixedPointToString(char * s, temperature rawValue, uint8_t numDecimals, uint8_t maxLength) {
    return fixedPointToString(s, long_temperature(rawValue), numDecimals, maxLength);
}

// this gets rid of snprintf_P

void mysnprintf_P(char* buf, int len, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(buf, len, fmt, args);
    va_end(args);
}

char * fixedPointToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength) {
    s[0] = ' ';
    if (rawValue < 0l) {
        s[0] = '-';
        rawValue = -rawValue;
    }
    int intPart = rawValue >> TEMP_FIXED_POINT_BITS; // do not use longTempDiffToInt because it rounds up
    uint16_t fracPart;
    const char* fmt;
    uint16_t scale;
    switch (numDecimals) {
        case 1:
            fmt = PSTR("%d.%01d");
            scale = 10;
            break;
        case 2:
            fmt = PSTR("%d.%02d");
            scale = 100;
            break;
        default:
            fmt = PSTR("%d.%03d");
            scale = 1000;
    }
    fracPart = ((rawValue & TEMP_FIXED_POINT_MASK) * scale + TEMP_FIXED_POINT_SCALE / 2) >> TEMP_FIXED_POINT_BITS; // add 256 for rounding
    if (fracPart >= scale) {
        intPart++;
        fracPart = 0;
    }
    mysnprintf_P(&s[1], maxLength - 1, fmt, intPart, fracPart);
    return s;
}

bool stringToTemp(temperature* result, const char * numberString) {
    if (0 == strcmp(PSTR("null"), numberString)) {
        *result = DISABLED_TEMP;
        return true;
    }
    long_temperature longResult;
    if (stringToFixedPoint(&longResult, numberString)) {
        *result = constrainTemp16(convertToInternalTemp(longResult));
        return true;
    }
    return false;
}

bool stringToTempDiff(temperature* result, const char * numberString) {
    long_temperature longResult;
    if (stringToFixedPoint(&longResult, numberString)) {
        longResult = convertToInternalTempDiff(longResult);
        *result = constrainTemp16(longResult);
        return true;
    }
    return false;
}

bool stringToFixedPoint(long_temperature * result, const char * numberString) {
    // receive new temperature as null terminated string: "19.20"
    long_temperature newValue;
    long_temperature decimalValue = 0;
    char* decimalPtr;
    char* end;
    // Check if - is in the string
    bool positive = (0 == strchr(numberString, '-'));

    newValue = strtol_impl(numberString, &end); // convert string to integer
    if (invalidStrtolResult(numberString, end)) {
        return false; // string was not valid
    }
    newValue = newValue << TEMP_FIXED_POINT_BITS; // shift to fixed point

    // find the point in the string to know whether we have decimals
    decimalPtr = strchr(numberString, '.'); // returns pointer to the point.
    if (decimalPtr != 0) {
        decimalPtr++; // skip decimal point
        // convert decimals to integer
        decimalValue = strtol_impl(decimalPtr, &end) << TEMP_FIXED_POINT_BITS;
        if (invalidStrtolResult(decimalPtr, end)) {
            return false; // string was not valid
        }
        uint8_t charsAfterPoint = end - decimalPtr; // actually used # digits after point
        while (charsAfterPoint-- > 0) {
            decimalValue = (decimalValue + 5) / 10;
        }
    }
    *result = positive ? newValue + decimalValue : newValue - decimalValue;
    return true;
}

bool stringToFixedPoint(temperature * result, const char * numberString) {
    long_temperature longResult;
    if (stringToFixedPoint(&longResult, numberString)) {
        *result = constrainTemp16(longResult);
        return true;
    }
    return false;
}


int fixedToTenths(long_temperature temp) {
    temp = convertFromInternalTemp(temp);
    temperature rounder = (temp < 0) ? -TEMP_FIXED_POINT_SCALE / 2 : TEMP_FIXED_POINT_SCALE / 2;
    return (10 * temp + rounder) / TEMP_FIXED_POINT_SCALE; // return rounded result in tenth of degrees
}

temperature tenthsToFixed(int temp) {
    long_temperature fixedPointTemp = convertToInternalTemp(((long_temperature) temp * TEMP_FIXED_POINT_SCALE + 5) / 10);
    return constrainTemp16(fixedPointTemp);
}

long_temperature constrainTempLong(long_temperature val, long_temperature lower, long_temperature upper) {
    if (val < lower) {
        return lower;
    }
    if (val > upper) {
        return upper;
    }
    return val;
}

temperature constrainTemp(long_temperature valLong, temperature lower, temperature upper) {
    temperature val = constrainTemp16(valLong);
    if (val < lower) {
        return lower;
    }
    if (val > upper) {
        return upper;
    }
    return val;
}

temperature constrainTemp16(long_temperature val) {
    if (val < MIN_TEMP) {
        return MIN_TEMP;
    }
    if (val > MAX_TEMP) {
        return MAX_TEMP;
    }
    return temperature(val);
}

temperature multiplyFactorTemperatureLong(temperature factor, long_temperature b) {
    return constrainTemp16(((long_temperature) factor * (b - C_OFFSET)) >> TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperatureDiffLong(temperature factor, long_temperature b) {
    return constrainTemp16(((long_temperature) factor * b) >> TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperature(temperature factor, temperature b) {
    return constrainTemp16(((long_temperature) factor * ((long_temperature) b - C_OFFSET)) >> TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperatureDiff(temperature factor, temperature b) {
    return constrainTemp16(((long_temperature) factor * (long_temperature) b) >> TEMP_FIXED_POINT_BITS);
}

#endif
