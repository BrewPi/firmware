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
        if(auto obj = objects.fetch(id).lock()){
            return obj->getApplicationInterface();
        }

        return nullptr; // TODO return weak_ptr in all cases
    }

private:
    cbox::obj_id_t id;
};
