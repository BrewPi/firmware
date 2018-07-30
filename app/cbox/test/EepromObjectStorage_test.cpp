/*
 * Copyright 2018 BrewPi
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
#include "TestObjects.h"

SCENARIO("Storing and retreiving blocks with EEPROM storage"){
    EepromAccess eeprom;
    cbox::EepromObjectStorage storage(eeprom);

    cbox::stream_size_t totalSpace = storage.freeSpace();

    THEN("Storage is one big disposed block initially"){
        CHECK(totalSpace == storage.continuousFreeSpace());
    }

    WHEN("An object is created"){
        LongIntObject obj(0x33333333);

        THEN("It can be saved to EEPROM"){
            auto res = storage.storeObject(cbox::obj_id_t(1), obj);

            THEN("Return value is success"){
                CHECK(res == cbox::StreamResult::success);
            }

            THEN("Free space has decreased by 13 bytes "
                    "(4 bytes object data + 2 bytes object id + 4 bytes overprovision + 3 bytes block header"){
                CHECK(storage.freeSpace() == totalSpace - 13);
            }

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
                bool success = storage.disposeObject(cbox::obj_id_t(1));
                THEN("Which returns true for success"){
                    CHECK(success);
                }

                THEN("Free space equals total space again"){
                    CHECK(storage.freeSpace() == totalSpace);
                }

                THEN("It cannot be retrieved anymore"){
                    LongIntObject received(0xFFFFFFFF);
                    auto res = storage.retreiveObject(cbox::obj_id_t(1), received);
                    CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::end_of_input));
                    CHECK(0xFFFFFFFF == uint32_t(received)); // received is unchanged
                }
                THEN("The id can be re-used to store another object"){
                    LongIntObject otherObject(0xAAAAAAAA);
                    auto res = storage.storeObject(cbox::obj_id_t(1), otherObject);

                    THEN("Return value is success"){
                        CHECK(res == cbox::StreamResult::success);
                    }

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
            THEN("Return value is success"){
                CHECK(res == cbox::StreamResult::success);
            }

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
                bool success = storage.disposeObject(cbox::obj_id_t(1));
                THEN("Which returns true for success"){
                    CHECK(success);
                }

                THEN("It cannot be retrieved anymore"){
                    LongIntVectorObject received;
                    auto res = storage.retreiveObject(cbox::obj_id_t(1), received);
                    CHECK(uint8_t(res) == uint8_t(cbox::StreamResult::end_of_input));
                }
                THEN("The id can be re-used, for a different object type"){
                    LongIntObject otherObject(0xAAAAAAAA);
                    auto res = storage.storeObject(cbox::obj_id_t(1), otherObject);

                    THEN("Return value is success"){
                        CHECK(res == cbox::StreamResult::success);
                    }

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

    WHEN("Multiple objects are created and saved to eeprom"){
        LongIntVectorObject obj1 = {0x11111111, 0x22222222};
        LongIntVectorObject obj2 = {0x11111111, 0x22222222, 0x33333333};
        LongIntVectorObject obj3 = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
        LongIntObject obj4 = 0x11111111;

        auto res1 = storage.storeObject(cbox::obj_id_t(1), obj1);
        auto res2 = storage.storeObject(cbox::obj_id_t(2), obj2);
        auto res3 = storage.storeObject(cbox::obj_id_t(3), obj3);
        auto res4 = storage.storeObject(cbox::obj_id_t(4), obj4);

        THEN("They are stored successfully"){
            CHECK(res1 == cbox::StreamResult::success);
            CHECK(res2 == cbox::StreamResult::success);
            CHECK(res3 == cbox::StreamResult::success);
            CHECK(res4 == cbox::StreamResult::success);


            AND_THEN("They can be retrieved successfully"){
                LongIntVectorObject received;
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(1), received));
                CHECK(obj1 == received);
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(2), received));
                CHECK(obj2 == received);
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(3), received));
                CHECK(obj3 == received);
                LongIntObject received2;
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(4), received2));
                CHECK(obj4 == received2);
            }

            AND_THEN("They can be updated in EEPROM"){
                obj2 = {0x33333333,0x33333333};
                CHECK(cbox::StreamResult::success == storage.storeObject(cbox::obj_id_t(2), obj2));
                LongIntVectorObject received;
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(2), received));
                CHECK(obj2 == received);
            }

            AND_THEN("If one is deleted, it doesn't affect the others"){
                storage.disposeObject(cbox::obj_id_t(2));
                LongIntVectorObject received;
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(1), received));
                CHECK(obj1 == received);
                CHECK(cbox::StreamResult::end_of_input == storage.retreiveObject(cbox::obj_id_t(2), received));
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(3), received));
                CHECK(obj3 == received);
                LongIntObject received2;
                CHECK(cbox::StreamResult::success == storage.retreiveObject(cbox::obj_id_t(4), received2));
                CHECK(obj4 == received2);
            }
        }
    }

    WHEN("A lot of big and small objects are created until EEPROM is full, alternating big and small"){
        LongIntVectorObject big = {
                0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
                0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
                0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777
        };

        LongIntVectorObject small = {0x11111111, 0x22222222};


        cbox::StreamResult res = cbox::StreamResult::success;
        cbox::obj_id_t id = 1; // first id will be 1, because 0 is an invalid id
        while(true){
            if(id % 2 == 0){
                res = storage.storeObject(id, big);
            }
            else{
                res = storage.storeObject(id, small);
            }
            if(res != cbox::StreamResult::success){
                break;
            }
            ++id;
        }

        // last id was not successfully created

        THEN("33 objects have been created"){
            CHECK(id -1 == 33);
        }

        THEN("Free space left is 68 bytes"){
            CHECK(storage.freeSpace() == 68);
        }

        THEN("Continuous free space left is also 68 bytes, which is too small for another big object"){
            CHECK(storage.continuousFreeSpace() == 68);
        }

        THEN("Last object has not been stored"){
            cbox::StreamResult res;
            res = storage.retreiveObject(id, big);
            CHECK(res == cbox::StreamResult::end_of_input);
        }

        THEN("But we can still create a small object"){
            CHECK(cbox::StreamResult::success == storage.storeObject(id, small));
        }

        AND_WHEN("Only the small objects are deleted"){
            cbox::obj_id_t id;
            for(id = 1; ; id++){
                if(id % 2 == 1){
                    if(!storage.disposeObject(id)){
                        break;// break when object cannot be deleted (id doesn't exist)
                    }
                }
            }
            THEN("Continuous free space is still only 93 bytes"){
                CHECK(storage.continuousFreeSpace() == 93);
            }
            THEN("But free space has increased to 493 bytes"){
                CHECK(storage.freeSpace() == 493);
            }

            AND_WHEN("We create 2 big objects again"){
                auto res1 = storage.storeObject(cbox::obj_id_t(1000), big);
                auto res2 = storage.storeObject(cbox::obj_id_t(1001), big);
                THEN("It succeeds"){
                    CHECK(res1 == cbox::StreamResult::success);
                    CHECK(res2 == cbox::StreamResult::success);
                }

                THEN("Eeprom was defragmented and continuous free space equals free space"){
                    INFO("Continuous free space after defrag: " << storage.continuousFreeSpace());
                    CHECK(storage.freeSpace() == storage.continuousFreeSpace());
                }

                THEN("All objects still have the right value"){
                    for(id = 2; id < 10; id = id + 2){
                        LongIntVectorObject received;
                        CHECK(cbox::StreamResult::success == storage.retreiveObject(id, received));
                        CHECK(received == big);
                    }
                }
            }
        }
    }
}

