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
#define TEMP_LONG_INTBITS       23 // 23 integer bits (-4194304/4194304), 8 fraction bits, 1 sign bit

// used when more precision is needed, 1 bit more for integer part than temp diff, so it is large enough to hold temp as well
#define TEMP_PRECISE_TYPE       int32_t
#define TEMP_PRECISE_INTBITS    7 // 7 integer bits (-128/128), 23 fraction bits, 1 sign bit)

// used for small temperature differences, for example for sensor calibration
#define TEMP_SMALL_TYPE       int8_t
#define TEMP_SMALL_INTBITS    3 // 3 integer bits (-8/+8), 4 fraction bits, 1 sign bit


class temp;
class temp_precise;
class temp_long;
class temp_small;

// static function to convert to string, used by all formats in a wrapper
char * toStringImpl(
        const int32_t raw, // raw value of fixed point
        const unsigned char F, // number of fixed point bits
        char buf[], // target buffer
        const uint8_t numDecimals, // number of decimals to print
        const uint8_t len); // maximum number of characters to print

// static function to convert from a string, used by all formats in a wrapper
bool fromStringImpl(
        int32_t * raw, // result is put in this variable upon success
        const unsigned char F, // number of fraction bits
        const char * s, // the string to convert
        int32_t min, // the minimum value for the result
        int32_t max); // the maximum value for the result


class temp : public fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>{

public:
    using fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::fixed_point_base; // inherit constructors from base class

    temp(){}

    // converting copy constructor which removes the extra precision bits
    temp(temp_precise const& rhs);

    // converting copy constructor which constrains to temp's limits
    temp(temp_long const& rhs);

    // converting copy constructor which adds extra fraction and int bits
    temp(temp_small const& rhs);

    // reserve lowest 5 values for special cases (invalid/disabled)
    static const fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::base_type min_val =
            fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::min_val + 5;

    // special value to indicate an invalid temp
    static const fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::base_type invalid_val =
                fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::min_val;

    // special value to indicate an disabled temp
    static const fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::base_type disabled_val =
                    fpml::fixed_point_base<temp, TEMP_TYPE, TEMP_INTBITS>::min_val + 1;

    // function that returns temp object with value min
    static temp min(){
        temp t;
        t.value_ = min_val;
        return t;
    }
    // function that returns temp object with value min
    static temp invalid(){
        temp t;
        t.value_ = invalid_val;
        return t;
    }
    // function that returns temp object with value min
    static temp disabled(){
        temp t;
        t.value_ = disabled_val;
        return t;
    }

    char * toString (char buf[], uint8_t numDecimals, uint8_t len){
            return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len);
    }

    bool fromString(char * const s, int32_t minimum = min_val, int32_t maximum = max_val){
        int32_t tempValue;
        bool success = fromStringImpl(&tempValue, fractional_bit_count, s, minimum, maximum);
        if( success ){
            value_ = tempValue;
        }
        return success;
    }

    temp operator+(temp const& rhs);
    temp_precise operator+(temp_precise const& rhs);
    temp_long operator+(temp_long const& rhs);

    temp operator-(temp const& rhs);
    temp_precise operator-(temp_precise const& rhs);
    temp_long operator-(temp_long const& rhs);

    temp operator*(temp const& rhs);
    temp_precise operator*(temp_precise const& rhs);
    temp_long operator*(temp_long const& rhs);

    friend class temp_precise;
    friend class temp_long;
    friend class temp_small;
};

class temp_precise : public fpml::fixed_point_base<temp_precise, TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS> {

public:

    using fpml::fixed_point_base<temp_precise, TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS>::fixed_point_base; // inherit constructors from base class

    temp_precise(){}

    // converting copy constructor with shifts the value to have more fraction bits
    temp_precise(temp const& rhs);

    // converting copy constructor with shifts the value to have more fraction bits and constrains the result to fit
    temp_precise(temp_long const& rhs);

    char * toString (char buf[], uint8_t numDecimals, uint8_t len){
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len);
    }

    bool fromString(char * const s, int32_t minimum = min_val, int32_t maximum = max_val){
        return fromStringImpl(&value_, fractional_bit_count, s, minimum, maximum);
    }

    temp_precise operator+(temp_precise const& rhs);
    temp_precise operator+(temp const& rhs);
    temp_long operator+(temp_long const& rhs);

    temp_precise operator-(temp_precise const& rhs);
    temp_precise operator-(temp const& rhs);
    temp_long operator-(temp_long const& rhs);

    temp_precise operator*(temp_precise const& rhs);
    temp_precise operator*(temp const& rhs);
    temp_long operator*(temp_long const& rhs);


    friend class temp;
    friend class temp_long;
    friend class temp_small;
};

class temp_long : public fpml::fixed_point_base<temp_long, TEMP_LONG_TYPE, TEMP_LONG_INTBITS> {

public:
    using fpml::fixed_point_base<temp_long, TEMP_LONG_TYPE, TEMP_LONG_INTBITS>::fixed_point_base; // inherit constructors from base class

    temp_long(){}

    // converting copy constructor from normal temp format
    temp_long(temp const& rhs);

    // converting copy constructor which removes extra precision bits
    temp_long(temp_precise const& rhs);

    char * toString (char buf[], uint8_t numDecimals, uint8_t len){
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len);
    }

    bool fromString(char * const s, int32_t minimum = min_val, int32_t maximum = max_val){
        return fromStringImpl(&value_, fractional_bit_count, s, minimum, maximum);
    }

    temp_long operator+(temp_long const& rhs);
    temp_long operator+(temp const& rhs);
    temp_long operator+(temp_precise const& rhs);

    temp_long operator-(temp_long const& rhs);
    temp_long operator-(temp const& rhs);
    temp_long operator-(temp_precise const& rhs);

    temp_long operator*(temp_long const& rhs);
    temp_long operator*(temp const& rhs);
    temp_long operator*(temp_precise const& rhs);

    friend class temp;
    friend class temp_precise;
    friend class temp_small;
};

class temp_small : public fpml::fixed_point_base<temp_small, TEMP_SMALL_TYPE, TEMP_SMALL_INTBITS> {

public:
    using fpml::fixed_point_base<temp_small, TEMP_SMALL_TYPE, TEMP_SMALL_INTBITS>::fixed_point_base; // inherit constructors from base class

    temp_small(){}

    char * toString (char buf[], uint8_t numDecimals, uint8_t len){
            return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len);
    }

    bool fromString(char * const s, int32_t minimum = min_val, int32_t maximum = max_val){
        int32_t tempValue;
        bool success = fromStringImpl(&tempValue, fractional_bit_count, s, minimum, maximum);
        if( success ){
            value_ = tempValue;
        }
        return success;
    }

    friend class temp;
    friend class temp_precise;
    friend class temp_long;
};


