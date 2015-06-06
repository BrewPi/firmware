#include "Actuator.h"
#include "ActuatorPwm.h"
#include "Ticks.h"

ActuatorPwm::ActuatorPwm(Actuator* _target, uint16_t _period) : target(_target) {
    periodStartTime = 0;
    periodLate = 0;
    dutyLate = 0;
    target->setActive(false);
    setPeriod(_period);
    setPwm(0);
}

uint8_t ActuatorPwm::getPwm() {
    return pwm;
}

void ActuatorPwm::setPwm(uint8_t val) {
    if(pwm != val){
        pwm = val;
        if(pwm != (val+1) || pwm != (val-1)){
            dutyLate = 0;
        }
    }
    dutyTime = ((pwm * period) / 255)*(period + periodLate) / period;
}

void ActuatorPwm::updatePwm() {
    int32_t adjDutyTime = dutyTime - dutyLate;
    int32_t currentTime = ticks.millis();
    int32_t elapsedTime = currentTime - periodStartTime;

    if ( pwm == 0 ){
        target->setActive(false);
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
            dutyTime = ((pwm * period) / 255)*(period + periodLate) / period;
            periodStartTime = currentTime;
        }
    }
}
