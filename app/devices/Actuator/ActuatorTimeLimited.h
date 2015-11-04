/*
 * Copyright 2013 Matthew McGowan
 * Copyright 2013 BrewPi/Elco Jacobs.
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
#include "ActuatorDriver.h"
#include "Ticks.h"

class ActuatorTimeLimited: public ActuatorDriver, public ActuatorDigital
{
public:
    ActuatorTimeLimited(ActuatorDigital * _target,
            ticks_seconds_t   _minOnTime = 120,
            ticks_seconds_t   _minOffTime = 180,
            ticks_seconds_t   _maxOnTime = UINT16_MAX) : ActuatorDriver(_target)
    {
        minOnTime  = _minOnTime;
        minOffTime = _minOffTime;
        maxOnTime  = _maxOnTime;
        toggleTime = 0;
        target     = _target;
        active     = _target -> isActive();
    }

    virtual ~ActuatorTimeLimited(){}

    void setActive(bool active);    // returns new actuator state

    bool isActive()
    {
        return active;    // target->isActive(); - this takes 20 bytes more
    }

    void update();

    void setTimes(ticks_seconds_t   _minOnTime,
                  ticks_seconds_t   _minOffTime,
                  ticks_seconds_t   _maxOnTime = UINT16_MAX){
        minOnTime = _minOnTime;
        minOffTime = _minOffTime;
        maxOnTime = _maxOnTime;
    }
    ticks_seconds_t timeSinceToggle(void) const;

    void serialize(JSON::Adapter& adapter){
        JSON::Class root(adapter, "ActuatorTimeLimited");
        JSON_E(adapter, minOnTime);
        JSON_E(adapter, minOffTime);
        JSON_E(adapter, maxOnTime);
        JSON_E(adapter, active);
        JSON_T(adapter, target);
    }

    private:
        ticks_seconds_t        minOnTime;
        ticks_seconds_t        maxOnTime;
        ticks_seconds_t        minOffTime;
        ticks_seconds_t        toggleTime;
        bool            active;
};
