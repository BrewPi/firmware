/*
 * Copyright 2015 Matthew McGowan
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

#include <stdint.h>
#include "ActuatorInterfaces.h"
#include "defaultDevices.h"

/*
 * An ActuatorDigitalReference holds a pointer to a digital actuator
 * when the actuator is not set (null) it points to the default actuator
 */
class ActuatorDigitalReference : public virtual ActuatorDigitalInterface
{
public:
    ActuatorDigitalReference() : target(nullptr) {};
    ~ActuatorDigitalReference() = default;

    void setActive(bool active, int8_t priority = 127) override {
        ref()->setActive(active);
    }

    bool isActive() const override {
        return ref()->isActive();
    }

    void setTarget(ActuatorDigitalInterface * act){
        target = act;
    }

    void unsetTarget(){
        target = nullptr;
    }

private:
    ActuatorDigitalInterface * ref() const {
        if (target == nullptr){
           return defaultActuator();
        }
        return target;
    }

    ActuatorDigitalInterface * target;
};


/*
 * An ActuatorRangeReference holds a pointer to a range (linear) actuator
 * when the actuator is not set (null) it points to the default linear actuator
 */
class ActuatorRangeReference : public virtual ActuatorRangeInterface
{
public:
    ActuatorRangeReference() : target(nullptr) {};
    ~ActuatorRangeReference() = default;

    void setValue(temp_t const& val) {
        ref()->setValue(val);
    }
    temp_t getValue() const override {
        return ref()->getValue();
    }
    temp_t readValue() const override {
        return ref()->readValue();
    }
    temp_t min() const override {
        return ref()->min();
    }
    temp_t max() const override {
        return ref()->max();
    }

    void setTarget(ActuatorRangeInterface * act){
        target = act;
    }

    void unsetTarget(){
        target = nullptr;
    }

private:
    ActuatorRangeInterface * ref() const {
        if (target == nullptr){
           return defaultLinearActuator();
        }
        return target;
    }

    ActuatorRangeInterface * target;
};

