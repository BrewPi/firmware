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
#include "Ticks.h"
#include <stdint.h>

#include "ActuatorForwarder.h"
#include "ControllerMixins.h"

class ActuatorPwm final : public ActuatorForwarder, public ActuatorRange, public ActuatorPwmMixin
{
    private:
        temp_t         value;
        int32_t        dutyLate;
        int32_t        periodLate;
        int32_t        dutyTime;
        ticks_millis_t periodStartTime;
        ticks_millis_t highToLowTime;
        ticks_millis_t lowToHighTime;
        // last elapsed time between two pulses. Could be different from period due to cycle skipping
        int32_t        cycleTime;
        int32_t        period_ms;
        temp_t         minVal;
        temp_t         maxVal;

    public:
        ActuatorPwm(ActuatorDigital * _target, uint16_t _period);

        ~ActuatorPwm() = default;

        temp_t min() const final {
            return minVal;
        }

        temp_t max() const final {
            return maxVal;
        }

        temp_t readValue() const final;

        temp_t getValue() const final {
            return value;
        }

        void setValue(temp_t const& val) final;

        void update() final;

        ticks_seconds_t getPeriod() const
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

        // calculates duty time based on expected period
        int32_t calculateDutyTime(int32_t expectedPeriod);

        // calculates priority from dutyTime and dutyLate
        int8_t priority();

        friend class ActuatorPwmMixin;
};
