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

#include "Board.h"
#include "delay_hal.h"

const uint8_t HIGH = 1;
const uint8_t LOW = 0;

bool
readAlarmPin()
{
    HAL_Pin_Mode(PIN_ALARM, INPUT);
#if PLATFORM_ID != 3
    HAL_Delay_Milliseconds(1); // give time to change
#endif
    bool result = HAL_GPIO_Read(PIN_ALARM);
    HAL_Pin_Mode(PIN_ALARM, OUTPUT);
    return result;
}

SparkVersion
getSparkVersion()
{

#if PLATFORM_ID == 8 || PLATFORM_ID == 3 // P1 or simulation
    return SparkVersion::V3;
#else
    // V2 has a pull down resistor, V1 has a pull up resistor on the alarm pin
    // If the pin is low, it is V2
    static SparkVersion version = readAlarmPin() ? SparkVersion::V1 : SparkVersion::V2;
    return version;
#endif
}

void
boardInit()
{
#if PLATFORM_ID == 8 || PLATFORM_ID == 3 // P1 or simulation
    HAL_Pin_Mode(PIN_V3_BOTTOM1, OUTPUT);
    HAL_Pin_Mode(PIN_V3_BOTTOM2, OUTPUT);
    HAL_Pin_Mode(PIN_V3_TOP1, OUTPUT);
    HAL_Pin_Mode(PIN_V3_TOP2, OUTPUT);
    HAL_Pin_Mode(PIN_V3_TOP3, OUTPUT);

    HAL_GPIO_Write(PIN_V3_BOTTOM1, LOW);
    HAL_GPIO_Write(PIN_V3_BOTTOM2, LOW);
    HAL_GPIO_Write(PIN_V3_TOP1, LOW);
    HAL_GPIO_Write(PIN_V3_TOP2, LOW);
    HAL_GPIO_Write(PIN_V3_TOP3, LOW);

#ifdef PIN_V3_TOP1_DIR
    HAL_Pin_Mode(PIN_V3_TOP1_DIR, OUTPUT);
    HAL_GPIO_Write(PIN_V3_TOP1_DIR, HIGH); // set as output
#endif

#ifdef PIN_V3_TOP2_DIR
    HAL_Pin_Mode(PIN_V3_TOP2_DIR, OUTPUT);
    HAL_GPIO_Write(PIN_V3_TOP2_DIR, HIGH); // set as output
#endif

#ifdef PIN_12V_ENABLE
    HAL_Pin_Mode(PIN_12V_ENABLE, OUTPUT);
    // TODO: temporary until 12V can be toggled by software
    HAL_GPIO_Write(PIN_12V_ENABLE, HIGH);
#endif

#ifdef PIN_5V_ENABLE
    HAL_Pin_Mode(PIN_5V_ENABLE, OUTPUT);
    // 5V on RJ12 enabled by default, 12V disabled to prevent damaging wrongly connected peripherals
    HAL_GPIO_Write(PIN_5V_ENABLE, HIGH);
#endif

    HAL_GPIO_Write(PIN_ALARM, LOW);
    HAL_Pin_Mode(PIN_LCD_BACKLIGHT, OUTPUT);
    HAL_GPIO_Write(PIN_LCD_BACKLIGHT, HIGH);

#elif PLATFORM_ID == 6
    HAL_Pin_Mode(PIN_ACTUATOR1, OUTPUT);
    HAL_Pin_Mode(PIN_ACTUATOR2, OUTPUT);
    HAL_Pin_Mode(PIN_ACTUATOR3, OUTPUT);
    HAL_GPIO_Write(PIN_ACTUATOR1, LOW);
    HAL_GPIO_Write(PIN_ACTUATOR2, LOW);
    HAL_GPIO_Write(PIN_ACTUATOR3, LOW);

    if (shieldIsV1()) {
        HAL_GPIO_Write(PIN_ALARM, HIGH); // alarm is inverted on V1
    } else {
        HAL_Pin_Mode(PIN_ACTUATOR0, OUTPUT); // actuator 0 is not available on V1, but is on V2
        HAL_GPIO_Write(PIN_ACTUATOR0, LOW);
    }
#endif

    HAL_Pin_Mode(PIN_ALARM, OUTPUT);

    HAL_Pin_Mode(PIN_RS485_TX, OUTPUT);
    HAL_Pin_Mode(PIN_RS485_RX, INPUT);
    HAL_Pin_Mode(PIN_RS485_TX_EN, OUTPUT);

    HAL_GPIO_Write(PIN_TOUCH_CS, HIGH);
    HAL_Pin_Mode(PIN_TOUCH_CS, OUTPUT);
    HAL_GPIO_Write(PIN_LCD_CS, HIGH);
    HAL_Pin_Mode(PIN_LCD_CS, OUTPUT);
    HAL_GPIO_Write(PIN_SD_CS, HIGH);
    HAL_Pin_Mode(PIN_SD_CS, OUTPUT);
    HAL_Pin_Mode(PIN_LCD_DC, OUTPUT);
    HAL_Pin_Mode(PIN_TOUCH_IRQ, INPUT);
}
