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
#include "ActuatorMutexGroup.h"
#include "ActuatorMutexDriver.h"
#include "ControllerMixins.h"

/* A driver actuator to wrap a digital Actuator and block SetActive calls if the mutex group does does not honor the request
 */

class ActuatorMutexDriver final : public ActuatorDigital, public ActuatorMutexDriverMixin{
public:
    ActuatorMutexDriver(ActuatorDigital & target) : target(target), mutexGroup(nullptr){}
    ActuatorMutexDriver(ActuatorDigital & target, ActuatorMutexGroup * m);

    ~ActuatorMutexDriver(){
        setMutex(nullptr);
    }

    virtual void accept(VisitorBase & v) override final{
    	v.visit(*this);
    }

    virtual void update() override final {
        target.update();
    }

    virtual void fastUpdate() override final {
        target.fastUpdate();
    }

    void setMutex(ActuatorMutexGroup * mutex);

    void signalDeletedMutexGroup(ActuatorMutexGroup * mutex){
		if(mutexGroup == mutex){
			mutexGroup = nullptr;
		}
	}

    ActuatorMutexGroup * getMutex(){
        return mutexGroup;
    }

    // To activate actuator, permission is asked from mutexGroup, false is always allowed
    // when priority not specified, default to highest priority
    virtual void setState(State state, int8_t priority = 127) override final;

    virtual State getState() const override final {
        return target.getState();
    }

private:
    ActuatorDigital & target;
    ActuatorMutexGroup * mutexGroup;

friend class ActuatorMutexDriverMixin;
};



