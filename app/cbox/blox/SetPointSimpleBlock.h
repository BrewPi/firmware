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

    SetPointSimple& get()
    {
        return setpoint;
    }

    virtual Interface* getApplicationInterfaceImpl() override final
    {
        return &setpoint;
    }
};
