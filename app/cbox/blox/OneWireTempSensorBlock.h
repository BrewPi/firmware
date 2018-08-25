#pragma once

#include "Block.h"
#include "Interface.h"
#include "OneWire.h"
#include "OneWireTempSensor.h"
#include "OneWireTempSensor.pb.h"
#include "theOneWire.h"

class OneWireTempSensorBlock : public Block {
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

    OneWireTempSensor& get()
    {
        return sensor;
    }

    virtual Interface* getApplicationInterfaceImpl() override final
    {
        return &sensor;
    }

    virtual cbox::obj_type_t typeId() const override final
    {
        // use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
        return resolveTypeId(this);
    }
};
