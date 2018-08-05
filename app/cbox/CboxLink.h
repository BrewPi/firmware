#pragma once

#include "RefTo.h"
#include "Interface.h"
#include "Container.h"

extern cbox::ObjectContainer objects;

class CboxLookup : public BaseLookup {
public:
    CboxLookup(){}
    CboxLookup(cbox::obj_id_t _id) : id(_id) {}
    virtual ~CboxLookup() = default;

    operator uint16_t(){ return id; }

    virtual Interface * operator()() const override final {
        auto obj = objects.fetch(id);

        return (obj) ? obj->getApplicationInterface() : nullptr;
    }

private:
    cbox::obj_id_t id;
};
