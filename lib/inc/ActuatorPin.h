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

class ActuatorPin final : public ActuatorDigital {
private:
    bool invert;
    uint8_t pin;

public:
    ActuatorPin(uint8_t pin,
                bool invert);

    ~ActuatorPin() = default;

    virtual void state(const State& state) override final
    {
        digitalWrite(pin, ((state == State::Active) ^ invert) ? HIGH : LOW);
    }

    virtual State state() const override final
    {
        return ((digitalRead(pin) != LOW) ^ invert) ? State::Active : State::Inactive;
    }

    virtual update_t update(const update_t& t) override final { return update_t_max(); }; // do nothing on periodic update
};
