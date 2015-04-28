#include "ActuatorPwm.h"
#include "Ticks.h"

ActuatorPwm::ActuatorPwm(Actuator * driver, uint8_t pwm = 0) {
    this->driver = driver;
    this->pwm = pwm;
    // Accumulate time errors to adjust for being late in next cycle
    this->dutyError = 0;
    this->periodError = 0;
    this->periodStartTime = 0;
    this->driver->setActive(false);
}

uint8_t ActuatorPwm::getPwm() {
    return this->pwm;
}

void ActuatorPwm::setPwm(uint8_t val) {
    this->pwm = val;
}

void ActuatorPwm::updatePwm() {
    ticks_millis_t dutyTime = (this->pwm * period) / 255 - dutyError;
    ticks_millis_t periodTime = period - periodError;
    ticks_millis_t currentTime = ticks.millis();
    ticks_millis_t elapsedTime = currentTime - this->periodStartTime;

    if (elapsedTime >= periodTime) {
        if (!this->driver->isActive()) {
            this->driver->setActive(true);
            this->periodError = (elapsedTime - periodTime)%period;
            this->periodStartTime = currentTime;
        }
    } else if (elapsedTime >= dutyTime) {
        if (this->driver->isActive()) {
            this->driver->setActive(false);
            this->dutyError = (elapsedTime - dutyTime)%dutyTime;
        }
    }
}
