/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
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
    object_id_t() : id(0){};
    object_id_t(uint16_t v) : id(v){};
    bool isValid(){
        return id > 0;
    }
    static object_id_t startId(){ return 1; };
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
    virtual uint8_t streamToSizeMax()=0; // the max size this value will output to the stream.

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
    virtual uint8_t streamFromSizeMax(){ return 0; }; // the max size this object expects from the stream.
};


/**
 * A base writable object type, which use the input stream to change its value
 */
class WritableObject : public Object {
public:
	virtual StreamFromResult writeFrom(DataIn& dataIn) = 0;
	virtual uint8_t writeStreamSize() = 0;
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

    virtual Object::StreamToResult streamTo(DataOut& out) {
        out.write(actualType);
        return Object::StreamToResult::success;
    }
    virtual uint8_t streamToSizeMax() override final { return 0; };

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

    virtual uint8_t streamToSizeMax() override final {
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

    virtual uint8_t streamToSizeMax() override final {
        return sizeof(T);
    }

    static std::shared_ptr<Object> create(DataIn & defn){
        auto obj = std::make_shared<T>();
        if(obj->streamFrom(defn)){
            return obj;
        }
        return nullptr;
    }

    T t;
};

} // end namespace cbox
