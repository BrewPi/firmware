

#include "ActuatorAnalogConstrained.h"
#include "Balancer.h"
#include "BrewBlox.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/AnalogConstraints.pb.h"

using Minimum = AAConstraints::Minimum<blox_AnalogConstraint_min_tag>;
using Maximum = AAConstraints::Maximum<blox_AnalogConstraint_max_tag>;

using Balanced_t = AAConstraints::Balanced<blox_AnalogConstraint_balancer_tag>;
using Balancer_t = Balancer<blox_AnalogConstraint_balancer_tag>;

class CboxBalanced : public AAConstraints::Base {
private:
    // CboxBalanced does not use Balanced_t as base, becaue it needs to initialize the lookup
    // before initializing the Balanced_t
    cbox::CboxPtr<Balancer_t> lookup;
    Balanced_t m_balanced;

public:
    CboxBalanced(const cbox::obj_id_t& id)
        : lookup(brewbloxBox().makeCboxPtr<Balancer_t>(id))
        , m_balanced(lookup)
    {
    }

    cbox::obj_id_t balancerId()
    {
        return lookup.getId();
    }

    virtual uint8_t id() const override final
    {
        return m_balanced.id();
    }

    virtual AAConstraints::value_t constrain(const AAConstraints::value_t& val) const override final
    {
        return m_balanced.constrain(val);
    }
};

void
setAnalogConstraints(const blox_AnalogConstraints& msg, ActuatorAnalogConstrained& act)
{
    act.removeAllConstraints();
    for (uint8_t i = 0; i < msg.constraints_count; ++i) {
        blox_AnalogConstraint constraintDfn = msg.constraints[i];
        switch (constraintDfn.which_constraint) {
        case blox_AnalogConstraint_min_tag:
            act.addConstraint(std::make_unique<Minimum>(
                cnl::wrap<ActuatorAnalog::value_t>(constraintDfn.constraint.min)));
            break;
        case blox_AnalogConstraint_max_tag:
            act.addConstraint(std::make_unique<Maximum>(
                cnl::wrap<ActuatorAnalog::value_t>(constraintDfn.constraint.max)));
            break;
        case blox_AnalogConstraint_balancer_tag:
            act.addConstraint(std::make_unique<CboxBalanced>(
                constraintDfn.constraint.balancer));
            break;
        }
    }
}

void
getAnalogConstraints(blox_AnalogConstraints& msg, const ActuatorAnalogConstrained& act)
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
        case blox_AnalogConstraint_min_tag: {
            auto obj = reinterpret_cast<Minimum*>((*it).get());
            msg.constraints[i].constraint.min = cnl::unwrap(obj->min());
        } break;
        case blox_AnalogConstraint_max_tag: {
            auto obj = reinterpret_cast<Maximum*>((*it).get());
            msg.constraints[i].constraint.max = cnl::unwrap(obj->max());
        } break;
        case blox_AnalogConstraint_balancer_tag: {
            auto obj = reinterpret_cast<CboxBalanced*>((*it).get());
            msg.constraints[i].constraint.balancer = obj->balancerId();
        } break;
        }
        msg.constraints_count++;
    }
}
