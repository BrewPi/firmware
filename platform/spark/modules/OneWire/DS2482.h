/*
  DS2482 library for Arduino
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

#define DS2482_CONFIG_APU (1<<0)
#define DS2482_CONFIG_PPM (1<<1)
#define DS2482_CONFIG_SPU (1<<2)
#define DS2484_CONFIG_WS  (1<<3)

#define DS2482_STATUS_BUSY 	(1<<0)
#define DS2482_STATUS_PPD 	(1<<1)
#define DS2482_STATUS_SD	(1<<2)
#define DS2482_STATUS_LL	(1<<3)
#define DS2482_STATUS_RST	(1<<4)
#define DS2482_STATUS_SBR	(1<<5)
#define DS2482_STATUS_TSB	(1<<6)
#define DS2482_STATUS_DIR	(1<<7)

class DS2482 /*: public OneWireLowLevelInterface */ {
public:
    //Address is 0-3

    DS2482(uint8_t address) : mAddress(address) {
        mAddress = 0x18 | mAddress;
        init();
    }
  
    virtual ~DS2482(){};

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

    // DS2482 specific functions below

    void resetMaster();

    //DS2482-800 only
    bool selectChannel(uint8_t channel);


    uint8_t wireReadStatus(bool setPtr = false);

    uint8_t hasTimeout() {
        return mTimeout;
    }

private:

    uint8_t mAddress;
    uint8_t mTimeout;
    uint8_t readByte();
    void setReadPtr(uint8_t readPtr);

    uint8_t busyWait(bool setReadPtr = false); //blocks until
};