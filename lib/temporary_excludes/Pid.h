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

#include "FilterCascaded.h"
#include "ProcessValue.h"
#include "Setpoint.h"
#include "TempSensor.h"
#include <cstring>

template <typename In, typename Out>
class Pid {
private:
    ProcessValue<In>& input;
    ProcessValue<Out>& output;
    Settings settings;
    State state;

    FilterCascaded inputFilter;
    FilterCascaded derivativeFilter;

    // remember previous setpoint, to be able to take the derivative of the error, instead of the input
    uint8_t failedReadCount;
    temp_t previousInputSetting;

    // state
    temp_t error = 0;
    temp_wide_t p = 0;
    temp_wide_t i = 0;
    temp_wide_t d = 0;
    temp_wide_t integral = 0;
    temp_precise_t derivative = 0;

    // settings
    temp_wide_t kp = 0; // proportional gain
    uint16_t ti = 0;    // integral time constant
    uint16_t td = 0;    // derivative time constant
    bool enabled = false;

public:
    Pid(ProcessValue<In>& _input,
        ProcessValue<Out>& _output);
    ~Pid() = default;

    void init();

    void update();

    void setConstants(temp_wide_t kp,
                      uint16_t ti,
                      uint16_t td);

    uint8_t getInputFiltering()
    {
        return inputFilter.getFiltering();
    }

    uint8_t getDerivativeFiltering()
    {
        return derivativeFilter.getFiltering();
    }

    void setInputFiltering(uint8_t b)
    {
        inputFilter.setFiltering(b);
    }

    void setDerivativeFiltering(uint8_t b)
    {
        derivativeFilter.setFiltering(b);
    }

    void enable()
    {
        settings.enabled = true;
    }

    void disable(bool turnOffOutput)
    {
        settings.enabled = false;
        state.error = 0.0;
        state.p = 0.0;
        state.i = 0.0;
        state.d = 0.0;
        if (turnOffOutput) {
            output.set(0.0);
        }
    }

    void setSettings(Settings const& from)
    {
        settings = from;
    }

    Settings const& getSettings()
    {
        return settings;
    }

    State const& getState()
    {
        return state;
    }
};
