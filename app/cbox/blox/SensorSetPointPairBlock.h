#pragma once

#include "Block.h"
#include "CboxLink.h"
#include "SensorSetPointPair.h"
#include "SensorSetPointPair.pb.h"
#include "SetPointDelegate.h"
#include "TempSensorDelegate.h"

class SensorSetPointPairBlock : public Block<SensorSetPointPairBlock> {
private:
    CboxLookup sensorLookup;
    CboxLookup setpointLookup;

    TempSensorDelegate sensor;
    SetPointDelegate setpoint;
    SensorSetPointPair pair;

public:
    SensorSetPointPairBlock()
        : sensor(sensorLookup)
        , setpoint(setpointLookup)
        , pair(sensor, setpoint)
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        blox_SensorSetPointPair newData;
        cbox::CboxError res = streamProtoFrom(in, &newData, blox_SensorSetPointPair_fields, blox_SensorSetPointPair_size);
        /* if no errors occur, write new settings to wrapped object */
        if (res == cbox::CboxError::OK) {
            sensorLookup.set(newData.sensor);
            setpointLookup.set(newData.setpoint);
        }
        return res;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_SensorSetPointPair message;
        message.sensor = sensorLookup.get();
        message.setpoint = setpointLookup.get();
        return streamProtoTo(out, &message, blox_SensorSetPointPair_fields, blox_SensorSetPointPair_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    SensorSetPointPair& get()
    {
        return pair;
    }

    virtual Interface* getApplicationInterfaceImpl() override final
    {
        return &pair;
    }
};
