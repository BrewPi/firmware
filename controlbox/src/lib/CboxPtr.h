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
#include "ObjectBase.h"
#include "ObjectContainer.h"
#include "ResolveType.h"
#include <memory>

namespace cbox {

template <typename T>
class CboxPtr {
private:
    obj_id_t id;
    ObjectContainer& objects;
    std::weak_ptr<Object> ptr;
    bool castible = false;

public:
    CboxPtr(ObjectContainer& _objects)
        : id(0)
        , objects(_objects)
    {
    }
    virtual ~CboxPtr() = default;

    void setId(obj_id_t newId)
    {
        id = std::move(newId);
    }
    bool isCastable(Object* obj)
    {
        return obj->implements(resolveTypeId<T>());
    }

    std::shared_ptr<T> lock()
    {

        // if the weak pointer was already set, the cast succeeded earlier, we can just return it
        if (auto sptr = ptr.lock()) {
            return std::move(std::static_pointer_cast<T>(sptr));
        }
        // otherwise we try to lookup the object and try if it it can be cast to the desired type using double dispatch

        // look up the object again

        ptr = objects.fetch(id);
        if (auto sptr = ptr.lock()) {
            auto requestedType = resolveTypeId<T>();
            if (sptr->implements(requestedType)) {
                ptr = sptr;
                return std::move(std::static_pointer_cast<T>(sptr));
            }
        }
        // the cast was not valid, reset weak ptr
        ptr.reset();

        return std::shared_ptr<T>(); // return empty share pointer
    }
};

} // end namespace cbox
