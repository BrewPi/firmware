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
#include "EepromAccess.h"
#include "CboxMixins.h"
#include "ResolveType.h"

namespace cbox {

class object_id_t {
public:
    object_id_t() : id(0){};
    object_id_t(uint16_t v) : id(v){};

    operator uint16_t() const {
        return id;
    }

    bool isValid() const{
        return id > 0;
    }

    object_id_t & operator++() // ++A
    {
        ++id;
        return *this ;
    }

    object_id_t operator++(int) // A++
    {
       object_id_t temp = *this ;
       ++id;
       return temp ;
    }

    static object_id_t startId() { return 1; };

    uint16_t id;
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
	 * update the object, returns time until it wants to be updated again (in ms).
	 */
	virtual uint32_t update() { return UINT32_MAX; };

	/**
	 * Each object is at least stream readable
	 */
    enum class StreamToResult {
        generic_error,
        stream_error,
        success,
    };

	virtual StreamToResult streamTo(DataOut& out)=0;
    virtual stream_size_t streamToMaxSize()=0; // the max size this value will output to the stream.


    /**
     * Some objects can be writable from the stream, they override the functions below
     */
    enum class StreamFromResult {
        generic_error,
        stream_error,
        not_writable,
        success_dont_persist,
        success_persist,
    };
    virtual StreamFromResult streamFrom(DataIn& dataIn){ return StreamFromResult::not_writable; };
    virtual stream_size_t streamFromMaxSize(){ return 0; }; // the max size this object expects from the stream.

    // default streaming persisted data to streamTo. Can be overridden by objects that do not persist all of their data
    virtual StreamToResult streamPersistedTo(DataOut& out){
        return streamTo(out);
    }
    virtual stream_size_t persistedMaxSize(){
        return streamToMaxSize();
    }
};


/**
 * A base writable object type, which use the input stream to change its value
 */
class WritableObject : public Object {
public:
	virtual StreamFromResult streamFrom(DataIn& dataIn) override = 0;
	virtual stream_size_t streamFromMaxSize() override { return streamToMaxSize(); };
};

/**
 * An object that does nothing. When read, it returns the type it becomes when it is activated.
 */
class InactiveObject : public Object
{
public:
    InactiveObject(obj_type_t type) : actualType(type){};
    virtual ~InactiveObject() = default;

    virtual obj_type_t typeID() override final {
        return resolveTypeID<InactiveObject>();
    }

    virtual Object::StreamToResult streamTo(DataOut& out) override final {
        out.write(actualType);
        return Object::StreamToResult::success;
    }
    virtual stream_size_t streamToMaxSize() override final { return sizeof(object_id_t); };

    obj_type_t actualType;
};

/**
 * An object that streams as its memory as raw bytes
 */
template<typename T>
class RawStreamObject : public Object
{
public:
    RawStreamObject(T data) : t(data){};
    virtual ~RawStreamObject() = default;

    virtual obj_type_t typeID() override final {
        return resolveTypeID<RawStreamObject<T>>();
    }


    virtual void streamTo(DataOut& out) override final {
        out.put(t);
    }

    virtual stream_size_t streamToMaxSize() override final {
        return sizeof(T);
    }

    T t;
};

/**
 * A writable object that streams as its memory as raw bytes
 */
template<typename T>
class RawStreamWritableObject : public WritableObject
{
public:
    RawStreamWritableObject(T data) : t(data){};
    virtual ~RawStreamWritableObject() = default;

    virtual obj_type_t typeID() override final {
        return resolveTypeID<RawStreamWritableObject<T>>();
    }


    virtual StreamFromResult streamFrom(DataIn& in) override final {
        T newValue;
        if(in.get(t)){
            t = newValue;
            return StreamFromResult::success_persist;
        }
        return StreamFromResult::stream_error;
    }

    virtual stream_size_t streamToMaxSize() override final {
        return sizeof(T);
    }

    T t;
};


template<class T>
std::shared_ptr<Object> createObject(DataIn & defn, Object::StreamFromResult &streamResult){
    auto obj = std::make_shared<T>();
    streamResult = obj->streamFrom(defn);
    return obj;
}

} // end namespace cbox
