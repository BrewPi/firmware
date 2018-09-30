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
#include <algorithm>
#include <array>
#include <cstdint>

/*
 * An ActuatorDigital wrapper that logs the most recent changes
 */
class ActuatorDigitalChangeLogged {
public:
    using ticks_millis_t = uint32_t;
    using State = ActuatorDigital::State;
    // uneven length makes last entry equal to first for toggling (PWM) behavior
    static const uint8_t historyLength = 5;

    struct StateChange {
        State newState;
        ticks_millis_t startTime;
    };

private:
    ActuatorDigital& actuator;
    std::array<StateChange, historyLength> history;

public:
    ActuatorDigitalChangeLogged(ActuatorDigital& act, const ticks_millis_t& now)
        : actuator(act)
    {
        history.fill(StateChange{State::Unknown, now - 1});
        history[0] = {actuator.state(), now};
    };
    ~ActuatorDigitalChangeLogged() = default;

    void state(const State& state, const ticks_millis_t& now)
    {
        State previous = actuator.state();
        actuator.state(state);
        if (previous != state) {
            std::rotate(history.rbegin(), history.rbegin() + 1, history.rend());
            history[0] = {state, now};
        }
    };

    State state() const
    {
        return actuator.state();
    };

    auto getLastStartEndTime(const State& state, const ticks_millis_t& now)
    {
        struct {
            ticks_millis_t start;
            ticks_millis_t end;
        } result;

        result.start = now + 1;
        result.end = (actuator.state() == state) ? now : now + 1;

        ticks_millis_t end = result.end;
        for (const auto& h : history) {
            if (h.newState == state) {
                result.start = h.startTime;
                result.end = end;
                break;
            }
            end = h.startTime;
        }
        return result;
    }

    auto durations(const State& state, const ticks_millis_t& now)
    {
        struct {
            ticks_millis_t stateTotal;
            ticks_millis_t total;
        } result;

        result.stateTotal = 0;
        ticks_millis_t end = now;
        for (const auto& h : history) {
            if (h.newState == state) {
                result.stateTotal += end - h.startTime;
            }
            end = h.startTime;
        }
        // correct windowing behavior so that the period is rather constant
        // this is done by not counting part of the oldest state equal length to the current state (newest log until now)
        result.total = history.front().startTime - history.back().startTime;
        if (history.back().newState == state) {
            result.stateTotal -= now - history.front().startTime;
        }
        return result;
    }
};
