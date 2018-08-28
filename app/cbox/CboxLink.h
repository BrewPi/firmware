#pragma once

#include "Interface.h"
#include "ObjectIds.h"
#include "RefTo.h"

// extern cbox::ObjectContainer objects;

class CboxLookup : public BaseLookup {
public:
    CboxLookup() {}
    CboxLookup(cbox::obj_id_t _id)
        : id(_id)
    {
    }
    virtual ~CboxLookup() = default;

    operator uint16_t() { return id; }

    void set(uint16_t newId)
    {
        id = newId;
    }

    uint16_t get() const
    {
        return uint16_t(id);
    }

    virtual Interface* operator()() const override final
    {
        /*if (auto obj = brewbloxBox().getObject(id).lock()) {
            return obj->getApplicationInterface();
        }*/

        return nullptr; // TODO return weak_ptr in all cases
    }

private:
    cbox::obj_id_t id;
};
