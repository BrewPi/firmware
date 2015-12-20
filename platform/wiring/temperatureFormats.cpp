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

#include "temperatureFormats.h"

// Converting constructors, which shift and constrain the value.

temp_t::temp_t(const temp_precise_t& rhs) {
    const unsigned char shift = temp_precise_t::fractional_bit_count
            - temp_t::fractional_bit_count;
    this->value_ = rhs.value_ >> shift; // could result in a 1 bit error due to rounding
}

temp_t::temp_t(const temp_long_t& rhs) {
    // temp and temp_long have same number of fraction bits, no shifting needed
    static_assert(temp_t::fractional_bit_count == temp_long_t::fractional_bit_count,
            "temp and temp_long should have same number of fraction bits");

    if (rhs.value_ < min_val) {
        this->value_ = min_val;
    } else if (rhs.value_ > max_val) {
        this->value_ = max_val;
    } else {
        this->value_ = rhs.value_;
    }
}

temp_precise_t::temp_precise_t(const temp_t& rhs) {
    // temp and temp_precise have same number of integer bits, so this will not overflow
    static_assert(temp_t::integer_bit_count == temp_precise_t::integer_bit_count,
            "temp and temp_long should have same number of integer bits");

    unsigned char shift = temp_precise_t::fractional_bit_count
            - temp_t::fractional_bit_count;
    this->value_ = temp_precise_t::base_type(rhs.value_) << shift;
}

temp_precise_t::temp_precise_t(const temp_long_t& rhs) {
    const unsigned char shift = temp_precise_t::fractional_bit_count
            - temp_long_t::fractional_bit_count;

    // convert to temp first to make sure it fits
    temp_t t = rhs;
    this->value_ = temp_precise_t::base_type(t.value_) << shift;
}

temp_long_t::temp_long_t(const temp_t& rhs) {
    this->value_ = rhs.value_;
}

temp_long_t::temp_long_t(const temp_precise_t& rhs) {
    const unsigned char shift = temp_precise_t::fractional_bit_count
            - temp_long_t::fractional_bit_count;
    this->value_ = rhs.value_ >> shift; //// could result in a 1 bit error due to rounding
}

// With operators for mixed types always returns the bigger type
// which is automatically converted afterwards if assigned to a small type

// Addition

// this looks recursive, but it prevents ambiguity
temp_t temp_t::operator+(temp_t const& rhs) {
    temp_t result(*this);
    result += rhs;
    return result;
}

temp_precise_t temp_t::operator+(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result += rhs;
    return result;
}

temp_long_t temp_t::operator+(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result += rhs;
    return result;
}

temp_long_t temp_long_t::operator+(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result += rhs;
    return result;
}

temp_long_t temp_long_t::operator+(temp_precise_t const& rhs) {
    temp_long_t result(*this);
    result += temp_long_t(rhs);
    return result;
}

temp_long_t temp_long_t::operator+(temp_t const& rhs) {
    temp_long_t result(*this);
    result += temp_long_t(rhs);
    return result;
}

temp_precise_t temp_precise_t::operator+(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result += rhs;
    return result;
}

temp_precise_t temp_precise_t::operator+(temp_t const& rhs) {
    temp_precise_t result(*this);
    result += temp_precise_t(rhs);
    return result;
}

temp_long_t temp_precise_t::operator+(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result += temp_long_t(rhs);
    return result;
}

// Unary Minus (change sign)
temp_t temp_t::operator-() const{
    temp_t result(*this);
    result.value_ = -result.value_;
    return result;
}

temp_long_t temp_long_t ::operator-() const{
    temp_long_t result(*this);
    result.value_ = -result.value_;
    return result;
}

temp_precise_t temp_precise_t::operator-() const{
    temp_precise_t result(*this);
    result.value_ = -result.value_;
    return result;
}


// Subtraction

// this looks recursive, but it prevents ambiguity
temp_t temp_t::operator-(temp_t const& rhs) {
    temp_t result(*this);
    result -= rhs;
    return result;
}

temp_precise_t temp_t::operator-(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result -= rhs;
    return result;
}

temp_long_t temp_t::operator-(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result -= rhs;
    return result;
}

temp_long_t temp_long_t::operator-(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result -= rhs;
    return result;
}

temp_long_t temp_long_t::operator-(temp_precise_t const& rhs) {
    temp_long_t result(*this);
    result -= temp_long_t(rhs);
    return result;
}

temp_long_t temp_long_t::operator-(temp_t const& rhs) {
    temp_long_t result(*this);
    result -= temp_long_t(rhs);
    return result;
}

temp_precise_t temp_precise_t::operator-(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result -= rhs;
    return result;
}

temp_precise_t temp_precise_t::operator-(temp_t const& rhs) {
    temp_precise_t result(*this);
    result -= temp_precise_t(rhs);
    return result;
}

temp_long_t temp_precise_t::operator-(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result -= temp_long_t(rhs);
    return result;
}

// Multiplication

temp_t temp_t::operator*(temp_t const& rhs) {
    temp_t result(*this);
    result *= rhs;
    return result;
}

temp_precise_t temp_t::operator*(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result *= rhs;
    return result;
}

temp_long_t temp_t::operator*(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result *= rhs;
    return result;
}

temp_long_t temp_long_t::operator*(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result *= rhs;
    return result;
}

temp_long_t temp_long_t::operator*(temp_precise_t const& rhs) {
    temp_long_t result(*this);
    result *= temp_long_t(rhs);
    return result;
}

temp_long_t temp_long_t::operator*(temp_t const& rhs) {
    temp_long_t result(*this);
    result *= temp_long_t(rhs);
    return result;
}

temp_precise_t temp_precise_t::operator*(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result *= rhs;
    return result;
}

temp_precise_t temp_precise_t::operator*(temp_t const& rhs) {
    temp_precise_t result(*this);
    result *= temp_precise_t(rhs);
    return result;
}

temp_long_t temp_precise_t::operator*(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result *= temp_long_t(rhs);
    return result;
}

// multiplication with uint16_t returns long temperature. Will be constrained later if assigned to temp_t
temp_long_t temp_t::operator*(const uint16_t rhs) {
    temp_long_t result(*this);
    result.value_ *= rhs;
    return result;
}

temp_long_t temp_precise_t::operator*(const uint16_t rhs) {
    temp_long_t resultUpper(*this); // lower precision bits will be discarded
    temp_precise_t resultLower(*this);
    const uint8_t duplicatedBits = temp_precise_t::fractional_bit_count - temp_long_t::fractional_bit_count;

    resultLower.value_ = resultLower.value_ & ((0x1 << duplicatedBits) - 1); // discard upper bits from lower, which are already in upper
    resultUpper.value_ *= rhs;
    resultLower.value_ *= rhs;

    return resultUpper + resultLower;
}

// Division

temp_t temp_t::operator/(temp_t const& rhs) {
    temp_t result(*this);
    result /= rhs;
    return result;
}

temp_precise_t temp_t::operator/(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result /= rhs;
    return result;
}

temp_long_t temp_t::operator/(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result /= rhs;
    return result;
}

temp_long_t temp_long_t::operator/(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result /= rhs;
    return result;
}

temp_long_t temp_long_t::operator/(temp_precise_t const& rhs) {
    temp_long_t result(*this);
    result /= temp_long_t(rhs);
    return result;
}

temp_long_t temp_long_t::operator/(temp_t const& rhs) {
    temp_long_t result(*this);
    result /= temp_long_t(rhs);
    return result;
}

temp_precise_t temp_precise_t::operator/(temp_precise_t const& rhs) {
    temp_precise_t result(*this);
    result /= rhs;
    return result;
}

temp_precise_t temp_precise_t::operator/(temp_t const& rhs) {
    temp_precise_t result(*this);
    result /= temp_precise_t(rhs);
    return result;
}

temp_long_t temp_precise_t::operator/(temp_long_t const& rhs) {
    temp_long_t result(*this);
    result /= temp_long_t(rhs);
    return result;
}

temp_t temp_t::operator/(const uint16_t rhs) {
    temp_t result(*this);
    result.value_ = (result.value_ + (rhs >> 1) ) / rhs; // rounded divide
    return result;
}

temp_precise_t temp_precise_t::operator/(const uint16_t rhs) {
    temp_precise_t result(*this);
    result.value_ = (result.value_ + (rhs >> 1) ) / rhs; // rounded divide
    return result;
}

temp_long_t temp_long_t::operator/(const uint16_t rhs) {
    temp_long_t result(*this);
    result.value_ = (result.value_ + (rhs >> 1) ) / rhs; // rounded divide
    return result;
}

// converts fixed point value to string, without using double/float
// resulting string is always length len (including \0). Spaces are prepended to achieve that
char * toStringImpl(const int32_t raw, // raw value of fixed point
        const unsigned char F, // number of fraction bits
        char buf[], // target buffer
        const uint8_t numDecimals, // number of decimals to print
        const uint8_t len, // maximum number of characters to print
        char format, // C or F
        bool absolute) // is this an absolute temperature? need to subtract 32 for F
        {

    char const digit[] = "0123456789";
    char* p;
    bool negative = false;

    // Use larger type to prevent overflow.
    int64_t shifter = raw;

    if(format =='F'){
        int8_t rounder = (shifter < 0) ? -25 : 25;
        shifter = (shifter * 90 + rounder) / 50;
        if(absolute){
            shifter += 32 << F;
        }
    }

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
    lldiv_t dv { };
    dv.quot = shifter;
    do { //Move back, inserting digits as u go
        if (p == &buf[len - 1 - numDecimals]) {
            *--p = '.'; // insert decimal point at right moment
        } else {
            dv = lldiv(dv.quot, 10);
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

// Converts string into fixed point and returns bool on success.
bool fromStringImpl(int32_t * raw, // result is put in this variable upon success
        unsigned char F, // number of fraction bits
        const char * s, // the string to convert
        char format,
        bool absolute,
        int32_t minimum, // minimum value for result
        int32_t maximum) // maximum value for result
        {
    // receive new value as null terminated string: "19.20"
    int64_t newValue;

    // larger type to prevent overflow
    int64_t decimalValue = 0;

    char* decimalPtr;
    char* end;
    // Check if - is in the string
    bool positive = (0 == strchr(s, '-'));

    newValue = strtol_impl(s, &end); // convert string to integer
    if (invalidStrtolResult(s, end)) {
        return false; // string was not valid
    }
    newValue = newValue << F; // shift to fixed point

    // find the point in the string to know whether we have decimals
    decimalPtr = strchr(s, '.'); // returns pointer to the point.
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
    newValue = positive ? newValue + decimalValue : newValue - decimalValue;
    if(format == 'F'){
        if(absolute){
            newValue -= 32 << F;
        }
        int8_t rounder = (newValue < 0) ? -45 : 45;
        newValue = (newValue * 50 + rounder) / 90; // rounded conversion from F to C
    }
    if (newValue >= minimum && newValue <= maximum) {
        *raw = newValue;
        return true;
    }
    return false; // if value is not within limits, it is likely invalid
}
