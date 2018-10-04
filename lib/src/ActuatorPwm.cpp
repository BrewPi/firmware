#include "ActuatorPwm.h"
#include "future_std.h"
#include <cstdint>

ActuatorPwm::ActuatorPwm(ActuatorDigitalChangeLogged& _target, duration_millis_t period)
    : m_target(_target)
    , m_dutyTime(period)
    , m_period(period)
{
}

void
ActuatorPwm::setting(value_t const& val)
{
    m_dutySetting = std::clamp(val, value_t(0), value_t(100));
    m_dutyTime = duration_millis_t((m_dutySetting * m_period) / value_t(100));
}

// returns the actual achieved PWM value, not the set value
ActuatorPwm::value_t
ActuatorPwm::value() const
{
    return m_dutyAchieved;
}

ActuatorPwm::update_t
ActuatorPwm::update(const update_t& now)
{
    auto durations = m_target.durations(State::Active, now);
    m_dutyAchieved = (value_t(100) * durations.stateTotal) / durations.total;

    if (m_target.state() == State::Active) {
        if (m_dutySetting == value_t(100)) {
            return now + 1000;
        }

        auto timesHigh = m_target.getLastStartEndTime(State::Active, now);
        // for checking the currently achieved value, look back max 2 periods (toggles)
        // limit the time looked back to 3 periods, but longer for higher duty cycles
        auto maxHistory = std::max(m_period * 3, m_dutyTime * 16);
        auto highTimeDurations = m_target.durations(State::Active, now, false, 4, maxHistory);
        auto twoPeriodHighTime = highTimeDurations.stateTotal;
        auto twoPeriodTotalTime = highTimeDurations.total;
        auto twoPeriodTargetHighTime = duration_millis_t(twoPeriodTotalTime * (m_dutySetting / 100));

        auto thisPeriodHighTime = timesHigh.end - timesHigh.start;

        auto maxHighTime = (m_dutySetting < value_t(75)) ? m_dutyTime : (durations.total * 3) >> 1;
        auto minHighTime = (m_dutySetting > value_t(25)) ? m_dutyTime / 2 : 1u;

        if (thisPeriodHighTime < maxHighTime) {
            if (thisPeriodHighTime < minHighTime) {
                return now + std::min(update_t(1000), update_t(minHighTime - thisPeriodHighTime) >> 1);
            }
            if (twoPeriodHighTime < twoPeriodTargetHighTime) {
                return now + std::min(update_t(1000), update_t(twoPeriodTargetHighTime - twoPeriodHighTime) >> 1);
            }
        }
        m_target.state(State::Inactive, now);
    } else if (m_target.state() == State::Inactive) {
        if (m_dutySetting == value_t(0)) {
            return now + 1000;
        }
        auto timesLow = m_target.getLastStartEndTime(State::Inactive, now);
        // for checking the currently achieved value, look back max 2 periods (toggles) and 4 periods (time)
        auto invDutyTime = m_period - m_dutyTime;
        auto maxHistory = std::max(m_period * 3, invDutyTime * 16);
        auto lowTimeDurations = m_target.durations(State::Inactive, now, false, 4, maxHistory);
        auto twoPeriodLowTime = lowTimeDurations.stateTotal;
        auto twoPeriodTotalTime = lowTimeDurations.total;
        auto twoPeriodTargetLowTime = duration_millis_t(twoPeriodTotalTime * ((value_t(100) - m_dutySetting) / 100));

        auto thisPeriodLowTime = timesLow.end - timesLow.start;

        // when not on low duty cycle, do not make period longer. High period should adapt.
        auto maxLowTime = (m_dutySetting > value_t(25)) ? invDutyTime : (durations.total * 3) >> 1;
        auto minLowTime = (m_dutySetting < value_t(75)) ? invDutyTime : 1u;

        if (thisPeriodLowTime < maxLowTime) {
            if (thisPeriodLowTime < minLowTime) {
                return now + std::min(update_t(1000), update_t(minLowTime - thisPeriodLowTime) >> 1);
            }
            if (twoPeriodLowTime < twoPeriodTargetLowTime) {
                return now + std::min(update_t(1000), update_t(twoPeriodTargetLowTime - twoPeriodLowTime) >> 1);
            }
        }
        m_target.state(State::Active, now);
    } else {
        m_target.state(ActuatorDigital::State::Inactive, now); // force back into known state
    }
    return now + 1; // after toggling, do next update as quickly as possible
}
