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

#pragma once
#include "core_hal.h"
#include "gpio_hal.h"
#if PLATFORM_ID == 3
#include "pinmap_hal.h"
#else
#include "pinmap_impl.h"
#endif
#include <stdint.h>

// Most pins are only conditionally defined here, allowing definitions to be provided in Config.h for
// local overrides
enum class SparkVersion : uint8_t {
    V1,
    V2,
    V3,
};

#if !defined(PLATFORM_ID)
#error
#endif

#if PLATFORM_ID == 8 // P1
#define PIN_V3_BOTTOM1 P1S1
#define PIN_V3_BOTTOM2 P1S0
#define PIN_V3_TOP2 P1S5
#define PIN_V3_TOP3 P1S4

#if !defined(SWD_JTAG_ENABLE)
#define PIN_V3_TOP1 D4
#define PIN_V3_TOP1_DIR D3
#define PIN_V3_TOP2_DIR D5
#endif

#if !defined(SWD_JTAG_ENABLE) && !defined(SWD_ENABLE)
#define PIN_12V_ENABLE D6
#define PIN_5V_ENABLE D7
#endif

#define PIN_ALARM WKP

#define PIN_RS485_TX_EN DAC1
#define PIN_RS485_TX TX
#define PIN_RS485_RX RX

#define PIN_TOUCH_CS P1S2
#define PIN_TOUCH_IRQ A1
#define PIN_LCD_CS A2
#define PIN_LCD_DC A0
#define PIN_SD_CS P1S3
#define PIN_LCD_BACKLIGHT D2

#elif PLATFORM_ID == 6 // photon
// A7 is the leftmost pin. V1 does not have A7 on the green connector
#define PIN_ACTUATOR0 A7
#define PIN_ACTUATOR1 A6
#define PIN_ACTUATOR2 A1
#define PIN_ACTUATOR3 A0
#define PIN_ALARM A2

#define PIN_RS485_TX_EN D6
#define PIN_RS485_TX TX
#define PIN_RS485_RX RX

#define PIN_TOUCH_CS D3
#define PIN_TOUCH_IRQ D2
#define PIN_LCD_CS D4
#define PIN_LCD_DC D5
#define PIN_SD_CS D7

#elif PLATFORM_ID == 3 // simulation

#define PIN_V3_BOTTOM1 0
#define PIN_V3_BOTTOM2 1
#define PIN_V3_TOP1 2
#define PIN_V3_TOP2 3
#define PIN_V3_TOP3 4

#define PIN_V3_TOP1_DIR 5
#define PIN_V3_TOP2_DIR 6
#define PIN_ALARM 7
#endif

SparkVersion
getSparkVersion();

void
boardInit();
