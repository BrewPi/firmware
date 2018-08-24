
/*
 * Copyright 2018 Elco Jacobs / BrewBlox, based on earlier work of Matthew McGowan
 *
 * This file is part of ControlBox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "DataStream.h"
#include "Object.h"
#include <array>
#include <memory>

namespace cbox {

std::unique_ptr<Object>
createApplicationObject(obj_type_t typeId, DataIn& in, CboxError& errorCode);

template <class T>
std::unique_ptr<Object>
createObject()
{
    return std::make_unique<T>();
}

// An object factory combines the create function with a type ID.
// They can be put in a container that can be walked to find the matching typeId
struct ObjectFactoryEntry {
    obj_type_t typeId;
    std::unique_ptr<Object> (*createFn)();
};

class ObjectFactory {
private:
    const std::vector<ObjectFactoryEntry> objTypes;

public:
    ObjectFactory(std::initializer_list<ObjectFactoryEntry> _objTypes)
        : objTypes(_objTypes)
    {
    }

    CboxError make(const obj_type_t& t, std::unique_ptr<Object>& objTarget) const
    {
        auto factoryEntry = std::find_if(objTypes.begin(), objTypes.end(), [&t](const ObjectFactoryEntry& entry) { return entry.typeId == t; });
        if (factoryEntry == objTypes.end()) {
            return CboxError::object_not_creatable;
        } else {
            objTarget = (*factoryEntry).createFn();
            if (!objTarget) {
                return CboxError::insufficient_heap;
            }
        }
        return CboxError::no_error;
    }
};

/* macro to help define the object factory, use like this:

ObjectFactory objectFactory = {
        OBJECT_FACTORY_ENTRY(Class1),
        OBJECT_FACTORY_ENTRY(Class2)
};

*/
#define OBJECT_FACTORY_ENTRY(classname)                                 \
    {                                                                   \
        cbox::resolveTypeId<classname>(), cbox::createObject<classname> \
    }

} // end namespace cbox
