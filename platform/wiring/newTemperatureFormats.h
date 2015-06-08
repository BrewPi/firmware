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

#define TEMP_BASE_CLASS          fpml::fixed_point<int16_t,  6> // 6 integer bits (-64/64, 9 fraction bits, 1 sign bit)
#define TEMP_LONG_BASE_CLASS     fpml::fixed_point<int32_t, 22> // 22 integer bits (-64/64, 9 fraction bits, 1 sign bit)
#define TEMP_PRECISE_BASE_CLASS  fpml::fixed_point<int32_t,  6> // 6 integer bits (-64/64, 24 fraction bits, 1 sign bit)


class temp_diff : public TEMP_BASE_CLASS {
public:
    using TEMP_BASE_CLASS::fixed_point; // inherit constructors from base class
};


class temp : public temp_diff {
public:
    temp(double rhs) : temp_diff(rhs){
        *this += temp_diff(-48.0); // store temperature with -48C offset when initializing from double
    }
    temp(int rhs) : temp_diff(rhs){
        *this += temp_diff(-48.0); // store temperature with -48C offset when initializing from int
    }
    template<typename T> temp(T t) : temp_diff(t){
        // construct same as temp diff
    }
};


class temp_diff_long : public TEMP_LONG_BASE_CLASS {
public:
    using TEMP_LONG_BASE_CLASS::fixed_point; // inherit constructors from base class

    // for conversion, type cast argument back to base class and let the base class handle construction
    temp_diff_long(temp_diff const& rhs) : TEMP_LONG_BASE_CLASS((TEMP_BASE_CLASS &) rhs) {}
};


class temp_long : public temp_diff_long {
public:
    temp_long(double rhs) : temp_diff_long(rhs){
        *this += temp_diff_long(-48.0); // store temperature with -48C offset when initializing from double
    }
    temp_long(int rhs) : temp_diff_long(rhs){
        *this += temp_diff_long(-48.0); // store temperature with -48C offset when initializing from int
    }
    template<typename T> temp_long(T t) : temp_diff_long(t){
        // construct same as temp diff
    }

    // for conversion, type cast argument back to fixed point class and let the base class handle construction
    temp_long(temp const& rhs) : temp_long((TEMP_BASE_CLASS &) rhs) {}
};


class temp_diff_precise : public TEMP_PRECISE_BASE_CLASS {
public:
    using TEMP_PRECISE_BASE_CLASS::fixed_point; // inherit constructors from base class

    // for conversion, type cast argument back to base class and let the base class handle construction
    temp_diff_precise(temp_diff const& rhs) : TEMP_PRECISE_BASE_CLASS((TEMP_BASE_CLASS &) rhs) {}
};


class temp_precise : public temp_diff_precise {
public:
    temp_precise(double rhs) : temp_diff_precise(rhs){
       *this += temp_diff_precise(-48.0); // store temperature with -48C offset when initializing from double
    }
    temp_precise(int rhs) : temp_diff_precise(rhs){
       *this += temp_diff_precise(-48.0); // store temperature with -48C offset when initializing from int
    }
       template<typename T> temp_precise(T t) : temp_diff_precise(t){
       // construct same as temp diff
    }

    // for conversion, type cast argument back to base class and let the base class handle construction
    temp_precise(temp const& rhs) : temp_precise((TEMP_BASE_CLASS &) rhs) {}
};

