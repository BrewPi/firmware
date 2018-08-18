#pragma once

#include "Block.h"
#include "SetPointSimple.pb.h"
#include "SetPoint.h"

class SetPointSimpleBlock: public Block {
private:
    SetPointSimple setpoint;

public:
    SetPointSimpleBlock() :
        setpoint()
{}

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final {
        blox_SetPointSimple newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_SetPointSimple_fields, blox_SetPointSimple_size);
        if(result == cbox::CboxError::no_error){
            setpoint.write(temp_t::raw(newData.setting));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) override final {
        blox_SetPointSimple message;
        message.setting = setpoint.read().getRaw();

        return streamProtoTo(out, &message, blox_SetPointSimple_fields, blox_SetPointSimple_size);
    }

    SetPointSimple & get() {
        return setpoint;
    }

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &setpoint;
    }


    virtual cbox::obj_type_t typeID() override {
    	return resolveTypeID(this);
    }
};
