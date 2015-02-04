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

        crc code is from OneWire library


Updates:
        dec 5th, 2009: included a search function fix by Don T
        see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1242137666
 */

#include "DS2482.h"
#include "application.h"


#define PTR_STATUS 0xf0
#define PTR_READ 0xe1
#define PTR_CONFIG 0xc3

//-------helpers

void DS2482::setReadPtr(uint8_t readPtr) {
    Wire.beginTransmission(mAddress);
    Wire.write(0xe1);
    Wire.write(readPtr);
    Wire.endTransmission();
}

uint8_t DS2482::readByte() {
    Wire.requestFrom(mAddress, (uint8_t) 1);
    return Wire.read();
}

uint8_t DS2482::wireReadStatus(bool setPtr) {
    if (setPtr)
        setReadPtr(PTR_STATUS);

    return readByte();
}

uint8_t DS2482::busyWait(bool setReadPtr) {
    uint8_t status;
    int loopCount = 1000;
    while ((status = wireReadStatus(setReadPtr)) & DS2482_STATUS_BUSY) {
        if (--loopCount <= 0) {
            mTimeout = 1;
            break;
        }
        delayMicroseconds(20);
    }
    return status;
}

//----------interface

bool DS2482::init(){
    Wire.begin();
    resetMaster();
    return configure(DS2482_CONFIG_APU);
}

void DS2482::resetMaster() {
    mTimeout = 0;
    Wire.beginTransmission(mAddress);
    Wire.write(0xf0);
    Wire.endTransmission();
}

bool DS2482::configure(uint8_t config) {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(0xd2);
    Wire.write(config | (~config) << 4);
    Wire.endTransmission();

    return readByte() == config;
}

bool DS2482::selectChannel(uint8_t channel) {
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
    Wire.write(0xc3);
    Wire.write(ch);
    Wire.endTransmission();
    busyWait();

    uint8_t check = readByte();

    return check == ch_read;
}

bool DS2482::reset() {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(0xb4);
    Wire.endTransmission();

    uint8_t status = busyWait();

    return status & DS2482_STATUS_PPD ? true : false;
}

void DS2482::write(uint8_t b, uint8_t power) {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(0xa5);
    Wire.write(b);
    Wire.endTransmission();
}

uint8_t DS2482::read() {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(0x96);
    Wire.endTransmission();
    busyWait();
    setReadPtr(PTR_READ);
    return readByte();
}

void DS2482::write_bit(uint8_t bit) {
    busyWait(true);
    Wire.beginTransmission(mAddress);
    Wire.write(0x87);
    Wire.write(bit ? 0x80 : 0);
    Wire.endTransmission();
}

uint8_t DS2482::read_bit() {
    write_bit(1);
    uint8_t status = busyWait(true);
    return status & DS2482_STATUS_SBR ? 1 : 0;
}