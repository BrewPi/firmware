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
#include "ResolveType.h"
#include "Object.h"
#include "Container.h"
#include "ObjectFactory.h"
#include "TestObjects.h"

SCENARIO("An object can be created by an ObjectFactory by resolving the type id"){
    using namespace cbox; 

    ObjectFactory factory = {
        OBJECT_FACTORY_ENTRY(LongIntObject),
        OBJECT_FACTORY_ENTRY(LongIntVectorObject)
    };

    const obj_type_t longIntType = resolveTypeId<LongIntObject>();
    const obj_type_t longIntVectorType = resolveTypeId<LongIntVectorObject>();

    WHEN("The factory is given a valid type ID, the object with type ID is created"){
        std::unique_ptr<Object> obj1;
        auto status1 = factory.make(longIntType, obj1);
        CHECK(status1 == CboxError::no_error);
        CHECK(obj1->typeId() == longIntType);

        std::unique_ptr<Object> obj2;
        auto status2 = factory.make(longIntVectorType, obj2);
        CHECK(status2 == CboxError::no_error);
        CHECK(obj2->typeId() == longIntVectorType);
    }

    WHEN("The factory is given an invalid type ID, nullptr is returned with status object_not_creatable"){
        std::unique_ptr<Object> obj;
        auto status = factory.make(9999, obj);
        CHECK(status == CboxError::object_not_creatable);
        CHECK(obj == nullptr);
    }
}
