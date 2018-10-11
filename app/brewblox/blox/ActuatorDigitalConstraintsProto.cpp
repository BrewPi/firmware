

#include "ActuatorDigitalConstrained.h"
#include "BrewBlox.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/DigitalConstraints.pb.h"

using MinOff = ADConstraints::MinOffTime<blox_DigitalConstraint_minOff_tag>;
using MinOn = ADConstraints::MinOnTime<blox_DigitalConstraint_minOn_tag>;

class CboxMutex : public ADConstraints::Mutex<blox_DigitalConstraint_mutex_tag> {
private:
    cbox::CboxPtr<TimedMutex> lookup;

public:
    CboxMutex(cbox::CboxPtr<TimedMutex>&& ptr, const cbox::obj_id_t& id)
        : Mutex(lookup)
        , lookup(ptr)
    {
        lookup.setId(id);
    }

    cbox::obj_id_t mutexId()
    {
        return lookup.getId();
    }
};

void
setDigitalConstraints(const blox_DigitalConstraints& msg, ActuatorDigitalConstrained& act)
{
    act.removeAllConstraints();
    for (uint8_t i = 0; i < msg.constraints_count; ++i) {
        blox_DigitalConstraint constraintDfn = msg.constraints[i];
        switch (constraintDfn.which_constraint) {
        case blox_DigitalConstraint_minOff_tag:
            act.addConstraint(std::make_unique<MinOff>(constraintDfn.constraint.minOff));
            break;
        case blox_DigitalConstraint_minOn_tag:
            act.addConstraint(std::make_unique<MinOn>(constraintDfn.constraint.minOn));
            break;
        case blox_DigitalConstraint_mutex_tag: {
            act.addConstraint(std::make_unique<CboxMutex>(
                brewbloxBox().makeCboxPtr<TimedMutex>(),
                constraintDfn.constraint.mutex));
            break;
        }
        }
    }
}

void
getDigitalConstraints(blox_DigitalConstraints& msg, const ActuatorDigitalConstrained& act)
{
    auto& constraints = act.constraintsList();
    auto it = constraints.cbegin();
    msg.constraints_count = 0;
    uint8_t numConstraints = sizeof(msg.constraints) / sizeof(msg.constraints[0]);
    for (uint8_t i = 0; i < numConstraints; ++i, ++it) {
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
