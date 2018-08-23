#pragma once

#include "Object.h"
#include "ResolveType.h"
#include <vector>

class LongIntObject : public cbox::RawStreamWritableObject<uint32_t> {
public:
    using cbox::RawStreamWritableObject<uint32_t>::RawStreamWritableObject;

    virtual cbox::obj_type_t typeId() const override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeId(this);
    }

    bool operator==(const LongIntObject & rhs) const {
        return obj == rhs.obj;
    }
};

// variable size object of multiple long ints
class LongIntVectorObject : public cbox::Object
{
public:
    LongIntVectorObject() : values(){}
    LongIntVectorObject(std::initializer_list<LongIntObject> l) : values(l){}

    virtual cbox::obj_type_t typeId() const override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeId(this);
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final {
        cbox::CboxError res = cbox::CboxError::no_error;
        // first write number of elements as uint16_t
        uint16_t size = values.size();
        if(!out.put(size)){
            return cbox::CboxError::output_stream_write_error;
        }
        for(auto & value : values){
            res = value.streamTo(out);
            if(res != cbox::CboxError::no_error){
                break;
            }
        }
        return res;
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final {
        cbox::CboxError res = cbox::CboxError::no_error;
        uint16_t newSize = 0;
        if(!in.get(newSize)){
            return cbox::CboxError::input_stream_read_error;
        }
        values.resize(newSize);
        for(auto & value : values){
            res = value.streamFrom(in);
            if(res != cbox::CboxError::no_error){
                break;
            }
        }
        return res;
    };

    bool operator==(const LongIntVectorObject & rhs) const {
        return values == rhs.values;
    }

    std::vector<LongIntObject> values;
};
