/*
 * Copyright 2018 Elco Jacobs / BrewBlox
 *
 * This file is part of ControlBox
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

#include "ContainedObject.h"
#include "Object.h"
#include <functional>
#include <stdint.h>
#include <vector>

namespace cbox {

class ObjectContainer {
private:
    std::vector<ContainedObject> objects;
    obj_id_t startId;

public:
    using Iterator = decltype(objects)::iterator;
    using CIterator = decltype(objects)::const_iterator;

    ObjectContainer()
        : objects()
        , startId(obj_id_t::start())
    {
    }

    ObjectContainer(std::initializer_list<ContainedObject> systemObjects)
        : objects(systemObjects)
        , startId(obj_id_t::start())
    {
    }

    virtual ~ObjectContainer() = default;

private:
    auto findPosition(const obj_id_t& id)
    {
        // equal_range is used instead of find, because it is faster for a sorted container
        // the returned pair can be used as follows:
        // first == second means not found, first points to the insert position for the new object id
        // first != second means the object is found and first points to it

        struct IdLess {
            bool operator()(const ContainedObject& c, const obj_id_t& i) const { return c.id() < i; }
            bool operator()(const obj_id_t& i, const ContainedObject& c) const { return i < c.id(); }
        };

        auto pair = std::equal_range(
            objects.begin(),
            objects.end(),
            id,
            IdLess{});
        return pair;
    }

    obj_id_t nextId() const
    {
        return std::max(startId, objects.empty() ? startId : ++obj_id_t(objects.back().id()));
    }

public:
    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */
    ContainedObject* fetchContained(const obj_id_t id)
    {
        auto p = findPosition(id);
        if (p.first == p.second) {
            return nullptr;
        } else {
            return &(*p.first);
        }
    }

    const std::weak_ptr<Object> fetch(const obj_id_t id)
    {
        auto p = findPosition(id);
        if (p.first == p.second) {
            return std::weak_ptr<Object>(); // empty weak ptr if not found
        }
        return p.first->object(); // weak_ptr to found object
    }

    /**
     * set start ID for user objects.
     * ID's smaller than the start ID are  assumed to be system objects and considered undeletable.
     **/
    void setObjectsStartId(const obj_id_t& id)
    {
        startId = id;
    }

    // create a new object and let box assign id
    obj_id_t add(std::unique_ptr<Object> obj, const uint8_t active_in_profiles)
    {
        return add(std::move(obj), active_in_profiles, obj_id_t::invalid());
    }

    // create a new object with specific id, optionally replacing an existing object
    obj_id_t add(std::unique_ptr<Object> obj, const uint8_t active_in_profiles, const obj_id_t& id, bool replace = false)
    {
        obj_id_t newId;
        Iterator position;

        if (id == obj_id_t::invalid()) { // use 0 to let the container assign a free slot
            newId = nextId();
            position = objects.end();
        } else {
            if (id < startId) {
                return obj_id_t::invalid(); // refuse to add system objects
            }
            // find insert position
            auto p = findPosition(id);
            if (p.first != p.second) {
                // existing object found
                if (!replace) {
                    return obj_id_t::invalid(); // refuse to overwrite existing objects
                }
            }
            newId = id;
            position = p.first;
        }

        if (replace) {
            *position = ContainedObject(newId, active_in_profiles, std::move(obj));
        } else {
            // insert new entry in container in sorted position
            objects.emplace(position, newId, active_in_profiles, std::move(obj));
        }
        return newId;
    }

    CboxError remove(obj_id_t id)
    {
        if (id < startId) {
            return CboxError::OBJECT_NOT_DELETABLE; // refuse to remove system objects
        }
        // find existing object
        auto p = findPosition(id);
        objects.erase(p.first, p.second); // doesn't remove anything if no objects found (first == second)
        return p.first == p.second ? CboxError::INVALID_OBJECT_ID : CboxError::OK;
    }

    // only const iterators are exposed. We don't want the caller to be able to modify the container
    CIterator cbegin()
    {
        return objects.cbegin();
    }

    CIterator cend()
    {
        return objects.cend();
    }

    CIterator userbegin()
    {
        return findPosition(startId).first;
    }

    // replace an object with an inactive object by const iterator
    void deactivate(const CIterator& cit)
    {
        auto it = objects.erase(cit, cit); // convert to non-const iterator
        it->deactivate();
    }

    // replace an object with an inactive object by id
    void deactivate(const obj_id_t& id)
    {
        auto p = findPosition(id);
        if (p.first != p.second) {
            p.first->deactivate();
        }
    }

    // remove all non-system objects from the container
    void clear()
    {
        objects.erase(userbegin(), cend());
    }

    void update(const update_t& now)
    {
        for (auto& cobj : objects) {
            cobj.update(now);
        }
    }

    void forcedUpdate(const update_t& now)
    {
        for (auto& cobj : objects) {
            cobj.forcedUpdate(now);
        }
    }
};

} // end namespace cbox
