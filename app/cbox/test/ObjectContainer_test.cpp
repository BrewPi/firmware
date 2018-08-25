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

#include "Container.h"
#include "DataStreamConverters.h"
#include "Object.h"
#include "ResolveType.h"
#include "TestMatchers.hpp"
#include "TestObjects.h"
#include "catch.hpp"
#include <cstdio>

using namespace cbox;

SCENARIO("A container to hold objects")
{
    ObjectContainer container;

    WHEN("Some objects are added to the container")
    {
        obj_id_t id1 = container.add(std::make_unique<LongIntObject>(0x11111111), 0xFF);
        obj_id_t id2 = container.add(std::make_unique<LongIntObject>(0x22222222), 0xFF);
        obj_id_t id3 = container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF);

        THEN("They are assigned a valid unique ID")
        {
            CHECK(id1 != obj_id_t::invalid());
            CHECK(id2 != obj_id_t::invalid());
            CHECK(id3 != obj_id_t::invalid());

            CHECK(id1 != id2);
            CHECK(id1 != id3);
            CHECK(id2 != id3);
        }

        THEN("The objects can be fetched from the container")
        {
            auto obj2 = container.fetch(id2).lock();
            REQUIRE(obj2);
            CHECK(obj2->typeId() == resolveTypeId<LongIntObject>());
            // to be able to compare the value, we first dereference the smart pointer before typecasting
            CHECK(*static_cast<LongIntObject*>(&(*obj2)) == LongIntObject(0x22222222));
        }

        THEN("The objects can be removed from the container")
        {
            container.remove(id2);
            CHECK(!container.fetch(id2).lock()); // id2 now removed
            CHECK(container.fetch(id3).lock());  // id3 still exists
        }

        THEN("An object can be added with a specific id")
        {
            obj_id_t id4 = container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, obj_id_t(123));
            CHECK(id4 == 123);
            CHECK(container.fetch(id4).lock());

            AND_WHEN("the id already exist, adding fails")
            {
                obj_id_t id5 = container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, obj_id_t(123));
                CHECK(id5 == obj_id_t::invalid());
            }

            AND_WHEN("replace is used instead of add, it succeeds")
            {
                obj_id_t id6 = container.add(std::make_unique<LongIntObject>(0x44444444), 0xFF, obj_id_t(123), true);
                CHECK(id6 == obj_id_t(123));
                auto obj6 = container.fetch(id6).lock();
                REQUIRE(obj6);
                CHECK(*static_cast<LongIntObject*>(&(*obj6)) == LongIntObject(0x44444444));
            }
        }

        THEN("Removing an object that doesn't exist returns invalid_object_id")
        {
            CHECK(container.remove(obj_id_t(10)) == CboxError::INVALID_OBJECT_ID);
        }

        THEN("A list of all object IDs can be created using the container's const iterators")
        {
            std::vector<obj_id_t> ids;
            for (auto it = container.cbegin(); it != container.cend(); it++) {
                ids.emplace_back(it->id());
            };
            std::vector<obj_id_t> correct_list = {id1, id2, id3};
            CHECK(ids == correct_list);
        }

        THEN("All contained objects can be streamed out using the container's const_iterators")
        {
            char buf[1000] = {0};
            BufferDataOut outBuffer(reinterpret_cast<uint8_t*>(buf), sizeof(buf));
            BinaryToHexTextOut out(outBuffer);
            CboxError res = CboxError::OK;

            for (auto it = container.cbegin(); it != container.cend() && res == CboxError::OK; it++) {
                out.writeListSeparator();
                res = it->streamTo(out);
            }

            CHECK(res == CboxError::OK);

            INFO(std::string(buf));
        }
    }

    WHEN("Objects with out of order IDs are added to the container, the container stays sorted by id")
    {
        container.add(std::make_unique<LongIntObject>(0x11111111), 0xFF, 20);
        container.add(std::make_unique<LongIntObject>(0x22222222), 0xFF, 18);
        container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, 23);
        container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, 2);
        container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, 19);
        container.add(std::make_unique<LongIntObject>(0x33333333), 0xFF);
        uint16_t lastId = 0;
        uint16_t count = 0;
        for (auto it = container.cbegin(); it != container.cend(); it++) {
            CHECK(it->id() > lastId);
            INFO(it->id());
            lastId = it->id();
            ++count;
        };
        CHECK(count == 6);
        CHECK(lastId == 24); // new randomly assigned ID is always highest ID in the system
    }
}

SCENARIO("A container with system objects passed in the initializer list")
{
    ObjectContainer objects = {
        ContainedObject(1, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
        ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x22222222))};

    objects.setObjectsStartId(3); // this locks the system objects

    CHECK(obj_id_t(3) == objects.add(std::make_unique<LongIntObject>(0x33333333), 0xFF)); // will get next free ID (3)
    CHECK(obj_id_t(4) == objects.add(std::make_unique<LongIntObject>(0x33333333), 0xFF)); // will get next free ID (4)

    THEN("The system objects can be read like normal objects")
    {
        uint8_t buf[100] = {0};
        BufferDataOut out(buf, sizeof(buf));
        BinaryToHexTextOut hexOut(out);
        auto spobj = objects.fetch(1).lock();
        REQUIRE(spobj);
        spobj->streamTo(hexOut);
        uint8_t hexRepresentation[] = "11111111";
        CHECK_THAT(buf, (equalsArray<uint8_t, sizeof(hexRepresentation)>(hexRepresentation)));
    }

    THEN("The system objects can be read written")
    {
        uint8_t buf[] = "44332211"; // LSB first
        BufferDataIn in(buf, sizeof(buf));
        HexTextToBinaryIn hexIn(in);

        auto spobj = objects.fetch(1).lock();
        REQUIRE(spobj);
        spobj->streamFrom(hexIn);
        CHECK(*static_cast<LongIntObject*>(&(*spobj)) == LongIntObject(0x11223344));
    }

    THEN("The system objects cannot be deleted, but user objects can be deleted")
    {
        CHECK(objects.remove(1) == CboxError::OBJECT_NOT_DELETABLE);
        CHECK(objects.fetch(1).lock());

        CHECK(objects.remove(2) == CboxError::OBJECT_NOT_DELETABLE);
        CHECK(objects.fetch(2).lock());

        CHECK(objects.fetch(3).lock());
        CHECK(objects.remove(3) == CboxError::OK);
        CHECK(!objects.fetch(3).lock());
    }

    THEN("No objects can be added in the system ID range")
    {
        objects.setObjectsStartId(100);
        CHECK(obj_id_t::invalid() == objects.add(std::make_unique<LongIntObject>(0x33333333), 0xFF, 99, true));
    }

    THEN("Objects added after construction can also be marked system by moving the start ID")
    {
        objects.setObjectsStartId(100); // all objects with id  < 100 will now be system objects

        CHECK(objects.remove(1) == CboxError::OBJECT_NOT_DELETABLE);
        CHECK(objects.remove(2) == CboxError::OBJECT_NOT_DELETABLE);
        CHECK(objects.remove(3) == CboxError::OBJECT_NOT_DELETABLE);
        CHECK(objects.remove(4) == CboxError::OBJECT_NOT_DELETABLE);

        CHECK(obj_id_t(100) == objects.add(std::make_unique<LongIntObject>(0x33333333), 0xFF)); // will get start ID (100)
    }
}
