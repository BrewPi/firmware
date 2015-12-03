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

#include "OneWire.h"
#include "OneWireAddress.h"
#include <string.h>


class OneWireSwitch {
public:
    OneWireSwitch();
    OneWireSwitch(const OneWireSwitch& orig);
    virtual ~OneWireSwitch();

    void init(OneWire* oneWire, DeviceAddress address);
    DeviceAddress& getDeviceAddress();
    bool validAddress(OneWire* oneWire, DeviceAddress deviceAddress);

protected:
    OneWire* oneWire;
    DeviceAddress address;

public:    
    /*
     * Read all values at once, both current state and sensed values for the DS2413.
     * Output state of 8 pins for the DS2408.
     */
    uint8_t accessRead();

    /*
     * Writes the state of all PIOs in one operation.
     * /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1 for DS2413. All bits are used for DS2408
     * /param maxTries the maximum number of attempts before giving up.
     * /return true on success
     */
    bool accessWrite(uint8_t b, uint8_t maxTries = 3);
};


