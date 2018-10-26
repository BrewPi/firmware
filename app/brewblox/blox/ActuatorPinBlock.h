#pragma once

#include "ActuatorDigitalConstrained.h"
#include "ActuatorDigitalConstraintsProto.h"
#include "ActuatorPin.h"
#include "blox/Block.h"
#include "proto/cpp/ActuatorPin.pb.h"
#include <cstdint>

class ActuatorPinBlock : public Block<blox_ActuatorPin_msgid> {
private:
    cbox::ObjectContainer& objectsRef; // remember object container reference to create constraints
    ActuatorPin actuator;
    ActuatorDigitalConstrained constrained;

public:
    ActuatorPinBlock(cbox::ObjectContainer& objects, uint8_t pinNr)
        : objectsRef(objects)
        , actuator(pinNr, false)
        , constrained(actuator)
    {
    }
    virtual ~ActuatorPinBlock() = default;

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_ActuatorPin newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_ActuatorPin_fields, blox_ActuatorPin_size);
        if (result == cbox::CboxError::OK) {
            actuator.invert(newData.invert);
            setDigitalConstraints(newData.constrainedBy, constrained, objectsRef);
            constrained.state(ActuatorDigital::State(newData.state));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorPin message = blox_ActuatorPin_init_default;
        message.state = blox_AD_State(actuator.state());
        message.invert = actuator.invert();
        getDigitalConstraints(message.constrainedBy, constrained);

        return streamProtoTo(out, &message, blox_ActuatorPin_fields, blox_ActuatorPin_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        constrained.update(now);
        return now + 1000;
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_ActuatorPin_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<ActuatorDigitalConstrained>()) {
            // return the member that implements the interface in this case
            ActuatorDigitalConstrained* ptr = &constrained;
            return ptr;
        }
        return nullptr;
    }

    ActuatorPin& getPin()
    {
        return actuator;
    }

    ActuatorDigitalConstrained& getConstrained()
    {
        return constrained;
    }
};
