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

#include "Object.h"
#include "ObjectIds.h"
#include <type_traits>

namespace cbox {

#if !defined(PLATFORM_ID) || PLATFORM_ID == 3
uint16_t
throwIdNotUnique(uint16_t id);
#endif

template <uint16_t id>
class ObjectBase : public Object {
public:
    ObjectBase() = default;
    virtual ~ObjectBase() = default;

    static obj_type_t staticTypeId()
    {
#if !defined(PLATFORM_ID) || PLATFORM_ID == 3 // check that ID is unique if building for cross platform (tests)
        static auto uniqueId = throwIdNotUnique(id);
        return uniqueId;
#else
        return id;
#endif
    }

    /**
	 * The application defined typeID for this object instance. Defined by derived class
	 */
    virtual obj_type_t typeId() const override
    {
        return id;
    }

    virtual void* implements(const obj_type_t& iface) override
    {
        if (id == iface) {
            return this;
        }
        return nullptr;
    }
};

// any type can be assigned a typeid by explicit template instantiation
// this allows objects to implement returning a pointer for that type, without needing to inherit from it
template <typename T>
const obj_type_t
interfaceIdImpl();

// for objects, the object id is the interface id
template <typename T>
const obj_type_t
interfaceId(typename std::enable_if_t<std::is_base_of<Object, T>::value>* = 0)
{
    return T::staticTypeId();
}

// for interface, we check uniqueness on first use, if compiling with gcc (test code)
template <typename T>
const obj_type_t
interfaceId(typename std::enable_if_t<!std::is_base_of<Object, T>::value>* = 0)
{
#if !defined(PLATFORM_ID) || PLATFORM_ID == 3 // check that ID is unique if building for cross platform (tests)
    static auto uniqueId = throwIdNotUnique(interfaceIdImpl<T>());
    return uniqueId;
#else
    return interfaceIdImpl<T>();
#endif
}

} // end namespace cbox
