#pragma once

#include "Object.h"
#include "ResolveType.h"
#include <vector>

class LongIntObject : public cbox::RawStreamWritableObject<uint32_t> {
public:
    using cbox::RawStreamWritableObject<uint32_t>::RawStreamWritableObject;

    virtual cbox::obj_type_t typeID() override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeID(this);
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

    virtual cbox::obj_type_t typeID() override final {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeID(this);
    }

    virtual cbox::StreamResult streamTo(cbox::DataOut& out) override final {
        cbox::StreamResult res = cbox::StreamResult::success;
        if(!out.put(values.size())){ // first write number of elements
            return cbox::StreamResult::stream_error;
        }
        for(auto & value : values){
            res = value.streamTo(out);
            if(res != cbox::StreamResult::success){
                break;
            }
        }
        return res;
    }

    virtual cbox::StreamResult streamFrom(cbox::DataIn& in) override final {
        cbox::StreamResult res = cbox::StreamResult::success;
        decltype(values)::size_type newSize = 0;
        if(!in.get(newSize)){
            return cbox::StreamResult::stream_error;
        }
        values.resize(newSize);
        for(auto & value : values){
            res = value.streamFrom(in);
            if(res != cbox::StreamResult::success){
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
