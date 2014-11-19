/* 
 * File:   BrewPiTouch.h
 * Author: Elco
 *
 * Created on 18 november 2014, 9:32
 */

#ifndef BREWPITOUCH_H
#define	BREWPITOUCH_H

#include <inttypes.h>
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"

class BrewPiTouch {
public:
    BrewPiTouch(uint8_t cs, uint8_t irq);
    virtual ~BrewPiTouch();
    void init(uint8_t configuration = BrewPiTouch::START);
    void update(uint16_t numSamples = 32);
    int16_t getXRaw();
    int16_t getYRaw();
    int16_t getX();
    int16_t getY();
    void set8bit();
    void set12bit();
    bool is8bit();
    bool is12bit();
    void calibrate(Adafruit_ILI9341 * tft);
    bool isTouched();
       
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
    
private:
    int16_t width; // can be negative when display is flipped
    int16_t height; // can be negative when display is flipped
    int16_t tftWidth; 
    int16_t tftHeight;    
    int16_t xOffset;
    int16_t yOffset;
    uint16_t xRaw;
    uint16_t yRaw;
    uint8_t pinCS;
    uint8_t pinIRQ;
    uint8_t config;
    
    void spiWrite(uint8_t c);
    uint8_t spiRead(void);
    uint16_t readChannel();
};

#endif	/* BREWPITOUCH_H */

