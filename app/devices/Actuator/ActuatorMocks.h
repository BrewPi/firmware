/*
 * Copyright 2013 Matthew McGowan
 * Copyright 2013 BrewPi/Elco Jacobs.
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

#include <stdint.h>
#include "temperatureFormats.h"
#include "ActuatorInterfaces.h"

/*
 * A range actuator that simply remembers the set value. This is primary used for testing.
 */
class ActuatorValue : public ActuatorRange
{
public:
    // construct without arguments, val = invalid, min and max are defaults
    ActuatorValue() : value(temp_t::invalid()), minimum(temp_t::min()), maximum(temp_t::max()){}

    // construct with just val, min and max are defaults
    ActuatorValue(temp_t initial) : value(initial), minimum(temp_t::min()), maximum(temp_t::max()){}

    // construct with val, min, max
    ActuatorValue(temp_t initial, temp_t minVal, temp_t maxVal) : value(initial), minimum(minVal), maximum(maxVal) {}

    virtual ~ActuatorValue(){}

    virtual void setValue(temp_t const& val) {
        if(val < minimum){
            value = minimum;
        }
        else if(val > maximum){
            value = maximum;
        }
        else{
            value = val;
        }
    }
    virtual temp_t getValue() const{
        return value;
    }
    virtual void update(){}; //no actions required

    temp_t min() const{
        return minimum;
    }

    temp_t max() const{
        return maximum;
    }

private:
    temp_t value;
    temp_t minimum;
    temp_t maximum;
};

/*
 * An toggle actuator that simply remembers a true/false set value. This is primary used for testing.
 */
class ActuatorBool : public ActuatorDigital
{
public:
    ActuatorBool() : state(false) {}
    ActuatorBool(bool initial) : state(initial) {}

    virtual void setActive(bool active) { state = active; }
    virtual bool isActive() { return state; }

    virtual void update(){}; //no actions required

private:
    bool state;
};
