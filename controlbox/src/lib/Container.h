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
#include "ObjectFactory.h"

namespace cbox {
/**
 * A wrapper around an object that stores which type it is and in which profiles it is active
 */
class ContainedObject : public Object {
public:
    explicit ContainedObject(obj_id_t id, uint8_t profiles, std::shared_ptr<Object> obj) :
        _id(std::move(id)),
        _profiles(std::move(profiles)),
        _obj(std::move(obj)){};

private:
    obj_id_t _id;
    uint8_t _profiles; // active in these profiles
    std::shared_ptr<Object> _obj; // pointer to runtime object

public:    
    const obj_id_t & id() const {
        return _id;
    }

    const uint8_t & profiles() const {
        return _profiles;
    }

    const std::shared_ptr<Object>& object() const {
        return _obj;
    }

    virtual obj_type_t typeId() const override final {
        return resolveTypeId<ContainedObject>();
    }

    virtual CboxError streamTo(DataOut & out) const override final {
        if(!out.put(_id)){
            return CboxError::output_stream_write_error;
        }
        if(!out.put(_profiles)){
            return CboxError::output_stream_write_error;
        }
        if(!out.put(_obj->typeId())){
            return CboxError::output_stream_write_error;
        }
        return _obj->streamTo(out);
    }

    virtual CboxError streamFrom(DataIn & in) override final {
        // id is not streamed in. It is immutable and assumed to be already read to find this entry

        uint8_t newProfiles;
        obj_type_t expectedType;
        if(!in.get(newProfiles)){
            return CboxError::input_stream_read_error;
        }
        if(!in.get(expectedType)){
            return CboxError::input_stream_read_error;
        }

        if(expectedType == _obj->typeId()){
            _profiles = newProfiles;
            return _obj->streamFrom(in);
        }
        return CboxError::invalid_object_type;
    }

    virtual CboxError streamPersistedTo(DataOut& out) const override final{
        // id is not streamed out. It is passed to storage separately
        if(_obj->typeId() == resolveTypeId<InactiveObject>()){
            return CboxError::no_error; // don't persist inactive objects
        }
        if(!out.put(_profiles)){
            return CboxError::persisted_storage_write_error;
        }
        if(!out.put(_obj->typeId())){
            return CboxError::persisted_storage_write_error;
        }
        return _obj->streamPersistedTo(out);
    }
};

class ObjectContainer
{
private:
    std::vector<ContainedObject> objects;
    obj_id_t startId;

public:

    using Iterator = decltype(objects)::iterator;
    using CIterator = decltype(objects)::const_iterator;

    ObjectContainer() :
        objects(), 
        startId(obj_id_t::start())
    {}

    ObjectContainer(std::initializer_list<ContainedObject> systemObjects) : 
        objects(systemObjects),
        startId(obj_id_t::start())
    {
        setObjectsStartId(nextId()); // set startId to next free ID to lock system objects
    }
    virtual ~ObjectContainer() = default;


private:
    auto findPosition(const obj_id_t & id) {
        // equal_range is used instead of find, because it is faster for a sorted container
        // the returned pair can be used as follows:
        // first == second means not found, first points to the insert position for the new object id
        // first != second means the object is found and first points to it

        struct IdLess
        {
            bool operator() ( const ContainedObject& c, const obj_id_t & i) const { return c.id() < i; }
            bool operator() ( const obj_id_t & i, const ContainedObject& c) const { return i < c.id(); }
        };

        auto pair = std::equal_range(
            objects.begin(),
            objects.end(),
            id,
            IdLess{}
        );
        return pair;
    }

public:
    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */
    ContainedObject * fetchContained(const obj_id_t id){
        auto p = findPosition(id);
        if(p.first == p.second){
            return nullptr;
        }
        else{
            return &(*p.first);
        }
    }
    
    const std::weak_ptr<Object> fetch(const obj_id_t id) {
        auto p = findPosition(id);
        if(p.first == p.second){
			return std::weak_ptr<Object>(); // empty weak ptr if not found
        }
		return p.first->object(); // weak_ptr to found object
    }

    obj_id_t nextId() const{
        return std::max(startId, objects.empty() ? startId : ++obj_id_t(objects.back().id()));
    }

    /**
     * set start ID for user objects.
     * ID's smaller than the start ID are  assumed to be system objects and considered undeletable.
     **/
    void setObjectsStartId(const obj_id_t & id){
        startId = id;
    }

    obj_id_t add (std::unique_ptr<Object> obj, const uint8_t active_in_profiles) {        
        return add(std::move(obj), active_in_profiles, obj_id_t::invalid());
    }

    obj_id_t add (std::unique_ptr<Object> obj, const uint8_t active_in_profiles, const obj_id_t & id) {
        obj_id_t newId;
        Iterator insertPosition;

        if(id == obj_id_t::invalid()){ // use 0 to let the container assign a free slot
            newId = nextId();
            insertPosition = objects.end();
        }
        else {
			if(id < startId){
				return obj_id_t::invalid(); // refuse to overwrite system objects
			}
        	// find insert position
			auto p = findPosition(id);
			if(p.first != p.second){
				return obj_id_t::invalid(); // refuse to overwrite existing objects
			}
        	newId = id;
        	insertPosition = p.first;
        }

        // insert new entry in container in sorted position
        objects.emplace(insertPosition, newId, active_in_profiles, std::move(obj));
        return newId;
    }


    // create contained object directly from DataIn stream defining it
    CboxError addFromStream(DataIn & in, const ObjectFactory & factory, obj_id_t & newId){
        auto id = obj_id_t::invalid();
        auto typeId = obj_type_t::invalid();
        uint8_t profiles = 0x00;

        if(!in.get(id)){
            return CboxError::input_stream_read_error;
        }
        if(!in.get(profiles)){
            return CboxError::input_stream_read_error;
        }
        if(!in.get(typeId)){
            return CboxError::input_stream_read_error;
        }

        std::unique_ptr<Object> obj;
        auto status = factory.make(typeId, obj);
        if(!obj){
            return status;
        }
        status = obj->streamFrom(in);
        if(status == CboxError::no_error){
            newId = add(std::move(obj), profiles, id);
        }
        return status;
    }

    obj_id_t replace (std::unique_ptr<Object> obj, const uint8_t active_in_profiles, const obj_id_t id) {
        if(id < startId){
            return obj_id_t::invalid(); // refuse to replace system objects
        }
        // find existing object
        auto p = findPosition(id);

        if(p.first != p.second){
            // replace object with newly constructed object
            *p.first = decltype(objects)::value_type(id, active_in_profiles, std::move(obj));
            return id;
        }
        return obj_id_t::invalid();
    }

    CboxError remove(obj_id_t id) {
        if(id < startId){
            return CboxError::object_not_deletable; // refuse to remove system objects
        }
        // find existing object
        auto p = findPosition(id);
        objects.erase(p.first, p.second); // doesn't remove anything if no objects found (first == second)
        return p.first == p.second ? CboxError::invalid_object_id : CboxError::no_error;
    }

    // only const iterators are exposed. We don't want the caller to be able to modify the container
    CIterator cbegin(){
        return objects.cbegin();
    }

    CIterator cend(){
        return objects.cend();
    }
};

} // end namespace cbox
