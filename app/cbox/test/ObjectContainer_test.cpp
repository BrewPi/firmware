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
#include "ResolveType.h"
#include "Object.h"
#include "Container.h"
#include "Comms.h"
#include "TestObjects.h"

SCENARIO("A container to hold objects"){
    cbox::ObjectContainer container;

    WHEN("Some objects are added to the container"){
        cbox::obj_id_t id1 = container.add(std::make_unique<LongIntObject>(0x11111111), 0xFF);
        cbox::obj_id_t id2 = container.add(std::make_unique<LongIntObject>(0x22222222), 0xFF);
        cbox::obj_id_t id3 = container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF);

        THEN("They are assigned a valid unique ID"){
            CHECK(id1 != cbox::obj_id_t::invalid());
            CHECK(id2 != cbox::obj_id_t::invalid());
            CHECK(id3 != cbox::obj_id_t::invalid());

            CHECK(id1 != id2);
            CHECK(id1 != id3);
            CHECK(id2 != id3);
        }

        THEN("The objects can be fetched from the container"){
            auto obj2 = container.fetch(id2);
            CHECK(obj2->typeID() == cbox::resolveTypeID<LongIntObject>());
            // to be able to compare the value, we first dereference the smart pointer before typecasting
            CHECK(*static_cast<LongIntObject*>(&(*obj2)) == LongIntObject(0x22222222));
        }

        THEN("The objects can be removed from the container"){
            container.remove(id2);
            CHECK(container.fetch(id2) == nullptr);
            CHECK(container.fetch(id3) != nullptr);
        }

        THEN("An object can be added with a specific id"){
            cbox::obj_id_t id4 = container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, cbox::obj_id_t(123));
            CHECK(id4 == 123);
            CHECK(container.fetch(id4) != nullptr);

            AND_WHEN("the id already exist, adding fails"){
                cbox::obj_id_t id5 = container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, cbox::obj_id_t(123));
                CHECK(id5 == cbox::obj_id_t::invalid());
            }

            AND_WHEN("replace is used instead of add, it succeeds"){
                cbox::obj_id_t id6 = container.replace(std::make_unique<LongIntObject>(0x44444444), 0xFF, cbox::obj_id_t(123));
                CHECK(id6 == cbox::obj_id_t(123));
                auto obj6 = container.fetch(id6);
                REQUIRE(obj6 != nullptr);
                CHECK(*static_cast<LongIntObject*>(&(*obj6)) == LongIntObject(0x44444444));
            }
        }

        THEN("An action can be applied on all contained objects, using map, like printing listing all object ids"){
            std::vector<cbox::obj_id_t> ids;
            container.map([&ids] (cbox::ContainedObject & entry){
                ids.emplace_back(entry.id);
            });
            std::vector<cbox::obj_id_t> correct_list = {id1, id2, id3};
            CHECK(ids == correct_list);
        }


        THEN("An action can be applied on all contained objects, using map, like streaming out all objects"){
            char buf[1000];
            cbox::BufferDataOut outBuffer(reinterpret_cast<uint8_t*>(buf), sizeof(buf));
            cbox::BinaryToHexTextOut out(outBuffer);
            cbox::StreamResult res;
            container.mapWhileTrue([&out, &res] (cbox::ContainedObject & entry) -> bool {
                out.writeListSeparator();
                res = entry.streamTo(out);
                return res == cbox::StreamResult::success;
            });

            INFO(std::string(buf));
        }
    }
}

