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

#include "TempSensor.h"
#include "Temperature.h"

class TempSensorMock final : public TempSensor {
private:
    temp_t m_value = 0;
    bool m_connected = false;

public:
    TempSensorMock()
    {
    }

    TempSensorMock(temp_t initial)
        : m_value(initial)
        , m_connected(true)
    {
    }

    void connected(bool _connected)
    {
        m_connected = _connected;
    }

    bool connected() const
    {
        return m_connected;
    }

    void value(temp_t val)
    {
        m_value = val;
    }

    temp_t value() const override final
    {
        return m_value;
    }

    virtual bool valid() const override final
    {
        return m_connected;
    }

    void add(temp_t delta)
    {
        m_value += delta;
    }
};
