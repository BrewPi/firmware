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
#include <memory>
#include <mutex>
#include <vector>

class TimedMutex {
private:
    std::mutex m_mutex;
    duration_millis_t m_differentActuatorWait = 0;
    ticks_millis_t lastActive = 0;
    const ActuatorDigitalChangeLogged* lastActuator = nullptr;

public:
    TimedMutex() = default;
    ~TimedMutex() = default;

    bool try_lock(const duration_millis_t& now, const ActuatorDigitalChangeLogged& act)
    {
        if (lastActuator == nullptr || lastActuator == &act || (now - lastActive) >= m_differentActuatorWait) {
            return m_mutex.try_lock();
        }
        return false;
    }

    void unlock(const duration_millis_t& now, const ActuatorDigitalChangeLogged& act)
    {
        if (act.state() == ActuatorDigital::State::Active) {
            lastActive = now;
            lastActuator = &act;
        }
        m_mutex.unlock();
    }

    duration_millis_t differentActuatorWait() const
    {
        return m_differentActuatorWait;
    }

    void differentActuatorWait(const duration_millis_t& v)
    {
        m_differentActuatorWait = v;
    }
};

namespace ADConstraints {
using State = ActuatorDigital::State;

class Base {
public:
    Base() = default;
    virtual ~Base() = default;

    virtual bool allowed(const State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act) = 0;

    virtual uint8_t id() const = 0;
};

template <uint8_t ID>
class MinOnTime : public Base {
private:
    duration_millis_t m_limit;

public:
    MinOnTime(const duration_millis_t& min)
        : m_limit(min)
    {
    }

    bool allowed(const State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act) override final
    {
        auto times = act.getLastStartEndTime(State::Active, now);
        return newState == State::Active || times.end - times.start >= m_limit;
    }

    virtual uint8_t id() const override final
    {
        return ID;
    }

    duration_millis_t limit()
    {
        return m_limit;
    }
};

template <uint8_t ID>
class MinOffTime : public Base {
private:
    duration_millis_t m_limit;

public:
    MinOffTime(const duration_millis_t& min)
        : m_limit(min)
    {
    }

    virtual bool allowed(const State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act) override final
    {
        auto times = act.getLastStartEndTime(State::Inactive, now);
        return newState == State::Inactive || times.end - times.start >= m_limit;
    }

    virtual uint8_t id() const override final
    {
        return ID;
    }

    duration_millis_t limit()
    {
        return m_limit;
    }
};

template <uint8_t ID>
class Mutex : public Base {
private:
    const std::function<std::shared_ptr<TimedMutex>()> m_mutex;
    bool hasLock = false;

public:
    explicit Mutex(
        std::function<std::shared_ptr<TimedMutex>()>&& mut)
        : m_mutex(mut)
    {
    }

    virtual bool allowed(const State& newState, const ticks_millis_t& now, const ActuatorDigitalChangeLogged& act) override final
    {
        if (newState == State::Inactive) {
            if (hasLock) {
                if (auto mutPtr = m_mutex()) {
                    mutPtr->unlock(now, act);
                    hasLock = false;
                }
            }
            return true;
        }

        if (auto mutPtr = m_mutex()) {
            if (act.state() != State::Active && newState == State::Active) {
                hasLock = mutPtr->try_lock(now, act);
                return hasLock;
            }
        }
        return false;
    }

    virtual uint8_t id() const override final
    {
        return ID;
    }
};

} // end namespace ADConstraints

class ActuatorDigitalConstrained : public ActuatorDigitalChangeLogged {
public:
    using Constraint = ADConstraints::Base;

private:
    std::vector<std::unique_ptr<Constraint>> constraints;
    uint8_t m_blockingConstraint = 0;

public:
    ActuatorDigitalConstrained(ActuatorDigital& act)
        : ActuatorDigitalChangeLogged(act){};

    ActuatorDigitalConstrained(const ActuatorDigitalConstrained&) = delete;
    ActuatorDigitalConstrained& operator=(const ActuatorDigitalConstrained&) = delete;
    ActuatorDigitalConstrained(ActuatorDigitalConstrained&&) = default;
    ActuatorDigitalConstrained& operator=(ActuatorDigitalConstrained&&) = default;

    virtual ~ActuatorDigitalConstrained() = default;

    void addConstraint(std::unique_ptr<Constraint>&& newConstraint)
    {
        constraints.push_back(std::move(newConstraint));
    }

    void removeAllConstraints()
    {
        constraints.clear();
    }

    void resetHistory()
    {
        ActuatorDigitalChangeLogged::resetHistory();
    }

    uint8_t checkConstraints(const State& val, const ticks_millis_t& now)
    {
        uint8_t firstBlockingConstraint = 0;
        uint8_t count = 1;
        for (auto& c : constraints) {
            if (!c->allowed(val, now, *this)) {
                // don't exit early, all constraints need to be updated
                if (firstBlockingConstraint == 0) {
                    firstBlockingConstraint = count;
                }
            }
            ++count;
        }
        return firstBlockingConstraint;
    }

    uint8_t blockingConstraint() const
    {
        return m_blockingConstraint;
    }

    virtual void state(const State& val, const ticks_millis_t& now) override final
    {
        m_blockingConstraint = checkConstraints(val, now);
        if (m_blockingConstraint != 0) {
            // Check constraints again with current state to reset any state keeping contstraints like mutex
            checkConstraints(ActuatorDigitalChangeLogged::state(), now);
        } else {
            ActuatorDigitalChangeLogged::state(val, now);
        }
    }

    virtual void state(const State& val) override final
    {
        state(val, lastUpdateTime);
    }

    virtual State state() const override
    {
        return ActuatorDigitalChangeLogged::state();
    }

    const std::vector<std::unique_ptr<Constraint>>& constraintsList() const
    {
        return constraints;
    };
};
