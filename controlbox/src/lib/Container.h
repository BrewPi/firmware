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
/**
 * A wrapper around an object that stores which type it is and in which profiles it is active
 */
class ContainedObject {
public:
    explicit ContainedObject(obj_id_t _id, uint8_t _profiles, std::shared_ptr<Object> _obj) :
        id(_id),
        profiles(_profiles),
        obj(_obj){};

    ContainedObject(const ContainedObject&) = delete; // not copyable
    ContainedObject& operator=(const ContainedObject&) = delete;

    ContainedObject(const ContainedObject&&) = default; // but movable
    ContainedObject& operator=(const ContainedObject&&) = default;

private:
    obj_id_t id;
    uint8_t profiles; // active in these profiles
    std::shared_ptr<Object> obj; // pointer to runtime object

public:    
    const obj_id_t& id(){
        return id;
    }

    const uint8_t& profiles(){
        return profiles;
    }

    std::shared_ptr<Object> obj & object(){
        return obj;
    }

    StreamResult streamTo(DataOut & out){
        bool success = true;
        success &= out.put(id);
        success &= out.put(profiles);
        success &= out.put(obj->typeID());
        if(success){
            return obj->streamTo(out);
        }
        else{
            return StreamResult::stream_error;
        }
    }

    StreamResult streamFrom(DataIn & in){
        bool success = true;
        // id is not streamed in. It is immutable and assumed to be already read to find this entry

        uint8_t newProfiles = profiles;
        obj_type_t expectedType = 0;
        success &= in.get(newProfiles);
        success &= in.get(expectedType);

        if(success){
            if(expectedType == obj->typeID()){
                profiles = newProfiles;
                return obj->streamFrom(in);
            }
            return StreamResult::type_mismatch;
        }
        else{
            return StreamResult::stream_error;
        }
    }

    StreamResult streamPersistedTo(DataOut& out){
        if(obj->typeID() == resolveTypeID<InactiveObject>()){
            return StreamResult::not_persisted; // don't persist inactive objects
        }
        bool success = true;
        success &= out.put(profiles);
        success &= out.put(obj->typeID());
        if(success){
            return obj->streamPersistedTo(out);
        }
        else{
            return StreamResult::stream_error;
        }
    }
};

class ObjectContainer
{
public:
    ObjectContainer() :
        objects(), 
        startId(obj_id_t::start()), 
        nextId(obj_id_t::start()) 
    {}

    ObjectContainer(std::initializer_list<ContainedObject> systemObjects) : 
        objects(systemObjects),
        startId(obj_id_t::start()),
        nextId(obj_id_t::start())
    {
        setObjectsStartId(freeId()); // set startId to next free ID to lock system objects
        nextId = startId; // Reset because we're not using the free ID we just got.
    }
    virtual ~ObjectContainer() = default;

private:
    std::vector<ContainedObject> objects;
    obj_id_t startId;
    obj_id_t nextId;

    decltype(objects)::iterator findContainedObject(const obj_id_t & id) {
        return std::find_if(
            objects.begin(),
            objects.end(),
            [&id](const ContainedObject& item){ return item.id == id;} 
        );
    }

public:
    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */
    ContainedObject * fetchContainedObject(const obj_id_t id){
        decltype(objects)::iterator found = findContainedObject(id);
        if(found == objects.end()){
            return nullptr;
        }
        else{
            return &(*found);
        }
    }
    
    std::shared_ptr<Object> fetch(const obj_id_t id) {
        std::shared_ptr<Object> ptr;
        decltype(objects)::iterator entry = findContainedObject(id);
        if(entry != objects.end()){
            ptr = entry->obj;
        }
        return ptr;
    }

    obj_id_t freeId(){
        while(nextId < startId || findContainedObject(nextId) != objects.end()){
            nextId++;
        }
        return nextId++;
    }

    /**
     * set start ID for user objects.
     * ID's smaller than the start ID are  assumed to be system objects and considered undeletable.
     **/
    void setObjectsStartId(obj_id_t id){
        startId = id;
    }

    obj_id_t add (std::unique_ptr<Object> obj, const uint8_t active_in_profiles) {        
        return add(std::move(obj), active_in_profiles, obj_id_t::invalid());
    }

    obj_id_t add (std::unique_ptr<Object> obj, const uint8_t active_in_profiles, const obj_id_t id) {
        obj_id_t newId = id;
        if(newId == obj_id_t::invalid()){ // use 0 to let the container assign a free slot
            newId = freeId();
        }
        else { // check if the id already exists
            if(findContainedObject(newId) != objects.end() || newId < startId){
                return obj_id_t::invalid(); // refuse to overwrite existing objects or in ID range for system
            }
        }
        objects.emplace_back(newId, active_in_profiles, std::move(obj)); // add entry to container
        return newId;
    }

    obj_id_t replace (std::unique_ptr<Object> obj, const uint8_t active_in_profiles, const obj_id_t id) {
        if(id < startId){
            return obj_id_t::invalid(); // refuse to replace system objects
        }
        decltype(objects)::iterator it = findContainedObject(id);
        if(it != objects.end()){
            it->profiles = active_in_profiles;
            it->obj = std::move(obj);
            return id;
        }
        return obj_id_t::invalid();
    }

    bool remove(obj_id_t id) {
        if(id < startId){
            return false;
        }
        decltype(objects)::iterator it = findContainedObject(id);
        if(it != objects.end()){
            objects.erase(it);
            return true;
        }
        return false;
    }

    void map(std::function<void(ContainedObject & obj)> func) {
        for(auto it : objects){
            func(it);
        }
    }

    void mapWhileTrue(std::function<bool(ContainedObject & obj)> func) {
        for(auto it : objects){
            if(!func(it)){
                break;
            }
        }
    }

};

} // end namespace cbox
