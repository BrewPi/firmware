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


/*
 * An actuator simply turns something on or off.                        
 */
class Actuator
{
	public:
    Actuator(){}
    virtual ~Actuator() {}
    virtual void setActive(bool active) = 0;
	virtual bool isActive() = 0;
	virtual void write(temp val) = 0;
};


/*
 * An actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator
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
	virtual void write(temp val) {
	    if(val < min){
	        val = min;
	    }
	    if(val > max){
	        val = max;
	    }
	    value = val;
	}

private:
	temp value;
	temp min;
	temp max;
};

/*
 * An actuator that simply remembers a true/false set value. This is primary used for testing.
 */
class BoolActuator : public Actuator
{
public:
	BoolActuator() : state(false) {}
	BoolActuator(bool initial) : state(initial) {}

	virtual void setActive(bool active) { state = active; }
	virtual bool isActive() { return state; }
	virtual void write(temp val) { state = val > temp(0.0);}

private:
	bool state;	
};

