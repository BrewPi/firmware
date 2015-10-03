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
#include "Actuator.h"
#include "Ticks.h"

class ActuatorTimeLimited:
    public ActuatorDriver, public ActuatorDigital
{
    public:
        ActuatorTimeLimited(ActuatorDigital * _target,
                      uint16_t   _minOnTime = 120,
                      uint16_t   _minOffTime = 180,
                      uint16_t   _maxOnTime = UINT16_MAX) : ActuatorDriver(_target)
        {
            minOnTime  = _minOnTime;
            minOffTime = _minOffTime;
            maxOnTime  = _maxOnTime;
            target     = _target;
            toggleTime = 0;
            active     = _target -> isActive();
        }

        virtual ~ActuatorTimeLimited(){}

        void setActive(bool active);    // returns new actuator state

        bool isActive()
        {
            return active;    // target->isActive(); - this takes 20 bytes more
        }

        void update();

        void setTimes(uint16_t   _minOnTime,
                      uint16_t   _minOffTime,
                      uint16_t   _maxOnTime = UINT16_MAX){
            minOnTime = _minOnTime;
            minOffTime = _minOffTime;
            maxOnTime = _maxOnTime;
        }
        ticks_seconds_t timeSinceToggle(void) const;

    private:
        uint16_t        minOnTime;
        uint16_t        maxOnTime;
        uint16_t        minOffTime;
        ticks_seconds_t toggleTime;
        bool            active;
};
