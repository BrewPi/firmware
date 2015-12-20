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

        crc code is from OneWire library


Updates:
        dec 5th, 2009: included a search function fix by Don T
        see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1242137666
 */

#include "DS248x.h"
#include "application.h"


#define PTR_STATUS 0xf0
#define PTR_READ 0xe1
#define PTR_CONFIG 0xc3
#define PTR_PORTCONFIG 0xb4 //DS2484 only

//-------helpers

void DS248x::setReadPtr(uint8_t readPtr) {
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_SRP);
    Wire.write(readPtr);
    Wire.endTransmission();
}

uint8_t DS248x::readByte() {
    Wire.requestFrom(mAddress, (uint8_t) 1);
    return Wire.read();
}

uint8_t DS248x::wireReadStatus(bool setPtr) {
    if (setPtr)
        setReadPtr(PTR_STATUS);

    return readByte();
}

uint8_t DS248x::busyWait(bool setReadPtr) {
    uint8_t status;
    int loopCount = 1000;
    while ((status = wireReadStatus(setReadPtr)) & DS248X_STATUS_BUSY) {
        if (--loopCount <= 0) {
            mTimeout = 1;
            break;
        }
        delayMicroseconds(20);
    }
    return status;
}

//----------interface

bool DS248x::init() {
    Wire.begin();
    resetMaster();
    return configure(DS248X_CONFIG_APU);
}

void DS248x::resetMaster() {
    mTimeout = 0;
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_DRST);
    Wire.endTransmission();
}

bool DS248x::configure(uint8_t config) {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_WCFG);
    Wire.write(config | (~config) << 4);
    Wire.endTransmission();

    return readByte() == config;
}

bool DS248x::selectChannel(uint8_t channel) {
    uint8_t ch, ch_read;

    switch (channel) {
        case 0:
        default:
            ch = 0xf0;
            ch_read = 0xb8;
            break;
        case 1:
            ch = 0xe1;
            ch_read = 0xb1;
            break;
        case 2:
            ch = 0xd2;
            ch_read = 0xaa;
            break;
        case 3:
            ch = 0xc3;
            ch_read = 0xa3;
            break;
        case 4:
            ch = 0xb4;
            ch_read = 0x9c;
            break;
        case 5:
            ch = 0xa5;
            ch_read = 0x95;
            break;
        case 6:
            ch = 0x96;
            ch_read = 0x8e;
            break;
        case 7:
            ch = 0x87;
            ch_read = 0x87;
            break;
    };

    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_CHSL);
    Wire.write(ch);
    Wire.endTransmission();
    busyWait();

    uint8_t check = readByte();

    return check == ch_read;
}

bool DS248x::reset() {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_1WRS);
    Wire.endTransmission();

    uint8_t status = busyWait();

    return status & DS248X_STATUS_PPD ? true : false;
}

void DS248x::write(uint8_t b, uint8_t power) {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_1WWB);
    Wire.write(b);
    Wire.endTransmission();
}

uint8_t DS248x::read() {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_1WRB);
    Wire.endTransmission();
    busyWait();
    setReadPtr(PTR_READ);
    return readByte();
}

void DS248x::write_bit(uint8_t bit) {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_1WSB);
    Wire.write(bit ? 0x80 : 0);
    Wire.endTransmission();
}

uint8_t DS248x::read_bit() {
    write_bit(1);
    uint8_t status = busyWait(true);
    return status & DS248X_STATUS_SBR ? 1 : 0;
}

uint8_t DS248x::search_triplet(uint8_t * search_direction, uint8_t * id_bit, uint8_t * cmp_id_bit) {
    // 1-Wire Triplet (Case B)
    //   S AD,0 [A] 1WT [A] SS [A] Sr AD,1 [A] [Status] A [Status] A\ P
    //                                         \--------/
    //                           Repeat until 1WB bit has changed to 0
    //  [] indicates from slave
    //  SS indicates byte containing search direction bit value in msbit
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(DS248X_1WT);
    Wire.write(*search_direction ? 0x80 : 0x00);
    Wire.endTransmission();

    uint8_t status = busyWait();

    // check bit results in status byte
    *id_bit = ((status & DS248X_STATUS_SBR) == DS248X_STATUS_SBR);
    *cmp_id_bit = ((status & DS248X_STATUS_TSB) == DS248X_STATUS_TSB);
    *search_direction =
            ((status & DS248X_STATUS_DIR) == DS248X_STATUS_DIR) ? (byte) 1 : (byte) 0;

    return status;
}