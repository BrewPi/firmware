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

#include "stddef.h"
#include "stdint.h"
#include <memory>
#include "DataStream.h"
#include "CboxMixins.h"

namespace cbox {

enum class StreamResult {
    success,
    not_persisted,
    generic_error,
    stream_error,
    not_writable,
};


class id_type_base {
public:
    id_type_base() : id(invalid()){};
    id_type_base(uint16_t v) : id(v){};

    operator uint16_t() const {
        return id;
    }

    bool isValid() const{
        return id > invalid();
    }

    StreamResult streamTo(DataOut & out){
        return out.put(id) ? StreamResult::success : StreamResult::stream_error;
    }

    StreamResult streamFrom(DataIn & in){
        return in.get(id) ? StreamResult::success : StreamResult::stream_error;
    }

    static const id_type_base start() { return 1; };
    static const id_type_base invalid() { return 0; };

protected:
    uint16_t id;
};

class obj_type_t : public id_type_base {
public:
    using id_type_base::id_type_base;

    obj_type_t(id_type_base & id){
        *this = id;
    }
};

class obj_id_t : public id_type_base {
public:
    using id_type_base::id_type_base;

    obj_id_t(const id_type_base id){
        *this = id;
    }

    obj_id_t & operator++() // ++A
    {
        ++id;
        return *this ;
    }

    obj_id_t operator++(int) // A++
    {
       obj_id_t temp = *this ;
       ++id;
       return temp ;
    }
};


class Object : virtual public ObjectMixin
{
public:
	Object() = default;
	virtual ~Object() = default;

	/**
	 * The application defined typeID for this object instance. Defined by derived class
	 */
	virtual obj_type_t typeID() = 0;

	/**
	 * update the object, returns timestamp at which it wants to be updated again (in ms since boot).
	 */
	virtual uint32_t update(uint32_t currentTime) { return 0; };

	/**
	 * Each object is at least stream readable
	 */
	virtual StreamResult streamTo(DataOut& out)=0;

    /**
     * Some objects can be writable from the stream, they override the streamFrom function
     */
    virtual StreamResult streamFrom(DataIn& dataIn){
        return StreamResult::not_writable;
    };

    /**
     * Objects that want to store persisted data override the streamPersistedTo function
     */
    virtual StreamResult streamPersistedTo(DataOut& out){
        return StreamResult::not_persisted;
    }
};


/**
 * An object that does nothing. When read, it returns the type it becomes when it is activated.
 */
class InactiveObject : public Object
{
public:
    InactiveObject(obj_type_t type) : actualType(type){};
    virtual ~InactiveObject() = default;

    virtual obj_type_t typeID() override final;

    virtual StreamResult streamTo(DataOut& out) override final {
        out.write(actualType);
        return StreamResult::success;
    }

    obj_type_t actualType;
};

/**
 * An object that streams as its memory as raw bytes, can be used as base class for simple object wrapping
 */
template<typename T>
class RawStreamObject : public Object
{
public:
    RawStreamObject() : obj({0}){};
    RawStreamObject(T data) : obj(data){};
    virtual ~RawStreamObject() = default;

    virtual StreamResult streamTo(DataOut& out) override final {
        out.put(obj);
        return StreamResult::success;
    }

protected:
    T obj;
};

/**
 * A writable object that streams as its memory as raw bytes
 */
template<typename T>
class RawStreamWritableObject : public RawStreamObject<T>
{
public:
    virtual ~RawStreamWritableObject() = default;

    virtual StreamResult streamFrom(DataIn& in) override final {
        T newValue;
        if(in.get(newValue)){
            this->obj = newValue;
            return StreamResult::success;
        }
        return StreamResult::stream_error;
    }
    virtual StreamResult streamPersistedTo(DataOut& out) override final {
        return RawStreamObject<T>::streamTo(out);
    }
};


template<class T>
std::shared_ptr<Object> createObject(DataIn & defn, StreamResult &streamResult){
    auto obj = std::make_shared<T>();
    streamResult = obj->streamFrom(defn);
    return obj;
}

// An object factory combines the create function with a type ID.
// They can be put in a container that can be walked to find the matching typeId
struct ObjectFactory {
    obj_type_t typeId;
    std::shared_ptr<Object> (*createFn)(DataIn & defn, StreamResult &streamResult);
};


} // end namespace cbox

