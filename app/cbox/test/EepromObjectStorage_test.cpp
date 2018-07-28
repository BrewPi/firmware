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
#include <vector>

class LongIntObject : public cbox::RawStreamWritableObject<uint32_t> {
public:
    using cbox::RawStreamWritableObject<uint32_t>::RawStreamWritableObject;

    virtual cbox::obj_type_t typeID() override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeID(this);
    }

    bool operator==(const LongIntObject & rhs) const {
        return obj == rhs.obj;
    }
};

// variable size object of multiple long ints
class LongIntVectorObject : public cbox::Object
{
public:
    LongIntVectorObject() : values(){}
    LongIntVectorObject(std::initializer_list<LongIntObject> l) : values(l){}

    virtual cbox::obj_type_t typeID() override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeID(this);
    }

    virtual cbox::StreamResult streamTo(cbox::DataOut& out) override final {
        cbox::StreamResult res = cbox::StreamResult::success;
        if(!out.put(values.size())){ // first write number of elements
            return cbox::StreamResult::stream_error;
        }
        for(auto & value : values){
            res = value.streamTo(out);
            if(res != cbox::StreamResult::success){
                break;
            }
        }
        return res;
    }

    virtual cbox::StreamResult streamFrom(cbox::DataIn& in) override final {
        cbox::StreamResult res = cbox::StreamResult::success;
        decltype(values)::size_type newSize = 0;
        if(!in.get(newSize)){
            return cbox::StreamResult::stream_error;
        }
        values.resize(newSize);
        for(auto & value : values){
            res = value.streamFrom(in);
            if(res != cbox::StreamResult::success){
                break;
            }
        }
        return res;
    };

    bool operator==(const LongIntVectorObject & rhs) const {
        return values == rhs.values;
    }

    std::vector<LongIntObject> values;
};



SCENARIO("Storing and retreiving blocks with EEPROM storage"){
    EepromAccess eeprom;
    cbox::EepromObjectStorage storage(eeprom);

    WHEN("An object is created"){
        LongIntObject obj(0x33333333);

        THEN("It can be saved to EEPROM"){
            auto res = storage.storeObject(cbox::obj_id_t(1), obj);
            CHECK(res == cbox::StreamResult::success);

            THEN("The data can be streamed back from EEPROM"){
                LongIntObject target(0xFFFFFFFF);
                auto res = storage.retreiveObject(cbox::obj_id_t(1), target);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(uint32_t(obj) == uint32_t(target));
            }

            THEN("It can be changed and rewritten to EEPROM"){
                obj = 0xAAAAAAAA;
                auto res = storage.storeObject(cbox::obj_id_t(1), obj);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));

                LongIntObject received(0xFFFFFFFF);
                res = storage.retreiveObject(cbox::obj_id_t(1), received);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(uint32_t(obj) == uint32_t(received));
            }

            THEN("It can be disposed"){
                CHECK(storage.disposeObject(cbox::obj_id_t(1)));

                THEN("It cannot be retrieved anymore"){
                    LongIntObject received(0xFFFFFFFF);
                    auto res = storage.retreiveObject(cbox::obj_id_t(1), received);
                    CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::end_of_input));
                    CHECK(0xFFFFFFFF == uint32_t(received)); // received is unchanged
                }
                THEN("The id can be re-used"){
                    LongIntObject otherObject(0xAAAAAAAA);
                    auto res = storage.storeObject(cbox::obj_id_t(1), otherObject);
                    CHECK(res == cbox::StreamResult::success);

                    AND_THEN("The id returns the new object's data"){
                        LongIntObject received(0xFFFFFFFF);
                        auto res = storage.retreiveObject(cbox::obj_id_t(1), received);
                        CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                        CHECK(uint32_t(0xAAAAAAAA) == uint32_t(received));
                    }
                }
            }
        }
    }

    WHEN("An variable size object is created"){
        LongIntVectorObject obj = {0x11111111, 0x22222222};

        THEN("It can be saved to EEPROM"){
            auto res = storage.storeObject(cbox::obj_id_t(1), obj);
            CHECK(res == cbox::StreamResult::success);

            THEN("The data can be streamed back from EEPROM"){
                LongIntVectorObject target;
                auto res = storage.retreiveObject(cbox::obj_id_t(1), target);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(obj == target);
            }

            THEN("It can be changed and rewritten to EEPROM, same size"){
                obj = {0x22222222, 0x33333333};
                auto res = storage.storeObject(cbox::obj_id_t(1), obj);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));

                LongIntVectorObject received;
                res = storage.retreiveObject(cbox::obj_id_t(1), received);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(obj == received);
            }

            THEN("It can be changed and rewritten to EEPROM, 4 bytes bigger size"){
                obj = {0x22222222, 0x33333333, 0x44444444};
                auto res = storage.storeObject(cbox::obj_id_t(1), obj);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));

                LongIntVectorObject received;
                res = storage.retreiveObject(cbox::obj_id_t(1), received);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(obj == received);
            }

            THEN("It can be changed and rewritten to EEPROM, 12 bytes bigger size"){
                obj = {0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777};
                auto res = storage.storeObject(cbox::obj_id_t(1), obj);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));

                LongIntVectorObject received;
                res = storage.retreiveObject(cbox::obj_id_t(1), received);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(obj == received);
            }

            THEN("It can be changed and rewritten to EEPROM, 4 bytes smaller size"){
                obj = {0x22222222};
                auto res = storage.storeObject(cbox::obj_id_t(1), obj);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));

                LongIntVectorObject received;
                res = storage.retreiveObject(cbox::obj_id_t(1), received);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(obj == received);
            }

            THEN("It can be changed and rewritten to EEPROM, 8 bytes smaller size (empty vector)"){
                obj = {};
                auto res = storage.storeObject(cbox::obj_id_t(1), obj);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));

                LongIntVectorObject received;
                res = storage.retreiveObject(cbox::obj_id_t(1), received);
                CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                CHECK(obj == received);
            }

            THEN("It can be disposed"){
                CHECK(storage.disposeObject(cbox::obj_id_t(1)));

                THEN("It cannot be retrieved anymore"){
                    LongIntVectorObject received;
                    auto res = storage.retreiveObject(cbox::obj_id_t(1), received);
                    CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::end_of_input));
                }
                THEN("The id can be re-used, for a different object type"){
                    LongIntObject otherObject(0xAAAAAAAA);
                    auto res = storage.storeObject(cbox::obj_id_t(1), otherObject);
                    CHECK(res == cbox::StreamResult::success);

                    AND_THEN("The id returns the new object's data"){
                        LongIntObject received(0xFFFFFFFF);
                        auto res = storage.retreiveObject(cbox::obj_id_t(1), received);
                        CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::success));
                        CHECK(uint32_t(0xAAAAAAAA) == uint32_t(received));
                    }
                }
            }
        }
    }
}

