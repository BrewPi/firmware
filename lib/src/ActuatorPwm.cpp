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

    auto timesHigh = m_target.getLastStartEndTime(State::Active, now);
    auto timesLow = m_target.getLastStartEndTime(State::Inactive, now);

    if (m_target.state() == State::Active) {
        // for checking the currently achieved value, look back max 2 periods
        auto highTimeDurations = m_target.durations(State::Active, now, false, 4);
        auto twoPeriodHighTime = highTimeDurations.stateTotal;
        auto twoPeriodTotalTime = highTimeDurations.total;
        auto twoPeriodTargetHighTime = duration_millis_t(twoPeriodTotalTime * (m_dutySetting / 100));

        auto thisPeriodHighTime = timesHigh.end - timesHigh.start;

        auto maxHighTime = (m_dutySetting < value_t(75)) ? m_dutyTime : std::max(durations.total * 3 / 2, m_period);
        auto minHighTime = (m_dutySetting > value_t(25)) ? m_dutyTime : 1u;
        if ((twoPeriodHighTime >= twoPeriodTargetHighTime
             && thisPeriodHighTime > minHighTime)
            || (m_dutySetting != 100 && (thisPeriodHighTime > maxHighTime))) {
            m_target.state(State::Inactive, now);
        }
    } else if (m_target.state() == State::Inactive) {
        // for checking the currently achieved value, look back max 2 periods
        auto lowTimeDurations = m_target.durations(State::Inactive, now, false, 4);
        auto twoPeriodLowTime = lowTimeDurations.stateTotal;
        auto twoPeriodTotalTime = lowTimeDurations.total;
        auto twoPeriodTargetLowTime = duration_millis_t(twoPeriodTotalTime * ((value_t(100) - m_dutySetting) / 100));

        auto thisPeriodLowTime = timesLow.end - timesLow.start;

        // when not on low duty cycle, do not make period longer
        auto maxLowTime = (m_dutySetting > value_t(25)) ? m_period - m_dutyTime : std::max(durations.total * 3 / 2, m_period);
        auto minLowTime = (m_dutySetting < value_t(75)) ? m_period - m_dutyTime : 1u;

        if ((twoPeriodLowTime >= twoPeriodTargetLowTime
             && thisPeriodLowTime > minLowTime)
            || (m_dutySetting != 0 && (thisPeriodLowTime > maxLowTime))) {
            m_target.state(State::Active, now);
        }
    } else {
        m_target.state(ActuatorDigital::State::Inactive, now); // force back into known state
    }
    return now + 1; // TODO, don't request an update so often
}
