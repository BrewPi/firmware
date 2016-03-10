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

#include "ActuatorForwarder.h"
#include "ActuatorInterfaces.h"
#include "Ticks.h"
#include "ControllerMixins.h"

class ActuatorTimeLimited final : public ActuatorForwarder, public ActuatorDigital, public ActuatorTimeLimitedMixin
{
public:
    ActuatorTimeLimited(ActuatorDigital * _target,
            ticks_seconds_t   _minOnTime = 120,
            ticks_seconds_t   _minOffTime = 180,
            ticks_seconds_t   _maxOnTime = UINT16_MAX) : ActuatorForwarder(_target)
    {
        minOnTime  = _minOnTime;
        minOffTime = _minOffTime;
        maxOnTime  = _maxOnTime;
        toggleTime = 0;
        state      = _target -> isActive();
    }

    ~ActuatorTimeLimited() = default;

    void setActive(bool active) final;    // returns new actuator state

    bool isActive() const final
    {
        return state;    // target->isActive(); - this takes 20 bytes more
    }

    void update() final;

    void fastUpdate() final {} // time limit is in seconds, no fast update needed


    void setTimes(ticks_seconds_t   _minOnTime,
                  ticks_seconds_t   _minOffTime,
                  ticks_seconds_t   _maxOnTime = UINT16_MAX){
        minOnTime = _minOnTime;
        minOffTime = _minOffTime;
        maxOnTime = _maxOnTime;
    }
    ticks_seconds_t timeSinceToggle(void) const;

private:
    ticks_seconds_t        minOnTime;
    ticks_seconds_t        maxOnTime;
    ticks_seconds_t        minOffTime;
    ticks_seconds_t        toggleTime;
    // shadow copy to prevent sending unnecessary updates to target
    bool                   state;

    friend class ActuatorTimeLimitedMixin;
};
