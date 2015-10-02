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

enum {
    ACTUATOR_RANGE,
    ACTUATOR_TOGGLE,
    ACTUATOR_THRESHOLD
};

/*
 * An actuator simply turns something on or off.                        
 */
class Actuator
{
public:
    Actuator(){}
    virtual ~Actuator() {}
    virtual uint8_t type() const{ return ACTUATOR_TOGGLE; };
    virtual void setActive(bool active) = 0;
	virtual bool isActive() const = 0;
	virtual Actuator ** getDeviviceTarget() const{
	    return 0;  // recursive call for super classes until this level is reached.
	}
};

/*
 * An LinearActuator has a linear range output between min and max
 */
class LinearActuator : public Actuator
{
public:
    LinearActuator(){}
    virtual ~LinearActuator() {}
    virtual uint8_t type() const { return ACTUATOR_RANGE; };
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t readValue() const = 0;
    virtual temp_t min() const = 0;
    virtual temp_t max() const = 0;
};

/*
 * An ThresholdActuator has switches on at a certain threshold. TODO: add hysteresis
 */
class ActuatorThreshold : public Actuator
{
ActuatorThreshold(){}
    virtual ~ActuatorThreshold() {}
    virtual uint8_t type() const{ return ACTUATOR_THRESHOLD; };
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t readValue() const = 0;
    virtual temp_t onValue() const = 0;
    virtual temp_t offValue() const = 0;
};

/*
 * A DriverActuator drivers another actuator, for example a PWM actuator can drive a pin actuator
 */
class DriverActuator : public Actuator
{
protected:
    Actuator * target;

public:
    DriverActuator(Actuator * _target){
        target = _target;
    }
    virtual ~DriverActuator(){};

    Actuator ** getDeviviceTarget() {
        if( target->getDeviviceTarget() == 0){
            return &target; // this is bottom
        }
        else{
            return target->getDeviviceTarget();  // this is not bottom
        }
    }

    virtual Actuator * getBareActuator() {
        return *(getDeviviceTarget());
    }
};

/*
 * A linear actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator : public LinearActuator
{
public:
	ValueActuator(temp_t initial, temp_t minVal, temp_t maxVal) : value(initial), min(minVal), max(maxVal) {}

	virtual void setActive(bool active) {
	    if(active){
	        value = max;
	    }
	    else{
	        value = min;
	    }
	}
	virtual bool isActive() const { return value > min; }
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
	virtual temp_t readValue() const{
	    return value;
	}

private:
	temp_t value;
	temp_t min;
	temp_t max;
};

/*
 * An toggle actuator that simply remembers a true/false set value. This is primary used for testing.
 */
class BoolActuator : public Actuator
{
public:
	BoolActuator() : state(false) {}
	BoolActuator(bool initial) : state(initial) {}

	virtual void setActive(bool active) { state = active; }
	virtual bool isActive() const { return state; }

private:
	bool state;	
};


/*
 * An linear actuator that does nothing and always returns invalid().
 */
class LinearActuatorInvalid : public LinearActuator
{
public:
    LinearActuatorInvalid() {}

    void setActive(bool active) {}
    bool isActive() const {
        return false;
    }
    void setValue(temp_t const& val) {}
    temp_t readValue() const {
        return temp_t::invalid();
    }
    temp_t min() const {
        return temp_t::invalid();
    }
    temp_t max() const {
        return temp_t::invalid();
    }
};
