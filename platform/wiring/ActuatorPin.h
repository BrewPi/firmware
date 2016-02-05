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

#include "Platform.h"
#include "ActuatorInterfaces.h"
#include "ActuatorBottom.h"

CONTROL_LIB_BEGIN

class ActuatorPin :
    private ActuatorBottom, public ActuatorDigital, public ActuatorPinMixin
{
    protected:
        bool    invert;
        uint8_t pin;

    public:
        ActuatorPin(uint8_t pin,
                           bool    invert);

        ~ActuatorPin() = default;

        inline void setActive(bool active)
        {
            digitalWrite(pin, (active ^ invert) ? HIGH : LOW);
        }

        inline bool isActive() const
        {
            return ((digitalRead(pin) != LOW) ^ invert);
        }

        void write(uint8_t val)
        {
        }

        void update(){} // do nothing on periodic update
    friend class ActuatorPinMixin;
};

CONTROL_LIB_END
