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



#include "Brewpi.h"
#include "newTemperatureFormats.h"
#include "FilterFixed.h"
#include "FilterCascaded.h"
#include <stdlib.h>

FilterCascaded::FilterCascaded()
{
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        sections[i].setCoefficients(2);    // default to a b value of 2
    }
    init();
}

void FilterCascaded::setCoefficients(uint8_t bValue)
{
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        sections[i].setCoefficients(bValue);
    }
}

temp FilterCascaded::add(temp & val)
{
    temp_precise valPrecise= toPrecise(val);

    valPrecise = add(valPrecise);

    // return output, converted back to normal precision
    return toTemp(valPrecise);
}

temp_precise FilterCascaded::add(temp_precise & val)
{
    temp_precise temporary = val;

    // temporary is input for next section, which is the output of the previous section
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        temporary = sections[i].add(temporary);
    }

    return temporary;
}

temp_precise FilterCascaded::readInput(void)
{
    return sections[0].readInput();    // return input of first section
}

temp_precise FilterCascaded::readOutput(void)
{
    return sections[NUM_SECTIONS - 1].readOutput();    // return output of last section
}

bool FilterCascaded::detectPosPeak(temp_precise * peak)
{
    return sections[NUM_SECTIONS - 1].detectPosPeak(peak);    // detect peaks in last section
}

bool FilterCascaded::detectNegPeak(temp_precise * peak)
{
    return sections[NUM_SECTIONS - 1].detectNegPeak(peak);    // detect peaks in last section
}

temp_precise FilterCascaded::readPrevOutput(void)
{
    return sections[NUM_SECTIONS - 1].readPrevOutput();    // return previous output of last section
}

temp_precise FilterCascaded::readOldestOutput(void)	// return oldest output in filter
{
    return sections[0].readOldestOutput();    // return output of last section
}

void FilterCascaded::init(temp_precise val)
{
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        sections[i].init(val);
    }
}
