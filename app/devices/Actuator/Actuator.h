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
#include "newTemperatureFormats.h"

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
    virtual uint8_t type(){ return ACTUATOR_TOGGLE; };
    virtual void setActive(bool active) = 0;
	virtual bool isActive() = 0;
};

/*
 * An LinearActuator has a linear range output between min and max
 */
class LinearActuator : public Actuator
{
    public:
    LinearActuator(){}
    virtual ~LinearActuator() {}
    virtual uint8_t type(){ return ACTUATOR_RANGE; };
    virtual void setValue(temp const& val) = 0;
    virtual temp readValue() = 0;
    virtual temp min() = 0;
    virtual temp max() = 0;
};

/*
 * An LinearActuator has a linear range output between min and max
 */
class ActuatorThreshold : public Actuator
{
    public:
    ActuatorThreshold(){}
    virtual ~ActuatorThreshold() {}
    virtual uint8_t type(){ return ACTUATOR_THRESHOLD; };
    virtual void setValue(temp const& val) = 0;
    virtual temp readValue() = 0;
    virtual temp onValue() = 0;
    virtual temp offValue() = 0;
};


/*
 * An linear actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator : public LinearActuator
{
public:
	ValueActuator(temp initial, temp minVal, temp maxVal) : value(initial), min(minVal), max(maxVal) {}

	virtual void setActive(bool active) {
	    if(active){
	        value = max;
	    }
	    else{
	        value = min;
	    }
	}
	virtual bool isActive() { return value > min; }
	virtual void setValue(temp const& val) {
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
	virtual temp readValue(){
	    return value;
	}

private:
	temp value;
	temp min;
	temp max;
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
	virtual bool isActive() { return state; }

private:
	bool state;	
};

