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

#include "Ticks.h"
#include "Actuator.h"
#include <vector>

struct ActuatorPriority{
    ActuatorDigital * actuator;
    int8_t priority; // valid priorities are 0-127, at -128 the actuator is removed from the group
};

class ActuatorMutexGroup {
public:
    ActuatorMutexGroup(){
        deadTime = 0;
        lastActiveTime = 0;
    }
    ~ActuatorMutexGroup(){

    };

    void registerActuator(ActuatorDigital * act, int8_t prio);
    void unRegisterActuator(uint8_t index);

    uint8_t find(ActuatorDigital * act);

    bool requestActive(ActuatorDigital * requester, int8_t newPriority);

    ticks_millis_t getDeadTime(){
        return deadTime;
    }
    void setDeadTime(ticks_millis_t);

    ticks_millis_t getWaitTime();

    void update();

private:
    ticks_millis_t deadTime;
    ticks_millis_t lastActiveTime;
    std::vector<ActuatorPriority> actuatorPriorities;
};

/* ActuatorMutexInterface should be inherited by abstract base classes for digital actuators
 */

class ActuatorMutexInterface {
public:
    ActuatorMutexInterface(){}
    virtual ~ActuatorMutexInterface(){}

    virtual void setMutex(ActuatorMutexGroup * mutex) = 0;
    virtual ActuatorMutexGroup * getMutex() = 0;
};

/* ActuatorMutex base class should be inherited by lowest level digital actuators
 * Only the lowest level should have a pointer to the mutex group
 */

class ActuatorMutexBase : public virtual ActuatorMutexInterface {
public:
    ActuatorMutexBase(){
        mutexGroup = 0;
    }
    virtual ~ActuatorMutexBase(){}

    void setMutex(ActuatorMutexGroup * mutex){
        mutexGroup = mutex;
    }
    ActuatorMutexGroup * getMutex(){
        return mutexGroup;
    }
private:
    ActuatorMutexGroup * mutexGroup;
};
