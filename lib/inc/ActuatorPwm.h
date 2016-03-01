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

/**
	ActuatorPWM drives a digital actuator and makes it available as range actuator, by quickly turning it on and off repeatedly.


 */
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
    /** Constructor.
     *  @param _target Digital actuator to be toggled with PWM
     *  @param _period PWM period in seconds
     *  @sa getPeriod(), setPeriod(), getTarget(), setTarget()
     */
    ActuatorPwm(ActuatorDigital * _target, uint16_t _period);

    ~ActuatorPwm() = default;

    /** Returns minimum value
     */
    temp_t min() const final {
        return minVal;
    }

    /** Returns maximum value
     */
    temp_t max() const final {
        return maxVal;
    }

    /** ActuatorPWM keeps track of the last high and low transition.
     *  This function returns the actually achieved value. This can differ from
     *  the set value, because the target actuator is not toggling.
     *
     * @return achieved duty cycle in fixed point.
     */
    temp_t readValue() const final;

    /** Returns the set duty cycle
     * @return duty cycle setting in fixed point
     */
    temp_t getValue() const final {
        return value;
    }

    /** Sets a new duty cycle
     * @param val new duty cycle in fixed point
     */
    void setValue(temp_t const& val) final;

    //** Calculates whether the target should toggle and tries to toggle it if necessary
    /** Each update, the PWM actuator checks whether it should toggle to achieve the set duty cycle.
     * It checks wether the output pin toggled and updates it's internal counters to keep track of
     * the achieved duty cycle. When it toggles late, it tries to compensate for this in the next cycle.
     * To maintain the correct duty cycle average, it can make the next high time shorter or longer.
     * If needed, it can even skip going high or low. This will happen, for example, when the target is
     * a time limited actuator with a minimum on and/or off time.
     */
    void update() final;

    /** returns the PWM period
     * @return PWM period in seconds
     */
    ticks_seconds_t getPeriod() const
    {
        return period_ms / 1000; // return in seconds, same as set period
    }

    /** sets the PWM period
     * @param sec new period in seconds
     */
    void setPeriod(uint16_t sec){
        period_ms = int32_t(sec) * 1000;
    }



private:
    /** Calculates priority to be used with the MutexDriver.
     * Actuators will get a higher priority if their duty cycle is higher, or they are far behind
     * @return priority for this actuator to become active
     * @sa MutexDriver
     */
    int8_t priority();

    /** Calculates duty time based on expected period
     * @param expectedPeriod estimate of the duration of the period in ms
     * @return duration of the high period in ms
     */
    int32_t calculateDutyTime(int32_t expectedPeriod);

    friend class ActuatorPwmMixin;
};
