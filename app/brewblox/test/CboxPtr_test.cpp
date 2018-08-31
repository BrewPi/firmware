

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

#include "cbox/CboxPtr.h"

#include "catch.hpp"
#include "cbox/ObjectContainer.h"
#include "test/TestObjects.h"

using namespace cbox;

SCENARIO("A CboxPtr is a dynamic lookup that checks type compatibility and works similar to a weak pointer")
{

    ObjectContainer objects = {
        ContainedObject(1, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
        ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
    };

    CboxPtr<LongIntObject> liPtr(objects);
    CboxPtr<LongIntVectorObject> livPtr(objects);

    WHEN("lock() is called on a CboxPtr that does not have a valid ID, it returns an empty shared pointer")
    {
        CHECK(!liPtr.lock());
        CHECK(!livPtr.lock());
    }

    WHEN("lock() is called on a CboxPtr that does not implement the requested type, it returns an empty shared pointer")
    {
        liPtr.setId(1);
        livPtr.setId(1);

        CHECK(liPtr.lock());
        CHECK(!livPtr.lock());

        CboxPtr<Nameable> nameablePtr(objects);
        nameablePtr.setId(1);
        CHECK(!nameablePtr.lock());
    }

    WHEN("a CboxPtr of certain type is created, it can point to objects implementing that interface")
    {
        objects.add(std::make_unique<NameableLongIntObject>(0x22222222), 0xFF, 100);
        CboxPtr<NameableLongIntObject> nameableLiPtr(objects);
        CboxPtr<LongIntObject> liPtr(objects);
        CboxPtr<Nameable> nameablePtr(objects);

        nameableLiPtr.setId(100);
        liPtr.setId(100);
        nameablePtr.setId(100);

        REQUIRE(nameableLiPtr.lock());
        REQUIRE(liPtr.lock());
        REQUIRE(nameablePtr.lock());

        if (auto ptr = nameablePtr.lock()) {
            ptr->setName("Test!");
            CHECK(ptr->getName() == "Test!");
        }

        THEN("The use count is correct on all shared pointers")
        {
            {
                auto ptr1 = nameableLiPtr.lock();
                auto ptr2 = liPtr.lock();
                auto ptr3 = nameablePtr.lock();

                CHECK(ptr1.use_count() == 4); // 4 pointers in use(1 in the container)

                AND_THEN("The addresses match what static cast from most derived class would generate")
                {
                    // pointer 1 and 2 point to the same memory location, because they share a base
                    CHECK(ptr1.get() == ptr2.get());

                    // pointer 3 points to a different location in the same object, that implements the Nameable interface
                    CHECK(reinterpret_cast<void*>(ptr2.get()) != reinterpret_cast<void*>(ptr3.get()));

                    // The offset it got matches static cast
                    CHECK(static_cast<Nameable*>(ptr1.get()) == ptr3.get());
                }

                THEN("If an object is removed, the pointers are still valid, because they share ownership")
                {
                    objects.remove(100);
                    CHECK(ptr1.use_count() == 3); // objects doesn't share ownership anymore
                }
            }

            THEN("After all shared pointers go out of scope, we cannot get a new shared pointer from the CboxPtr class")
            {
                objects.remove(100);
                auto ptr4 = nameablePtr.lock();
                CHECK(!ptr4);

                objects.add(std::make_unique<NameableLongIntObject>(0x22222222), 0xFF, 100);
                THEN("If a new compatible object is created with the same id, the CboxPtr can be locked again")
                {
                    auto ptr5 = nameablePtr.lock();
                    CHECK(ptr5);
                }
            }

            THEN("When an object is deactivated, the CboxPtr cannot be locked")
            {
                objects.deactivate(obj_id_t(100));
                auto ptr4 = nameablePtr.lock();
                CHECK(!ptr4);
            }
        }
    }
}
