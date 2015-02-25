/*
 * Buzzer.cpp
 *
 * Copyright 2015 BrewPi.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Brewpi.h"
#include "Ticks.h"
#include "Pins.h"
#include "Buzzer.h"

#if BREWPI_BUZZER

#define BEEP_ON() digitalWrite(alarmPin, LOW);
#define BEEP_OFF() digitalWrite(alarmPin, HIGH);

void Buzzer::init(void) {
    // set up square wave PWM for buzzer
    pinMode(alarmPin, OUTPUT);
}

void Buzzer::setActive(bool active) {
    if (active != this->isActive()) {
        ValueActuator::setActive(active);
        if (active) {
            BEEP_ON();
        } else {
            BEEP_OFF();
        }
    }
}

void Buzzer::beep(uint8_t numBeeps, uint16_t duration) {
    for (uint8_t beepCount = 0; beepCount < numBeeps; beepCount++) {
        BEEP_ON();
        wait.millis(duration);
        BEEP_OFF();
        if (beepCount < numBeeps - 1) {
            wait.millis(duration); // not the last beep
        }
    }
}

Buzzer buzzer;

#endif