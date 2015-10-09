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
    ActuatorValue(temp_t initial, temp_t minVal, temp_t maxVal) : value(initial), min(minVal), max(maxVal) {}
    virtual ~ActuatorValue(){}

    virtual void setValue(temp_t const& val) {
        if(val < min){
            value = min;
        }
        else if(val > max){
            value = max;
        }
        else{
            value = val;
        }
    }
    virtual temp_t getValue() const{
        return value;
    }
    virtual void update(){}; //no actions required

private:
    temp_t value;
    temp_t min;
    temp_t max;
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
