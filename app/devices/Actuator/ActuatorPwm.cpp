#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "Ticks.h"
#include "ActuatorMutexDriver.h"

ActuatorPwm::ActuatorPwm(ActuatorDigital* _target, uint16_t _period) :
    ActuatorDriver(_target) {
    periodStartTime = ticks.millis();
    periodLate = 0;
    dutyLate = 0;
    value = 0.0;
    minVal = 0.0;
    maxVal = 100.0;
    target->setActive(false);
    setPeriod(_period);
    // at init, pretend last high period was tiny spike in the past
    lowToHighTime = periodStartTime - period_ms;
    highToLowTime = lowToHighTime + 2;
    cycleTime = period_ms;
    dutyTime = calculateDutyTime(period_ms);
}

int32_t ActuatorPwm::calculateDutyTime(int32_t expectedPeriod) {
    // shift by 6 makes calculation work for period up to 11 hours
    int32_t duty = int32_t(temp_long_t(value) << uint8_t(6)) * ((expectedPeriod + 50) / 100) >> 6;
    return duty;
}

void ActuatorPwm::setValue(temp_t const& val) {
    temp_t val_(val);
    if (val_ <= minVal) {
        val_ = minVal;
    }
    if (val_ >= maxVal) {
        val_ = maxVal;
    }

    if (value != val_) {
        value = val_;
        dutyTime = calculateDutyTime(period_ms + periodLate);
    }
}

// returns the actual achieved PWM value, not the set value
temp_t ActuatorPwm::readValue() const {
    ticks_millis_t windowDuration = cycleTime; // previous time between two pulses
    ticks_millis_t totalHigh = 0;
    ticks_millis_t sinceLowToHigh = ticks.timeSinceMillis(lowToHighTime);
    ticks_millis_t sinceHighToLow = ticks.timeSinceMillis(highToLowTime);
    if(sinceLowToHigh > sinceHighToLow){
        // pulse is finished, and we are in the low period:   ___|--|__
        totalHigh = sinceLowToHigh - sinceHighToLow;
        if(sinceLowToHigh > windowDuration){
            windowDuration = sinceLowToHigh; // pulse is far in the past  _|--|_____________
        }
    }
    else{
        if(sinceLowToHigh <= windowDuration){
            // low to high transition is in window (still high)  __________|---
            if(sinceHighToLow >= windowDuration){
                // high after a long low period, extend window   --|______________________|-
                // keep cycle time as window.
                // not using windowDuration = sinceHighToLow, because this is only valid if previous cycle
                // showed that we are running skip cycles
                if(int32_t(windowDuration) > 2*period_ms && dutyTime > period_ms/4){
                    // was low abnormally long before going high for a duty over 25%
                    // probably actuator was held at zero. Assume a normal window for the future
                    windowDuration = period_ms;
                }
            }
            else{
                // high to low transition is in window (window start was high)  ---|_____|----
                totalHigh += windowDuration - sinceHighToLow;
            }
            totalHigh += sinceLowToHigh;
        }
        else{
            // entire window is high
            return 100.0;
        }
    }
    temp_t pastValue = short(temp_long_t(temp_long_t::base_type(totalHigh)) / temp_long_t(temp_long_t::base_type((windowDuration + 50) / 100)));
    return pastValue;
}

void ActuatorPwm::update() {
    target->update();
    int32_t adjDutyTime = dutyTime - dutyLate;
    int32_t currentTime = ticks.millis();
    int32_t elapsedTime = currentTime - periodStartTime;

    int32_t sinceLowToHigh = ticks.timeSinceMillis(lowToHighTime);
    int32_t sinceHighToLow = ticks.timeSinceMillis(highToLowTime);
    int32_t lastHighDuration = sinceLowToHigh - sinceHighToLow;

    if (target->isActive()) {
        if (elapsedTime >= adjDutyTime) {
            // end of duty cycle
            int32_t lowDuration = (period_ms > dutyTime) ? period_ms - dutyTime : 0;
            if(periodLate >= lowDuration){
                // built up low periods are higher then required low time, skip a low cycle
                dutyTime = calculateDutyTime(period_ms);
                periodLate -= lowDuration;
                // dutyLate -= (period_ms - dutyTime);
                periodStartTime = currentTime;
                cycleTime = period_ms;
                highToLowTime = 0; // set to zero to indicate we are stringing high periods together
            }
            else{
                target->setActive(false);
                // check if turning the output off has succeeded (OnOff actuator could stay active due to time limit)
                if (target->isActive()) {
                    return; // try next time
                }
                int32_t thisDutyLate = elapsedTime - dutyTime;
                dutyLate += thisDutyLate;
                if(highToLowTime != 0){
                    cycleTime = ticks.timeSinceMillis(highToLowTime);
                }
                highToLowTime =currentTime;
            }
        }
    }
    else if (!target->isActive()) {
        bool goHigh = false;
        bool newPeriod = false;
        int32_t estimatedCycleTime = 0;
        if (lastHighDuration > 0 && lastHighDuration < calculateDutyTime(sinceLowToHigh) - dutyLate){
            // new PWM value is higher than what was achieved in  cycle so far.
            // staying low longer is bad
            // The shortened cycle would have period sinceLowTime
            // The duty would be lastHighDuration.
            // If this duty is already lower than the target, staying low will only make things worse.
            goHigh = true;
            // do not recalculate cycle time, we can estimate it better here
            // estimatedCycleTime = sinceHighToLow + dutyTime; // last low period + expected high period
        }
        else if (elapsedTime >= period_ms) {
            // end of PWM cycle
            if (adjDutyTime < 0) {
                // skip going high for 1 period when previous periods built up
                // more than one entire duty cycle (duty is ahead)
                // subtract duty cycle form duty late accumulator
                dutyLate = dutyLate - dutyTime;
                newPeriod = true;
            } else {
                if(dutyTime > 0){
                    goHigh = true;
                }
            }
        }
        if(goHigh){
            if(target->type() == ACTUATOR_TOGGLE_MUTEX){
                static_cast<ActuatorMutexDriver*>(target)->setActive(true, priority());
            }
            else{
                target->setActive(true);
            }
            if(target->isActive()){
                newPeriod = true;
                if(estimatedCycleTime){
                    cycleTime = estimatedCycleTime; // already had an estimate from ending cycle early
                }
                else{
                    cycleTime = ticks.timeSinceMillis(lowToHighTime);
                }
                lowToHighTime = currentTime;
            }
        }
        if(newPeriod){
            if(readValue() < maxVal * temp_t(0.2)){
                // If target actuator was kept low externally, periodLate should not be used.
                // This could be due to the mutex group blocking going active, for example.
                // If the read value is under 20% of maximum, this is not likely to be normal behavior
                // If the value is low, the effect of doing this during normal behavior is small,
                // because periodLate is used to stretch scale the dutyCycle, which is small in that case.
                periodLate = 0;
            }
            else{
                periodLate = elapsedTime - period_ms;
                // make sure it is positive
                periodLate = (periodLate > 0) ? periodLate : 0;
                // limit to half of the period
                periodLate = (periodLate < period_ms / 2) ? periodLate : period_ms / 2;
            }
            // adjust next duty time to account for longer period due to infrequent updates
            // low period was longer, increase high period (duty cycle) with same ratio
            dutyTime = calculateDutyTime(period_ms + periodLate);
            periodStartTime = currentTime;
        }
    }
}

int8_t ActuatorPwm::priority(){
    int32_t adjDutyTime = dutyTime - dutyLate;
    int32_t priority = (adjDutyTime*100)/period_ms;
    if(priority > 127){
        priority = 127;
    }
    if(priority < 0){
        priority = 0;
    }
    return priority;
}
