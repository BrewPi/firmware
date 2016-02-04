/*
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
#include "temperatureFormats.h"
#include "ActuatorInterfaces.h"
#include "ActuatorDriver.h"
#include "ActuatorMutexGroup.h"
#include "ControllerMixins.h"

#include "ControlLib.h"

CONTROL_LIB_BEGIN


/* A driver actuator to wrap a digital Actuator and block SetActive calls if the mutex group does does not honor the request
 */

class ActuatorMutexDriver : public ActuatorDriver, public ActuatorDigital, public ActuatorMutexDriverMixin{
public:
    ActuatorMutexDriver(ActuatorDigital * target) : ActuatorDriver(target), mutexGroup(nullptr){}
    ActuatorMutexDriver(ActuatorDigital * target, ActuatorMutexGroup * m) : ActuatorDriver(target), mutexGroup(m){}

    ~ActuatorMutexDriver(){
        setMutex(nullptr);
    }

    uint8_t type() const final { return ACTUATOR_TOGGLE_MUTEX; };

    void setMutex(ActuatorMutexGroup * mutex){
        if(mutexGroup != nullptr){
            mutexGroup->unRegisterActuator(this);
        }
        mutexGroup = mutex;
    }
    ActuatorMutexGroup * getMutex(){
        return mutexGroup;
    }

    // To activate actuator, permission is asked from mutexGroup, false is always allowed
    void setActive(bool active, int8_t priority) {
        if(!active || !mutexGroup || mutexGroup->requestActive(this, priority)){
            target->setActive(active);
        }
    }

    void setActive(bool active) final{
        setActive(active, 127); // when priority not specified, default to highest priority
    }

    bool isActive() const final {
        return target->isActive();
    }

protected:
    ActuatorMutexGroup * mutexGroup;

friend class ActuatorMutexDriverMixin;
};


CONTROL_LIB_END
