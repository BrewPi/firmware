#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "Ticks.h"
#include "ActuatorMutexDriver.h"

ActuatorPwm::ActuatorPwm(ActuatorDigital* _target, uint16_t _period) :
    ActuatorDriver(_target) {
    periodStartTime = ticks.millis();
    lowTime = 0;
    periodLate = 0;
    dutyLate = 0;
    value = 0.0;
    minVal = 0.0;
    maxVal = 100.0;
    target->setActive(false);
    setPeriod(_period);
    recalculate();
}

void ActuatorPwm::recalculate() {
    recalculate(period_ms);
}

void ActuatorPwm::recalculate(int32_t expectedPeriod) {
    // shift by 6 makes calculation work for period up to 11 hours
    dutyTime = int32_t(temp_long_t(value) << uint8_t(6)) * ((expectedPeriod + 50) / 100) >> 6;
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
        temp_t delta = val_ - value;
        value = val_;
        if(delta > temp_t(5.0)){
            // big positive change, go high immediately by starting a new period
            dutyLate = 0;
            periodLate = 0;
            recalculate();
            periodStartTime = ticks.millis() - period_ms;

        }
        else if(delta < temp_t(-5.0)){
            // big negative change, go to low part of period immediately
            recalculate();
            periodStartTime = ticks.millis() - dutyTime + dutyLate;
        }
        else{
            recalculate(period_ms + periodLate);
        }
    }
}

// returns the actual achieved PWM value, not the set value
temp_t ActuatorPwm::getValue() const {
    ticks_millis_t windowDuration = period_ms;// + periodLate;
    ticks_millis_t currentTime = ticks.millis();
    ticks_millis_t windowStartTime = currentTime - windowDuration;
    ticks_millis_t totalHigh = 0;
    if(lowTime > highTime){
        // pulse is finished
        totalHigh = lowTime - highTime;
        if(highTime < windowStartTime){
            windowDuration = currentTime - highTime; // pulse is far in the past
        }
    }
    else{
        if(highTime > windowStartTime){ // low to high transition is in window
            totalHigh += ticks.millis() - highTime;
            if(lowTime > windowStartTime){ // high to low transition is in window
                totalHigh += lowTime - windowStartTime;
            }
            else{
                return value; // return set value when high after long low period
            }
        }
    }
    temp_t pastValue = totalHigh / ((windowDuration + 50) / 100);
    return (value < pastValue) ? value : pastValue;
}

void ActuatorPwm::update() {
    target->update();
    int32_t adjDutyTime = dutyTime - dutyLate;
    int32_t currentTime = ticks.millis();
    int32_t elapsedTime = currentTime - periodStartTime;

    if (target->isActive()) {
        if (elapsedTime >= adjDutyTime) {
            // end of duty cycle
            int32_t lowDuration = (period_ms > dutyTime) ? period_ms - dutyTime : 0;
            if(periodLate >= lowDuration){
                // built up low periods are higher then required low time, skip a low cycle
                recalculate();
                periodLate -= lowDuration;
                // dutyLate -= (period_ms - dutyTime);
                periodStartTime = currentTime;
            }
            else{
                target->setActive(false);
                // check if turning the output off has succeeded (OnOff actuator could stay active due to time limit)
                if (target->isActive()) {
                    return; // try next time
                }
                dutyLate += elapsedTime - dutyTime;
                lowTime = ticks.millis();
            }
        }
    }
    if (!target->isActive()) { // <- do not replace with else if
        if (elapsedTime >= period_ms) {
            // end of PWM cycle
            if (adjDutyTime < 0) {
                // skip going high for 1 period when previous periods built up
                // more than one entire duty cycle (duty is ahead)
                // subtract duty cycle form duty late accumulator
                dutyLate = dutyLate - dutyTime;
            } else {
                if(dutyTime > 0){
                    if(target->type() == ACTUATOR_TOGGLE_MUTEX){
                        static_cast<ActuatorMutexDriver*>(target)->setActive(true, priority());
                    }
                    else{
                        target->setActive(true);
                    }
                    if (!target->isActive()) {
                        return; // try next time
                    }
                    highTime = ticks.millis();
                }
            }
            periodLate = elapsedTime - period_ms;
            // limit to half of the period
            periodLate = (periodLate < period_ms / 2) ? periodLate : period_ms / 2;
            // adjust next duty time to account for longer period due to infrequent updates
            // low period was longer, increase high period (duty cycle) with same ratio
            recalculate(period_ms + periodLate);
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
