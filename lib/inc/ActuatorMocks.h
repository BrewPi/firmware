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
#include "ControllerMixins.h"

/*
 * A range actuator that simply remembers the set value. This is primary used for testing.
 */
class ActuatorValue final : public ActuatorAnalog, public ActuatorValueMixin
{
public:
    // construct without arguments, val = invalid, min and max are defaults
    ActuatorValue() : currentValue(temp_t::invalid()), minimum(temp_t::min()), maximum(temp_t::max()){}

    // construct with just val, min and max are defaults
    ActuatorValue(temp_t initial) : currentValue(initial), minimum(temp_t::min()), maximum(temp_t::max()){}

    // construct with val, min, max
    ActuatorValue(temp_t initial, temp_t minVal, temp_t maxVal) : currentValue(initial), minimum(minVal), maximum(maxVal) {}

    ~ActuatorValue() = default;

    void accept(VisitorBase & v) override final{
    	v.visit(*this);
    }

    void set(temp_t const& val) override final {
        if(val < minimum){
            currentValue = minimum;
        }
        else if(val > maximum){
            currentValue = maximum;
        }
        else{
            currentValue = val;
        }
    }
    temp_t setting() const override final {
        return currentValue;
    }

    virtual temp_t value() const override final {
    	return currentValue;
    }


    void update() override final {}
    void fastUpdate() override final {}

    temp_t min() const {
        return minimum;
    }

    temp_t max() const {
        return maximum;
    }

private:
    temp_t currentValue;
    temp_t minimum;
    temp_t maximum;

friend class ActuatorValueMixin;
};

/*
 * An toggle actuator that simply remembers a true/false set value. This is primary used for testing.
 */
class ActuatorBool final : public ActuatorDigital, public ActuatorBoolMixin
{
public:
    ActuatorBool() : state(State::Inactive) {}
    ActuatorBool(State initial) : state(initial) {}
    ~ActuatorBool() = default;

    void accept(VisitorBase & v) override final{
    	v.visit(*this);
    }

    void setState(State s, int8_t priority = 127) override final { state = s; }
    State getState() const override final { return state; }

    void update() override final {}
    void fastUpdate() override final {}

private:
    State state;

friend class ActuatorBoolMixin;
};


/*
 * An digital actuators that does absolutely nothing. Used as default actuator
 */
class ActuatorNop final : public ActuatorDigital, public ActuatorNopMixin
{
public:
    ActuatorNop(){}
    ~ActuatorNop() = default;

    void accept(VisitorBase & v) override final{
    	v.visit(*this);
    }

    void setState(State state, int8_t priority = 127) override final {}
    State getState() const override final { return State::Inactive;}
    void update() override final {}
    void fastUpdate() override final {}

friend class ActuatorNopMixin;
};

/*
 * An linear actuator that does nothing and always returns invalid(). Linear equivalent of ActuatorNop
 */
class ActuatorInvalid final : public ActuatorAnalog, public ActuatorInvalidMixin
{
public:
    ActuatorInvalid() {}
    ~ActuatorInvalid() = default;

    void accept(VisitorBase & v) override final{
    	v.visit(*this);
    }

    void set(temp_t const& val) override final {}
    temp_t setting() const override final {
        return temp_t::invalid();
    }
    temp_t value() const override final {
        return temp_t::invalid();
    }

    void update() override final {}
    void fastUpdate() override final {}

friend class ActuatorInvalidMixin;
};
