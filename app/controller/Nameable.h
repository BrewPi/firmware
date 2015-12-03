/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#include <string.h>

const size_t MAX_NAME_LENGTH = 15;

class Nameable {
public:
    Nameable(){
        name[0] = '\0';
        name[MAX_NAME_LENGTH] = '\0'; // ensure last character is always \0
    }
    virtual ~Nameable(){};

    void setName(const char * newName){
        strncpy(name, newName, MAX_NAME_LENGTH); // doesn't overwrite last character
    }

    const char * getName(){
        return name;
    }

protected:
    char name[MAX_NAME_LENGTH+1]; // store as constant size array
};
