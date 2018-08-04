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
    not_writable,
    stream_error,
    unexpected_end_of_input,
    unexpected_end_of_output,
    end_of_input,
    end_of_output,
};


class id_type_base {
public:
    id_type_base() : id(0){};
    id_type_base(uint16_t v) : id(v){};
    id_type_base(const id_type_base & v){
        id = v.id;
    }

    operator uint16_t() const {
        return id;
    }

    StreamResult streamTo(DataOut & out){
        return out.put(id) ? StreamResult::success : StreamResult::stream_error;
    }

    StreamResult streamFrom(DataIn & in){
        return in.get(id) ? StreamResult::success : StreamResult::stream_error;
    }

protected:
    uint16_t id;
};

class obj_type_t : public id_type_base {
public:
    using id_type_base::id_type_base;
    obj_type_t(){
        id = invalid_id;
    }

    bool isValid() const{
        return id > invalid_id;
    }

    static const obj_type_t start() {
        obj_type_t retv(start_id);
        return retv;
    };
    static const obj_type_t invalid() {
        obj_type_t retv(invalid_id);
        return retv;
    };

private:
    static const uint16_t start_id = 1;
    static const uint16_t invalid_id = 0;
};

class obj_id_t : public id_type_base {
public:
    using id_type_base::id_type_base;
    obj_id_t(){
        id = invalid_id;
    }

    bool isValid() const{
        return id > invalid_id;
    }

    static const obj_id_t start() {
        obj_id_t retv(start_id);
        return retv;
    };
    
    static const obj_id_t invalid() {
        obj_id_t retv(invalid_id);
        return retv;
    };

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

private:
    static const uint16_t start_id = 1;
    static const uint16_t invalid_id = 0;
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
	virtual StreamResult streamTo(DataOut& out) = 0;

    /**
     * Some objects can be writable from the stream, they override the streamFrom function
     */
    virtual StreamResult streamFrom(DataIn& in){
        return StreamResult::not_writable;
    };

    /**
     * Default to persisting the same data as streamTo. Objects that only want to persist some data can override this
     */
    virtual StreamResult streamPersistedTo(DataOut& out) {
        return streamTo(out);
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
    RawStreamObject() : obj(T()){};
    RawStreamObject(T data) : obj(data){};
    virtual ~RawStreamObject() = default;

    virtual StreamResult streamTo(DataOut& out) override final {
        out.put(obj);
        return StreamResult::success;
    }

    operator T(){
        T copy = obj;
        return copy;
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
    using RawStreamObject<T>::RawStreamObject;

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
std::shared_ptr<Object> createObject(DataIn & in, StreamResult &streamResult){
    auto obj = std::make_shared<T>();
    streamResult = obj->streamFrom(in);
    return obj;
}

// An object factory combines the create function with a type ID.
// They can be put in a container that can be walked to find the matching typeId
struct ObjectFactory {
    obj_type_t typeId;
    std::shared_ptr<Object> (*createFn)(DataIn & defn, StreamResult &streamResult);
};


} // end namespace cbox

