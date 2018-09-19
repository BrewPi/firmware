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

#pragma once

#include "FixedPoint.h"
#include "FpFilterChain.h"
#include "ProcessValue.h"
#include <cstring>
#include <functional>

class Pid {
public:
    using in_t = safe_elastic_fixed_point<11, 12, int32_t>;
    using out_t = safe_elastic_fixed_point<11, 12, int32_t>;
    using integral_t = safe_elastic_fixed_point<11, 30, int64_t>;
    using derivative_t = safe_elastic_fixed_point<1, 23, int32_t>;

private:
    const std::function<std::shared_ptr<ProcessValue<in_t>>()> inputPtr;
    const std::function<std::shared_ptr<ProcessValue<out_t>>()> outputPtr;

    FpFilterChain<in_t> filter;

    // state
    in_t m_error = 0;
    in_t m_p = 0;
    in_t m_i = 0;
    in_t m_d = 0;
    integral_t m_integral = 0;
    derivative_t m_derivative = 0;

    uint8_t inputFailureCount = 0;

    // settings
    in_t m_kp = 0;              // proportional gain
    uint16_t m_ti = 0;          // integral time constant
    uint16_t m_td = 0;          // derivative time constant
    uint8_t m_filterChoice = 0; // input filter index
    bool m_enabled = false;

public:
    explicit Pid(
        std::function<std::shared_ptr<ProcessValue<in_t>>()>&& _input,
        std::function<std::shared_ptr<ProcessValue<out_t>>()>&& _output)
        : inputPtr(_input)
        , outputPtr(_output)
        , filter(0)
    {
    }

    ~Pid() = default;

    void init();

    void update();

    // state
    auto error()
    {
        return m_error;
    }

    auto p()
    {
        return m_p;
    }

    auto i()
    {
        return m_i;
    }

    auto d()
    {
        return m_d;
    }

    // settings
    auto kp()
    {
        return m_kp;
    }

    void kp(const in_t& arg)
    {
        m_kp = arg;
    }

    auto ti()
    {
        return m_ti;
    }

    void ti(const uint16_t& arg)
    {
        m_ti = arg;
    }

    auto td()
    {
        return m_td;
    }

    void td(const uint16_t& arg)
    {
        m_td = arg;
    }

    auto filterChoice()
    {
        return m_filterChoice;
    }

    auto filterThreshold()
    {
        return filter.getStepThreshold();
    }

    void configureFilter(const uint8_t& choice, const in_t& threshold)
    {
        m_filterChoice = choice;
        filter.setParams(choice, threshold);
    }

    void enabled(bool state)
    {
        if (!state) {
            m_error = 0;
            m_p = 0;
            m_i = 0;
            m_d = 0;
            if (auto ptr = outputPtr()) {
                ptr->setting(in_t(0));
            }
        }
        m_enabled = state;
    }

    auto enabled()
    {
        return m_enabled;
    }
};
