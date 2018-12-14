#pragma once

#include "ActuatorAnalogConstrained.h"
#include "ActuatorAnalogConstraintsProto.h"
#include "ActuatorDigitalConstrained.h"
#include "ActuatorPwm.h"
#include "blox/Block.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/ActuatorPwm.pb.h"
#include "proto/cpp/AnalogConstraints.pb.h"

class ActuatorPwmBlock : public Block<blox_ActuatorPwm_msgid> {
private:
    cbox::ObjectContainer& objectsRef; // remember object container reference to create constraints
    cbox::CboxPtr<ActuatorDigitalConstrained> actuator;
    ActuatorPwm pwm;
    ActuatorAnalogConstrained constrained;

public:
    ActuatorPwmBlock(cbox::ObjectContainer& objects)
        : objectsRef(objects)
        , actuator(objects)
        , pwm(
              [this]() {
                  // convert ActuatorDigitalConstrained to base ActuatorDigitalChangeLogged
                  return std::shared_ptr<ActuatorDigitalChangeLogged>(this->actuator.lock());
              },
              4000)
        , constrained(pwm)
    {
    }
    virtual ~ActuatorPwmBlock() = default;

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_ActuatorPwm newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_ActuatorPwm_fields, blox_ActuatorPwm_size);
        if (result == cbox::CboxError::OK) {
            actuator.setId(newData.actuatorId);
            pwm.period(newData.period);
            setAnalogConstraints(newData.constrainedBy, constrained, objectsRef);
            constrained.setting(cnl::wrap<ActuatorAnalog::value_t>(newData.setting));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorPwm message;
        message.actuatorId = actuator.getId();
        message.actuatorValid = pwm.valid();
        message.period = pwm.period();
        message.setting = cnl::unwrap(constrained.setting());
        message.value = cnl::unwrap(constrained.value());
        message.valid = constrained.valid();
        getAnalogConstraints(message.constrainedBy, constrained);

        return streamProtoTo(out, &message, blox_ActuatorPwm_fields, blox_ActuatorPwm_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        blox_ActuatorPwm persisted = blox_ActuatorPwm_init_default;
        persisted.actuatorId = actuator.getId();
        persisted.period = pwm.period();
        persisted.setting = cnl::unwrap(constrained.setting());
        getAnalogConstraints(persisted.constrainedBy, constrained);

        return streamProtoTo(out, &persisted, blox_ActuatorPwm_fields, blox_ActuatorPwm_size);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        constrained.update();
        return pwm.update(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_ActuatorPwm_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<ActuatorAnalogConstrained>()) {
            // return the member that implements the interface in this case
            ActuatorAnalogConstrained* ptr = &constrained;
            return ptr;
        }
        if (iface == cbox::interfaceId<ProcessValue<ActuatorAnalog::value_t>>()) {
            // return the member that implements the interface in this case
            ProcessValue<ActuatorAnalog::value_t>* ptr = &constrained;
            return ptr;
        }
        return nullptr;
    }

    ActuatorPwm& getPwm()
    {
        return pwm;
    }

    ActuatorAnalogConstrained& getConstrained()
    {
        return constrained;
    }
};
