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

        if (m_dutySetting <= 50) {
            // high period is fixed, low period adapts
            if (thisPeriodHighTime < m_dutyTime) {
                return now + std::min(update_t(1000), update_t(m_dutyTime - thisPeriodHighTime) >> 1);
            }
        } else {
            // high period can adapt between boundaries
            // maximum high time is 1.5x the previous high time or 1.5 the normal duty time, whichever is higher
            auto previousHighTime = twoPeriodHighTime - thisPeriodHighTime;
            auto maxHighTime = std::max(m_dutyTime, previousHighTime) * 3 / 2;

            // make sure that periods following each other do not alternate in high time
            // if the current period is already longer than the duty, diminish it by 25% of the extra time
            // This prevents alternating between 1500 and 2500 when the total of 2 periods should be 4000.
            if (thisPeriodHighTime > m_dutyTime && twoPeriodHighTime < 2 * m_dutyTime) {
                twoPeriodTargetHighTime -= (thisPeriodHighTime - m_dutyTime) / 4;
            }

            if (1) { //thisPeriodHighTime < maxHighTime) {
                if (twoPeriodHighTime < twoPeriodTargetHighTime) {
                    return now + std::min(update_t(1000), update_t(twoPeriodTargetHighTime - twoPeriodHighTime) >> 1);
                }
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

        if (m_dutySetting > 50) {
            // low period is fixed, high period adapts
            if (thisPeriodLowTime < invDutyTime) {
                return now + std::min(update_t(1000), update_t(invDutyTime - thisPeriodLowTime) >> 1);
            }
        } else {
            // maximum low time is 1.5 the previous low time, or 1.5 the normal time whichever is higher
            auto previousLowTime = twoPeriodLowTime - thisPeriodLowTime;
            auto maxLowTime = std::max(invDutyTime, previousLowTime) * 3 / 2;

            // make sure that periods following each other do not alternate in low time
            // if the current period is already longer than the invDuty, diminish it by 25% of the extra time
            // This prevents alternating between 1500 and 2500 when the total of 2 periods should be 4000.
            if (thisPeriodLowTime > invDutyTime && twoPeriodLowTime < 2 * invDutyTime) {
                twoPeriodTargetLowTime -= (thisPeriodLowTime - invDutyTime) / 4;
            }

            if (thisPeriodLowTime < maxLowTime) {
                if (twoPeriodLowTime < twoPeriodTargetLowTime) {
                    return now + std::min(update_t(1000), update_t(twoPeriodTargetLowTime - twoPeriodLowTime) >> 1);
                }
            }
        }
        m_target.state(State::Active, now);
    } else {
        m_target.state(ActuatorDigital::State::Inactive, now); // force back into known state
    }
    return now + 1; // after toggling, do next update as quickly as possible
}
