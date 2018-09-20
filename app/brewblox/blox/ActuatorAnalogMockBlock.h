#pragma once

#include "ActuatorAnalogMock.h"
#include "blox/Block.h"
#include "proto/cpp/ActuatorAnalogMock.pb.h"

class ActuatorAnalogMockBlock : public Block<blox_ActuatorAnalogMock_msgid> {
private:
    ActuatorAnalogMock actuator = ActuatorAnalogMock(0, 0, 100);

public:
    ActuatorAnalogMockBlock()
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_ActuatorAnalogMock newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_ActuatorAnalogMock_fields, blox_ActuatorAnalogMock_size);
        if (result == cbox::CboxError::OK) {
            actuator.setting(cnl::wrap<ActuatorAnalog::value_t>(newData.setting));
            actuator.minSetting(cnl::wrap<ActuatorAnalog::value_t>(newData.minSetting));
            actuator.maxSetting(cnl::wrap<ActuatorAnalog::value_t>(newData.maxSetting));
            actuator.minValue(cnl::wrap<ActuatorAnalog::value_t>(newData.minValue));
            actuator.maxValue(cnl::wrap<ActuatorAnalog::value_t>(newData.maxValue));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorAnalogMock message;
        message.setting = cnl::unwrap(actuator.setting());
        message.value = cnl::unwrap(actuator.value());
        message.valid = actuator.valid();
        message.minSetting = cnl::unwrap(actuator.minSetting());
        message.maxSetting = cnl::unwrap(actuator.maxSetting());
        message.minValue = cnl::unwrap(actuator.minValue());
        message.maxValue = cnl::unwrap(actuator.maxValue());

        return streamProtoTo(out, &message, blox_ActuatorAnalogMock_fields, blox_ActuatorAnalogMock_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorAnalogMock message;
        message.setting = cnl::unwrap(actuator.setting());
        message.minSetting = cnl::unwrap(actuator.minSetting());
        message.maxSetting = cnl::unwrap(actuator.maxSetting());
        message.minValue = cnl::unwrap(actuator.minValue());
        message.maxValue = cnl::unwrap(actuator.maxValue());

        return streamProtoTo(out, &message, blox_ActuatorAnalogMock_fields, blox_ActuatorAnalogMock_size);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        return update_never(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_ActuatorAnalogMock_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<ActuatorAnalog>()) {
            // return the member that implements the interface in this case
            ActuatorAnalog* ptr = &actuator;
            return ptr;
        }
        if (iface == cbox::interfaceId<ProcessValue<ActuatorAnalog::value_t>>()) {
            // return the member that implements the interface in this case
            ProcessValue<ActuatorAnalog::value_t>* ptr = &actuator;
            return ptr;
        }
        return nullptr;
    }

    ActuatorAnalogMock& get()
    {
        return actuator;
    }
};
