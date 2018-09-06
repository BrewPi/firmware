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

#include "Object.h"
#include "ObjectContainer.h"
#include "ObjectFactory.h"
#include "TestObjects.h"
#include <catch.hpp>
#include <tuple>

using namespace cbox;

SCENARIO("An object can be created by an ObjectFactory by resolving the type id")
{
    ObjectFactory factory = {
        {LongIntObject::staticTypeId(), std::make_shared<LongIntObject>},
        {LongIntVectorObject::staticTypeId(), std::make_shared<LongIntVectorObject>},
    };

    const obj_type_t longIntType = LongIntObject::staticTypeId();
    const obj_type_t longIntVectorType = LongIntVectorObject::staticTypeId();

    WHEN("The factory is given a valid type ID, the object with type ID is created")
    {
        std::shared_ptr<Object> obj1;
        CboxError status1;
        std::tie(status1, obj1) = factory.make(longIntType);
        CHECK(status1 == CboxError::OK);
        CHECK(obj1->typeId() == longIntType);

        CboxError status2;
        std::shared_ptr<Object> obj2;
        std::tie(status2, obj2) = factory.make(longIntVectorType);

        CHECK(status2 == CboxError::OK);
        CHECK(obj2->typeId() == longIntVectorType);
    }

    WHEN("The factory is given an invalid type ID, nullptr is returned with status object_not_creatable")
    {
        std::shared_ptr<Object> obj;
        CboxError status;
        std::tie(status, obj) = factory.make(9999);
        CHECK(status == CboxError::OBJECT_NOT_CREATABLE);
        CHECK(obj == nullptr);
    }
}
