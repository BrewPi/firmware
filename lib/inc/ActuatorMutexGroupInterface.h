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


struct ActuatorPriority{
    ActuatorDigital * actuator;
    int8_t priority; // valid priorities are 0-127, at -128 the actuator is removed from the group
};


class ActuatorMutexGroupInterface : public virtual Interface
{
public:
    ActuatorMutexGroupInterface() = default;
    virtual ~ActuatorMutexGroupInterface() = default;

    virtual bool request(ActuatorDigital * requester, bool active, int8_t newPriority) = 0;
    virtual void cancelRequest(ActuatorDigital * requester) = 0;
};


