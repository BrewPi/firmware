

#include "ActuatorAnalogConstrained.h"
#include "Balancer.h"
#include "BrewBlox.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/AnalogConstraints.pb.h"

using Minimum = AAConstraints::Minimum<blox_AnalogConstraint_min_tag>;
using Maximum = AAConstraints::Maximum<blox_AnalogConstraint_max_tag>;

using Balanced = AAConstraints::Balanced<blox_AnalogConstraint_balanced_tag>;
using BalancerT = Balancer<blox_AnalogConstraint_balanced_tag>;

class CboxBalanced : public Balanced {
private:
    cbox::CboxPtr<BalancerT> lookup;

public:
    CboxBalanced(cbox::CboxPtr<BalancerT>&& ptr, const cbox::obj_id_t& id)
        : Balanced(lookup)
        , lookup(ptr)
    {
        lookup.setId(id);
    }

    cbox::obj_id_t balancerId()
    {
        return lookup.getId();
    }
};

void
setAnalogConstraints(const blox_AnalogConstraints& msg, ActuatorAnalogConstrained& act)
{
    act.removeAllConstraints();
    for (uint8_t i = 0; i < msg.constraint_count; ++i) {
        blox_AnalogConstraint constraintDfn = msg.constraint[i];
        switch (constraintDfn.which_constraint) {
        case blox_AnalogConstraint_min_tag:
            act.addConstraint(std::make_unique<Minimum>(
                cnl::wrap<ActuatorAnalog::value_t>(constraintDfn.constraint.min.min)));
            break;
        case blox_AnalogConstraint_max_tag:
            act.addConstraint(std::make_unique<Maximum>(
                cnl::wrap<ActuatorAnalog::value_t>(constraintDfn.constraint.max.max)));
            break;
        case blox_AnalogConstraint_balanced_tag:
            act.addConstraint(std::make_unique<CboxBalanced>(
                brewbloxBox().makeCboxPtr<BalancerT>(),
                constraintDfn.constraint.balanced.balancerId));
            break;
        }
    }
}

void
getAnalogConstraints(blox_AnalogConstraints& msg, const ActuatorAnalogConstrained& act)
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
        case blox_AnalogConstraint_min_tag: {
            auto obj = reinterpret_cast<Minimum*>((*it).get());
            msg.constraint[i].constraint.min.min = cnl::unwrap(obj->min());
        } break;
        case blox_AnalogConstraint_max_tag: {
            auto obj = reinterpret_cast<Maximum*>((*it).get());
            msg.constraint[i].constraint.max.max = cnl::unwrap(obj->max());
        } break;
        case blox_AnalogConstraint_balanced_tag: {
            auto obj = reinterpret_cast<CboxBalanced*>((*it).get());
            msg.constraint[i].constraint.balanced.balancerId = obj->balancerId();
        } break;
        }
        msg.constraint_count++;
    }
}
