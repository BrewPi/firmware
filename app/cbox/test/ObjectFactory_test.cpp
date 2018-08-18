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
        CboxError status1 = CboxError::unknown_error;
        auto obj1 = factory.create(longIntType, status1);
        CHECK(status1 == CboxError::no_error);
        CHECK(obj1->typeId() == longIntType);

        CboxError status2 = CboxError::unknown_error;
        auto obj2 = factory.create(longIntVectorType, status2);
        CHECK(status2 == CboxError::no_error);
        CHECK(obj2->typeId() == longIntVectorType);
    }

    WHEN("The factory is given an invalid type ID, nullptr is returned with status invalid_object_type"){
        CboxError status1 = CboxError::unknown_error;
        auto obj1 = factory.create(9999, status1);
        CHECK(status1 == CboxError::invalid_object_type);
        CHECK(obj1 == nullptr);
    }
}
