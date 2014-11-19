/* 
 * File:   BrewPiTouch.cpp
 * Author: Elco
 * 
 * Created on 18 november 2014, 9:32
 */

#include "BrewPiTouch.h"
#include "application.h"
#include <limits.h>

BrewPiTouch::BrewPiTouch(uint8_t cs, uint8_t irq) : pinCS(cs), pinIRQ(irq) {
}

BrewPiTouch::~BrewPiTouch() {
}

void BrewPiTouch::init(uint8_t configuration) {
    // default is:
    // 12bit (mode=0)
    // power down between conversions, penIRQ enabled (PD1,PD0 = 00)
    // Differential reference (SER/DFR = 0)    
    config = configuration;
    pinMode(pinCS, OUTPUT);
    pinMode(pinIRQ, INPUT);
    // SPI is initialized externally

    digitalWrite(pinCS, LOW);
    spiWrite(config);
    digitalWrite(pinCS, HIGH);
    update();
}

void BrewPiTouch::set8bit() {
    config = config | MODE;
}

void BrewPiTouch::set12bit() {
    config = config & ~MODE;
}

bool BrewPiTouch::is8bit() {
    return (config & MODE) ? 1 : 0;
}

bool BrewPiTouch::is12bit() {
    return (config & MODE) ? 0 : 1;
}

void BrewPiTouch::spiWrite(uint8_t c) {
    SPI.transfer(c);
}

uint8_t BrewPiTouch::spiRead() {
    uint8_t r = 0;
    r = SPI.transfer(0x00);
    return r;
}

uint16_t BrewPiTouch::readChannel() {
    uint16_t data;
    data = SPI.transfer(0x00);
    if (is12bit()) {
        data = data << 8;
        data += SPI.transfer(0x00);
        data = data >> 4;
    }
    return data;
}

uint8_t BrewPiTouch::isTouched() {
    return (digitalRead(pinIRQ) == HIGH ? 0 : 1);
}

uint16_t BrewPiTouch::getX() {
    return xRaw;
}

uint16_t BrewPiTouch::getY() {
    return yRaw;
}

void BrewPiTouch::update(uint16_t numSamples) {
    uint32_t sumX=0;
    uint32_t sumY=0;
    
    pinMode(pinIRQ, OUTPUT); // reverse bias diode during conversion
    digitalWrite(pinIRQ, LOW); // as recommended in SBAA028
    digitalWrite(pinCS, LOW);
    
    for (uint16_t i = 0; i < numSamples; i++) {
        spiWrite((config & CHMASK) | CHX); // select channel x
        sumX += readChannel();
        
        spiWrite((config & CHMASK) | CHY); // select channel y
        sumY += readChannel();
        
    }
    xRaw = sumX / numSamples;
    yRaw = sumY / numSamples;
    
    
    pinMode(pinIRQ, INPUT);
    digitalWrite(pinCS, HIGH);
}

void BrewPiTouch::calibrate(Adafruit_ILI9341 * tft) {
    volatile int32_t xDisplay[3] = {40, 40, 280};
    volatile int32_t yDisplay[3] = {40, 200, 200};

    volatile int32_t xTouch[3] = {0, 0, 0};
    volatile int32_t yTouch[3] = {0, 0, 0};

    volatile int16_t samples;

    const int16_t minSamples = 1024;
    const int16_t maxSamples = INT16_MAX; // more samples will overflow

    tft->fillScreen(ILI9341_BLACK);
    for (uint8_t i = 0; i < 3; i++) {
        uint16_t width = tft->width();
        tft->drawCrossHair(xDisplay[i], yDisplay[i], 10, ILI9341_GREEN);
        while (!isTouched()) { // wait for touch
            delay(10);
        }
        while (samples < minSamples || isTouched()) {
            samples = 0;
            tft->drawFastHLine(0, 0, width, ILI9341_RED);
            while (isTouched()) {
                update();
                int32_t xSample = getX();
                int32_t ySample = getY();
                if (samples < maxSamples) {
                    xTouch[i] += xSample;
                    yTouch[i] += ySample;
                    samples++;
                }
                int32_t xAverage = xTouch[i] / samples;
                int32_t yAverage = yTouch[i] / samples;

                Serial.print(xSample);
                Serial.print("\t");
                Serial.print(xAverage);
                Serial.print("\t\t");

                Serial.print(ySample);
                Serial.print("\t");
                Serial.print(yAverage);
                Serial.print("\t\t");
                Serial.println(samples);

                uint16_t progress = samples * width / minSamples;
                tft->drawFastHLine(0, 0, progress, ILI9341_GREEN);

                if (abs(xSample - xAverage) > 100 || abs(ySample - yAverage) > 100) {
                    // if new sample deviates too much from average, reset
                    xTouch[i] = 0;
                    yTouch[i] = 0;
                    samples = 0;
                    Serial.println("reset");
                    break;
                }
            }
        }
        // touch signal is 12 bits, so we have 3 bits extra room for scale value
        // Divide by number of (samples/2^3)
        xTouch[i] = xTouch[i] / (samples / 8);
        yTouch[i] = yTouch[i] / (samples / 8);

        Serial.print("Display coordinates:");
        Serial.print(xDisplay[i]);
        Serial.print("\t");
        Serial.println(yDisplay[i]);
        Serial.print("Touch coordinates:");
        Serial.print(xTouch[i]);
        Serial.print("\t");
        Serial.println(yTouch[i]);
        Serial.print("Number of samples:");
        Serial.print(samples);
        Serial.print("\t");
        Serial.println(yTouch[i]);
        tft->drawCircle(xDisplay[i], yDisplay[i], 3, ILI9341_RED);
    }

    xScale = (xTouch[2] - xTouch[0]) / (xDisplay[2] - xDisplay[0]);
    yScale = (yTouch[1] - yTouch[0]) / (yDisplay[1] - yDisplay[0]);
    Serial.println("Calibration finished.");
    Serial.print("xScale: ");
    Serial.println(xScale);
    Serial.print("yScale: ");
    Serial.println(yScale);
}