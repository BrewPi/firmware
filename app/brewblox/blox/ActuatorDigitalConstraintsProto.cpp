

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
    for (uint8_t i = 0; i < msg.constraint_count; ++i) {
        blox_DigitalConstraint constraintDfn = msg.constraint[i];
        switch (constraintDfn.which_constraint) {
        case blox_DigitalConstraint_minOff_tag:
            act.addConstraint(std::make_unique<MinOff>(constraintDfn.constraint.minOff.timeLimit));
            break;
        case blox_DigitalConstraint_minOn_tag:
            act.addConstraint(std::make_unique<MinOn>(constraintDfn.constraint.minOn.timeLimit));
            break;
        case blox_DigitalConstraint_mutex_tag: {
            act.addConstraint(std::make_unique<CboxMutex>(
                brewbloxBox().makeCboxPtr<TimedMutex>(),
                constraintDfn.constraint.mutex.mutexId));
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
    msg.constraint_count = 0;
    uint8_t numConstraints = sizeof(msg.constraint) / sizeof(msg.constraint[0]);
    for (uint8_t i = 0; i < numConstraints; ++i, ++it) {
        if (it == constraints.cend()) {
            return;
        }
        auto constraintId = (*it)->id();
        msg.constraint[i].which_constraint = constraintId;
        switch (constraintId) {
        case blox_DigitalConstraint_minOff_tag: {
            auto obj = reinterpret_cast<MinOff*>((*it).get());
            msg.constraint[i].constraint.minOff.timeLimit = obj->limit();
        } break;
        case blox_DigitalConstraint_minOn_tag: {
            auto obj = reinterpret_cast<MinOn*>((*it).get());
            msg.constraint[i].constraint.minOn.timeLimit = obj->limit();
        } break;
        case blox_DigitalConstraint_mutex_tag: {
            auto obj = reinterpret_cast<CboxMutex*>((*it).get());
            msg.constraint[i].constraint.minOn.timeLimit = obj->mutexId();
        } break;
        }
        msg.constraint_count++;
    }
}
