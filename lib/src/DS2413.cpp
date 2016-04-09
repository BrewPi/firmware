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


#include "OneWire.h"

#include "DS2413.h"

bool DS2413::cacheIsValid() const
{
    uint8_t upperInverted = (~cachedState & 0xf0) >> 4;
    uint8_t lower = cachedState & 0x0f;

    return upperInverted == lower;
}

bool DS2413::latchWrite(pio_t pio,
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

bool DS2413::latchRead(pio_t pio,
               bool defaultValue,
               bool useCached)
{
    if(!useCached || !cacheIsValid()){
        update();
    }

    return latchReadCached(pio, defaultValue);
}

bool DS2413::latchReadCached(pio_t pio,
               bool defaultValue) const
{
    if(cacheIsValid()){
        return ((cachedState & latchReadMask(pio)) == 0);
    }
    else
    {
        return defaultValue;
    }
}

void DS2413::update()
{
    cachedState = accessRead();
    bool success = cacheIsValid();
    if(connected && !success){
        connected = false;
        char addressString[17];
        printBytes(address, 8, addressString);
        logWarningString(DS2413_DISCONNECTED, addressString);
    }
    else if (!connected && success){
        connected = true;
        char addressString[17];
        printBytes(address, 8, addressString);
        logInfoString(DS2413_CONNECTED, addressString);
    }
}

uint8_t DS2413::writeByteFromCache()
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
bool DS2413::sense(pio_t pio,
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
