/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 * Copyright 2015 Matthew McGowan
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

#include "ActuatorInterfaces.h"
#include "ActuatorDriver.h"
#include "Ticks.h"
#include <stdint.h>

class ActuatorPwm : public ActuatorDriver, public ActuatorRange
{
    private:
        temp_t         value;
        int32_t        dutyLate;
        int32_t        periodLate;
        int32_t        dutyTime;
        ticks_millis_t periodStartTime;
        int32_t        period_ms;
        temp_t         minVal;
        temp_t         maxVal;

    public:
        ActuatorPwm(ActuatorDigital * _target, uint16_t _period);

        virtual ~ActuatorPwm(){}

        temp_t min() const {
            return minVal;
        }

        temp_t max() const {
            return maxVal;
        }

        temp_t getValue() const {
            return value;
        }

        void setValue(temp_t const& val);

        void update();

        ticks_seconds_t getPeriod()
        {
            return period_ms / 1000; // return in seconds, same as set period
        }

        void setTarget(ActuatorDigital * t)
        {
            target = t;
        }

        void setPeriod(uint16_t sec){
            period_ms = int32_t(sec) * 1000;
        }

        // recalculates duty time based on configured period
        void recalculate();

        // recalculates duty time based on expected period
        void recalculate(int32_t expectedPeriod);

        // calculates priority from dutyTime and dutyLate
        int8_t priority();

        void serialize(JSON::Adapter& adapter){
            JSON::Class root(adapter, "ActuatorPwm");
            JSON_E(adapter, value);
            ticks_seconds_t period = getPeriod(); // don't use member directly, but value in seconds
            JSON_E(adapter, period);
            JSON_E(adapter, minVal);
            JSON_E(adapter, maxVal);
            JSON_T(adapter, target);
        }
};
