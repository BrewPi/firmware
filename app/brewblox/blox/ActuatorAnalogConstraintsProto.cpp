

#include "ActuatorAnalogConstrained.h"
#include "Balancer.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/AnalogConstraints.pb.h"

using Minimum = AAConstraints::Minimum<blox_AnalogConstraint_min_tag>;
using Maximum = AAConstraints::Maximum<blox_AnalogConstraint_max_tag>;

using Balanced_t = AAConstraints::Balanced<blox_AnalogConstraint_balanced_tag>;
using Balancer_t = Balancer<blox_AnalogConstraint_balanced_tag>;

class CboxBalanced : public AAConstraints::Base {
private:
    // CboxBalanced does not use Balanced_t as base, becaue it needs to initialize the lookup
    // before initializing the Balanced_t
    cbox::CboxPtr<Balancer_t> lookup;
    Balanced_t m_balanced;

public:
    CboxBalanced(cbox::ObjectContainer& objects, const cbox::obj_id_t& objId)
        : lookup(cbox::CboxPtr<Balancer_t>(objects, objId))
        , m_balanced(lookup.lockFunctor())
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

    AAConstraints::value_t granted() const
    {
        return m_balanced.granted();
    }

    virtual uint8_t order() const override final
    {
        return m_balanced.order();
    }
};

void
setAnalogConstraints(const blox_AnalogConstraints& msg, ActuatorAnalogConstrained& act, cbox::ObjectContainer& objects)
{
    act.removeAllConstraints();
    pb_size_t numConstraints = std::min(msg.constraints_count, pb_size_t(sizeof(msg.constraints) / sizeof(msg.constraints[0])));
    for (pb_size_t i = 0; i < numConstraints; ++i) {
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
        case blox_AnalogConstraint_balanced_tag:
            act.addConstraint(std::make_unique<CboxBalanced>(objects, constraintDfn.constraint.balanced.balancerId));
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
    pb_size_t numConstraints = pb_size_t(sizeof(msg.constraints) / sizeof(msg.constraints[0]));
    for (pb_size_t i = 0; i < numConstraints; ++i, ++it) {
        if (it == constraints.cend()) {
            break;
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
        case blox_AnalogConstraint_balanced_tag: {
            auto obj = reinterpret_cast<CboxBalanced*>((*it).get());
            msg.constraints[i].constraint.balanced.balancerId = obj->balancerId();
            msg.constraints[i].constraint.balanced.granted = cnl::unwrap(obj->granted());
        } break;
        }
        msg.constraints[i].limiting = act.limiting() & (uint8_t(1) << i);
        msg.constraints_count++;
    }
    msg.unconstrained = cnl::unwrap(act.unconstrained());
}
