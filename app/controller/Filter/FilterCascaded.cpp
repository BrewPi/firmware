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
#include "temperatureFormats.h"
#include "FilterFixed.h"
#include "FilterCascaded.h"
#include <stdlib.h>

FilterCascaded::FilterCascaded()
{
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        sections[i].setFiltering(2);    // default to a b value of 2
    }
    init();
}

void FilterCascaded::setFiltering(uint8_t bValue)
{
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        sections[i].setFiltering(bValue);
    }
}

uint8_t FilterCascaded::getFiltering()
{
    return sections[0].getFiltering();
}

temp_t FilterCascaded::add(const temp_t & val)
{
    temp_precise_t p = val;

    // return output, converted back to normal precision
    return add(p);
}

temp_precise_t FilterCascaded::add(const temp_precise_t & val)
{
    temp_precise_t temporary = val;

    // temporary is input for next section, which is the output of the previous section
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        temporary = sections[i].add(temporary);
    }

    return temporary;
}

temp_precise_t FilterCascaded::readInput(void)
{
    return sections[0].readInput();    // return unfiltered input of first section
}

temp_precise_t FilterCascaded::readOutput(void)
{
    return sections[NUM_SECTIONS - 1].readOutput();    // return output of last section (which is most filtered)
}

bool FilterCascaded::detectPosPeak(temp_precise_t * peak)
{
    return sections[NUM_SECTIONS - 1].detectPosPeak(peak);    // detect peaks in last section
}

bool FilterCascaded::detectNegPeak(temp_precise_t * peak)
{
    return sections[NUM_SECTIONS - 1].detectNegPeak(peak);    // detect peaks in last section
}

bool FilterCascaded::isRising()
{
    return sections[NUM_SECTIONS - 1].isRising(); // return true if in the last section output > previous output
}

bool FilterCascaded::isFalling()
{
    return sections[NUM_SECTIONS - 1].isFalling(); // return true if in the last section output < previous output
}

temp_precise_t FilterCascaded::readPrevOutput(void)
{
    return sections[NUM_SECTIONS - 1].readPrevOutput();    // return previous output of last section
}

void FilterCascaded::init(temp_precise_t val)
{
    for (uint8_t i = 0; i < NUM_SECTIONS; i++)
    {
        sections[i].init(val);
    }
}

uint16_t FilterCascaded::getDelay(){
    return delayTimes[sections[0].b];
}

void FilterCascaded::setFilteringForDelay(uint16_t maxDelay){
    uint8_t i;
    for( i = 0; i < 6; i++ ){
        if(maxDelay <= delayTimes[i+1]){
            break;
        }
    }
    setFiltering(i);
}
