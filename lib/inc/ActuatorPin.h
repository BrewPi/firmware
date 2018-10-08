/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 * Copyright 2015 Matthew McGowan.
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

#include "ActuatorDigital.h"
#include "pinmap_hal.h"

#include "gpio_hal.h"

class ActuatorPin final : public ActuatorDigital {
private:
    uint8_t m_pin = 255;
    bool m_invert = false;

public:
    ActuatorPin(){};
    ActuatorPin(uint8_t pin, bool invert)
        : m_pin(pin)
        , m_invert(invert)
    {
    }

    virtual ~ActuatorPin() = default;

    virtual void state(const State& state) override final
    {
        HAL_GPIO_Write(m_pin, ((state == State::Active) ^ m_invert));
    }

    virtual State state() const override final
    {
        return ((HAL_GPIO_Read(m_pin) != 0) ^ m_invert) ? State::Active : State::Inactive;
    }

    uint8_t pin() const
    {
        return m_pin;
    }

    void pin(uint8_t p)
    {
        m_pin = p;
    }

    bool invert() const
    {
        return m_invert;
    }

    void invert(bool inv)
    {
        m_invert = inv;
    }
};
