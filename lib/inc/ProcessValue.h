/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#include "FixedPoint.h"

/*
 * A process value has a setting and an current value
 */
template <typename T>
class ProcessValue {

public:
    ProcessValue() = default;
    virtual ~ProcessValue() = default;

    // set the setting for the process value
    virtual void setting(const T& setting) = 0;
    // get the setting for the process value
    virtual T setting() const = 0;
    // read the actual value of the process value
    virtual T value() const = 0;
    // returns whether the process value is valid (data can be trusted)
    virtual bool valid() const = 0;
    // writes valid flag (will set setting to invalid)
    virtual void valid(bool v) = 0;
};
