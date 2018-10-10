/*
 * Copyright 2018 BrewPi B.V.
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

#include "ActuatorAnalog.h"
#include "ActuatorDigitalChangeLogged.h"
#include "FixedPoint.h"
#include <functional>
#include <memory>
#include <stdint.h>

/**
	ActuatorPWM drives a (change logged) digital actuator and makes it available as range actuator, by quickly turning it on and off repeatedly.
 */
class ActuatorPwm final : public ActuatorAnalog {
public:
    using value_t = ActuatorAnalogValue;
    using State = ActuatorDigital::State;
    using update_t = ticks_millis_t;

private:
    const std::function<std::shared_ptr<ActuatorDigitalChangeLogged>()> m_target;
    duration_millis_t m_period;
    duration_millis_t m_dutyTime = 0;
    value_t m_dutySetting = 0;
    value_t m_dutyAchieved = 0;

public:
    /** Constructor.
     *  @param _m_
     target Digital actuator to be toggled with PWM
     *  @param _period PWM period in seconds
     *  @sa getPeriod(), setPeriod(), getTarget(), setTarget()
     */
    explicit ActuatorPwm(
        std::function<std::shared_ptr<ActuatorDigitalChangeLogged>()>&& target,
        duration_millis_t period = 4000);

    ~ActuatorPwm() = default;

    /** ActuatorPWM keeps track of the last high and low transition.
     *  This function returns the actually achieved value. This can differ from
     *  the set value, because the m_
     target actuator is not toggling.
     *
     * @return achieved duty cycle in fixed point.
     */
    virtual value_t value() const override final;

    /** Returns the set duty cycle
     * @return duty cycle setting in fixed point
     */
    virtual value_t setting() const override final
    {
        return m_dutySetting;
    }

    /** Sets a new duty cycle
     * @param val new duty cycle in fixed point
     */
    virtual void setting(value_t const& val) override final;

    //** Calculates whether the m_target should toggle and tries to toggle it if necessary
    /** Each update, the PWM actuator checks whether it should toggle to achieve the set duty cycle.
     * It checks wether the output pin toggled and updates it's internal counters to keep track of
     * the achieved duty cycle. When it toggles late, it tries to compensate for this in the next cycle.
     * To maintain the correct duty cycle average, it can make the next high time shorter or longer.
     * If needed, it can even skip going high or low. This will happen, for example, when the m_
     target is
     * a time limited actuator with a minimum on and/or off time.
     */
    update_t update(const update_t& now);

    /** returns the PWM period
     * @return PWM period in seconds
     */
    duration_millis_t period() const
    {
        return m_period;
    }

    /** sets the PWM period
     * @param sec new period in seconds
     */
    void period(const duration_millis_t& p)
    {
        m_period = p;
    }

    virtual bool valid() const override final;
};
