/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#include "Temperature.h"

class Setpoint {
public:
    Setpoint() = default;
    virtual ~Setpoint() = default;
    virtual temp_t read() const = 0;
    virtual void write(temp_t val) = 0;
    virtual bool valid() = 0;
};

class SetpointSimple final : public Setpoint {
private:
    temp_t setting = 0;
    bool _valid = false;

public:
    SetpointSimple()
    {
    }

    SetpointSimple(temp_t _value)
        : setting(_value)
        , _valid(true)
    {
    }
    ~SetpointSimple() = default;

    virtual temp_t read() const override final
    {
        return setting;
    }

    virtual void write(temp_t val) override final
    {
        setting = val;
        _valid = true;
    }

    virtual bool valid() override final
    {
        return _valid;
    }
};
