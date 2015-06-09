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
#include <fixed_point.h>
#include <type_traits>

#define TEMP_BASE_CLASS          fpml::fixed_point<int16_t,  6> // 6 integer bits (-64/64, 9 fraction bits, 1 sign bit)
#define TEMP_LONG_BASE_CLASS     fpml::fixed_point<int32_t, 22> // 22 integer bits (-64/64, 9 fraction bits, 1 sign bit)
#define TEMP_PRECISE_BASE_CLASS  fpml::fixed_point<int32_t,  6> // 6 integer bits (-64/64, 24 fraction bits, 1 sign bit)



template <typename base>
class temp_diff_template : public base {
public:
    using base::fixed_point; // inherit constructors from base class
};

template <typename base>
class temp_template : public base {
public:
    using base::fixed_point; // inherit constructors from base class
    temp_template(double rhs) : base(rhs){
        *this += base(-48.0); // store temperature with -48C offset when initializing from double
    }
    temp_template(int rhs) : base(rhs){
        *this += base(-48.0); // store temperature with -48C offset when initializing from int
    }
};

using temp_diff = temp_diff_template<TEMP_BASE_CLASS>;
using temp = temp_template<TEMP_BASE_CLASS>;
using temp_diff_long = temp_diff_template<TEMP_LONG_BASE_CLASS>;
using temp_long = temp_template<TEMP_LONG_BASE_CLASS>;
using temp_diff_precise = temp_diff_template<TEMP_PRECISE_BASE_CLASS>;
using temp_precise = temp_template<TEMP_PRECISE_BASE_CLASS>;



// To convert, cast back to base class. Base class handles conversion for different fixed point types
static inline temp_diff_long toLong(temp_diff & val){
    temp_diff_long copy((TEMP_BASE_CLASS &) val);
    return copy;
}

static inline temp_long toLong(temp & val){
    temp_long copy((TEMP_BASE_CLASS &) val);
    return copy;
}

static inline temp_diff_precise toPrecise(temp_diff & val){
    temp_diff_precise copy((TEMP_BASE_CLASS &) val);
    return copy;
}

static inline temp_precise toPrecise(temp & val){
    temp_precise copy((TEMP_BASE_CLASS &) val);
    return copy;
}

