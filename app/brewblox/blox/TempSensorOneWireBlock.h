#pragma once

#include "TempSensorOneWire.h"
#include "Temperature.h"
#include "blox/Block.h"
#include "proto/cpp/TempSensorOneWire.pb.h"

OneWire&
theOneWire();

class TempSensorOneWireBlock : public Block<blox_TempSensorOneWire_msgid> {
private:
    TempSensorOneWire sensor;

public:
    TempSensorOneWireBlock()
        : sensor(theOneWire())
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        blox_TempSensorOneWire newData;
        cbox::CboxError res = streamProtoFrom(in, &newData, blox_TempSensorOneWire_fields, blox_TempSensorOneWire_size);
        /* if no errors occur, write new settings to wrapped object */
        if (res == cbox::CboxError::OK) {
            sensor.setAddress(OneWireAddress(newData.address));
            sensor.setCalibration(cnl::wrap<temp_t>(newData.offset));
        }
        return res;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_TempSensorOneWire message;
        message.address = sensor.getAddress();
        message.offset = cnl::unwrap(sensor.getCalibration());
        message.connected = sensor.valid();
        message.value = cnl::unwrap((sensor.read()));
        return streamProtoTo(out, &message, blox_TempSensorOneWire_fields, blox_TempSensorOneWire_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        blox_TempSensorOneWire message = blox_TempSensorOneWire_init_zero;
        message.address = sensor.getAddress();
        message.offset = cnl::unwrap(sensor.getCalibration());
        return streamProtoTo(out, &message, blox_TempSensorOneWire_fields, blox_TempSensorOneWire_size);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        // No updates for now. Alternatively, a periodic bus scan for new devices?
        return update_never(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_TempSensorOneWire_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<TempSensor>()) {
            // return the member that implements the interface in this case
            TempSensor* ptr = &sensor;
            return ptr;
        }
        return nullptr;
    }

    TempSensorOneWire& get()
    {
        return sensor;
    }
};
