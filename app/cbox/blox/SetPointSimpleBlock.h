#pragma once

#include "Block.h"
#include "SetPoint.h"
#include "SetPointSimple.pb.h"

class SetPointSimpleBlock : public Block<SetPointSimpleBlock> {
private:
    SetPointSimple setpoint;

public:
    SetPointSimpleBlock()
        : setpoint()
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_SetPointSimple newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_SetPointSimple_fields, blox_SetPointSimple_size);
        if (result == cbox::CboxError::OK) {
            setpoint.write(temp_t::raw(newData.setting));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_SetPointSimple message;
        message.setting = setpoint.read().getRaw();

        return streamProtoTo(out, &message, blox_SetPointSimple_fields, blox_SetPointSimple_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        return update_never(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == cbox::resolveTypeId(this)) {
            return this; // me!
        }
        if (iface == cbox::resolveTypeId<SetPoint>()) {
            // return the member that implements the interface in this case
            SetPoint* ptr = &setpoint;
            return ptr;
        }
        return nullptr;
    }

    SetPointSimple& get()
    {
        return setpoint;
    }
};
