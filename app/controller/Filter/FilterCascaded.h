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

#include "Brewpi.h"
#include "newTemperatureFormats.h"
#include "FilterFixed.h"

// Use 3 filter sections. This gives excellent filtering, without adding too much delay.
// For 3 sections the stop band attenuation is 3x the single section attenuation in dB.
// The delay is also tripled.
#define NUM_SECTIONS 3
class FilterCascaded
{
    public:

        // CascadedFilter implements a filter that consists of multiple second order secions.
        FixedFilter sections[NUM_SECTIONS];

    public:
        FilterCascaded();

        ~FilterCascaded()
        {
        }

        void init(temp val);

        void setCoefficients(uint8_t bValue);

        temp_precise add(temp_precise val);    // adds a value and returns the most recent filter output

        temp add(temp val);                    // adds a value and returns the most recent filter output as temp

        temp_precise addDoublePrecision(temp_precise val);

        temp_precise readInput(void);          // returns the most recent filter input

        temp_precise readOutput(void);

        temp_precise readOutputPrecise(void);

        temp_precise readPrevOutputPrecise(void);

        temp_precise readOldestOutputPrecise(void);

        temp_precise detectPosPeak(void);

        temp_precise detectNegPeak(void);
};
