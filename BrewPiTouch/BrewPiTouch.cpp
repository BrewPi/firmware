/* 
 * File:   BrewPiTouch.cpp
 * Author: Elco
 * 
 * Created on 18 november 2014, 9:32
 */

#include "BrewPiTouch.h"
#include "application.h"
#include <limits.h>
// includes for getting a median:
#undef min
#undef max
#undef swap
#include <vector>
#include <algorithm>

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
    width = 1600;
    height = 1600;
    tftWidth = 320;
    tftHeight = 240;
    xOffset = 0;
    yOffset = 0;
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

bool BrewPiTouch::isTouched() {
    return (digitalRead(pinIRQ) == HIGH ? 0 : 1);
}

int16_t BrewPiTouch::getXRaw() {
    return xRaw;
}

int16_t BrewPiTouch::getYRaw() {
    return yRaw;
}

int16_t BrewPiTouch::getX() {
    int32_t val = xRaw; // create room for multiplication
    val -= xOffset; // remove offset
    val = val * tftWidth / width; //scale
    return val;
}

int16_t BrewPiTouch::getY() {
    int32_t val = yRaw; // create room for multiplication
    val -= yOffset; // remove offset
    val = val * tftHeight / height; //scale
    return val;
}

void BrewPiTouch::update(uint16_t numSamples) {
    std::vector<int16_t> samplesX;
    std::vector<int16_t> samplesY;

    pinMode(pinIRQ, OUTPUT); // reverse bias diode during conversion
    digitalWrite(pinIRQ, LOW); // as recommended in SBAA028
    digitalWrite(pinCS, LOW);

    for (uint16_t i = 0; i < numSamples; i++) {
        spiWrite((config & CHMASK) | CHX); // select channel x
        samplesX.push_back(readChannel());

        spiWrite((config & CHMASK) | CHY); // select channel y
        samplesY.push_back(readChannel());

    }
    // get median
    size_t middle = samplesX.size() / 2;
    std::nth_element(samplesX.begin(), samplesX.begin() + middle, samplesX.end());
    std::nth_element(samplesY.begin(), samplesY.begin() + middle, samplesY.end());
    xRaw = samplesX[middle];
    yRaw = samplesY[middle];

    pinMode(pinIRQ, INPUT);
    digitalWrite(pinCS, HIGH);
}

void BrewPiTouch::calibrate(Adafruit_ILI9341 * tft) {
    const int32_t xDisplay[3] = {40, 40, 280};
    const int32_t yDisplay[3] = {40, 200, 200};

    volatile int32_t xTouch[3];
    volatile int32_t yTouch[3];

    tftWidth = tft->width();
    tftHeight = tft->height();

    volatile int16_t samples;

    const int16_t requiredSamples = 256;
    tft->fillScreen(ILI9341_BLACK);
    for (uint8_t i = 0; i < 3; i++) {
        tft->drawCrossHair(xDisplay[i], yDisplay[i], 10, ILI9341_GREEN);
        while (!isTouched()); // wait for touch
        while (samples < requiredSamples || isTouched()) {
            samples = 0;
            xTouch[i] = 0;
            yTouch[i] = 0;
            tft->drawFastHLine(0, 0, tftWidth, ILI9341_RED);
            while (isTouched()) {
                update();
                int32_t xSample = getXRaw();
                int32_t ySample = getYRaw();

                xTouch[i] += xSample;
                yTouch[i] += ySample;
                samples++;

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
                Serial.print(samples);

                int16_t xCalibrated = getX();
                int16_t yCalibrated = getY();
                Serial.print("\t\t");
                Serial.print(xCalibrated);
                Serial.print("\t");
                Serial.println(yCalibrated);
                tft->fillCircle(xCalibrated, yCalibrated, 3, ILI9341_WHITE);

                // print progress line
                uint16_t progress = samples * tftWidth / requiredSamples;
                tft->drawFastHLine(0, 0, progress, ILI9341_BLUE);

                // stop when required number of samples is reached
                if (samples >= requiredSamples) {
                    tft->drawFastHLine(0, 0, tftWidth, ILI9341_GREEN);
                    tft->fillCircle(xDisplay[i], yDisplay[i], 8, ILI9341_BLUE);
                    while (isTouched()); // wait until released
                    break;
                }

                if (abs(xSample - xAverage) > 50 || abs(ySample - yAverage) > 50) {
                    // if new sample deviates too much from average, reset
                    Serial.println("reset");
                    break;
                }
            }
        }
        xTouch[i] = xTouch[i] / samples;
        yTouch[i] = yTouch[i] / samples;

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
    }

    width = tftWidth * (xTouch[2] - xTouch[0]) / (xDisplay[2] - xDisplay[0]);
    height = tftHeight * (yTouch[1] - yTouch[0]) / (yDisplay[1] - yDisplay[0]);
    xOffset = xTouch[0] - xDisplay[0] * width / tftWidth;
    yOffset = yTouch[0] - yDisplay[0] * height / tftHeight;
    Serial.println("Calibration finished.");
    Serial.print("width: ");
    Serial.println(width);
    Serial.print("yScale: ");
    Serial.println(width);
    Serial.print("height: ");
    Serial.println(xOffset);
    Serial.print("yOffset: ");
    Serial.println(yOffset);
}