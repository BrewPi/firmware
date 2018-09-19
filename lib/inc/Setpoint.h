/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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
    virtual temp_t setting() const = 0;
    virtual void setting(const temp_t& val) = 0;
    virtual bool valid() = 0;
};

class SetpointSimple final : public Setpoint {
private:
    temp_t m_setting = 0;
    bool m_valid = false;

public:
    SetpointSimple()
    {
    }

    SetpointSimple(temp_t _value)
        : m_setting(_value)
        , m_valid(true)
    {
    }
    ~SetpointSimple() = default;

    virtual temp_t setting() const override final
    {
        return m_setting;
    }

    virtual void setting(const temp_t& val) override final
    {
        m_setting = val;
        m_valid = true;
    }

    virtual bool valid() override final
    {
        return m_valid;
    }
};
