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

#include "ActuatorForwarder.h"
#include "temperatureFormats.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMutexGroup.h"
#include "ControllerMixins.h"

/* A driver actuator to wrap a digital Actuator and block SetActive calls if the mutex group does does not honor the request
 */

class ActuatorMutexDriver final : public ActuatorForwarder, public ActuatorDigitalInterface, public ActuatorMutexDriverMixin{
public:
    ActuatorMutexDriver(ActuatorDigitalInterface * target) : ActuatorForwarder(target), mutexGroup(nullptr){}
    ActuatorMutexDriver(ActuatorDigitalInterface * target, ActuatorMutexGroup * m) : ActuatorForwarder(target), mutexGroup(m){}

    ~ActuatorMutexDriver(){
        setMutex(nullptr);
    }

    uint8_t interfaceType() const override final { return ACTUATOR_TOGGLE_MUTEX; };

    void update() override final {
        target->update();
    }

    void fastUpdate() override final {
        target->fastUpdate();
    }

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
        if(mutexGroup){
            if(mutexGroup->request(this, active, priority)){
                target->setActive(active);
                if(target->isActive() != active){
                    // if setting the target failed, cancel the request to prevent blocking other actuators
                     mutexGroup->cancelRequest(this);
                }
            }
        }
        else{
            target->setActive(active); // if mutex group is not set, just pass on the call
        }
    }

    void setActive(bool active) override final{
        setActive(active, 127); // when priority not specified, default to highest priority
    }

    bool isActive() const override final {
        return target->isActive();
    }

private:
    ActuatorMutexGroup * mutexGroup;

friend class ActuatorMutexDriverMixin;
};



