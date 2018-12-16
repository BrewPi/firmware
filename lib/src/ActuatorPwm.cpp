#include "ActuatorPwm.h"
#include "future_std.h"
#include <cstdint>

ActuatorPwm::ActuatorPwm(
    std::function<std::shared_ptr<ActuatorDigitalChangeLogged>()>&& target,
    duration_millis_t period)
    : m_target(target)
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
    if (auto actPtr = m_target()) {
        auto durations = actPtr->activeDurations(now);
        auto twoPeriodTotalTime = durations.previousPeriod + durations.currentPeriod;
        auto twoPeriodHighTime = durations.previousActive + durations.currentActive;
        auto twoPeriodLowTime = twoPeriodTotalTime - twoPeriodHighTime;
        auto currentHighTime = durations.currentActive;
        auto currentPeriod = durations.currentPeriod;
        auto wait = duration_millis_t(0);
        auto currentState = actPtr->state();

        if (currentState == State::Active) {
            if (m_dutySetting == value_t(100)) {
                m_dutyAchieved = 100;
                return now + 1000;
            }

            if (m_dutySetting <= value_t(50)) {
                // high period is fixed, low period adapts
                if (currentHighTime < m_dutyTime) {
                    wait = m_dutyTime - currentHighTime;
                }
            } else {
                // high period can adapt between boundaries

                // for checking the currently achieved value, for this cycle so far and the previous
                auto twoPeriodTargetHighTime = duration_millis_t(twoPeriodTotalTime * (m_dutySetting / 100));
                // maximum high time is 1.5x the previous high time or 1.5 the normal duty time, whichever is higher
                auto maxHighTime = std::max(m_dutyTime, durations.previousActive) * 3 / 2;

                // make sure that periods following each other do not alternate in high time
                // if the current period is already longer than the duty, diminish it by 25% of the extra time
                // This prevents alternating between 1500 and 2500 when the total of 2 periods should be 4000.
                if (currentHighTime > m_dutyTime && twoPeriodHighTime < 2 * m_dutyTime) {
                    twoPeriodTargetHighTime -= (currentHighTime - m_dutyTime) / 4;
                }

                if (currentHighTime < maxHighTime) {
                    if (twoPeriodHighTime < twoPeriodTargetHighTime) {
                        wait = std::min(twoPeriodTargetHighTime - twoPeriodHighTime, maxHighTime - currentHighTime);
                    }
                }
            }
        } else if (currentState == State::Inactive) {
            if (m_dutySetting == value_t(0)) {
                m_dutyAchieved = 0;
                return now + 1000;
            }
            // for checking the currently achieved value, look back max 2 periods (toggles)
            auto invDutyTime = m_period - m_dutyTime;
            auto twoPeriodTargetLowTime = duration_millis_t(twoPeriodTotalTime * ((value_t(100) - m_dutySetting) / 100));

            auto thisPeriodLowTime = currentPeriod - currentHighTime;

            if (m_dutySetting > value_t(50)) {
                // low period is fixed, high period adapts
                if (thisPeriodLowTime < invDutyTime) {
                    wait = invDutyTime - thisPeriodLowTime;
                }
            } else {
                // low period can adapt between boundaries
                auto previousLowTime = twoPeriodLowTime - thisPeriodLowTime;
                // maximum low time is 1.5 the previous low time, or 1.5 the normal time whichever is higher
                auto maxLowTime = std::max(invDutyTime, previousLowTime) * 3 / 2;

                // make sure that periods following each other do not alternate in low time
                // if the current period is already longer than the invDuty, diminish it by 25% of the extra time
                // This prevents alternating between 1500 and 2500 when the total of 2 periods should be 4000.
                if (thisPeriodLowTime > invDutyTime && twoPeriodLowTime < 2 * invDutyTime) {
                    twoPeriodTargetLowTime -= (thisPeriodLowTime - invDutyTime) / 4;
                }

                if (thisPeriodLowTime < maxLowTime) {
                    if (twoPeriodLowTime < twoPeriodTargetLowTime) {
                        wait = std::min(twoPeriodTargetLowTime - twoPeriodLowTime, maxLowTime - thisPeriodLowTime);
                    }
                }
            }
        }

        twoPeriodTotalTime += wait;
        if (twoPeriodTotalTime == 0) {
            m_dutyAchieved = 0;
        } else {
            auto dutyAchieved = (value_t(100) * twoPeriodHighTime) / twoPeriodTotalTime;
            if (wait == 0) {
                // end of period
                m_dutyAchieved = dutyAchieved;
            } else if ((currentState == State::Inactive && dutyAchieved < m_dutyAchieved)
                       || (currentState == State::Active && dutyAchieved > m_dutyAchieved)) {
                // current period is long enough to start using the current achieved value including this period
                m_dutyAchieved = dutyAchieved;
            }
        }

        // Toggle actuator if necessary
        if (wait == 0) {
            if (currentState == State::Inactive) {
                actPtr->state(State::Active, now);
            } else {
                actPtr->state(State::Inactive, now);
            }
        }

        return now + std::min(update_t(1000), wait >> 1);
    }
    m_dutyAchieved = 0;
    return now + 1000;
}

bool
ActuatorPwm::valid() const
{
    if (auto actPtr = m_target()) {
        return actPtr->state() != State::Unknown;
    }
    return false;
}

void
ActuatorPwm::valid(bool v)
{
    if (!v) {
        if (auto actPtr = m_target()) {
            actPtr->state(State::Inactive);
        }
        setting(0);
    }
}

ActuatorPwm::State
ActuatorPwm::targetState() const
{
    if (auto actPtr = m_target()) {
        return actPtr->state();
    }
    return State::Unknown;
}