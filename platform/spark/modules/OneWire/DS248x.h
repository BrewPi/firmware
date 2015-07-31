/*
  DS2482/DS2484 library for Arduino
  Copyright (C) 2009 Paeae Technologies

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <inttypes.h>
#include "application.h"
#include "OneWireLowLevelInterface.h"

#define DS248X_CONFIG_APU (0x1<<0)
#define DS248X_CONFIG_PPM (0x1<<1)
#define DS248X_CONFIG_SPU (0x1<<2)
#define DS2484_CONFIG_WS  (0x1<<3)

#define DS248X_STATUS_BUSY 	(0x1<<0)
#define DS248X_STATUS_PPD 	(0x1<<1)
#define DS248X_STATUS_SD	(0x1<<2)
#define DS248X_STATUS_LL	(0x1<<3)
#define DS248X_STATUS_RST	(0x1<<4)
#define DS248X_STATUS_SBR	(0x1<<5)
#define DS248X_STATUS_TSB	(0x1<<6)
#define DS248X_STATUS_DIR	(0x1<<7)

// I2C commands
#define DS248X_DRST	0xf0 // Device Reset
#define DS248X_WCFG	0xd2 // Write Configuration
#define DS248X_CHSL	0xc3 // Channel Select (DS248X-800 only)
#define DS248X_SRP	0xe1 // Set Read Pointer
#define DS248X_1WRS	0xb4 // 1-Wire Reset
#define DS248X_1WWB	0xa5 // 1-Wire Write Byte
#define DS248X_1WRB	0x96 // 1-Wire Read Byte
#define DS248X_1WSB	0x87 // 1-Wire Single Bit
#define DS248X_1WT	0x78 // 1-Wire Triplet
#define DS248X_ADJP	0xc3 // Adjust OneWire port config (DS2484 only))


class DS248x /*: public OneWireLowLevelInterface */ {
public:
    //Address is 0-3

    DS248x(uint8_t address) : mAddress(address) {
        mAddress = 0x18 | mAddress;
        init();
    }
  
    virtual ~DS248x(){};

    bool init();
    
    bool configure(uint8_t config);
    
    uint8_t pinNr(){
        return mAddress & 0b11; // return lower bits of I2C address instead of pin
    }

    // Perform the onewire reset function.  We will wait up to 250uS for
    // the bus to come high, if it doesn't then it is broken or shorted
    // and we return a 0;
    //
    // Returns 1 if a device asserted a presence pulse, 0 otherwise.   
    bool reset();

    void write(uint8_t b, uint8_t power = 0);
    uint8_t read();

    void write_bit(uint8_t bit);
    uint8_t read_bit();

    // DS248X specific functions below

    void resetMaster();

    //DS2482-800 only
    bool selectChannel(uint8_t channel);


    uint8_t wireReadStatus(bool setPtr = false);

    uint8_t hasTimeout() {
        return mTimeout;
    }
    
    //--------------------------------------------------------------------------
    // Use the DS248X help command '1-Wire triplet' to perform one bit of a
    // 1-Wire search.
    // This command does two read bits and one write bit. The write bit
    // is either the default direction (all device have same bit) or in case of
    // a discrepancy, the 'search_direction' parameter is used.
    //
    // Returns – The DS248X status byte result from the triplet command
    // Updates search direction, id_bit and cmp_id_bit
    uint8_t search_triplet(uint8_t * search_direction, uint8_t * id_bit, uint8_t * cmp_id_bit);

private:

    uint8_t mAddress;
    uint8_t mTimeout;
    uint8_t readByte();
    void setReadPtr(uint8_t readPtr);

    uint8_t busyWait(bool setReadPtr = false); //blocks until
};
