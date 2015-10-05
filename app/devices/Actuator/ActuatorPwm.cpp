#include "Actuator.h"
#include "ActuatorPwm.h"
#include "Ticks.h"

ActuatorPwm::ActuatorPwm(ActuatorDigital* _target, uint16_t _period) : ActuatorDriver(_target) {
    periodStartTime = 0;
    periodLate = 0;
    dutyLate = 0;
    value = 0.0;
    minVal = 0.0;
    maxVal = 100.0;
    target->setActive(false);
    setPeriod(_period);
    recalculate();
}

void ActuatorPwm::recalculate(){
    temp_long_t newPeriod = temp_long_t(value) * temp_long_t(period) / temp_long_t(100);
    temp_long_t correctionFactor = temp_long_t(period + periodLate) / temp_long_t(period);
    dutyTime = int32_t(newPeriod * correctionFactor);
}


void ActuatorPwm::setValue(temp_t const& val) {
    temp_t val_(val);
    if (val_ <= minVal){
        val_ = minVal;
    }
    if (val_ >= maxVal){
        val_ = maxVal;
    }

    if(value != val_){
        temp_t delta = (val_ > value) ? val_ - value : value - val_;
        value = val_;
        if(delta > temp_t(1.0)){
            dutyLate = 0;
        }
    }
    recalculate();
}

void ActuatorPwm::update() {
    target->update();
    int32_t adjDutyTime = dutyTime - dutyLate;
    int32_t currentTime = ticks.millis();
    int32_t elapsedTime = currentTime - periodStartTime;

    if ( value <= minVal ){
        target->setActive(false);
        return;
    }
    if ( value >= maxVal ){
        target->setActive(true);
        return;
    }
    if (target->isActive()) {
        if (elapsedTime >= adjDutyTime) {
            // end of duty cycle
            target->setActive(false);
            // check if turning the output off has succeeded (OnOff actuator could stay active due to time limit)
            if(target->isActive()){
                return; // try next time
            }
            dutyLate += elapsedTime - dutyTime;
        }
    }
    if (!target->isActive()) { // <- do not replace with else if
        if (elapsedTime >= period) {
            // end of PWM cycle
            if (adjDutyTime < 0) {
                // skip going high for 1 period when previous periods built up
                // more than one entire duty cycle (duty is ahead)
                // subtract duty cycle form duty late accumulator
                dutyLate = dutyLate - dutyTime;
            } else {
                target->setActive(true);
                if(!target->isActive()){
                    return; // try next time
                }
            }
            periodLate = elapsedTime - period;
            // limit to half of the period
            periodLate = (periodLate < period / 2) ? periodLate : period / 2;
            // adjust next duty time to account for longer period due to infrequent updates
            // low period was longer, increase high period (duty cycle) with same ratio
            recalculate();
            periodStartTime = currentTime;
        }
    }
}
