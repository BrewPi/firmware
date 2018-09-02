#pragma once

#include "OneWireTempSensor.h"
#include "blox/Block.h"
#include "proto/cpp/OneWireTempSensor.pb.h"

OneWire&
theOneWire();

class OneWireTempSensorBlock : public Block<OneWireTempSensorBlock> {
private:
    OneWireTempSensor sensor;

public:
    OneWireTempSensorBlock()
        : sensor(theOneWire())
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        blox_OneWireTempSensor newData;
        cbox::CboxError res = streamProtoFrom(in, &newData, blox_OneWireTempSensor_fields, blox_OneWireTempSensor_size);
        /* if no errors occur, write new settings to wrapped object */
        if (res == cbox::CboxError::OK) {
            sensor.setAddress(newData.address);
            sensor.setCalibration(temp_t::raw(newData.offset));
        }
        return res;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_OneWireTempSensor message;
        message.address = sensor.getAddress();
        message.offset = sensor.getCalibration().getRaw();
        message.connected = sensor.isConnected();
        message.value = sensor.read().getRaw();
        return streamProtoTo(out, &message, blox_OneWireTempSensor_fields, blox_OneWireTempSensor_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        // No updates for now. Alternatively, a periodic bus scan for new devices?
        return update_never(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == cbox::resolveTypeId(this)) {
            return this; // me!
        }
        if (iface == cbox::resolveTypeId<TempSensor>()) {
            // return the member that implements the interface in this case
            TempSensor* ptr = &sensor;
            return ptr;
        }
        return nullptr;
    }

    OneWireTempSensor& get()
    {
        return sensor;
    }
};
