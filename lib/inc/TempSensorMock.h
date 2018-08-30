/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
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

#include "TempSensor.h"
#include "temperatureFormats.h"

class TempSensorMock final : public TempSensor {
public:
    TempSensorMock(temp_t initial)
        : value(initial)
        , connected(true)
    {
    }

    void setConnected(bool _connected)
    {
        connected = _connected;
    }

    void setTemp(temp_t val)
    {
        value = val;
    }

    virtual bool isConnected() const override final { return connected; }

    void add(temp_t delta)
    {
        value += delta;
    }

    virtual temp_t read() const override final
    {
        if (!isConnected())
            return temp_t::invalid();

        // limit precision to mimic DS18B20 sensor
        const uint8_t shift = temp_t::fractional_bit_count - 4; // DS18B20 has 0.0625 (1/16) degree C steps
        temp_t rounder;
        rounder.setRaw(1 << (shift - 1));
        temp_t noise;
        noise.setRaw(rand() % (1 << (shift - 1))); // noise max 1/2 bit
        rounder += noise;
        temp_t quantified = ((value + rounder) >> shift) << shift;
        return quantified;
    }

private:
    temp_t value;
    bool connected;
};
