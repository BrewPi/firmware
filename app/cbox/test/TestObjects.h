#pragma once

#include "Object.h"
#include "ResolveType.h"
#include <vector>

class LongIntObject : public cbox::RawStreamWritableObject<uint32_t> {
public:
    using cbox::RawStreamWritableObject<uint32_t>::RawStreamWritableObject;

    virtual cbox::obj_type_t typeId() const override final
    {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeId(this);
    }

    bool operator==(const LongIntObject& rhs) const
    {
        return obj == rhs.obj;
    }
};

// variable size object of multiple long ints
class LongIntVectorObject : public cbox::Object {
public:
    LongIntVectorObject()
        : values()
    {
    }
    LongIntVectorObject(std::initializer_list<LongIntObject> l)
        : values(l)
    {
    }

    virtual cbox::obj_type_t typeId() const override final
    {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeId(this);
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        cbox::CboxError res = cbox::CboxError::OK;
        // first write number of elements as uint16_t
        uint16_t size = values.size();
        if (!out.put(size)) {
            return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        for (auto& value : values) {
            res = value.streamTo(out);
            if (res != cbox::CboxError::OK) {
                break;
            }
        }
        return res;
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        cbox::CboxError res = cbox::CboxError::OK;
        uint16_t newSize = 0;
        if (!in.get(newSize)) {
            return cbox::CboxError::INPUT_STREAM_READ_ERROR;
        }
        values.resize(newSize);
        for (auto& value : values) {
            res = value.streamFrom(in);
            if (res != cbox::CboxError::OK) {
                break;
            }
        }
        return res;
    };

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        return cbox::update_t_max();
    }

    bool operator==(const LongIntVectorObject& rhs) const
    {
        return values == rhs.values;
    }

    std::vector<LongIntObject> values;
};

/**
 * A simple class to test:
 * - updating objects at the interval they request
 * - different output, input and persisted streams
 */
class Counter : public cbox::Object {
private:
    uint32_t _interval;
    uint16_t _count1;  // not writable
    uint16_t _count10; // writable and persisted

public:
    Counter()
        : _interval(0)
        , _count1(0)
        , _count10(0)
    {
    }

    uint16_t count1()
    {
        return _count1;
    }
    uint16_t count10()
    {
        return _count1;
    }

    virtual cbox::obj_type_t typeId() const override final
    {
        // use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
        return cbox::resolveTypeId(this);
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        // stream out all values
        if (!out.put(_interval)) {
            return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        if (!out.put(_count1)) {
            return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        if (!out.put(_count10)) {
            return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        return cbox::CboxError::OK;
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        uint32_t newInterval;
        uint16_t newCount10;

        if (!in.get(newInterval)) {
            return cbox::CboxError::INPUT_STREAM_READ_ERROR;
        }
        if (!in.get(newCount10)) {
            return cbox::CboxError::INPUT_STREAM_READ_ERROR;
        }

        _interval = newInterval;
        _count10 = newCount10;

        return cbox::CboxError::OK;
    };

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        if (!out.put(_interval)) {
            return cbox::CboxError::PERSISTED_STORAGE_WRITE_ERROR;
        }
        if (!out.put(_count10)) {
            return cbox::CboxError::PERSISTED_STORAGE_WRITE_ERROR;
        }
        return cbox::CboxError::OK;
    }
};
