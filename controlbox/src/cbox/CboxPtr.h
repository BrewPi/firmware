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
        ptr.reset();
    }

    obj_id_t getId() const
    {
        return id;
    }

    /**
     * @param a shared pointer to an Object class
     * @param a raw pointer to the same object, but for the interface implemented by T
     * @return a new shared pointer with the same ref counting block, but a different type and offset pointer
     */
    auto convert_ptr(std::shared_ptr<Object>&& ptr, void* thisPtr)
    {
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
            // if the lookup succeeded, check if the Object implements the requested interface using the object types
            auto requestedType = interfaceId<T>();
            void* thisPtr = sptr->implements(requestedType);
            if (thisPtr != nullptr) {
                // If the object returned a non-zero pointer, it supports the interface
                // If multiple-inheritance is involved, it is possible that the shared pointer and interface pointer
                // do not point to the same address. That is why the this pointer is returned by the base that implements
                // the interface. convert_ptr ensures the block managing the lifetime of the object is still used.
                return convert_ptr(std::move(sptr), thisPtr);
            }
        }
        // the cast was not allowed, reset weak ptr
        ptr.reset();
        // return empty share pointer
        return std::shared_ptr<T>();
    }

    std::shared_ptr<const T>
    const_lock() const
    {
        return std::const_pointer_cast<const T>(
            std::move(const_cast<CboxPtr<T>*>(this)->lock()));
    }

    operator std::function<std::shared_ptr<T>()>()
    {
        return std::bind(&cbox::CboxPtr<T>::lock, this);
    }

    operator std::function<std::shared_ptr<T>()>() const
    {
        return std::bind(&cbox::CboxPtr<T>::const_lock, this);
    }

    /*
     * Returns whether the weak pointer is still valid. This does not do a new object fetch.
     * Don't query this before trying to use the pointer, just try to lock it.
     * Use this function after using the sensor with lock() to print the status.
     */
    bool valid() const
    {
        return !ptr.expired();
    }
};

} // end namespace cbox
