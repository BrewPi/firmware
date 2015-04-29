#include "ActuatorPwm.h"
#include "Ticks.h"

ActuatorPwm::ActuatorPwm(Actuator * driver, uint8_t pwm = 0) {
    this->driver = driver;
    this->pwm = pwm;
    // Accumulate time errors to adjust for being late in next cycle
    this->dutyLate = 0;
    this->periodLate = 0;
    this->periodStartTime = 0;
    this->driver->setActive(false);
}

uint8_t ActuatorPwm::getPwm() {
    return this->pwm;
}

void ActuatorPwm::setPwm(uint8_t val) {
    this->pwm = val;
    this->dutyLate = 0;
    this->periodLate = 0;
    this->periodStartTime = ticks.millis();
    this->updatePwm();
}

void ActuatorPwm::updatePwm() {
    int32_t dutyTime = (this->pwm * period) / 255;
    int32_t adjDutyTime = dutyTime - dutyLate;

    int32_t currentTime = ticks.millis();
    int32_t elapsedTime = currentTime - this->periodStartTime;

    // limit period correction to half period
    int32_t adjPeriodTime = period - ((periodLate <= int32_t(period) / 2) ? periodLate : period / 2);

    if (this->driver->isActive()) {
        if (elapsedTime >= adjDutyTime) {
            this->driver->setActive(false);
            this->dutyLate += elapsedTime - dutyTime;
        }
    }
    if (!this->driver->isActive()) {
        if (elapsedTime >= adjPeriodTime) {
            // end of PWM cycle
            if (adjDutyTime < 0) {
                // skip going high for 1 period when previous periods built up
                // more than one entire duty cycle (duty is ahead)
                // subtract duty form duty late
                this->dutyLate = this->dutyLate - dutyTime;
            } else {
                this->driver->setActive(true);
            }
            this->periodLate += elapsedTime - period;
            this->periodStartTime = currentTime;
        }
    }
}
