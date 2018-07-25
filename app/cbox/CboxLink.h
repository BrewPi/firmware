#pragma once

#include "RefTo.h"
#include "Interface.h"
#include "Integration.h"
#include <cstring>
#include "Object.h"


class CboxLookup : public BaseLookup {
public:
    CboxLookup(){}
    CboxLookup(cbox::obj_id_t _id) : id(_id) {}
    virtual ~CboxLookup() = default;

    operator uint16_t(){ return id; }

    virtual Interface * operator()() const override final {
        auto obj = cbox::systemContainer().fetch(id);

        return (obj) ? obj->getApplicationInterface() : nullptr;
    }

private:
    cbox::obj_id_t id;
};
