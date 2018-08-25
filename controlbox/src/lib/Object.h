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

#include "CboxError.h"
#include "CboxMixins.h"
#include "DataStream.h"
#include <cstdint>
#include <memory>

namespace cbox {

class obj_type_t {
public:
    obj_type_t()
        : id(0){};
    obj_type_t(const uint16_t& rhs)
        : id(rhs){};
    obj_type_t& operator=(const uint16_t& rhs)
    {
        id = rhs;
        return *this;
    }

    operator uint16_t() const
    {
        return id;
    }

    bool isValid() const
    {
        return id > invalid().id;
    }

    static const obj_type_t start()
    {
        return obj_type_t(1);
    };
    static const obj_type_t invalid()
    {
        return obj_type_t(0);
    };

private:
    uint16_t id;
};

class obj_id_t {
public:
    obj_id_t()
        : id(0){};
    obj_id_t(const uint16_t& rhs)
        : id(rhs){};
    obj_id_t& operator=(const uint16_t& rhs)
    {
        id = rhs;
        return *this;
    }

    operator uint16_t() const
    {
        return id;
    }

    bool isValid() const
    {
        return id > invalid().id;
    }

    static const obj_id_t start()
    {
        return obj_id_t(1);
    };
    static const obj_id_t invalid()
    {
        return obj_id_t(0);
    };

    obj_id_t& operator++() // ++A
    {
        ++id;
        return *this;
    }

    obj_id_t operator++(int) // A++
    {
        obj_id_t temp = *this;
        ++id;
        return temp;
    }

private:
    uint16_t id;
};

class Object : virtual public ObjectMixin {
public:
    Object() = default;
    virtual ~Object() = default;

    /**
	 * The application defined typeID for this object instance. Defined by derived class
	 */
    virtual obj_type_t typeId() const = 0;

    /**
	 * update the object, returns timestamp at which it wants to be updated again (in ms since boot).
	 */
    virtual uint32_t update(const uint32_t& currentTime) { return 0; };

    /**
	 * Each object is at least stream readable
	 */
    virtual CboxError streamTo(DataOut& out) const = 0;

    /**
     * Some objects can be writable from the stream, they override the streamFrom function
     */
    virtual CboxError streamFrom(DataIn& in)
    {
        return CboxError::OBJECT_NOT_WRITABLE;
    };

    /**
     * Default to persisting the same data as streamTo. Objects that only want to persist some data can override this
     */
    virtual CboxError streamPersistedTo(DataOut& out) const
    {
        return streamTo(out);
    }
};

/**
 * An object that does nothing. When read, it returns the type it becomes when it is activated.
 */
class InactiveObject : public Object {
public:
    InactiveObject(obj_type_t type)
        : actualType(type){};
    virtual ~InactiveObject() = default;

    virtual obj_type_t typeId() const override final;

    virtual CboxError streamTo(DataOut& out) const override final
    {
        out.put(actualType);
        return CboxError::OK;
    }

    virtual CboxError streamPersistedTo(DataOut& out) const override final
    {
        return CboxError::OK; // inactive objects are never persisted
    }

    obj_type_t actualTypeId()
    {
        return actualType;
    }

    obj_type_t actualType;
};

/**
 * An object that streams as its memory as raw bytes, can be used as base class for simple object wrapping
 */
template <typename T>
class RawStreamObject : public Object {
public:
    RawStreamObject()
        : obj(T()){};
    RawStreamObject(T data)
        : obj(data){};
    virtual ~RawStreamObject() = default;

    virtual CboxError streamTo(DataOut& out) const override final
    {
        if (out.put(obj)) {
            return CboxError::OK;
        }
        return CboxError::OUTPUT_STREAM_WRITE_ERROR;
    }

    operator T()
    {
        T copy = obj;
        return copy;
    }

protected:
    T obj;
};

/**
 * A writable object that streams as its memory as raw bytes
 */
template <typename T>
class RawStreamWritableObject : public RawStreamObject<T> {
public:
    using RawStreamObject<T>::RawStreamObject;

    virtual ~RawStreamWritableObject() = default;

    virtual CboxError streamFrom(DataIn& in) override final
    {
        T newValue;
        if (in.get(newValue)) {
            this->obj = newValue;
            return CboxError::OK;
        }
        return CboxError::INPUT_STREAM_READ_ERROR;
    }
    virtual CboxError streamPersistedTo(DataOut& out) const override final
    {
        return RawStreamObject<T>::streamTo(out);
    }
};

} // end namespace cbox
