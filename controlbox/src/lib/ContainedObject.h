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

#include "DataStream.h"
#include "InactiveObject.h"
#include "Object.h"
#include "ResolveType.h"
#include <limits>
#include <memory>

namespace cbox {

/**
 * A wrapper around an object that stores which type it is and in which profiles it is active
 */
class ContainedObject {
public:
    explicit ContainedObject(obj_id_t id, uint8_t profiles, std::shared_ptr<Object> obj)
        : _id(std::move(id))
        , _profiles(std::move(profiles))
        , _obj(std::move(obj))
        , _nextUpdateTime(0){};

private:
    obj_id_t _id;                 // unique id of object
    uint8_t _profiles;            // active in these profiles
    std::shared_ptr<Object> _obj; // pointer to runtime object
    update_t _nextUpdateTime;     // next time update should be called on _obj

public:
    const obj_id_t& id() const
    {
        return _id;
    }

    const uint8_t& profiles() const
    {
        return _profiles;
    }

    const std::shared_ptr<Object>& object() const
    {
        return _obj;
    }

    void deactivate()
    {
        obj_type_t oldType = _obj->typeId();
        _obj = std::make_shared<InactiveObject>(oldType);
    }

    void update(const update_t& now)
    {
        const update_t overflowGuard = std::numeric_limits<update_t>::max() / 2;
        if (overflowGuard - now + _nextUpdateTime <= overflowGuard) {
            _nextUpdateTime = _obj->update(now);
        }
    }

    void forcedUpdate(const uint32_t& now)
    {
        _nextUpdateTime = _obj->update(now);
    }

    CboxError streamTo(DataOut& out) const
    {
        if (!out.put(_id)) {
            return CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        if (!out.put(_profiles)) {
            return CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        if (!out.put(_obj->typeId())) {
            return CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        return _obj->streamTo(out);
    }

    CboxError streamFrom(DataIn& in)
    {
        // id is not streamed in. It is immutable and assumed to be already read to find this entry

        uint8_t newProfiles;
        obj_type_t expectedType;
        if (!in.get(newProfiles)) {
            return CboxError::INPUT_STREAM_READ_ERROR;
        }
        if (!in.get(expectedType)) {
            return CboxError::INPUT_STREAM_READ_ERROR;
        }

        if (expectedType == _obj->typeId()) {
            _profiles = newProfiles;
            return _obj->streamFrom(in);
        }
        return CboxError::INVALID_OBJECT_TYPE;
    }

    CboxError streamPersistedTo(DataOut& out) const
    {
        // id is not streamed out. It is passed to storage separately
        if (_obj->typeId() == resolveTypeId<InactiveObject>()) {
            return CboxError::OK; // don't persist inactive objects
        }
        if (!out.put(_profiles)) {
            return CboxError::PERSISTED_STORAGE_WRITE_ERROR;
        }
        if (!out.put(_obj->typeId())) {
            return CboxError::PERSISTED_STORAGE_WRITE_ERROR;
        }
        return _obj->streamPersistedTo(out);
    }
};
}
