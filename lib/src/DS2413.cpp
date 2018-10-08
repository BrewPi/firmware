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

#include "../inc/DS2413.h"
#include "../inc/Logger.h"
#include "../inc/OneWire.h"

bool
DS2413::cacheIsValid() const
{
    uint8_t upperInverted = (~m_cachedState & 0xf0) >> 4;
    uint8_t lower = m_cachedState & 0x0f;

    return upperInverted == lower;
}

bool
DS2413::writeLatchBit(Pio pio,
                      bool set,
                      bool useCached)
{
    bool ok = false;
    uint8_t retries = 5;

    if (!useCached || !cacheIsValid()) {
        // read a fresh value form the device
        do {
            update();
        } while (!cacheIsValid() && (retries-- > 0));

        if (!cacheIsValid()) {
            return false; // cannot read from device successfully
        }
    }

    uint8_t mask = latchWriteMask(pio);
    uint8_t oldVal = writeByteFromCache();
    uint8_t newVal = oldVal;

    if (set) {
        newVal &= ~mask; // 0 means latch transistor is active
    } else {
        newVal |= mask; // 1 means latch transistor is inactive
    }

    if (oldVal == newVal) {
        ok = true; // skip write if already correct value to reduce OneWire communication
    } else {
        ok = channelWriteAll(newVal);
        ok = update();
    }

    return ok;
}

bool
DS2413::readLatchBit(Pio pio, bool& result, bool useCached)
{
    if (!useCached || !cacheIsValid()) {
        update();
    }

    return latchReadCached(pio, result);
}

bool
DS2413::latchReadCached(Pio pio, bool& result) const
{
    if (cacheIsValid()) {
        result = ((m_cachedState & latchReadMask(pio)) == 0);
        return true;
    } else {
        result = false;
        return false;
    }
}

bool
DS2413::update()
{
    m_cachedState = accessRead();
    bool success = cacheIsValid();
    if (m_connected && !success) {
        m_connected = false;
        CL_LOG_WARN("DS2413 disconnected: ") << getDeviceAddress().toString();
    } else if (!m_connected && success) {
        m_connected = true;
        CL_LOG_INFO("DS2413 connected: ") << getDeviceAddress().toString();
    }
    return success;
}

uint8_t
DS2413::writeByteFromCache()
{
    uint8_t returnval = 0;

    if (m_cachedState & latchReadMask(Pio::A)) {
        returnval |= latchWriteMask(Pio::A);
    }
    if (m_cachedState & latchReadMask(Pio::B)) {
        returnval |= latchWriteMask(Pio::B);
    }

    return returnval;
}

bool
DS2413::sense(Pio pio, bool& result)
{
    update();
    if (cacheIsValid()) {
        return false;
    } else {
        result = ((m_cachedState & senseMask(pio)) != 0);
        return true;
    }
}

/**
 *
 * @return
 */
uint8_t
DS2413::accessRead() /* const */
{
    oneWire.reset();
    oneWire.select(address.asUint8ptr());
    oneWire.write(ACCESS_READ);

    uint8_t data;
    data = oneWire.read();

    return data;
}

/**
 *    Writes the state of all PIOs in one operation.
 *    /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1
 *    /param maxTries the maximum number of attempts before giving up.
 *    /return true on success
 */
bool
DS2413::accessWrite(uint8_t b,
                    uint8_t maxTries)
{
    // b |= 0xFC;        /* Upper 6 bits should be set to 1's */
    uint8_t ack = 0;

    do {
        oneWire.reset();
        oneWire.select(address.asUint8ptr());
        oneWire.write(ACCESS_WRITE);
        oneWire.write(b);

        /* data is sent again, inverted to guard against transmission errors */
        oneWire.write(~b);

        /* Acknowledgement byte, 0xAA for success, 0xFF for failure. */
        ack = oneWire.read();

        if (ack == ACK_SUCCESS) {
            oneWire.read(); // status byte sent after ack
        }
    } while ((ack != ACK_SUCCESS) && (maxTries-- > 0));

    oneWire.reset();

    return ack == ACK_SUCCESS;
}
