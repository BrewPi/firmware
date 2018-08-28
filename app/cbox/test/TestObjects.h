#pragma once

#include "ObjectBase.h"
#include <vector>

class LongIntObject : public cbox::ObjectBase<LongIntObject> {
private:
    uint32_t obj;

public:
    LongIntObject()
        : obj(0){};
    LongIntObject(uint32_t rhs)
        : obj(std::move(rhs)){};
    virtual ~LongIntObject() = default;

    bool operator==(const LongIntObject& rhs) const
    {
        return obj == rhs.obj;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        if (out.put(obj)) {
            return cbox::CboxError::OK;
        }
        return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        uint32_t newValue;
        if (in.get(newValue)) {
            obj = newValue;
            return cbox::CboxError::OK;
        }
        return cbox::CboxError::INPUT_STREAM_READ_ERROR;
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override
    {
        return cbox::Object::update_never(now);
    }

    operator uint32_t()
    {
        uint32_t copy = obj;
        return copy;
    }
};

// variable size object of multiple long ints
class LongIntVectorObject : public cbox::ObjectBase<LongIntVectorObject> {
public:
    LongIntVectorObject()
        : values()
    {
    }
    LongIntVectorObject(std::initializer_list<LongIntObject> l)
        : values(l)
    {
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
        return cbox::Object::update_never(now);
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
class UpdateCounter : public cbox::ObjectBase<UpdateCounter> {
private:
    uint16_t _interval; // writable and persisted
    uint16_t _count;    // not writable

public:
    UpdateCounter()
        : _interval(1000)
        , _count(0)
    {
    }
    virtual ~UpdateCounter() = default;

    uint16_t count()
    {
        return _count;
    }

    uint16_t interval()
    {
        return _interval;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        // stream out all values
        if (!out.put(_interval)) {
            return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }
        if (!out.put(_count)) {
            return cbox::CboxError::OUTPUT_STREAM_WRITE_ERROR;
        }

        return cbox::CboxError::OK;
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        uint16_t newInterval;

        if (!in.get(newInterval)) {
            return cbox::CboxError::INPUT_STREAM_READ_ERROR;
        }
        if (newInterval < 10) {
            return cbox::CboxError::OBJECT_DATA_NOT_ACCEPTED;
        }

        _interval = newInterval;

        return cbox::CboxError::OK;
    };

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        if (!out.put(_interval)) {
            return cbox::CboxError::PERSISTED_STORAGE_WRITE_ERROR;
        }

        return cbox::CboxError::OK;
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        ++_count;
        return now + _interval;
    }
};
