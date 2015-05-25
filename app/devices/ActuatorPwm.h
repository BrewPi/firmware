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

#include "Actuator.h"
#include "Ticks.h"
#include <stdint.h>

class ActuatorPwm
{
    private:
        Actuator *     target;
        uint8_t        pwm;
        int32_t        dutyLate;
        int32_t        dutyTime;
        ticks_millis_t periodStartTime;
        const int32_t  period = 10000;

    public:
        ActuatorPwm(Actuator * driver);

        ActuatorPwm(const ActuatorPwm &obj){};

        void setPwm(uint8_t pwm);

        uint8_t getPwm();

        void updatePwm();

        ticks_millis_t getPeriod()
        {
            return period;
        }

        bool isActive()
        {
            return target -> isActive();
        }

#if ACTUATOR_VIRTUAL
        virtual ~ActuatorPwm(){}
#endif

        Actuator * getTarget()
        {
            return target;
        }
        void setTarget(Actuator * driver)
        {
            target = driver;
        }};
