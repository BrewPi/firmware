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
#include "ResolveType.h"
#include "Object.h"
#include "ArrayEepromAccess.h"

class LongIntObject : public cbox::RawStreamWritableObject<uint32_t> {
public:
    using cbox::RawStreamWritableObject<uint32_t>::RawStreamWritableObject;

    virtual cbox::obj_type_t typeID() override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeID(this);
    }

};

SCENARIO("Storing and retreiving blocks with EEPROM storage"){
    EepromAccess eeprom;
    cbox::EepromObjectStorage storage(eeprom);

    WHEN("An object is created"){
        LongIntObject obj(0x12345678);

        THEN("It can be saved to EEPROM"){
            storage.storeObject(1, obj);

            THEN("The data can be streamed back into another object from EEPROM"){
                LongIntObject target(0xFFFFFFFF);
                auto res = storage.retreiveObject(1, target);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(uint32_t(obj) == uint32_t(target));
            }
        }
    }
}

