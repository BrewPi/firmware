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




#include "Platform.h"
#if BREWPI_DS2413
#include "OneWire.h"
#include "DS2413.h"
#include "ActuatorInterfaces.h"

bool DS2413::cacheIsValid() const
{
    uint8_t upperInverted = (~cachedState & 0xf0) >> 4;
    uint8_t lower = cachedState & 0x0f;

    return upperInverted == lower;
}

bool DS2413::writeLatchBit(uint8_t pos,
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

    uint8_t mask   = latchWriteMask(pos);
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
        ok = update();
    }

    return ok;
}

bool DS2413::readLatchBit(pio_t pio, bool & result, bool useCached)
{
    if(!useCached || !cacheIsValid()){
        update();
    }

    return latchReadCached(pio, result);
}

bool DS2413::latchReadCached(pio_t pio, bool & result) const
{
    if(cacheIsValid()){
        return ((cachedState & latchReadMask(pio)) == 0);
    }
    else
    {
        return ActuatorDigital::State::Unknown;
    }
}

bool DS2413::update()
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
    return success;
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


bool DS2413::sense(pio_t pio, bool & result)
{
    update();
    if (cacheIsValid())
    {
        return false;
    }
    else
    {
        result = ((cachedState & senseMask(pio)) != 0);
    	return true;
    }
}

/**
 *
 * @return
 */
uint8_t DS2413::accessRead()    /* const */
{
    oneWire -> reset();
    oneWire -> select(address);
    oneWire -> write(ACCESS_READ);

    uint8_t data;
    data = oneWire -> read();

    return data;
}

/**
 *    Writes the state of all PIOs in one operation.
 *    /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1
 *    /param maxTries the maximum number of attempts before giving up.
 *    /return true on success
 */
bool DS2413::accessWrite(uint8_t b,
                         uint8_t maxTries)
{
    // b |= 0xFC;        /* Upper 6 bits should be set to 1's */
    uint8_t ack = 0;

    do{
        oneWire -> reset();
        oneWire -> select(address);
        oneWire -> write(ACCESS_WRITE);
        oneWire -> write(b);

        /* data is sent again, inverted to guard against transmission errors */
        oneWire -> write(~b);

        /* Acknowledgement byte, 0xAA for success, 0xFF for failure. */
        ack = oneWire -> read();

        if (ack == ACK_SUCCESS){
            oneWire -> read();    // status byte sent after ack
        }
    } while ((ack != ACK_SUCCESS) && (maxTries-- > 0));

    oneWire -> reset();

    return ack == ACK_SUCCESS;
}
#endif
