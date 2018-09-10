#pragma once

#include "Setpoint.h"
#include "Temperature.h"
#include "blox/Block.h"
#include "proto/cpp/SetpointSimple.pb.h"

class SetpointSimpleBlock : public Block<blox_SetpointSimple_msgid> {
private:
    SetpointSimple setpoint;

public:
    SetpointSimpleBlock()
        : setpoint()
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_SetpointSimple newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_SetpointSimple_fields, blox_SetpointSimple_size);
        if (result == cbox::CboxError::OK) {
            setpoint.write(temp_t_from_base(newData.setting));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_SetpointSimple message;
        message.setting = to_base(setpoint.read());

        return streamProtoTo(out, &message, blox_SetpointSimple_fields, blox_SetpointSimple_size);
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
        if (iface == blox_SetpointSimple_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<Setpoint>()) {
            // return the member that implements the interface in this case
            Setpoint* ptr = &setpoint;
            return ptr;
        }
        return nullptr;
    }

    SetpointSimple& get()
    {
        return setpoint;
    }
};
