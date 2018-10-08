#pragma once

#include "ActuatorPin.h"
#include "blox/Block.h"
#include "proto/cpp/ActuatorPin.pb.h"

class ActuatorPinBlock : public Block<blox_ActuatorPin_msgid> {
private:
    ActuatorPin actuator;

public:
    ActuatorPinBlock() {}

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_ActuatorPin newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_ActuatorPin_fields, blox_ActuatorPin_size);
        if (result == cbox::CboxError::OK) {
            actuator.pin(newData.pin);
            actuator.invert(newData.invert);
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorPin message;
        message.state = blox_AD_State(actuator.state());
        message.pin = actuator.pin();
        message.invert = actuator.invert();

        return streamProtoTo(out, &message, blox_ActuatorPin_fields, blox_ActuatorPin_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorPin persisted = blox_ActuatorPin_init_default;
        persisted.pin = actuator.pin();
        persisted.invert = actuator.invert();

        return streamProtoTo(out, &persisted, blox_ActuatorPin_fields, blox_ActuatorPin_size);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        return update_never(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_ActuatorPin_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<ActuatorDigital>()) {
            // return the member that implements the interface in this case
            ActuatorDigital* ptr = &actuator;
            return ptr;
        }
        return nullptr;
    }

    ActuatorPin& get()
    {
        return actuator;
    }
};
