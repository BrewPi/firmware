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

#include "Logger.h"
#include "OneWireDevice.h"

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
    public OneWireDevice
{
public:
    /**
     * Constructor, initializes cached state to 0xff, which is an invalid state to signal that the cache is not valid yet.
     */
    DS2413(OneWire * oneWire, DeviceAddress address) :
        OneWireDevice(oneWire, address),
        cachedState(0xff), connected(false)
    {
    }

    /**
     * Destructor is default
     */
    ~DS2413() = default;

    /**
     *  The DS2413 returns data in the last 4 bits, the upper 4 bits are the complement.
     *  This allows checking wether the data is valid
     *  @returns whether data is valid (upper bits are complement of lower bits)
     */
    bool cacheIsValid() const;

    /**
     * Writes to the latch for a given PIO.
     * @param pio           channel/pin to write
     * @param set           1 to switch the open drain ON (pin low), 0 to switch it off.
     * @param useCached     do not read the pin states from the device
     * @return              true on success
     */
    bool writeLatchBit(pio_t pio,
                    bool  set,
                    bool  useCached);

    /**
     * Read the latch state of an output. True means latch is active
     * @param pio               pin number to read
     * @param defaultValue      value to return when the read fails
     * @param useCached         do not read current pin state from device, but use cached state
     * @return					true on success
     */
    bool readLatchBit(pio_t pio, bool & result, bool useCached);

    bool latchReadCached(pio_t pio, bool & result) const;
    /**
     * Periodic update to make sure the cache is valid.
     * Performs a simultaneous read of both channels and saves value to the cache.
     * When read fails, prints a warning that the DS2413 is disconnected
     *
     * @return					true on successful communication
     */
    bool update();

    /**
     * Reads the output state of a given channel, defaulting to a given value on error.
     * Note that for a read to make sense the channel must be off (value written is 1).
     * @return true on success
     */
    bool sense(pio_t pio, bool & result);

private:
    uint8_t cachedState; /** last value of read */
    bool connected; /** stores whether last read was succesful */

    // assumes pio is either 0 or 1, which translates to masks 0x8 and 0x2
    uint8_t latchReadMask(pio_t pio) const
    {
        return pio ? 0x8 : 0x2;
    }

    // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x2
    uint8_t latchWriteMask(pio_t pio) const
    {
        return pio ? 0x2 : 0x1;
    }

    /*
     * Writes all a bit field of all channel latch states
     */
    bool channelWriteAll(uint8_t values)
    {
        return accessWrite(values);
    }

    /**
     *  Rearranges latch state bits from cached read to last write bit field.
     *  @return bits in order suitable for writing (different order than read)
     */
    uint8_t writeByteFromCache();


    /**
     * Read all values at once, both current state and sensed values for the DS2413.
     * Output state of 8 pins for the DS2408.
     */
    uint8_t accessRead();

    /**
     * Writes the state of all PIOs in one operation.
     * @param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1 for DS2413. All bits are used for DS2408
     * @param maxTries the maximum number of attempts before giving up.
     * @return true on success
     */
    bool accessWrite(uint8_t b, uint8_t maxTries = 3);


    /**
     * Returns bitmask to extract the sense channel for the given pin from a read
     * @return bitmask which can be used to extract the bit corresponding to the channel
     */
    uint8_t senseMask(pio_t pio) const
    {
        return pio ? 0x4 : 0x1;    // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x4
    }


private:
    static const uint8_t ACCESS_READ = 0xF5;
    static const uint8_t ACCESS_WRITE = 0x5A;
    static const uint8_t ACK_SUCCESS = 0xAA;
    static const uint8_t ACK_ERROR = 0xFF;
};
