/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "StreamUtil.h"
#include "Values.h"

/**
 *
 */
class AnalogPinSensor : public Value
{
private:
	uint8_t pin;

public:
    typedef uint16_t analog_value_t;

    AnalogPinSensor(uint8_t pin) {
            this->pin = pin;
            pinMode(pin, INPUT);
    }

    void readTo(DataOut& out) {
        analog_value_t value = analogRead(pin);
        writePlatformEndianBytes(&value, sizeof(value), out);
    }

    uint8_t readStreamSize() {
        return sizeof(analog_value_t);
    }

    /*
     * Params: 1 - byte, lower 7 bits is pin number.
     */
    static Object* create(ObjectDefinition& def) {
        uint8_t v = def.in->next();
        return new_object(AnalogPinSensor(v));
    }

};

