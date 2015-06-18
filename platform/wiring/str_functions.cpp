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


#include "str_functions.h"

// custom string conversion functions that are smaller than standard versions


long int my_strtol(const char* str, char** tail) {
    long int val = 0;
    bool positive = true;
    *tail = (char*) str;
    unsigned char read = 0;
    while (1) {
        if (**tail == '\0') {
            break;
        } else if (**tail == ' ') {
            if (read != 0) {
                break; // only leading zeros allowed
            }
        } else if (**tail == '-') {
            positive = false;
        } else if (**tail >= '0' && **tail <= '9') {
            val = val * 10 + (**tail - '0');
            read++;
        } else {
            break;
        }
        (*tail)++;
    }
    if (read == 0) {
        *tail = (char*) str;
    }
    return positive ? val : -val;
}

bool invalidStrtolResult(const char * start, const char * end){
    return ((*end != '\0' && *end != '.' && *end != ' ') // parsing did not end at end of string, space or decimal point
            || start == end); // no number found in string
}

