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

#include <functional>
#include <stdint.h>
#include <vector>
#include "Object.h"
#include "ResolveType.h"

namespace cbox {

#if 0
/**
 * A container that holds items.
 */
class Container
{
public:
    Container() = default;
    virtual ~Container() = default;

    /**
     * Fetches the object with the given id.
     * /return the object with that id, which may be null.
     *
     * After retrieving the item, callers must call returnItem()
     */
    virtual std::shared_ptr<Object> fetch(const object_id_t id) = 0;
    virtual object_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) = 0;
    virtual bool replace (object_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) = 0;
    virtual void remove(object_id_t id) = 0;
};
#endif


class ContainedObject{
public:
    ContainedObject(object_id_t _id, uint8_t _profiles, std::shared_ptr<Object> _obj) :
        id(_id),
        profiles(_profiles),
        obj(_obj){};

    object_id_t id;
    uint8_t profiles; // active in these profiles
    std::shared_ptr<Object> obj; // ptr to runtime object
};


class ObjectContainer
{
public:
    ObjectContainer() : activeProfiles(0), lastCreatedId(object_id_t::startId()){};
    ObjectContainer(std::initializer_list<ContainedObject> l) : activeProfiles(0), lastCreatedId(object_id_t::startId()){

    };
    virtual ~ObjectContainer() = default;

private:
    std::vector<ContainedObject> objects;
    uint8_t activeProfiles;
    object_id_t lastCreatedId;

public:

    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */

    ContainedObject * find(const object_id_t id) {
        decltype(objects)::iterator begin = objects.begin();
        decltype(objects)::iterator end = objects.end();
        decltype(objects)::iterator found = std::find_if(begin, end, [&id](const ContainedObject& item){ return item.id == id;} );
        if(found == end){
            return nullptr;
        }
        return &(*found);
    }

    std::shared_ptr<Object> fetch(const object_id_t id) {
        auto entry = find(id);
        std::shared_ptr<Object> ptr;
        if(entry != nullptr){
            ptr = entry->obj;
        }
        return ptr;
    }

    object_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) {
        object_id_t newId = lastCreatedId++;
        std::shared_ptr<Object> ptr = std::move(obj);
        ContainedObject entry(newId, active_in_profiles, ptr);
        return newId;
    }

    bool replace (object_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) {
        ContainedObject * entry = find(id);
        if(entry != nullptr){
            entry->profiles = active_in_profiles;
            entry->obj = std::move(obj);
            return true;
        }
        return false;
    }

    void remove(object_id_t id) {
        std::remove_if(objects.begin(), objects.end(), [&id](ContainedObject const& item){ return item.id == id;} );
    }

    void map(std::function<void(ContainedObject & obj)> func) {
        for(auto it : objects){
            func(it);
        }
    }
};

} // end namespace cbox
