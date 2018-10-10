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

// uneven length makes last entry equal to first for toggling (PWM) behavior
const uint8_t historyLength = 5;

class ActuatorDigitalChangeLogged : public ActuatorDigital {
public:
    using State = ActuatorDigital::State;

    struct StateChange {
        State newState;
        ticks_millis_t startTime;
    };

private:
    ActuatorDigital& actuator;
    std::array<StateChange, historyLength> history;

protected:
    ticks_millis_t lastUpdateTime = 0;

public:
    ActuatorDigitalChangeLogged(ActuatorDigital& act)
        : actuator(act)
    {
        history.fill(StateChange{State::Unknown, ticks_millis_t(-1)});
        history[0] = {actuator.state(), 0};
    };
    virtual ~ActuatorDigitalChangeLogged() = default;

    virtual void state(const State& val, const ticks_millis_t& now)
    {
        actuator.state(val);
        update(now);
    };

    virtual void state(const State& val) override
    {
        state(val, lastUpdateTime);
    };

    State state() const
    {
        return actuator.state();
    };

    void update(const ticks_millis_t& now)
    {
        if (state() != history.front().newState) {
            std::rotate(history.rbegin(), history.rbegin() + 1, history.rend());
            history[0] = {state(), now};
        }
        lastUpdateTime = now;
    }

    auto getLastStartEndTime(const State& state, const ticks_millis_t& now) const
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

    auto activeDurations(const ticks_millis_t& now)
    {
        struct {
            ticks_millis_t currentActive;
            ticks_millis_t currentPeriod;
            ticks_millis_t previousActive;
            ticks_millis_t previousPeriod;
        } result;

        result.currentActive = 0;
        result.currentPeriod = 0;
        result.previousActive = 0;
        result.previousPeriod = 0;
        auto end = now;
        auto start = ticks_millis_t(0);
        //auto minStartTime = now - maxHistory;
        uint8_t activePeriods = 0;

        auto h = history.cbegin();
        for (; h < history.cend(); ++h) {
            start = h->startTime;
            auto duration = end - start;
            end = start;
            if (h->newState == State::Active) {
                ++activePeriods;
                if (result.currentActive == 0) {
                    result.currentActive = duration;
                    result.currentPeriod += duration;
                } else if (result.previousActive == 0) {
                    result.previousActive = duration;
                    result.previousPeriod += duration;
                } else {
                    break;
                }
            } else {
                if (history.front().newState == State::Inactive) {
                    if (activePeriods == 0) {
                        result.currentPeriod += duration;
                    } else if (activePeriods <= 1) {
                        result.previousPeriod += duration;
                    } else {
                        break;
                    }
                } else {
                    if (activePeriods <= 1) {
                        result.currentPeriod += duration;
                    } else {
                        result.previousPeriod += duration;
                    }
                }
            }
        }

        return result;
    }
};
