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


class temp_diff : public fpml::fixed_point<int16_t, 7> {
    using fpml::fixed_point<int16_t, 7>::fixed_point; // inherit constructors

public:
    friend class temp_diff_long;
};

class temp : public temp_diff {
public:
    template<typename T> temp(T t) : temp_diff(t){
        *this += temp_diff(-48.0); // store temperature with -48C offset
    }
};

class temp_diff_long : public fpml::fixed_point<int32_t, 23> {
    using fpml::fixed_point<int32_t, 23>::fixed_point; // inherit constructors

    /*
    void operator = (const temp_diff& val){
        value_ = static_cast<int32_t>(val.value_);
    }*/
public:

    friend class temp_diff;
};

class temp_long : public temp_diff_long {
public:
    template<typename T> temp_long(T t) : temp_diff_long(t){
        *this += temp_diff_long(-48.0); // store temperature with -48C offset
    }
};
