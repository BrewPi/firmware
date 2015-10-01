/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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

// #include "Brewpi.h"

#include "temperatureFormats.h"
#include "FilterFixed.h"

// Use 3 filter sections. This gives excellent filtering, without adding too much delay.
// For 3 sections the stop band attenuation is 3x the single section attenuation in dB.
// The delay is longer too.

/*
 *  See MATLAB script in FixedFilter.h for analysis
 *       a=4,    b=0,    delay time = 9
 *       a=6,    b=1,    delay time = 20
 *       a=8,    b=2,    delay time = 43
 *       a=10,   b=3,    delay time = 88
 *       a=12,   b=4,    delay time = 179
 *       a=14,   b=5,    delay time = 360
 *       a=16,   b=6,    delay time = 723
 */
#define NUM_SECTIONS 3
class FilterCascaded
{
    public:

        // CascadedFilter implements a filter that consists of multiple second order secions.
        FixedFilter sections[NUM_SECTIONS];

    public:
        FilterCascaded();

        ~FilterCascaded(){}

        void init(temp_precise_t val = temp_precise_t(0.0));

        void setFiltering(uint8_t bValue);

        uint8_t getFiltering();

        void setFilteringForDelay(uint16_t maxDelay);

        uint16_t getDelay();

        temp_precise_t add(const temp_precise_t & val);    // adds a value and returns the most recent filter output

        temp_t add(const temp_t & val);                    // adds a value and returns the most recent filter output as temp

        temp_precise_t readInput(void);                  // returns the most recent filter input

        temp_precise_t readOutput(void);

        temp_precise_t readPrevOutput(void);

        bool detectPosPeak(temp_precise_t * peak);

        bool detectNegPeak(temp_precise_t * peak);

        bool isRising();

        bool isFalling();

        const uint16_t delayTimes[7] = { 9, 20, 43, 88, 179, 360, 723 };
};
