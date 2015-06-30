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

#define ACT_PWM_MIN 0.0
#define ACT_PWM_MAX 100.0

class ActuatorPwm : public Actuator
{
    private:
        Actuator *     target;
        temp           value;
        int32_t        dutyLate;
        int32_t        periodLate;
        int32_t        dutyTime;
        ticks_millis_t periodStartTime;
        int32_t  period;
        const temp min = temp(ACT_PWM_MIN);
        const temp max = temp(ACT_PWM_MAX);

    public:
        ActuatorPwm(Actuator * _target, uint16_t _period);

        ActuatorPwm(const ActuatorPwm &obj){};


        temp readValue();
        void setValue(temp const& val);

        void update();

        ticks_millis_t getPeriod()
        {
            return period;
        }

        bool isActive()
        {
            return (value > temp(0.0));
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
        }

        void setPeriod(uint16_t sec){
            period = int32_t(sec) * 1000;
        }

        void setActive(bool active){
            if(active){
                value = max;
            }
            else{
                value = min;
            }
        }

        // recalculates duty time based on value and dutyLate and periodLate
        void recalculate();
};
