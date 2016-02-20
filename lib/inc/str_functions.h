/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 * Copyright 2015 Matthew McGowan.
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
#include <cstring>
#include <stdint.h>

long int my_strtol(const char* str, char** tail);

// Use custom strtol to save space.
// std strtol is 616 byes on avr, my_strtol is 166
#if 1
inline long int strtol_impl(const char* str, char** tail){
    return my_strtol(str, tail);
}
#else
inline long int strtol_impl(const char* str, char** tail){
    return strol(str, tail, 10);
}
#endif

bool stringToBool(bool * result, const char * numberString);

bool stringToUint16(uint16_t * result, const char * numberString);

// check if strtol function (which has set the end pointer) was successful.
bool invalidStrtolResult(const char * start, const char * end);
