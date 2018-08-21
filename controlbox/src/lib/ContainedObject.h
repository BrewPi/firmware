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


#include "Object.h"
#include "ResolveType.h"

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

}
