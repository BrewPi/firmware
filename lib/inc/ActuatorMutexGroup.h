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
#include "ActuatorInterfaces.h"
#include "ActuatorMutexGroupInterface.h"
#include <vector>
#include "ControllerMixins.h"


class ActuatorMutexGroup final :
    public virtual ActuatorMutexGroupInterface,
    public ActuatorMutexGroupMixin
{
public:
    ActuatorMutexGroup() :
        deadTime(0),
        lastActiveTime(0),
        lastActiveActuator(nullptr)
    {
    }

    virtual ~ActuatorMutexGroup() = default;

    virtual void accept(VisitorBase & v) override final {
    	v.visit(*this);
    }

    virtual void update() override final;

    virtual void fastUpdate() override final {} // not needed

    virtual bool request(ActuatorDigital * requester, bool active, int8_t newPriority) override final;

    /**
     * Cancels an open request, by setting the priority to -1.
     * This can be used when a request is honored, but the actuator could not go active for other reasons.
     * The request should then be canceled to prevent holding back other actuators
     * @param requester: pointer to actuator previously requested to go active
     */
    virtual void cancelRequest(ActuatorDigital * requester) override final;

    ticks_millis_t getDeadTime(){
        return deadTime;
    }
    void setDeadTime(ticks_millis_t);

    ticks_millis_t getWaitTime();


private:
    ActuatorPriority * registerActuator(ActuatorDigital * act, int8_t prio);
    void unRegisterActuator(ActuatorDigital * act); // remove by pointer
    void unRegisterActuator(size_t index); // remove by index
    size_t find(ActuatorDigital * act);

    ticks_millis_t deadTime; // minimum time between switching from one actuator to the other
    ticks_millis_t lastActiveTime;
    ActuatorDigital * lastActiveActuator;
    std::vector<ActuatorPriority> actuatorPriorities;

friend class ActuatorMutexGroupMixin;
};


