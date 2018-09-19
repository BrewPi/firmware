/*
 * Copyright 2015 BrewPi / Elco Jacobs
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

#include "../inc/Pid.h"
#include "../inc/future_std.h"

void
Pid::update()
{
    auto input = inputPtr();
    if (input && input->valid()) {
        auto inputSetting = input->setting();
        auto inputValue = input->value();
        auto inputError = inputSetting - inputValue;

        filter.add(inputError);

    } else {
        // handle invalid input
        if (inputFailureCount < 30) {
            ++inputFailureCount;
        } else {
            enabled(false);
        }
    }

    if (!m_enabled) {
        return;
    }

    // calculate PID parts.
    m_error = filter.read();
    m_p = m_kp * m_error;

    m_integral = (m_ti != 0) ? m_integral + m_p / m_ti : 0;
    m_i = m_integral;

    m_derivative = filter.readDerivative<decltype(m_derivative)>();
    m_d = -m_kp * (m_derivative * m_td);

    auto pidResult = m_p + m_i + m_d;

    out_t outputValue = pidResult;

    // try to set the output to the desired setting
    auto output = outputPtr();
    if (output && output->valid()) {
        output->setting(outputValue);

        // get the clipped setting from the actuator
        out_t achievedSetting = output->setting();

        if (m_ti != 0) { // 0 has been chosen to indicate that the integrator is disabled. This also prevents divide by zero.
            // update integral with anti-windup back calculation
            // pidResult - output is zero when actuator is not saturated

            auto antiWindup = out_t(0);

            if (pidResult != achievedSetting) {
                // clipped to actuator min or max set in target actuator
                // calculate anti-windup from setting instead of actual value, so it doesn't dip under the maximum
                antiWindup = 3 * (pidResult - achievedSetting); // anti windup gain is 3
                // make sure anti-windup is at least increment when clipping to prevent further windup
                antiWindup = (m_p >= 0) ? std::max(m_p / m_ti, antiWindup) : std::min(m_p / m_ti, antiWindup);
            } else {

                // Actuator could be not reaching set value due to physics or limits in its target actuator
                // Get the actual achieved value in actuator. This could differ due to slowness time/mutex limits
                auto achievedValue = output->value();

                // Anti windup gain is 3
                antiWindup = 3 * (pidResult - achievedValue);

                // Disable anti-windup if integral part dominates. But only if it counteracts p.
                if (m_i < 0 && m_p < 0 && m_i < 3 * m_p) {
                    antiWindup = 0;
                }
                if (m_i > 0 && m_p > 0 && m_i > 3 * m_p) {
                    antiWindup = 0;
                }
            }

            // make sure integral does not cross zero and does not increase by anti-windup

            decltype(m_integral) newIntegral = m_integral - antiWindup;
            if (m_integral >= 0) {
                m_integral = std::clamp(newIntegral, decltype(m_integral)(0), m_integral);
            } else {
                m_integral = std::clamp(newIntegral, m_integral, decltype(m_integral)(0));
            }
        }
    }
}
