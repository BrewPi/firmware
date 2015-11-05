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

class DigitalPinActuator:
    private ActuatorBottom, public ActuatorDigital
{
    private:
        bool    invert;
        uint8_t pin;

    public:
        DigitalPinActuator(uint8_t pin,
                           bool    invert);

        ~DigitalPinActuator()
        {
        }

        inline void setActive(bool active)
        {
            digitalWrite(pin, (active ^ invert) ? HIGH : LOW);
        }

        inline bool isActive()
        {
            return ((digitalRead(pin) != LOW) ^ invert);
        }

        void write(uint8_t val)
        {
        }

        void serialize(JSON::Adapter& adapter){
            JSON::Class root(adapter, "ActuatorPin");
            bool state = isActive();
            JSON_E(adapter, state);
            JSON_E(adapter, pin);
            JSON_T(adapter, invert);
        }


        void update(){} // do nothing on periodic update
};
