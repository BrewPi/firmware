#pragma once

#include "Block.h"
#include "SensorSetPointPair.pb.h"
#include "SensorSetPointPair.h"
#include "TempSensorDelegate.h"
#include "SetPointDelegate.h"
#include "CboxLink.h"

class SensorSetPointPairBlock: public Block {
private:
    CboxLookup sensorLookup;
    CboxLookup setpointLookup;

    TempSensorDelegate sensor;
    SetPointDelegate setpoint;
    SensorSetPointPair pair;

public:
    SensorSetPointPairBlock() :
        sensor(sensorLookup),
        setpoint(setpointLookup),
        pair(sensor, setpoint)
    {}

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final{
        blox_SensorSetPointPair newData;
        cbox::CboxError res = streamProtoFrom(in, &newData, blox_SensorSetPointPair_fields, blox_SensorSetPointPair_size);
        /* if no errors occur, write new settings to wrapped object */
        if(res == cbox::CboxError::OK){
            sensorLookup.set(newData.sensor);
            setpointLookup.set(newData.setpoint);
        }
        return res;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final {
        blox_SensorSetPointPair message;
        message.sensor = sensorLookup.get();
        message.setpoint = setpointLookup.get();
        return streamProtoTo(out, &message, blox_SensorSetPointPair_fields, blox_SensorSetPointPair_size);
    }

    SensorSetPointPair & get() {
        return pair;
    }

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &pair;
    }

    virtual cbox::obj_type_t typeId() const override final {
        // use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
        return resolveTypeId(this);
    }
};
