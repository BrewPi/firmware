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
#include "TemperatureFormats.h"
#include "TempControl.h"

// convertToInternalTemp receives the external temp format in fixed point and converts it to the internal format
// It scales the value for Fahrenheit and adds the offset needed for absolute temperatures. For temperature differences, use no offset.

long_temperature convertToInternalTempImpl(long_temperature rawTemp, bool addOffset) {
    if (tempControl.cc.tempFormat == 'F') { // value received is in F, convert to C
        if (addOffset) {
            rawTemp = rawTemp - (temperature(32) << TEMP_FIXED_POINT_BITS);
        }
        int8_t rounder = (rawTemp < 0) ? -45 : 45;
        rawTemp = (rawTemp * 50 + rounder) / 90; // rounded result        
    }
    if (addOffset) {
        rawTemp += C_OFFSET;
    }
    return rawTemp;
}

// convertAndConstrain adds an offset, then scales with *9/5 for Fahrenheit. Use it without the offset argument for temperature differences

long_temperature convertFromInternalTempImpl(long_temperature rawTemp, bool addOffset) {
    if (tempControl.cc.tempFormat == 'F') { // value received is in F, convert to C
        if (addOffset) {
            rawTemp -= F_OFFSET;
        }
        int8_t rounder = (rawTemp < 0) ? -25 : 25;
        rawTemp = (rawTemp * 90 + rounder) / 50; // round result
    } else if (addOffset) {
        rawTemp -= C_OFFSET;
    }
    return rawTemp;
}


