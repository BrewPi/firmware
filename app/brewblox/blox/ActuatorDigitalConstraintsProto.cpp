

#include "ActuatorDigitalConstrained.h"
#include "BrewBlox.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/DigitalConstraints.pb.h"

using MinOff = ADConstraints::MinOffTime<blox_DigitalConstraint_minOff_tag>;
using MinOn = ADConstraints::MinOnTime<blox_DigitalConstraint_minOn_tag>;

class CboxMutex : public ADConstraints::Base {
private:
    using Mutex_t = ADConstraints::Mutex<blox_DigitalConstraint_mutex_tag>;
    cbox::CboxPtr<TimedMutex> lookup;
    Mutex_t m_mutexConstraint;

public:
    CboxMutex(const cbox::obj_id_t& id)
        : lookup(brewbloxBox().makeCboxPtr<TimedMutex>(id))
        , m_mutexConstraint(lookup)
    {
    }

    virtual uint8_t id() const override final
    {
        return m_mutexConstraint.id();
    }

    cbox::obj_id_t mutexId()
    {
        return lookup.getId();
    }

    virtual bool allowed(const ActuatorDigital::State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act) override final
    {
        return m_mutexConstraint.allowed(newState, now, act);
    };
};

void
setDigitalConstraints(const blox_DigitalConstraints& msg, ActuatorDigitalConstrained& act)
{
    act.removeAllConstraints();
    pb_size_t numConstraints = std::min(msg.constraints_count, pb_size_t(sizeof(msg.constraints) / sizeof(msg.constraints[0])));
    for (pb_size_t i = 0; i < numConstraints; ++i) {
        blox_DigitalConstraint constraintDfn = msg.constraints[i];
        switch (constraintDfn.which_constraint) {
        case blox_DigitalConstraint_minOff_tag:
            act.addConstraint(std::make_unique<MinOff>(constraintDfn.constraint.minOff));
            break;
        case blox_DigitalConstraint_minOn_tag:
            act.addConstraint(std::make_unique<MinOn>(constraintDfn.constraint.minOn));
            break;
        case blox_DigitalConstraint_mutex_tag:
            act.addConstraint(std::make_unique<CboxMutex>(constraintDfn.constraint.mutex));
            break;
        }
    }
}

void
getDigitalConstraints(blox_DigitalConstraints& msg, const ActuatorDigitalConstrained& act)
{
    auto& constraints = act.constraintsList();
    auto it = constraints.cbegin();
    msg.constraints_count = 0;
    pb_size_t numConstraints = sizeof(msg.constraints) / sizeof(msg.constraints[0]);
    for (pb_size_t i = 0; i < numConstraints; ++i, ++it) {
        if (it == constraints.cend()) {
            return;
        }
        auto constraintId = (*it)->id();
        msg.constraints[i].which_constraint = constraintId;
        switch (constraintId) {
        case blox_DigitalConstraint_minOff_tag: {
            auto obj = reinterpret_cast<MinOff*>((*it).get());
            msg.constraints[i].constraint.minOff = obj->limit();
        } break;
        case blox_DigitalConstraint_minOn_tag: {
            auto obj = reinterpret_cast<MinOn*>((*it).get());
            msg.constraints[i].constraint.minOn = obj->limit();
        } break;
        case blox_DigitalConstraint_mutex_tag: {
            auto obj = reinterpret_cast<CboxMutex*>((*it).get());
            msg.constraints[i].constraint.mutex = obj->mutexId();
        } break;
        }
        msg.constraints_count++;
    }
}
