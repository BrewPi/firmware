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

#include "ActuatorAnalog.h"
#include <stdint.h>
#include "future_std.h"

/*
 * A range actuator that simply remembers the set value. This is primary used for testing.
 */
class ActuatorAnalogMock final : public ActuatorAnalog {
private:
    value_t minimum = cnl::numeric_limits<value_t>::lowest();
    value_t maximum = cnl::numeric_limits<value_t>::max();
    value_t currentValue = 0;

public:
    // construct without arguments, val = invalid, min and max are defaults
    ActuatorAnalogMock() {}

    // construct with just val, min and max are defaults
    ActuatorAnalogMock(const value_t& initial)
        : currentValue(initial)
    {
    }

    // construct with val, min, max
    ActuatorAnalogMock(const value_t& initial, const value_t& minVal, const value_t& maxVal)
        : minimum(minVal)
        , maximum(maxVal)
    {
        set(initial);
    }

    ~ActuatorAnalogMock() = default;

    virtual void set(const value_t& val) override final
    {
        currentValue = std::clamp(val, minimum, maximum);
    }
    virtual value_t setting() const override final
    {
        return currentValue;
    }

    virtual value_t value() const override final
    {
        return currentValue;
    }

    value_t min() const
    {
        return minimum;
    }

    value_t max() const
    {
        return maximum;
    }

    bool valid() const
    {
        return true;
    }
};
