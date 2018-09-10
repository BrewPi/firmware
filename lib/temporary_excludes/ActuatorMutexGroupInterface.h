/*
 * Copyright 2018 BrewPi/Elco Jacobs.
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
#include "Interface.h"
#include "ActuatorInterfaces.h"

class ActuatorMutexGroupInterface : public virtual Interface
{
public:
    ActuatorMutexGroupInterface() = default;
    virtual ~ActuatorMutexGroupInterface() = default;

    virtual void registerActuator(ActuatorMutexDriver * act) = 0;
    virtual void unRegisterActuator(ActuatorMutexDriver * act) = 0;

    virtual bool request(ActuatorMutexDriver * requester, ActuatorDigital::State newState, int8_t newPriority) = 0;

    /**
     * Cancels an open request, by setting the priority to -1.
     * This can be used when a request is honored, but the actuator could not go active for other reasons.
     * The request should then be canceled to prevent holding back other actuators
     * @param requester: pointer to actuator previously requested to go active
     */
    virtual void cancelRequest(ActuatorMutexDriver * requester) = 0;
};


