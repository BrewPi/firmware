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

#include "OneWireSwitch.h"
#include "Logger.h"

typedef uint8_t pio_t;
#define  DS2413_FAMILY_ID 0x3A

/*
 * Provides access to a OneWire-addressable dual-channel I/O device.
 * The channel latch can be set to on (false) or off (true).
 * When a channel is off (PIOx=1), the channel state can be sensed. This is the power on-default.
 *
 * channelRead/channelWrite reads and writes the channel latch state to turn the output transistor on or off
 * channelSense senses if the channel is pulled high.
 */
class DS2413:
    public OneWireSwitch
{
public:
    DS2413() : cachedState(0), connected(false)
    {
    }

    /**
     *  The DS2413 returns data in the last 4 bits, the upper 4 bits are the complement.
     *  This allows checking wether the data is valid
     *  @return: whether data is valid (upper bits are complement of lower bits)
     */
    bool cacheIsValid() const;

    /**
     * Writes to the latch for a given PIO.
     * @param pio           channel/pin to write
     * @param set           1 to switch the open drain ON (pin low), 0 to switch it off.
     * @param useCached     do not read the pin states from the device
     * @return              true on success, false on failure
     */
    bool latchWrite(pio_t pio,
                    bool  set,
                    bool  useCached);

    /**
     * Read the latch state of an output. True means latch is active
     * @param pio               pin number to read
     * @param defaultValue      value to return when the read fails
     * @param useCached         do not read current pin state from device, but use cached state
     */
    bool latchRead(pio_t pio,
                   bool defaultValue,
                   bool useCached);

    bool latchReadCached(pio_t pio,
                   bool defaultValue) const;
    /**
     * Periodic update to make sure the cache is valid.
     * Performs a simultaneous read of both channels and saves value to the cache.
     * When read fails, prints a warning that the DS2413 is disconnected/
     */
    void update();


private:
    uint8_t cachedState; /** last value of read */
    bool connected; /** stores whether last read was succesful */

    // assumes pio is either 0 or 1, which translates to masks 0x8 and 0x2
    inline uint8_t latchReadMask(pio_t pio) const
    {
        return pio ? 0x8 : 0x2;
    }

    // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x2
    inline uint8_t latchWriteMask(pio_t pio) const
    {
        return pio ? 0x2 : 0x1;
    }

    /*
     * Writes all a bit field of all channel latch states
     */
    inline bool channelWriteAll(uint8_t values)
    {
        return accessWrite(values);
    }

    /**
     *  Rearranges latch state bits from cached read to last write bit field.
     *  @return bits in order suitable for writing (different order than read)
     */
    uint8_t writeByteFromCache();

public:

    /**
     * Returns bitmask to extract the sense channel for the given pin from a read
     * @return bitmask which can be used to extract the bit corresponding to the channel
     */
    inline uint8_t senseMask(pio_t pio) const
    {
        return pio ? 0x4 : 0x1;    // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x3
    }

    /**
     * Reads the output state of a given channel, defaulting to a given value on error.
     * Note that for a read to make sense the channel must be off (value written is 1).
     * @return value of channel when cache is valid, defautl value if cache is not valid
     */
    bool sense(pio_t pio,
               bool  defaultValue);

};
