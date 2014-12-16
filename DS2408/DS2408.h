/*
 * Copyright 2013 Matthew McGowan 
 * Copyright 2013 BrewPi/Elco Jacobs.
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

#include <inttypes.h>
#include "../OneWireSwitch/OneWireSwitch.h"

typedef uint8_t pio_t;

#define  DS2408_FAMILY_ID 0x29

/*
 * Provides access to a OneWire-addressable dual-channel I/O device. 
 * The channel latch can be set to on (false) or off (true).
 * When a channel is off (PIOx=1), the channel state can be sensed. This is the power on-default. 
 *
 * channelRead/channelWrite reads and writes the channel latch state to turn the output transistor on or off
 * channelSense senses if the channel is pulled high.
 */
class DS2408 : public OneWireSwitch {
public:

    DS2408() {
    }

    ~DS2408() {
    }

    // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x2

    uint8_t pioMask(pio_t pio) {
        return pio++;
    }

    /*
     * Reads the output state of a given channel, defaulting to a given value on error.
     * Note that for a read to make sense the channel must be off (value written is 1).
     */
    bool channelRead(pio_t pio, bool defaultValue) {
        uint8_t result = channelReadAll();
        if (result < 0)
            return defaultValue;
        return (result & pioMask(pio));
    }

    /*
     * Reads the output state of a given channel, defaulting to a given value on error.
     * Note that for a read to make sense the channel must be off (value written is 1).
     */
    bool channelSense(pio_t pio, bool defaultValue) {
        uint8_t result = channelSenseAll();
        if (result < 0)
            return defaultValue;
        return (result & pioMask(pio));
    }

    uint8_t channelSenseAll() {
        uint8_t result = accessRead();
        // save bit3 and bit1 (PIO
        return result < 0 ? result : ((result & 0x4) >> 1 | (result & 1));
    }

    /*
     * Performs a simultaneous read of both channels.
     * /return <0 if there was an error otherwise bit 1 is channel A state, bit 2 is channel B state.
     */
    uint8_t channelReadAll() {
        uint8_t result = accessRead();
        // save bit3 and bit1 (PIO
        return result < 0 ? result : ((result & 0x8) >> 2 | (result & 2) >> 1);
    }

    /*
     * Writes to the latch for a given PIO.
     * /param set	1 to switch the pin off, 0 to switch on. 
     */
    bool channelWrite(pio_t pio, bool set) {
        bool ok = false;
        uint8_t result = channelReadAll();
        if (result >= 0) {
            uint8_t mask = pioMask(pio);
            if (set)
                result |= mask;
            else
                result &= ~mask;
            ok = channelWriteAll((uint8_t) result);
        }
        return ok;
    }

    bool channelWriteAll(uint8_t values) {
        return accessWrite(values);
    }

private:

};