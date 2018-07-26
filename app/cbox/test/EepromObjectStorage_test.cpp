/*
 * Copyright 2017 BrewPi
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

#include "catch.hpp"
#include <cstdio>

#include "EepromObjectStorage.h"
#include "Object.h"

class LongIntObject : public cbox::RawStreamWritableObject<uint32_t> {
public:
    LongIntObject(uint32_t v) : cbox::RawStreamWritableObject<uint32_t>(v){
    }



};

SCENARIO("Storing and retreiving blocks with EEPROM storage"){
    WHEN("An object is created"){

    }
}

