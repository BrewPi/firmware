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

#include "newTemperatureFormats.h"

// Converting constructors, which shift and constrain the value.

temp::temp(const temp_precise& rhs){
    unsigned char shift = temp_precise::fractional_bit_count - temp::fractional_bit_count;
    this->value_ = rhs.value_ >> shift;
}


temp::temp(const temp_long& rhs){
    // temp and temp_long have same number of fraction bits, no shifting needed
    static_assert(temp::fractional_bit_count == temp_long::fractional_bit_count,
            "temp and temp_long should have same number of fraction bits");

    if(rhs.value_ < min_val){
        this->value_ = min_val;
    }
    else if(rhs.value_ > max_val){
        this->value_ = max_val;
    }
    else{
        this->value_ = rhs.value_;
    }
}

temp::temp(const temp_small& rhs){
    const unsigned char shift = temp::fractional_bit_count - temp_small::fractional_bit_count;
    this->value_ = rhs.value_ << shift;
}

temp_precise::temp_precise(const temp& rhs){
    // temp and temp_precise have same number of integer bits, so this will not overflow
    static_assert(temp::integer_bit_count == temp_precise::integer_bit_count,
                "temp and temp_long should have same number of integer bits");

    unsigned char shift = temp_precise::fractional_bit_count - temp::fractional_bit_count;
    this->value_ = rhs.value_ << shift;
}

temp_precise::temp_precise(const temp_long& rhs){
    const unsigned char shift = temp_precise::fractional_bit_count - temp_long::fractional_bit_count;

    // convert to temp first to make sure it fits
    temp t = rhs;
    this->value_ = t.value_ << shift;
}

temp_long::temp_long(const temp& rhs){
    this->value_ = rhs.value_;
}

temp_long::temp_long(const temp_precise& rhs){
    unsigned char shift = temp_precise::fractional_bit_count - temp_long::fractional_bit_count;
    this->value_ = rhs.value_ >> shift;
}




// Addition operators for mixed types. Always returns the bigger type
// which is automatically converted afterwards if assigned to a small type
temp_precise temp::operator+(temp_precise & y) {
    temp_precise result(*this);
    result += y;
    return result;
}

temp_long temp::operator+(temp_long & y) {
    temp_long result(*this);
    result += y;
    return result;
}

temp_long temp_long::operator+(temp_precise & y) {
    temp_long result(*this);
    result += temp_long(y);
    return result;
}

temp_long temp_long::operator+(temp & y) {
    temp_long result(*this);
    result += temp_long(y);
    return result;
}

temp_precise temp_precise::operator+(temp & y) {
    temp_precise result(*this);
    result += temp_precise(y);
    return result;
}

temp_long temp_precise::operator+(temp_long & y) {
    temp_long result(*this);
    result += temp_long(y);
    return result;
}


// operators for rhs from another type. Always return the result as the biggest type
// which is automatically converted afterwards if assigned to a small type

temp_precise temp::operator-(temp_precise & y) {
    temp_precise result(*this);
    result -= y;
    return result;
}

temp_long temp::operator-(temp_long & y) {
    temp_long result(*this);
    result -= y;
    return result;
}

temp_long temp_long::operator-(temp_precise & y) {
    temp_long result(*this);
    result -= temp_long(y);
    return result;
}

temp_long temp_long::operator-(temp & y) {
    temp_long result(*this);
    result -= temp_long(y);
    return result;
}

temp_precise temp_precise::operator-(temp & y) {
    temp_precise result(*this);
    result -= temp_precise(y);
    return result;
}

temp_long temp_precise::operator-(temp_long & y) {
    temp_long result(*this);
    result -= temp_long(y);
    return result;
}


// multiplication operators for mixed types. Always returns the bigger type
// which is automatically converted afterwards if assigned to a small type
// not defined for small type, it is only meant to be added as offset to sensors
temp_precise temp::operator*(temp_precise & y) {
    temp_precise result(*this);
    result *= y;
    return result;
}

temp_long temp::operator*(temp_long & y) {
    temp_long result(*this);
    result *= y;
    return result;
}

temp_long temp_long::operator*(temp_precise & y) {
    temp_long result(*this);
    result *= temp_long(y);
    return result;
}

temp_long temp_long::operator*(temp & y) {
    temp_long result(*this);
    result *= temp_long(y);
    return result;
}

temp_precise temp_precise::operator*(temp & y) {
    temp_precise result(*this);
    result *= temp_precise(y);
    return result;
}

temp_long temp_precise::operator*(temp_long & y) {
    temp_long result(*this);
    result *= temp_long(y);
    return result;
}


// converts fixed point value to string, without using double/float
// resulting string is always length len (including \0). Spaces are prepended to achieve that
char * toStringImpl(
        const int32_t raw, // raw value of fixed point
        const unsigned char F, // number of fraction bits
        char buf[], // target buffer
        const uint8_t numDecimals, // number of decimals to print
        const uint8_t len) // maximum number of characters to print
{

    char const digit[] = "0123456789";
    char* p;
    bool negative = false;

    // Use larger type to prevent overflow.
    int64_t shifter = raw;

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

// Converts string into fixed point and returns bool on success.
bool fromStringImpl(
        int32_t * raw, // result is put in this variable upon success
        unsigned char F, // number of fraction bits
        const char * s, // the string to convert
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

    newValue = strtol_impl(s, &end);// convert string to integer
    if (invalidStrtolResult(s, end)) {
        return false; // string was not valid
    }
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
    newValue = positive ? newValue + decimalValue : newValue - decimalValue;
    if (newValue >= minimum && newValue <= maximum){
        *raw = newValue;
        return true;
    }
    return false; // if value is not within limits, it is like invalid
}
