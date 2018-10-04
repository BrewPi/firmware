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
#include "ActuatorDigitalChangeLogged.h"
#include "TicksTypes.h"
#include <functional>
#include <vector>

/*
 * An ActuatorAnalog has a range output
 */
class ActuatorDigitalConstrained : public ActuatorDigitalChangeLogged {
public:
    using ConstrainFunc = std::function<bool(const State&, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act)>;

private:
    std::vector<ConstrainFunc> constraints;

public:
    ActuatorDigitalConstrained(ActuatorDigital& act, const ticks_millis_t& now)
        : ActuatorDigitalChangeLogged(act, now){};
    virtual ~ActuatorDigitalConstrained() = default;

    void addConstraint(ConstrainFunc&& newConstraint)
    {
        constraints.push_back(std::move(newConstraint));
    }

    void removeAllConstraints()
    {
        constraints.clear();
    }

    virtual void state(const State& val, const ticks_millis_t& now) override final
    {
        State result = val;
        for (auto& constrainFunc : constraints) {
            if (!constrainFunc(result, now, *this)) {
                return;
            };
        }
        ActuatorDigitalChangeLogged::state(result, now);
    }

    State state() const
    {
        return ActuatorDigitalChangeLogged::state();
    }
};

namespace ADConstraints {
using State = ActuatorDigital::State;
class MinOnTime {
private:
    duration_millis_t minTime;

public:
    MinOnTime(const duration_millis_t& min)
        : minTime(min)
    {
    }

    bool operator()(const State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act)
    {
        auto times = act.getLastStartEndTime(State::Active, now);
        return newState == State::Active || times.end - times.start >= minTime;
    }
};

class MinOffTime {
private:
    duration_millis_t minTime;

public:
    MinOffTime(const duration_millis_t& min)
        : minTime(min)
    {
    }

    bool operator()(const State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act)
    {
        auto times = act.getLastStartEndTime(State::Inactive, now);
        return newState == State::Inactive || times.end - times.start >= minTime;
    }
};
} // end namespace ADConstraints
