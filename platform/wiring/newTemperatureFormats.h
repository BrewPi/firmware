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

#define TEMP_BASE_TYPE          int16_t
#define TEMP_BASE_INTBITS       6 // 6 integer bits (-64/64, 9 fraction bits, 1 sign bit)

#define TEMP_LONG_TYPE          int32_t
#define TEMP_LONG_INTBITS       22 // 22 integer bits (-64/64, 9 fraction bits, 1 sign bit)

#define TEMP_PRECISE_TYPE       int32_t
#define TEMP_PRECISE_INTBITS    6 // 6 integer bits (-64/64, 24 fraction bits, 1 sign bit)

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
class temp_diff_template: public fpml::fixed_point<B, I, F> {
public:
    using fpml::fixed_point<B, I, F>::fixed_point; // inherit constructors from base class

    // converts fixed point value to string, without using double/float
    // resulting string is always length len (including \0). Spaces are prepended to achieve that.
    char * toString(char buf[], uint8_t numDecimals, uint8_t len, int offset = 0) {

        char const digit[] = "0123456789";
        char* p;
        int val = this->value_;
        int shifter = val + (offset<<F);
        bool negative = false;

        for(int i=0; i<numDecimals; i++){
            shifter = shifter*10;
        }
        shifter = (shifter + (1<<(F-1)))>>F; // divide rounded by fixed point scale

        if(shifter < 0){
            shifter = -shifter;
            negative = true;
        }


        p =  &buf[len-1]; // start at the end of buffer
        *p = '\0';
        std::div_t dv{}; dv.quot = shifter;
        do{ //Move back, inserting digits as u go
            if(p == &buf[len-1-numDecimals]){
                *--p = '.'; // insert decimal point at right moment
            }
            else {
                dv = std::div(dv.quot, 10);
                if((dv.quot || dv.rem)){ // check if end of digits
                    *--p = digit[std::abs(dv.rem)];
                }
                else if((p - buf) > (len - numDecimals - 3) ){ // still need to print some leading zeros
                    *--p = '0';
                }
                else if(negative){
                    // print minus sign if needed as last digit to print
                    *--p = '-';
                    break;
                }
                else{
                    break;
                }
            }
        } while (p > buf);

        while (p > buf){
            *(--p) = ' '; // prepend digits with spaces
        }
        return p; // return pointer to string skipping spaces
    }
};

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
class temp_template: public temp_diff_template<B, I, F> {
public:
    using temp_diff_template<B, I, F>::temp_diff_template; // inherit constructors from base class
    temp_template(double rhs) {
        *this = temp_diff_template<B, I, F>(rhs - 48.0); // store temperature with -48C offset when initializing from double
    }
    temp_template(int rhs) {
        *this = temp_diff_template<B, I, F>(rhs - 48); // store temperature with -48C offset when initializing from int
    }
    char * toString(char buf[], uint8_t numDecimals, uint8_t len){
        return temp_diff_template<B, I, F>::toString(buf, numDecimals, len, 48);
    }
};

using temp_diff = temp_diff_template<TEMP_BASE_TYPE, TEMP_BASE_INTBITS>;
using temp = temp_template<TEMP_BASE_TYPE, TEMP_BASE_INTBITS>;
using temp_diff_long = temp_diff_template<TEMP_LONG_TYPE,TEMP_LONG_INTBITS>;
using temp_long = temp_template<TEMP_LONG_TYPE,TEMP_LONG_INTBITS>;
using temp_diff_precise = temp_diff_template<TEMP_PRECISE_TYPE,TEMP_PRECISE_INTBITS>;
using temp_precise = temp_template<TEMP_PRECISE_TYPE,TEMP_PRECISE_INTBITS>;

// To convert, cast back to base class. Base class handles conversion for different fixed point types
static inline temp_diff_long toLong(temp_diff & val) {
    temp_diff_long copy(
            (fpml::fixed_point<TEMP_BASE_TYPE, TEMP_BASE_INTBITS> &) val);
    return copy;
}

static inline temp_long toLong(temp & val) {
    temp_long copy(
            (fpml::fixed_point<TEMP_BASE_TYPE, TEMP_BASE_INTBITS> &) val);
    return copy;
}

static inline temp_diff_precise toPrecise(temp_diff & val) {
    temp_diff_precise copy(
            (fpml::fixed_point<TEMP_BASE_TYPE, TEMP_BASE_INTBITS> &) val);
    return copy;
}

static inline temp_precise toPrecise(temp & val) {
    temp_precise copy(
            (fpml::fixed_point<TEMP_BASE_TYPE, TEMP_BASE_INTBITS> &) val);
    return copy;
}
