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

#include "ActuatorDigital.h"
#include <functional>
#include <vector>

/*
 * An ActuatorAnalog has a range output
 */
class ActuatorDigitalConstrained : public ActuatorDigital {
public:
    using ConstrainFunc = std::function<State(const State&)>;

private:
    std::vector<ConstrainFunc> constraints;
    ActuatorDigital& actuator;

public:
    ActuatorDigitalConstrained(ActuatorDigital& act)
        : actuator(act){};
    virtual ~ActuatorDigitalConstrained() = default;

    void addConstraint(ConstrainFunc&& newConstraint)
    {
        constraints.push_back(std::move(newConstraint));
    }

    void removeAllConstraints()
    {
        constraints.clear();
    }

    virtual void state(const State& val) override final
    {
        State result = val;
        for (auto& constrainFunc : constraints) {
            result = constrainFunc(result);
        }
        actuator.state(result);
    }

    virtual State state() const override final
    {
        return actuator.state();
    }
};