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
// #include "ActuatorMutexGroup.h"

enum {
    ACTUATOR_RANGE,
    ACTUATOR_TOGGLE,
    ACTUATOR_THRESHOLD,
    ACTUATOR_TOGGLE_MUTEX
};

/*
 * An actuator can be driven by other classes and acts on something.
 * Actuators can also drive other actuators, getDeviceTarget finds the lowest level actuator recursively
 */
class Actuator
{
public:
    Actuator(){}
    virtual ~Actuator() {}
    virtual uint8_t type() const = 0;
	virtual Actuator ** getDeviviceTarget() const{
	    return nullptr;  // recursive call for composite classes until this level is reached.
	}
	virtual void update() = 0;
};


/*
 * An ActuatorDigital simply turns something on or off.
 */
class ActuatorDigital : public virtual Actuator //, public ActuatorMutexInterface
{
public:
    ActuatorDigital(){}
    virtual ~ActuatorDigital() {}
    virtual uint8_t type() const { return ACTUATOR_TOGGLE; };
    virtual void setActive(bool active) = 0;
    virtual bool isActive() = 0;
};


/*
 * An ActuatorRange has a range output between min and max
 */
class ActuatorRange : public virtual Actuator
{
public:
    ActuatorRange(){}
    virtual ~ActuatorRange() {}
    virtual uint8_t type() const { return ACTUATOR_RANGE; };
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t getValue() const = 0;
    virtual temp_t min() const = 0;
    virtual temp_t max() const = 0;
};

/*
 * An ThresholdActuator has switches on at a certain threshold. TODO: add hysteresis
 */
class ActuatorThreshold : public virtual Actuator

{
ActuatorThreshold(){}
    virtual ~ActuatorThreshold() {}
    virtual uint8_t type() const { return ACTUATOR_THRESHOLD; };
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t readValue() const = 0;
    virtual temp_t onValue() const = 0;
    virtual temp_t offValue() const = 0;
};

/*
 * A DriverActuator drivers another digital actuator, for example a PWM actuator can drive a pin actuator
 */
class ActuatorDriver : public virtual Actuator
{
protected:
    ActuatorDigital * target;

public:
    ActuatorDriver(ActuatorDigital * _target){
        target = _target;
    }
    virtual ~ActuatorDriver(){};

    Actuator ** getDeviviceTarget() {
        if( target->getDeviviceTarget() == 0){
            return (Actuator **) &target; // this is bottom
        }
        else{
            return target->getDeviviceTarget();  // this is not bottom
        }
    }

    virtual Actuator * getBareActuator() {
        return *(getDeviviceTarget());
    }

    virtual void update(){
        target->update();
    }
};


/*
 * An digital actuators that does absolutely nothing. Used as default actuator
 */
class ActuatorNop : public ActuatorDigital
{
public:
    ActuatorNop(){}
    ~ActuatorNop(){}

    virtual void setActive(bool active) {}
    virtual bool isActive() { return false;}
    virtual void update(){}
};

/*
 * An linear actuator that does nothing and always returns invalid(). Linear equavalent of ActuatorNop
 */
class ActuatorInvalid : public ActuatorRange
{
public:
    ActuatorInvalid() {}
    ~ActuatorInvalid() {}

    void setValue(temp_t const& val) {}
    temp_t getValue() const {
        return temp_t::invalid();
    }
    temp_t min() const {
        return temp_t::invalid();
    }
    temp_t max() const {
        return temp_t::invalid();
    }
    virtual void update(){}; //no actions required
};


