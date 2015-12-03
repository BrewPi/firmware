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

typedef uint8_t pio_t;

#ifndef DS2413_SUPPORT_SENSE
#define DS2413_SUPPORT_SENSE 1
#endif

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
        DS2413()
        {
            cachedState = 0;
        }

        /*
         *  The DS2413 returns data in the last 4 bits, the upper 4 bits are the complement.
         * An XOR operation of the two should give 0 as result
         */
        bool cacheIsValid()
        {
            return ((cachedState & 0x0f) == (((~cachedState) & 0xf0) >> 4));
        }

        /*
         * Writes to the latch for a given PIO.
         * /param pio           channel/pin to write
         * /param set           1 to switch the open drain ON (pin low), 0 to switch it off.
         * /param useCached     do not read the pin states from the device
         * /return              true on success, false on failure
         */
        bool latchWrite(pio_t pio,
                        bool  set,
                        bool  useCached)
        {
            bool    ok      = false;
            uint8_t retries = 5;

            if (!useCached || !cacheIsValid())
            {
                // read a fresh value form the device
                do
                {
                    update();
                }
                while (!cacheIsValid() && (retries-- > 0));

                if (!cacheIsValid())
                {
                    return false;    // cannot read from device successfully
                }
            }

            uint8_t mask   = latchWriteMask(pio);
            uint8_t oldVal = writeByteFromCache();
            uint8_t newVal = oldVal;

            if (set)
            {
                newVal &= ~mask; // 0 means latch transistor is active
            }
            else
            {
                newVal |= mask; // 1 means latch transistor is inactive
            }

            if (oldVal == newVal)
            {
                ok = true;    // skip write if already correct value to reduce OneWire communication
            }
            else
            {
                ok = channelWriteAll(newVal);
                update();
            }

            return ok;
        }

        /*
         * Read the latch state of an output. True means latch is active
         * /param pio               pin number to read
         * /param defaultValue      value to return when the read fails
         * /param useCached         do not read current pin state from device, but use cached state
         */
        bool latchRead(pio_t pio,
                       bool defaultValue,
                       bool useCached)
        {
            if(!useCached || !cacheIsValid()){
                update();
            }

            if(cacheIsValid()){
                return ((cachedState & latchReadMask(pio)) == 0);
            }
            else
            {
                return defaultValue;
            }
        }

    private:
        uint8_t cachedState;

        // assumes pio is either 0 or 1, which translates to masks 0x8 and 0x2
        uint8_t latchReadMask(pio_t pio)
        {
            return pio ? 0x8 : 0x2;
        }

        // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x2
        uint8_t latchWriteMask(pio_t pio)
        {
            return pio ? 0x2 : 0x1;
        }

        /*
         * Performs a simultaneous read of both channels and saves value to the cache.
         */
        void update()
        {
            cachedState = accessRead();
        }

        /*
         * Writes all a bit field of all channel latch states
         */
        bool channelWriteAll(uint8_t values)
        {
            return accessWrite(values);
        }

        /*
         *  Rearranges latch state bits from cached read to last write bit field.
         */
        uint8_t writeByteFromCache()
        {
            uint8_t returnval = 0;

            for (uint8_t i = 0; i < 2; i++)
            {
                if (cachedState & latchReadMask(i))
                {
                    returnval |= latchWriteMask(i);
                }
            }

            return returnval;
        }

#if DS2413_SUPPORT_SENSE

    public:

        /*
         * Returns bitmask to extract the sense channel for the given pin from a read
         */
        uint8_t senseMask(pio_t pio)
        {
            return pio ? 0x4 : 0x1;    // assumes pio is either 0 or 1, which translates to masks 0x1 and 0x3
        }

        /*
         * Reads the output state of a given channel, defaulting to a given value on error.
         * Note that for a read to make sense the channel must be off (value written is 1).
         */
        bool sense(pio_t pio,
                   bool  defaultValue)
        {
            update();

            if (cacheIsValid())
            {
                return ((cachedState & senseMask(pio)) != 0);
            }
            else
            {
                return defaultValue;
            }
        }
#endif

};
