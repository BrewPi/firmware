#include "Actuator.h"
#include "ActuatorPwm.h"
#include "Ticks.h"

ActuatorPwm::ActuatorPwm(Actuator* driver) : target(driver) {
    this->periodStartTime = 0;
    this->target->setActive(false);
    this->setPwm(pwm);
}

uint8_t ActuatorPwm::getPwm() {
    return this->pwm;
}

void ActuatorPwm::setPwm(uint8_t val) {
    this->pwm = val;
    this->dutyLate = 0;
    this->dutyTime = ((this->pwm * period) / 255);
}

void ActuatorPwm::updatePwm() {
    int32_t adjDutyTime = dutyTime - this->dutyLate;
    int32_t currentTime = ticks.millis();
    int32_t elapsedTime = currentTime - this->periodStartTime;

    if (this->target->isActive()) {
        if (elapsedTime >= adjDutyTime) {
            // end of duty cycle
            this->target->setActive(false);
            this->dutyLate += elapsedTime - dutyTime;
        }
    }
    if (!this->target->isActive()) {
        if (elapsedTime >= period) {
            // end of PWM cycle
            if (adjDutyTime < 0) {
                // skip going high for 1 period when previous periods built up
                // more than one entire duty cycle (duty is ahead)
                // subtract duty cycle form duty late accumulator
                this->dutyLate = this->dutyLate - dutyTime;
            } else {
                this->target->setActive(true);
            }
            int32_t periodLate = elapsedTime - period;
            // limit to half of the period
            periodLate = (periodLate < period / 2) ? periodLate : period / 2;
            // adjust next duty time to account for longer period due to infrequent updates
            // low period was longer, increase high period (duty cycle) with same ratio
            this->dutyTime = ((this->pwm * period) / 255)*(period + periodLate) / period;
            this->periodStartTime = currentTime;
        }
    }
}
