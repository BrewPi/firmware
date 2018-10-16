#pragma once

#include "ActuatorAnalogConstraintsProto.h"
#include "Balancer.h"
#include "blox/Block.h"
#include "cbox/CboxPtr.h"
#include "nanopb_callbacks.h"
#include "proto/cpp/Balancer.pb.h"

class BalancerBlock : public Block<blox_Balancer_msgid> {
public:
    using Balancer_t = Balancer<blox_AnalogConstraint_balancer_tag>;

private:
    Balancer_t balancer;

protected:
    // stream result of a bus search, with arg pointing to the onewire bus
    static bool streamBalancedActuators(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
    {
        Balancer_t* balancerPtr = reinterpret_cast<Balancer_t*>(*arg);
        for (const auto& requester : balancerPtr->clients()) {
            auto act = blox_Balancer_BalancedActuator();
            act.id = 0;
            act.requested = cnl::unwrap(requester.requested);
            act.granted = cnl::unwrap(requester.granted);
            if (!pb_encode_tag_for_field(stream, field)) {
                return false;
            }
            if (!pb_encode_submessage(stream, blox_Balancer_BalancedActuator_fields, &act)) {
                return false;
            }
        }
        return true;
    }

public:
    BalancerBlock() = default;
    virtual ~BalancerBlock() = default;

    virtual cbox::CboxError
    streamFrom(cbox::DataIn& dataIn) override final
    {
        return cbox::CboxError::OK; // no settings to write (actuators register themselves)
    }

    virtual cbox::CboxError
    streamTo(cbox::DataOut& out) const override final
    {
        blox_Balancer message;
        message.clients.funcs.encode = streamBalancedActuators;
        message.clients.arg = const_cast<Balancer_t*>(&balancer); // arg is not const in message, but it is in callback

        return streamProtoTo(out, &message, blox_Balancer_fields, std::numeric_limits<size_t>::max());
    }

    virtual cbox::CboxError
    streamPersistedTo(cbox::DataOut& out) const override final
    {
        return cbox::CboxError::OK; // no settings to persist
    }

    virtual cbox::update_t
    update(const cbox::update_t& now) override final
    {
        balancer.update();
        return now + 1000;
    }

    virtual void*
    implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_Balancer_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<Balancer_t>()) {
            // return the member that implements the interface in this case
            Balancer_t* ptr = &balancer;
            return ptr;
        }
        return nullptr;
    }

    Balancer_t&
    getBalancer()
    {
        return balancer;
    }
};
