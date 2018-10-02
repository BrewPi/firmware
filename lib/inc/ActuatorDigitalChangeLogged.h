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
#include "Ticks.h"
#include <algorithm>
#include <array>
#include <cstdint>
/*
 * An ActuatorDigital wrapper that logs the most recent changes
 */

const uint8_t historyLength = 5;

class ActuatorDigitalChangeLogged {
public:
    using State = ActuatorDigital::State;
    // uneven length makes last entry equal to first for toggling (PWM) behavior

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

    auto durations(const State& state,
                   const ticks_millis_t& now,
                   const bool windowCorrection = true,
                   const uint8_t& maxChanges = historyLength,
                   const duration_millis_t& maxHistory = std::numeric_limits<duration_millis_t>::max())
    {
        struct {
            ticks_millis_t stateTotal;
            ticks_millis_t total;
        } result;

        result.stateTotal = 0;
        auto end = now;
        auto start = now - 1;
        auto lastState = state;
        auto minStartTime = now - maxHistory;

        for (auto h = history.cbegin(); h < history.cend() - (historyLength - maxChanges); ++h) {
            start = h->startTime;
            lastState = h->newState;
            if (lastState == state) {
                // don't remove now from statement below.
                // if we compare durations instead of timestamps, we're not affected by overflow
                if (now - start <= now - minStartTime) {
                    result.stateTotal += end - start;
                } else {
                    start = minStartTime;
                    result.stateTotal += end - start;
                    break; // max history length reached
                }
            }
            end = start;
        }

        result.total = now - start;

        if (windowCorrection) {
            // correct windowing behavior so that the period is rather constant
            // this is done by not counting part of the oldest state equal length to the current state (newest log until now)
            // but only if the last state is the same as the first

            if (lastState == history.front().newState) {
                auto correction = now - history.front().startTime;
                result.total -= correction;
                if (state == history.front().newState) {
                    result.stateTotal -= correction;
                }
            }
        }
        return result;
    }
};
