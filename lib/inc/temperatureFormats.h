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
#include "../inc/fixed_point.h"
#include "spark/modules/Platform/Platform.h"
#include "str_functions.h"
#include <cstdlib>
#include <cstring>

#undef constrain

// used for temp and temp, 16 bits
#define TEMP_TYPE int16_t
#define TEMP_INTBITS 7 // 7 integer bits (-128/128), 8 fraction bits, 1 sign bit

// used when higher integer part is needed
#define TEMP_LONG_TYPE int32_t
#define TEMP_LONG_INTBITS 23 // 23 integer bits (-4194304/4194304), 8 fraction bits, 1 sign bit

// used when more precision is needed, 1 bit more for integer part than temp diff, so it is large enough to hold temp as well
#define TEMP_PRECISE_TYPE int32_t
#define TEMP_PRECISE_INTBITS 7 // 7 integer bits (-128/128), 23 fraction bits, 1 sign bit)

class temp_t;
class temp_precise_t;
class temp_long_t;

// static function to convert to string, used by all formats in a wrapper
char*
toStringImpl(int32_t const raw,         // raw value of fixed point
             const unsigned char F,     // number of fixed point bits
             char buf[],                // target buffer
             const uint8_t numDecimals, // number of decimals to print
             const uint8_t len,         // maximum number of characters to print
             char format,               // C or F
             bool absolute);            // is this an absolute temperature? need to subtract 32 for Fahrenheit

// static function to convert from a string, used by all formats in a wrapper
bool
fromStringImpl(int32_t* raw,          // result is put in this variable upon success
               const unsigned char F, // number of fraction bits
               char const* const s,   // the string to convert
               char format,           // C or F
               bool absolute,         // is this an absolute temperature? need to subtract 32 for Fahrenheit
               int32_t min,           // the minimum value for the result
               int32_t max);          // the maximum value for the result

class temp_t : public fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS> {

    using base = fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>;

public:
    temp_t() {}

    // copy constructor
    temp_t(temp_t const& rhs)
    {
        value_ = rhs.value_;
    }

    temp_t(base::base_type value)
        : base(value)
    {
    }

    // constructor from base class, needed for inherited operators to work
    temp_t(fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS> const& rhs)
        : fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>(rhs)
    {
    }

    // converting copy constructor which removes the extra precision bits
    temp_t(temp_precise_t const& rhs);

    // converting copy constructor which constrains to temp's limits
    temp_t(temp_long_t const& rhs);

    // construction from double, use base class constructor
    temp_t(double d)
        : fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>(d)
    {
    }

    // reserve lowest 5 values for special cases (invalid/disabled)
    static const fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>::base_type min_val = fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>::min_val + 2;

    static const fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>::base_type max_val = fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>::max_val;

    // special value to indicate an invalid temp
    static const fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>::base_type invalid_val = fpml::fixed_point_base<temp_t, TEMP_TYPE, TEMP_INTBITS>::min_val;

    // special value to indicate an disabled temp
    static const base::base_type disabled_val = base::min_val + 1;

    static const base::base_type zero = base::base_type(0);

    // function that returns temp object with value min
    static temp_t min()
    {
        temp_t t;
        t.value_ = min_val;
        return t;
    }

    // function that returns temp object with value max
    static temp_t max()
    {
        temp_t t;
        t.value_ = max_val;
        return t;
    }

    // function that returns temp object with value invalid
    static temp_t invalid()
    {
        temp_t t;
        t.value_ = invalid_val;
        return t;
    }
    // function that returns temp object with value disabled
    static temp_t disabled()
    {
        temp_t t;
        t.value_ = disabled_val;
        return t;
    }

    void setRaw(TEMP_TYPE val)
    {
        value_ = val;
    }

    TEMP_TYPE getRaw()
    {
        return value_;
    }

    static temp_t raw(TEMP_TYPE val)
    {
        temp_t t;
        t.setRaw(val);
        return t;
    }

    bool isDisabledOrInvalid() const
    {
        return (value_ < min_val);
    }

    char* toString(char buf[], uint8_t numDecimals, uint8_t len) const
    {
        if (isDisabledOrInvalid()) {
            strcpy(buf, "null");
            return buf;
        }
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len, 'C', false);
    }

    // converts a string in decimal notation to fixed point format
    bool fromString(char const* const s, int32_t minimum = min_val, int32_t maximum = max_val)
    {
        int32_t tempValue;
        bool success = fromStringImpl(&tempValue, fractional_bit_count, s,
                                      'C', false, minimum, maximum);
        if (success) {
            value_ = tempValue;
        }
        return success;
    }

    // converts a temperature in decimal notation to fixed point format
    // for temperatures in C, this is the same as fromString
    // for temperatures in F, it converts to the internal format in C
    bool fromTempString(char const* const s, char format, bool absolute, int32_t minimum = min_val, int32_t maximum = max_val)
    {
        int32_t tempValue;
        bool success = fromStringImpl(&tempValue, fractional_bit_count, s,
                                      format, absolute, minimum, maximum);
        if (success) {
            value_ = tempValue;
        }
        return success;
    }

    // converts a temperature in decimal notation to fixed point format
    // for temperatures in C, this is the same as fromString
    // for temperatures in F, it converts to the internal format in C
    char* toTempString(char buf[], uint8_t numDecimals, uint8_t len, char format, bool absolute) const
    {
        if (isDisabledOrInvalid()) {
            strcpy(buf, "null");
            return buf;
        }
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len, format, absolute);
    }
    std::string toCstring() const
    {
        char temporary[10]; // max 3 integer digits, 4 decimals + period + minus sign + \0
        char* noLeadingSpace = temporary;
        noLeadingSpace = toString(temporary, 4, 10);

        return std::string(noLeadingSpace);
    }

    temp_t operator+(temp_t const& rhs);
    temp_precise_t operator+(temp_precise_t const& rhs);
    temp_long_t operator+(temp_long_t const& rhs);

    temp_t operator-() const; // unary minus (change sign)
    temp_t operator-(temp_t const& rhs);
    temp_precise_t operator-(temp_precise_t const& rhs);
    temp_long_t operator-(temp_long_t const& rhs);

    temp_t operator*(temp_t const& rhs);
    temp_precise_t operator*(temp_precise_t const& rhs);
    temp_long_t operator*(temp_long_t const& rhs);
    temp_long_t operator*(uint16_t const rhs);

    temp_t operator/(temp_t const& rhs);
    temp_precise_t operator/(temp_precise_t const& rhs);
    temp_long_t operator/(temp_long_t const& rhs);
    temp_t operator/(uint16_t const rhs);

    int8_t sign()
    {
        return (value_ > 0) - (value_ < 0);
    }

    friend class temp_precise_t;
    friend class temp_long_t;
};

class temp_precise_t : public fpml::fixed_point_base<temp_precise_t,
                                                     TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS> {

    using base = fpml::fixed_point_base<temp_precise_t,
                                        TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS>;

public:
    temp_precise_t() {}

    // copy constructor
    temp_precise_t(temp_precise_t const& rhs)
    {
        value_ = rhs.value_;
    }

    temp_precise_t(base::base_type value)
        : base(value)
    {
    }

    // constructor from base class, needed for inherited operators to work
    temp_precise_t(fpml::fixed_point_base<temp_precise_t, TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS> const& rhs)
        : fpml::fixed_point_base<temp_precise_t, TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS>(rhs)
    {
    }

    // converting copy constructor which shifts the value to have more fraction bits
    temp_precise_t(temp_t const& rhs);

    // converting copy constructor which shifts the value to have more fraction bits and constrains the result to fit
    temp_precise_t(temp_long_t const& rhs);

    // construction from double, use base class constructor
    temp_precise_t(double d)
        : fpml::fixed_point_base<temp_precise_t, TEMP_PRECISE_TYPE, TEMP_PRECISE_INTBITS>(d)
    {
    }

    void setRaw(TEMP_PRECISE_TYPE val)
    {
        value_ = val;
    }

    TEMP_PRECISE_TYPE getRaw()
    {
        return value_;
    }

    static temp_precise_t raw(TEMP_PRECISE_TYPE val)
    {
        temp_precise_t t;
        t.setRaw(val);
        return t;
    }

    char* toString(char buf[], uint8_t numDecimals, uint8_t len) const
    {
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len, 'C', false);
    }

    bool fromString(char const* const s, int32_t minimum = min_val, int32_t maximum = max_val)
    {
        return fromStringImpl(&value_, fractional_bit_count, s,
                              'C', false, minimum, maximum);
    }

    // converts a temperature in decimal notation to fixed point format
    // for temperatures in C, this is the same as fromString
    // for temperatures in F, it converts to the internal format in C
    bool fromTempString(char const* const s, char format, bool absolute, int32_t minimum = min_val, int32_t maximum = max_val)
    {
        int32_t tempValue;
        bool success = fromStringImpl(&tempValue, fractional_bit_count, s,
                                      format, absolute, minimum, maximum);
        if (success) {
            value_ = tempValue;
        }
        return success;
    }

    // converts a temperature in decimal notation to fixed point format
    // for temperatures in C, this is the same as fromString
    // for temperatures in F, it converts to the internal format in C
    bool toTempString(char buf[], uint8_t numDecimals, uint8_t len, char format, bool absolute) const
    {
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len, format, absolute);
    }

    std::string toCstring() const
    {
        char temporary[15]; // max 3 integer digits, 8 decimals + period + minus sign + \0
        char* noLeadingSpace = temporary;
        noLeadingSpace = toString(temporary, 8, 15);

        return std::string(noLeadingSpace);
    }

    temp_precise_t operator+(temp_precise_t const& rhs);
    temp_precise_t operator+(temp_t const& rhs);
    temp_long_t operator+(temp_long_t const& rhs);

    temp_precise_t operator-() const; // unary minus (change sign)
    temp_precise_t operator-(temp_precise_t const& rhs);
    temp_precise_t operator-(temp_t const& rhs);
    temp_long_t operator-(temp_long_t const& rhs);

    temp_precise_t operator*(temp_precise_t const& rhs);
    temp_precise_t operator*(temp_t const& rhs);
    temp_long_t operator*(temp_long_t const& rhs);
    temp_long_t operator*(uint16_t const rhs);

    temp_precise_t operator/(temp_precise_t const& rhs);
    temp_precise_t operator/(temp_t const& rhs);
    temp_long_t operator/(temp_long_t const& rhs);
    temp_precise_t operator/(uint16_t const rhs);

    int8_t sign()
    {
        return (value_ > 0) - (value_ < 0);
    }
    friend class temp_t;
    friend class temp_long_t;
};

class temp_long_t : public fpml::fixed_point_base<temp_long_t, TEMP_LONG_TYPE,
                                                  TEMP_LONG_INTBITS> {

    using base = fpml::fixed_point_base<temp_long_t, TEMP_LONG_TYPE,
                                        TEMP_LONG_INTBITS>;

public:
    temp_long_t() {}

    // copy constructor
    temp_long_t(temp_long_t const& rhs)
    {
        value_ = rhs.value_;
    }

    temp_long_t(base::base_type value)
        : base(value)
    {
    }

    // constructor from base class, needed for inherited operators to work
    temp_long_t(fpml::fixed_point_base<temp_long_t, TEMP_LONG_TYPE, TEMP_LONG_INTBITS> const& rhs)
        : fpml::fixed_point_base<temp_long_t, TEMP_LONG_TYPE, TEMP_LONG_INTBITS>(rhs)
    {
    }

    // converting copy constructor from normal temp format
    temp_long_t(temp_t const& rhs);

    // converting copy constructor which removes extra precision bits
    temp_long_t(temp_precise_t const& rhs);

    // construction from double, use base class constructor
    temp_long_t(double d)
        : fpml::fixed_point_base<temp_long_t, TEMP_LONG_TYPE, TEMP_LONG_INTBITS>(d)
    {
    }

    void setRaw(TEMP_LONG_TYPE val)
    {
        value_ = val;
    }

    TEMP_LONG_TYPE getRaw()
    {
        return value_;
    }

    static temp_long_t raw(TEMP_LONG_TYPE val)
    {
        temp_long_t t;
        t.setRaw(val);
        return t;
    }

    char* toString(char buf[], uint8_t numDecimals, uint8_t len) const
    {
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len, 'C', false);
    }

    bool fromString(char const* const s, int32_t minimum = min_val, int32_t maximum = max_val)
    {
        return fromStringImpl(&value_, fractional_bit_count, s,
                              'C', false, minimum, maximum);
    }

    std::string toCstring() const
    {
        char temporary[15]; // max 7 integer digits, 4 decimals + period + minus sign + \0
        char* noLeadingSpace = temporary;
        noLeadingSpace = toString(temporary, 4, 15);

        return std::string(noLeadingSpace);
    }

    // converts a temperature in decimal notation to fixed point format
    // for temperatures in C, this is the same as fromString
    // for temperatures in F, it converts to the internal format in C
    bool fromTempString(char const* const s, char format, bool absolute, int32_t minimum = min_val, int32_t maximum = max_val)
    {
        int32_t tempValue;
        bool success = fromStringImpl(&tempValue, fractional_bit_count, s,
                                      format, absolute, minimum, maximum);
        if (success) {
            value_ = tempValue;
        }
        return success;
    }

    // converts a temperature in decimal notation to fixed point format
    // for temperatures in C, this is the same as fromString
    // for temperatures in F, it converts to the internal format in C
    bool toTempString(char buf[], uint8_t numDecimals, uint8_t len, char format, bool absolute) const
    {
        return toStringImpl(value_, fractional_bit_count, buf, numDecimals, len, format, absolute);
    }

    temp_long_t operator+(temp_long_t const& rhs);
    temp_long_t operator+(temp_t const& rhs);
    temp_long_t operator+(temp_precise_t const& rhs);

    temp_long_t operator-() const; // unary minus (change sign)
    temp_long_t operator-(temp_long_t const& rhs);
    temp_long_t operator-(temp_t const& rhs);
    temp_long_t operator-(temp_precise_t const& rhs);

    temp_long_t operator*(temp_long_t const& rhs);
    temp_long_t operator*(temp_t const& rhs);
    temp_long_t operator*(temp_precise_t const& rhs);

    temp_long_t operator/(temp_long_t const& rhs);
    temp_long_t operator/(temp_t const& rhs);
    temp_long_t operator/(temp_precise_t const& rhs);
    temp_long_t operator/(uint16_t const rhs);

    int8_t sign()
    {
        return (value_ > 0) - (value_ < 0);
    }

    friend class temp_t;
    friend class temp_precise_t;
};
