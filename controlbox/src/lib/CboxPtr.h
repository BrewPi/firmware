/*
 * Copyright 2018 Elco Jacobs / BrewBlox
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

    obj_id_t getId()
    {
        return id;
    }

    auto convert_ptr(std::shared_ptr<Object>&& ptr, void* thisPtr)
    {
        // When multiple inheritance is involved, the requested interface can have
        // a different this pointer than the Object interface
        // To work around this issue, implements() returns the this pointer of the requested interface
        // We need to return a shared pointer with the same ref count block, but a different offset

        auto p = reinterpret_cast<typename std::shared_ptr<T>::element_type*>(thisPtr);
        return std::shared_ptr<T>(ptr, p);
    }

    std::shared_ptr<T>
    lock()
    {
        // try to lock the weak pointer we already had. If it cannot be locked, we need to do a lookup again
        std::shared_ptr<Object> sptr;
        sptr = ptr.lock();
        if (!sptr) {
            // Try to lookup the object in the container
            ptr = objects.fetch(id);
            sptr = ptr.lock();
        }
        if (sptr) {
            auto requestedType = resolveTypeId<T>();
            void* thisPtr = sptr->implements(requestedType);
            if (thisPtr != nullptr) {
                return convert_ptr(std::move(sptr), thisPtr);
            }
        }
        // the cast was not allowed, reset weak ptr
        ptr.reset();
        // return empty share pointer
        return std::shared_ptr<T>();
    }
};

} // end namespace cbox
