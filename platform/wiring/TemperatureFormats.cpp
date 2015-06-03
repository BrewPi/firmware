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

#include "Brewpi.h"
#include "TemperatureFormats.h"
#include "Platform.h"
#include <string.h>
#include <stdarg.h>

// See header file for details about the temp format used.

// result can have maximum length of : sign + 3 digits integer part + point + 3 digits fraction part + '\0' = 9 bytes;
// only 1, 2 or 3 decimals allowed.
// returns pointer to the string
// long_temperature is used to prevent overflow

char * tempToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength) {
    if (isDisabledOrInvalid(rawValue)) {
        strcpy_P(s, PSTR("null"));
        return s;
    }
    rawValue = convertFromInternalTemp(rawValue);
    return fixedPointToString(s, rawValue, numDecimals, maxLength);
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

bool invalidStrtolResult(const char * start, const char * end) {
    return ((*end != '\0' && *end != '.' && *end != ' ') // parsing did not end at end of string, space or decimal point
            || start == end); // no number found in string
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

bool stringToBool(bool * result, const char * numberString) {
    char* end;
    if (0 == strcmp(PSTR("true"), numberString)) {
        *result = true;
        return true;
    }
    if (0 == strcmp(PSTR("false"), numberString)) {
        *result = false;
        return true;
    }
    uint8_t newValue = strtol_impl(numberString, &end); // only accept 0 and 1
    if (invalidStrtolResult(numberString, end)) { // no number found in string
        return false;
    }
    if (newValue > 1) {
        return false;
    }
    *result = newValue;
    return true;
}

// returns UINT16_MIN on failure

bool stringToUint16(uint16_t* result, const char * numberString) {
    char* end;
    long newValue;
    newValue = strtol_impl(numberString, &end); // only accept 0 and 1
    if (invalidStrtolResult(numberString, end)) { // no number found in string
        return false;
    }
    if (newValue > UINT16_MAX) {
        *result = UINT16_MAX; // clip to max
    } else if (newValue < 0) {
        return false; // negative values are invalid
    } else {

        *result = newValue;
    }
    return true;
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

temperature constrainTemp(long_temperature valLong, temperature lower, temperature upper) {
    temperature val = constrainTemp16(valLong);
    if (val < lower) {
        return lower;
    }
    if (val > upper) {
        return upper;
    }
    return temperature(valLong);
}

temperature constrainTemp16(long_temperature val) {
    if (val < MIN_TEMP) {
        return MIN_TEMP;
    }
    if (val > MAX_TEMP) {
        return MAX_TEMP;
    }
    return val;
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

long int my_strtol(const char* str, char** tail) {
    long val = 0;
    bool positive = true;
    *tail = (char*) str;
    uint16_t read = 0;
    while (1) {
        if (**tail == '\0') {
            break;
        } else if (**tail == ' ') {
            if (read != 0) {
                break; // only leading zeros allowed
            }
        } else if (**tail == '-') {
            positive = false;
        } else if (**tail >= '0' && **tail <= '9') {
            val = val * 10 + (**tail - '0');
            read++;
        } else {
            break;
        }
        (*tail)++;
    }
    if (read == 0) {
        *tail = (char*) str;
    }
    return positive ? val : -val;
}
