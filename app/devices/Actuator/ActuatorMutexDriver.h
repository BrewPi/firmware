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

/* A driver actuator to wrap a digital Actuator and block SetActive calls if the mutex group does does not honor the request
 */

class ActuatorMutexDriver : public virtual ActuatorDriver, public ActuatorDigital{
public:
    ActuatorMutexDriver(ActuatorDigital * target) : ActuatorDriver(target), mutexGroup(nullptr){}
    ActuatorMutexDriver(ActuatorDigital * target, ActuatorMutexGroup * m) : ActuatorDriver(target), mutexGroup(m){}

    virtual ~ActuatorMutexDriver(){
        setMutex(nullptr);
    }

    virtual uint8_t type() const { return ACTUATOR_TOGGLE_MUTEX; };

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
    void setActive(bool active, int8_t priority){
        if(!active || !mutexGroup || mutexGroup->requestActive(this, priority)){
            target->setActive(active);
        }
    }

    void setActive(bool active){
        setActive(active, 127); // when priority not specified, default to highest priority
    }

    bool isActive(){
        return target->isActive();
    }

    void serialize(JSON::Adapter& adapter){
        JSON::Class root(adapter, "ActuatorMutexDriver");
        JSON_E(adapter, mutexGroup);
        JSON_T(adapter, target);
    }

private:
    ActuatorMutexGroup * mutexGroup;
};



