/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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
#include "Platform.h"
#include <fixed_point.h>
#include <cstdlib>
#include "str_functions.h"
#include "fixstl.h"
#undef constrain

// used for temp and temp, 16 bits
#define TEMP_TYPE          int16_t
#define TEMP_INTBITS       7 // 7 integer bits (-128/128), 8 fraction bits, 1 sign bit

// used when higher integer part is needed
#define TEMP_LONG_TYPE          int32_t
#define TEMP_LONG_INTBITS       22 // 22 integer bits (-4194304/4194304), 9 fraction bits, 1 sign bit

// used when more precision is needed, 1 bit more for integer part than temp diff, so it is large enough to hold temp as well
#define TEMP_PRECISE_TYPE       int32_t
#define TEMP_PRECISE_INTBITS    7 // 7 integer bits (-128/128), 23 fraction bits, 1 sign bit)

// used for small temperature differences, for example for sensor calibration
#define TEMP_SMALL_TYPE       int8_t
#define TEMP_SMALL_INTBITS    3 // 3 integer bits (-8/+8), 4 fraction bits, 1 sign bit

template<
/// The base type. Must be an integer type.
//!
//! If this is a signed type, the fixed_point number will behave signed too,
//! if this is an unsigned type, the fixed_point number will behave
//! unsigned.
        typename B,
        /// The integer part bit count.
        unsigned char I,
        /// The fractional part bit count.
        unsigned char F = std::numeric_limits<B>::digits - I>
class temp_template: public fpml::fixed_point<B, I, F> {
public:
    using fpml::fixed_point<B, I, F>::fixed_point; // inherit constructors from base class

    /*
    // Converting this back to base type
    fpml::fixed_point<B, I, F> & operator = (const temp_template&  rhs)
    {
        return (fpml::fixed_point<B, I, F> & )*this;
    }*/

    static inline const temp_template min(){
        return  std::numeric_limits<fpml::fixed_point<B, I, F> >::min();
    }
    static inline const temp_template max(){
        return std::numeric_limits<fpml::fixed_point<B, I, F> >::max();
    }

    temp_template constrain(const temp_template & min, const temp_template & max){
        if(*this < min){
            return min;
        }
        if(*this > max){
            return max;
        }
        return *this;
    }

    // converts fixed point value to string, without using double/float
    // resulting string is always length len (including \0). Spaces are prepended to achieve that.
    char * toString(char buf[], uint8_t numDecimals, uint8_t len, const int offset = 0) {

        char const digit[] = "0123456789";
        char* p;
        B val = this->value_;
        bool negative = false;

        // promote to larger type to prevent overflow. For B=int8, use int32 instead of int16. Int16 will overflow for more than 4 decimals
        using shifterType = typename std::conditional<std::is_same<int8_t, B>::value,
        int32_t,
        typename fpml::fixed_point<B, I, F>::template promote_type<B>::type
        >::type;

        shifterType shifter = val + (offset << F);

        if (shifter < 0) {
            shifter = -shifter;
            negative = true;
        }
        // code below looks a bit cryptic, but what it does is * 10^numDecimals / 2^F
        for (uint8_t i = 0; i < numDecimals; i++) {
            shifter = shifter * 5; // *5 instead of 10, combined with reduced shift below. Less chance of overflow
        }
        shifter = (shifter + (1 << (F - numDecimals - 1))) >> (F - numDecimals); // divide rounded by fixed point scale

        p = &buf[len - 1]; // start at the end of buffer
        *p = '\0';
        std::div_t dv { };
        dv.quot = shifter;
        do { //Move back, inserting digits as u go
            if (p == &buf[len - 1 - numDecimals]) {
                *--p = '.'; // insert decimal point at right moment
            } else {
                dv = std::div(dv.quot, 10);
                if ((dv.quot || dv.rem)) { // check if end of digits
                    *--p = digit[std::abs(dv.rem)];
                } else if ((p - buf) > (len - numDecimals - 3)) { // still need to print some leading zeros
                    *--p = '0';
                } else if (negative) {
                    // print minus sign if needed as last digit to print
                    *--p = '-';
                    break;
                } else {
                    break;
                }
            }
        } while (p > buf);
        char * pWithoutSpaces = p;
        while (p > buf) {
            *(--p) = ' '; // prepend digits with spaces
        }
        // return pointer to string skipping spaces
        // programmer can choose to use original buf pointer with spaces or return value without spaces
        return pWithoutSpaces;
    }

    bool fromString(char * const s, const int offset = 0) {
        // receive new value as null terminated string: "19.20"
        B newValue;

        // promote to larger type to prevent overflow. For B=int8, use int32 instead of int16. Int16 will overflow for more than 4 decimals
        using shifterType = typename std::conditional<std::is_same<int8_t, B>::value,
        int32_t,
        typename fpml::fixed_point<B, I, F>::template promote_type<B>::type
        >::type;

        shifterType decimalValue = 0;

        char* decimalPtr;
        char* end;
        // Check if - is in the string
        bool positive = (0 == strchr(s, '-'));

        newValue = strtol_impl(s, &end);// convert string to integer
        if (invalidStrtolResult(s, end)) {
            return false; // string was not valid
        }
        newValue += offset;
        newValue = newValue << F; // shift to fixed point

        // find the point in the string to know whether we have decimals
        decimalPtr = strchr(s, '.');// returns pointer to the point.
        if (decimalPtr != 0) {
            decimalPtr++; // skip decimal point
            // convert decimals to integer
            decimalValue = strtol_impl(decimalPtr, &end);
            if (invalidStrtolResult(decimalPtr, end)) {
                return false; // string was not valid
            }

            decimalValue = decimalValue << F;
            uint8_t charsAfterPoint = end - decimalPtr; // actually used # digits after point
            while (charsAfterPoint-- > 0) {
                decimalValue = (decimalValue + 5) / 10;
            }
        }
        this->value_ = positive ? newValue + decimalValue : newValue - decimalValue;
        return true;
    }
};


using temp = temp_template<TEMP_TYPE, TEMP_INTBITS>;
using temp_long = temp_template<TEMP_LONG_TYPE,TEMP_LONG_INTBITS>;
using temp_precise = temp_template<TEMP_PRECISE_TYPE,TEMP_PRECISE_INTBITS>;
using temp_small = temp_template<TEMP_SMALL_TYPE, TEMP_SMALL_INTBITS>;

// To convert, cast back to base class. Base class handles conversion for different fixed point types

static inline temp_long toLong(temp & val) {
    temp_long copy(
            (fpml::fixed_point<TEMP_TYPE, TEMP_INTBITS> &) val);
    return copy;
}

static inline temp_precise toPrecise(temp & val) {
    temp_precise copy(
            (fpml::fixed_point<TEMP_TYPE, TEMP_INTBITS> &) val);
    return copy;
}

static inline temp toTemp(temp_small & val) {
    temp copy(
            (fpml::fixed_point<TEMP_SMALL_TYPE, TEMP_SMALL_INTBITS> &) val);
    return copy;
}

static inline temp toTemp(temp_precise & val) {
    temp min = temp::min();
    temp max = temp::max();
    temp_precise constrained = val.constrain(toPrecise(min),toPrecise(max));
    temp copy((fpml::fixed_point<TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS> &) constrained );
    return copy;
}

static inline temp toTemp(temp_long & val) {
    temp min = temp::min();
    temp max = temp::max();
    temp_long constrained = val.constrain(toLong(min),toLong(max));
    temp copy((fpml::fixed_point<TEMP_LONG_TYPE, TEMP_LONG_INTBITS> &) constrained );
    return copy;
}

