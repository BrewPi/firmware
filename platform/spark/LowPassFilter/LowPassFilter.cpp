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

#include "LowPassFilter.h"
#include <stdlib.h>
#include <limits.h>

LowPassFilter::LowPassFilter() {
    setCoefficients(SETTLING_TIME_50_SAMPLES); // default to 50 samples settling time
}

LowPassFilter::~LowPassFilter() {

}

void LowPassFilter::setCoefficients(uint8_t aValue, uint8_t bValue) {
    a = aValue;
    b = bValue;
}

void LowPassFilter::setCoefficients(uint16_t ab) {
    a = (ab & 0xFF00) >> 8;
    b = ab & 0x00FF;
}

int16_t LowPassFilter::add(int16_t val) {
    int32_t returnVal = addDoublePrecision(((int32_t) val) << 16);
    return returnVal >> 16;
}

int32_t LowPassFilter::addDoublePrecision(int32_t val) {
    xv[2] = xv[1];
    xv[1] = xv[0];
    xv[0] = val;

    yv[2] = yv[1];
    yv[1] = yv[0];

    /* Implementation that prevents overflow as much as possible by order of operations: */
    yv[0] = ((yv[1] - yv[2]) + yv[1]) // expected value + 1*
            - (yv[1] >> b) + (yv[2] >> b) + // expected value +0*
            + (xv[0] >> a) + (xv[1]>>(a - 1)) + (xv[2] >> a) // expected value +(1>>(a-2))
            - (yv[2]>>(a - 2)); // expected value -(1>>(a-2))

    return yv[0];
}

int16_t LowPassFilter::readOutput(void) {
    return yv[0] >> 16; // return 16 most significant bits of most recent output
}

int16_t LowPassFilter::readInput(void) {
    return xv[0] >> 16; // return 16 most significant bits of most recent input
}

int32_t LowPassFilter::readOutputDoublePrecision(void) {
    return yv[0];
}

int32_t LowPassFilter::readPrevOutputDoublePrecision(void) {
    return yv[1];
}

void LowPassFilter::init(int16_t val) {
    xv[0] = val;
    xv[0] = xv[0] << 16; // 16 extra bits are used in the filter for the fraction part

    xv[1] = xv[0];
    xv[2] = xv[0];

    yv[0] = xv[0];
    yv[1] = xv[0];
    yv[2] = xv[0];
}
