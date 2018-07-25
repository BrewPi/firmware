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

namespace cbox {

class ContainedObject{
public:
    ContainedObject(obj_id_t _id, uint8_t _profiles, std::shared_ptr<Object> _obj) :
        id(_id),
        profiles(_profiles),
        obj(_obj){};

    obj_id_t id;
    uint8_t profiles; // active in these profiles
    std::shared_ptr<Object> obj; // ptr to runtime object
};


class ObjectContainer
{
public:
    ObjectContainer() : activeProfiles(0), lastAssignedId(obj_id_t::start()){};
    ObjectContainer(std::initializer_list<ContainedObject> l) : activeProfiles(0), lastAssignedId(obj_id_t::start()){

    };
    virtual ~ObjectContainer() = default;

private:
    std::vector<ContainedObject> objects;
    uint8_t activeProfiles;
    obj_id_t lastAssignedId;

public:

    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */

    ContainedObject * find(const obj_id_t id) {
        decltype(objects)::iterator begin = objects.begin();
        decltype(objects)::iterator end = objects.end();
        decltype(objects)::iterator found = std::find_if(begin, end, [&id](const ContainedObject& item){ return item.id == id;} );
        if(found == end){
            return nullptr;
        }
        return &(*found);
    }

    std::shared_ptr<Object> fetch(const obj_id_t id) {
        auto entry = find(id);
        std::shared_ptr<Object> ptr;
        if(entry != nullptr){
            ptr = entry->obj;
        }
        return ptr;
    }

    obj_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) {
        obj_id_t newId = lastAssignedId++;
        std::shared_ptr<Object> ptr = std::move(obj);
        ContainedObject entry(newId, active_in_profiles, ptr);
        objects.push_back(entry);
        return newId;
    }

    obj_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles, obj_id_t id) {
        auto existing = find(id);
        if(existing == nullptr){
            return obj_id_t::invalid();
        }
        std::shared_ptr<Object> ptr = std::move(obj);
        ContainedObject entry(id, active_in_profiles, std::move(obj));
        objects.push_back(entry);
        return id;
    }

    bool replace (obj_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) {
        ContainedObject * entry = find(id);
        if(entry != nullptr){
            entry->profiles = active_in_profiles;
            entry->obj = std::move(obj);
            return true;
        }
        return false;
    }

    void remove(obj_id_t id) {
        std::remove_if(objects.begin(), objects.end(), [&id](ContainedObject const& item){ return item.id == id;} );
    }

    void map(std::function<void(ContainedObject & obj)> func) {
        for(auto it : objects){
            func(it);
        }
    }
};

} // end namespace cbox
