#pragma once

#include "SetpointSensorPair.h"
#include "SetpointSensorPair.pb.h"
#include "blox/Block.h"
#include "cbox/CboxPtr.h"
#include <functional>

using std::placeholders::_1;

class SetpointSensorPairBlock : public Block<blox_SetpointSensorPair_msgid> {
private:
    cbox::CboxPtr<TempSensor> sensor;
    cbox::CboxPtr<Setpoint> setpoint;
    SetpointSensorPair pair;

public
    :
    SetpointSensorPairBlock(cbox::ObjectContainer& objects)
        : sensor(objects)
        , setpoint(objects)
        , pair(sensor, setpoint)
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        blox_SetpointSensorPair newData;
        cbox::CboxError res = streamProtoFrom(in, &newData, blox_SetpointSensorPair_fields, blox_SetpointSensorPair_size);
        /* if no errors occur, write new settings to wrapped object */
        if (res == cbox::CboxError::OK) {
            sensor.setId(newData.sensorId);
            setpoint.setId(newData.setpointId);
        }
        return res;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_SetpointSensorPair message;
        message.sensorId = sensor.getId();
        message.setpointId = setpoint.getId();
        message.sensorValue = to_base(pair.value());
        message.setpointValue = to_base(pair.setting());
        message.sensorValid = sensor.valid();
        message.setpointValid = setpoint.valid();

        return streamProtoTo(out, &message, blox_SetpointSensorPair_fields, blox_SetpointSensorPair_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        blox_SetpointSensorPair message;
        message.sensorId = sensor.getId();
        message.setpointId = setpoint.getId();

        return streamProtoTo(out, &message, blox_SetpointSensorPair_fields, blox_SetpointSensorPair_size);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        return update_never(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_SetpointSensorPair_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<ProcessValue>()) {
            // return the member that implements the interface in this case
            ProcessValue* ptr = &pair;
            return ptr;
        }
        return nullptr;
    }

    SetpointSensorPair& get()
    {
        return pair;
    }
};
