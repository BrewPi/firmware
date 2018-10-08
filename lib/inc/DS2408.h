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

#include "OneWireDevice.h"

#define DS2408_FAMILY_ID 0x29

/**
 * Provides access to a OneWire-addressable 8-channel I/O device.
 * Each output has a pull-down transistor, which can be enabled by writing 0 to the pio output latch.
 * This pulls the output pin to GND.
 *
 * When the output latch is disabled, the pio can be read as digital input (sense).
 * This is the power on-default if a reset signal is pulled low. Without reset, the state is random.
 */
class DS2408 : public OneWireDevice {

private:
    // cache of all of the DS2408 status registers
    struct {                      // bass address 0x0088 on chip
        uint8_t pio;              // 0 = pio logic state
        uint8_t latch;            // 1 = output latch state
        uint8_t activity;         // 2 = activity latch state
        uint8_t cond_search_mask; // 3 = conditional search channel selection mask
        uint8_t cond_search_pol;  // 4 = conditional search channel polarity selection
        uint8_t status;           // 5 = control/status register
    } m_regCache;
    bool m_connected = false;
    uint8_t m_claimed = 0x00;

    static const uint8_t READ_PIO_REG = 0xF0;
    static const uint8_t ACCESS_READ = 0xF5;
    static const uint8_t ACCESS_WRITE = 0x5A;
    static const uint8_t ACK_SUCCESS = 0xAA;
    static const uint8_t ACK_ERROR = 0xFF;

    // all addresses have upper bits 0x00
    static const uint8_t ADDRESS_UPPER = 0x00;
    static const uint8_t ADDRESS_PIO_STATE_LOWER = 0x88;
    static const uint8_t ADDRESS_LATCH_STATE_LOWER = 0x89;

public:
    /**
     * Constructor initializes both caches to 0xFF.
     * This means the output latches are disabled and all pins are sensed high
     */
    DS2408(OneWire& oneWire, const OneWireAddress& address)
        : OneWireDevice(oneWire, address)
    {
        m_regCache.pio = 0xFF;
        m_regCache.latch = 0xFF;
    }

    /**
     * Destructor is default.
     */
    ~DS2408() = default;

    /**
     * extracts a single bit from a byte
     *
     * @param target input byte to extract a bit from
     * @param pos position of the byte with the rightmost bit being A.
     * @returns extracted bit as bool
     */
    inline bool getBit(const uint8_t& target, const uint8_t& pos) const
    {
        return ((0b1 << pos) & target) != 0x0;
    }

    /**
     * sets a single bit in a byte
     * @param input byte to change a bit in
     * @param pos position of bit in the byte
     * @param state new state for the bit, 1 or 0
     * @returns new byte with the bit at position pos changed to state
     */
    inline uint8_t setBit(const uint8_t& input, const uint8_t& pos, bool state) const
    {
        uint8_t mask = (0b1 << pos);
        if (state) {
            return input | mask;
        } else {
            return input & ~mask;
        }
    }

    /**
     * Reads the pin state of a given channel.
     * Note that for a read to make sense the latch must be off (value written is 1).
     * @param pio channel number 0-7
     * @param fromCache when true, the bit is returned from cache, without communicating with the device
     * @returns state of the output pin for the given channel
     */
    bool readPioBit(uint8_t pos, bool fromCache = false)
    {
        return getBit(readPios(fromCache), pos);
    }

    /**
     * Performs a simultaneous read of all I/O pins.
     * @param fromCache when true, the bit is returned from cache, without communicating with the device
     * @returns bit field with all pio states
     */
    uint8_t readPios(bool fromCache = false)
    {
        if (!fromCache) {
            update(); // use update instead of accessRead(), because it has error checking
        }
        return m_regCache.pio;
    }

    /**
     * Reads the latch state of a given channel.
     * @param channel number 0-7
     * @param fromCache when true, the bit is returned from cache, without communicating with the device
     * @returns state of the latch for the given channel
     */
    bool readLatchBit(uint8_t pos, bool fromCache = false)
    {
        return getBit(readLatches(fromCache), pos);
    }

    /**
     * Performs a simultaneous read of all latches.
     * @param fromCache when true, the bit is returned from cache, without communicating with the device
     * @returns bit field with all pio states
     */
    uint8_t readLatches(bool fromCache = false)
    {
        if (!fromCache) {
            update(); // use update instead of accessRead(), because it has error checking
        }
        return m_regCache.latch;
    }

    /**
     * Change a single bit in the output latch register. Uses the cached value for the other bits
     * @param pio The bit position to write
     * @param set The value of the bit (1 or 0)
     * @returns true on success
     */
    bool writeLatchBit(uint8_t pos, bool set)
    {
        uint8_t newValues = setBit(m_regCache.latch, pos, set);
        bool success = writeLatches(newValues);
        return success;
    }

    /**
     * Write a new state to all output latches.
     * @param values new state for all latches as a bitfield. 0 = pull down latch enabled.
     * @return true on success
     */
    bool writeLatches(uint8_t values)
    {
        bool success = accessWrite(values);
        if (success) {
            m_regCache.latch = values;
        }
        return success;
    }

    /**
     * Reads the pio state of all pins and returns them as a single byte.
     * Note that the state that is returned is the actual pin state, not the state of the latch register.
     * If the pull down latch is disabled (written as 1), this can be used to read an input switch
     * @return bit field with all 8 pio pin states
     */
    uint8_t accessRead();

    /**
     * Writes the state of all PIOs in one operation.
     * @param latches pio data - a bit field with new values for the output latch
     * @param maxTries the maximum number of attempts before giving up.
     * @return true on success
     */
    bool accessWrite(uint8_t latches, uint8_t maxTries = 3);

    /**
     * Updates all cache registers by reading them from the device.
     * Performs CRC checking on communication and sets the connect state to false on CRC error or to true on success.
     */
    void update();

    /**
     * @return cached state of all I/O pins
     */
    uint8_t getPioCache()
    {
        return m_regCache.pio;
    }

    /**
     * @return cached state of all latches
     */
    uint8_t getLatchCache()
    {
        return m_regCache.latch;
    }

    /**
     * @return true if connected (hardware DS2408 is found)
     */
    bool connected()
    {
        return m_connected;
    }

    uint8_t claimed()
    {
        return m_claimed;
    }

    bool claim(const uint8_t& v)
    {
        if ((m_claimed & v) == 0) {
            m_claimed |= v;
            return true;
        }
        return false;
    }

    void unclaim(const uint8_t& v)
    {
        m_claimed &= ~v;
    }
};
