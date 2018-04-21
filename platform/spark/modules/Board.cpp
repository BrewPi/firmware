/*
 * Copyright 2015 BrewPi/Elco Jacobs/Matthew McGowan.
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

#include "Platform.h"
#include "Board.h"
#include "Ticks.h"

bool readAlarmPin(){
    pinMode(PIN_ALARM, INPUT);
    wait.millis(1); // give time to change
    bool result = digitalRead(PIN_ALARM);
    pinMode(PIN_ALARM, OUTPUT);
    return result;
}

uint8_t getShieldVersion(){

#if PLATFORM_ID == 8 // P1
    return BREWPI_SHIELD_SPARK_V3;
#else
    // V2 has a pull down resistor, V1 has a pull up resistor on the alarm pin
    // If the pin is low, it is V2
	static uint8_t shield = readAlarmPin() ? BREWPI_SHIELD_SPARK_V1 : BREWPI_SHIELD_SPARK_V2;
	return shield;
#endif
}

bool shieldIsV1(){
    return getShieldVersion() == BREWPI_SHIELD_SPARK_V1; // only test once and remember
}

void boardInit(){
#if PLATFORM_ID == 8 // P1, BrewPi Spark v3
    pinMode(PIN_ACTUATOR_BOTTOM1, OUTPUT);
    pinMode(PIN_ACTUATOR_BOTTOM2, OUTPUT);
    pinMode(PIN_ACTUATOR_TOP1, OUTPUT);
    pinMode(PIN_ACTUATOR_TOP2, OUTPUT);
    pinMode(PIN_ACTUATOR_TOP3, OUTPUT);

    digitalWrite(PIN_ACTUATOR_BOTTOM1, LOW);
    digitalWrite(PIN_ACTUATOR_BOTTOM2, LOW);
    digitalWrite(PIN_ACTUATOR_TOP1, LOW);
    digitalWrite(PIN_ACTUATOR_TOP2, LOW);
    digitalWrite(PIN_ACTUATOR_TOP3, LOW);

    pinMode(PIN_ACTUATOR_TOP1_DIR, OUTPUT);
    pinMode(PIN_ACTUATOR_TOP2_DIR, OUTPUT);
    digitalWrite(PIN_ACTUATOR_TOP1_DIR, HIGH); // set as output
    digitalWrite(PIN_ACTUATOR_TOP2_DIR, HIGH); // set as output

    pinMode(PIN_12V_ENABLE, OUTPUT);
    pinMode(PIN_5V_ENABLE, OUTPUT);
    // 5V on RJ12 enabled by default, 12V disabled to prevent damaging wrongly connected peripherals
    digitalWrite(PIN_5V_ENABLE, HIGH);
    // TODO: temporary until 12V can be toggled by software
    digitalWrite(PIN_12V_ENABLE, HIGH);

    digitalWrite(PIN_ALARM, LOW);
    pinMode(PIN_LCD_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_LCD_BACKLIGHT, HIGH);
#else
    pinMode(PIN_ACTUATOR1, OUTPUT);
    pinMode(PIN_ACTUATOR2, OUTPUT);
    pinMode(PIN_ACTUATOR3, OUTPUT);
    digitalWrite(PIN_ACTUATOR1, LOW);
    digitalWrite(PIN_ACTUATOR2, LOW);
    digitalWrite(PIN_ACTUATOR3, LOW);

    if (shieldIsV1()){
        digitalWrite(PIN_ALARM, HIGH); // alarm is inverted on V1
    }
    else{
        pinMode(PIN_ACTUATOR0, OUTPUT); // actuator 0 is not available on V1, but is on V2
        digitalWrite(PIN_ACTUATOR0, LOW);
    }
#endif
    pinMode(PIN_ALARM, OUTPUT);

    pinMode(PIN_RS485_TX, OUTPUT);
    pinMode(PIN_RS485_RX, INPUT);
    pinMode(PIN_RS485_TX_EN, OUTPUT);
    digitalWrite(PIN_RS485_TX_EN, LOW); // default to receive

    digitalWrite(PIN_TOUCH_CS, HIGH);
    pinMode(PIN_TOUCH_CS, OUTPUT);
    digitalWrite(PIN_LCD_CS, HIGH);
    pinMode(PIN_LCD_CS, OUTPUT);
    digitalWrite(PIN_SD_CS, HIGH);
    pinMode(PIN_SD_CS, OUTPUT);
    pinMode(PIN_LCD_DC, OUTPUT);
    pinMode(PIN_TOUCH_IRQ, INPUT);
}
