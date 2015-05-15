/*
 * Copyright 2014 BrewPi/Elco Jacobs.
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
#include "../LowPassFilter/LowPassFilter.h"

class BrewPiTouch {
public:
    BrewPiTouch(uint8_t cs, uint8_t irq);
    virtual ~BrewPiTouch();
    void init(uint8_t configuration = BrewPiTouch::START);
    bool update(uint16_t numSamples = 8);
    int16_t getXRaw();
    int16_t getYRaw();
    int16_t getX();
    int16_t getY();
    void set8bit();
    void set12bit();
    bool is8bit();
    bool is12bit();
    // void calibrate(Adafruit_ILI9341 * tft);
    bool isTouched();
    bool isStable();
    void setStabilityThreshold(int16_t treshold = 40);
       
    enum controlBits {
        START = 0x80,
        AN2    = 0x40,
        AN1    = 0x20,
        AN0    = 0x10,
        MODE = 0x08,
        SER = 0x04,
        PD1   = 0x02,
        PD0   = 0x01,
        CHX =  0x10,
        CHY = 0x50,
        CHMASK = 0x8F // AND with CHMASK to set A2, A1 A0 to 0
    };
    const int16_t CALIBRATE_FROM_EDGE = 40;
    
private:
    int16_t width; // can be negative when display is flipped
    int16_t height; // can be negative when display is flipped
    int16_t tftWidth; 
    int16_t tftHeight;    
    int16_t xOffset;
    int16_t yOffset;
    uint8_t pinCS;
    uint8_t pinIRQ;
    uint8_t config;
    int16_t stabilityThreshold;
    LowPassFilter filterX;
    LowPassFilter filterY;
    
    void spiWrite(uint8_t c);
    uint8_t spiRead(void);
    uint16_t readChannel();
};
