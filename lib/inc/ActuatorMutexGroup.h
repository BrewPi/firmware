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
#include "Interface.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMutexDriver.h"
#include <vector>
#include "ControllerMixins.h"

class ActuatorPriority {
public:
	ActuatorPriority(ActuatorMutexDriver * act, int8_t p) : actuator(act), priority(p){};
	~ActuatorPriority() = default;
    ActuatorMutexDriver * actuator;
    int8_t priority; // valid priorities are 0-127. -1 is used for actuators that are not trying to get the mutex
};

class ActuatorMutexGroup final :
    public Interface,
    public ActuatorMutexGroupMixin
{
public:
    ActuatorMutexGroup() :
        deadTime(0),
        lastActiveTime(0),
        lastActiveActuator(nullptr)
	{
    }

    ~ActuatorMutexGroup();

    void accept(VisitorBase & v) {
    	v.visit(*this);
    }

    void registerActuator(ActuatorMutexDriver * act);
    void unRegisterActuator(ActuatorMutexDriver * act);


    bool request(ActuatorMutexDriver * requester, ActuatorDigital::State newState, int8_t newPriority);

    /**
     * Cancels an open request, by setting the priority to -1.
     * This can be used when a request is honored, but the actuator could not go active for other reasons.
     * The request should then be canceled to prevent holding back other actuators
     * @param requester: pointer to actuator previously requested to go active
     */
    void cancelRequest(ActuatorMutexDriver * requester);

    ticks_millis_t getDeadTime(){
        return deadTime;
    }
    void setDeadTime(ticks_millis_t);

    ticks_millis_t getWaitTime();

    void update() final;

    void fastUpdate() final {} // not needed

private:
    ticks_millis_t deadTime; // minimum time between switching from one actuator to the other
    ticks_millis_t lastActiveTime;
    ActuatorMutexDriver * lastActiveActuator;
    std::vector<ActuatorPriority> actuatorPriorities;

friend class ActuatorMutexGroupMixin;
};
