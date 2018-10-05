/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "ActuatorAnalog.h"
#include "Balancer.h"
#include <functional>
#include <vector>

/*
 * An ActuatorAnalog has a range output
 */
class ActuatorAnalogConstrained : public ActuatorAnalog {
public:
    using ConstrainFunc = std::function<value_t(const value_t&)>;

private:
    std::vector<ConstrainFunc> constraints;
    ActuatorAnalog& actuator;

public:
    ActuatorAnalogConstrained(ActuatorAnalog& act)
        : actuator(act){};
    virtual ~ActuatorAnalogConstrained() = default;

    void addConstraint(ConstrainFunc&& newConstraint)
    {
        constraints.push_back(std::move(newConstraint));
    }

    void removeAllConstraints()
    {
        constraints.clear();
    }

    virtual void setting(const value_t& val) override final
    {
        value_t result = val;
        for (auto& constrainFunc : constraints) {
            result = constrainFunc(result);
        }
        actuator.setting(result);
    }

    virtual value_t setting() const override final
    {
        return actuator.setting();
    }

    virtual value_t value() const override final
    {
        return actuator.setting();
    }

    virtual bool valid() const override final
    {
        return actuator.valid();
    }
};

namespace AAConstraints {
using value_t = ActuatorAnalog::value_t;

class Minimum {
private:
    value_t m_min;

public:
    Minimum(const value_t& v)
        : m_min(v)
    {
    }

    value_t operator()(const value_t& val)
    {
        return std::max(val, m_min);
    }
};

class Maximum {
private:
    value_t m_max;

public:
    Maximum(const value_t& v)
        : m_max(v)
    {
    }

    value_t operator()(const value_t& val)
    {
        return std::min(val, m_max);
    }
};

using Balanced = Balanced;
}