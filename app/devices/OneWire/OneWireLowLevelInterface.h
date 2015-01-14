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
#include <stdint.h>

class OneWireLowLevelInterface {
public:
    // Configure the pin or chip
    virtual bool init() = 0;
    virtual uint8_t pinNr(void) = 0;

    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    virtual bool reset(void) = 0;

    // Issue a 1-Wire rom select command, you do the reset first.
    //    virtual void select(const uint8_t rom[8]) = 0;

    // Issue a 1-Wire rom skip command, to address all on bus.
    //    virtual void skip(void) = 0;

    // Write a byte. If 'power' is one then the wire is held high at
    // the end for parasitically powered devices. You are responsible
    // for eventually depowering it by calling depower() or doing
    // another read or write.
    virtual void write(uint8_t v, uint8_t power = 0) = 0;

    // virtual void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0) = 0;

    // Read a byte.
    virtual uint8_t read(void) = 0;

    // virtual void read_bytes(uint8_t *buf, uint16_t count) = 0;

    // Write a bit. The bus is always left powered at the end, see
    // note in write() about that.
    virtual void write_bit(uint8_t v) = 0;

    // Read a bit.
    virtual uint8_t read_bit(void) = 0;
};

