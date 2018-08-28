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
#include "ResolveType.h"
#include <type_traits>

namespace cbox {

template <typename T>
class ObjectBase : public Object {
public:
    ObjectBase() = default;
    virtual ~ObjectBase() = default;

    template <typename U,
              typename = std::enable_if_t<std::is_same<T, U>::value>>
    obj_type_t typeIdImpl(void) const
    {
        return resolveTypeId<U>();
    }

    /**
	 * The application defined typeID for this object instance. Defined by derived class
	 */
    virtual obj_type_t typeId() const override
    {
        return typeIdImpl<T>();
    }

    virtual void* implements(const obj_type_t& iface) override
    {
        return nullptr;
    }
};

} // end namespace cbox
