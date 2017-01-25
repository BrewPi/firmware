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

#include "Brewpi.h"

// Most pins are only conditionally defined here, allowing definitions to be provided in Config.h for
// local overrides
enum {
    BREWPI_SHIELD_DIY,
    BREWPI_SHIELD_REV_A,
    BREWPI_SHIELD_REV_C,
    BREWPI_SHIELD_SPARK_V1,
    BREWPI_SHIELD_SPARK_V2,
    BREWPI_SHIELD_SPARK_V3,
};

#define BREWPI_BOARD_LEONARDO 'l'
#define BREWPI_BOARD_STANDARD 's'
#define BREWPI_BOARD_MEGA 'm'
#define BREWPI_BOARD_SPARKCORE 'x'
#define BREWPI_BOARD_PHOTON 'y'
#define BREWPI_BOARD_P1 'p'
#define BREWPI_BOARD_GCC 'z'
#define BREWPI_BOARD_UNKNOWN '?'

#ifdef __cplusplus
#include "OneWire.h"
#if !BREWPI_SIMULATE
extern OneWire primaryOneWireBus; // OneWire is only used in CPP files, causes build error in C files
#endif
#endif

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#define oneWirePin 0x0 // actually the i2c address

#if PLATFORM_ID == 8

// A7 is the leftmost pin. V1 does not have A7 on the green connector
#define PIN_ACTUATOR0 P1S0
#define PIN_ACTUATOR_BOTTOM1 P1S0
#define PIN_ACTUATOR1 P1S1
#define PIN_ACTUATOR_BOTTOM2 P1S1
#define PIN_ACTUATOR3 P1S5
#define PIN_ACTUATOR_TOP2 P1S5
#define PIN_ACTUATOR_TOP3 P1S4

#if !defined(SWD_JTAG_ENABLE)
#define PIN_ACTUATOR_TOP2_DIR D3
#define PIN_ACTUATOR_TOP1 D4
#define PIN_ACTUATOR2 D4
#define PIN_ACTUATOR_TOP1_DIR D5
#else
#define PIN_ACTUATOR_TOP2_DIR (-1)
#define PIN_ACTUATOR_TOP1 (-1)
#define PIN_ACTUATOR2 (-1)
#define PIN_ACTUATOR_TOP1_DIR (-1)
#endif

#if !defined(SWD_JTAG_ENABLE) && !defined(SWD_ENABLE)
#define PIN_12V_ENABLE D6
#define PIN_5V_ENABLE D7
#else
#define PIN_12V_ENABLE -1
#define PIN_5V_ENABLE -1
#endif


#define PIN_ALARM WKP

#define RS485_TX_EN DAC
#define RS485_TX TX
#define RS485_RX RX

#define PIN_TOUCH_CS P1S2
#define PIN_TOUCH_IRQ A1
#define PIN_LCD_CS A2
#define PIN_LCD_DC A0
#define PIN_SD_CS P1S4
#define PIN_LCD_BACKLIGHT D2


#else

// A7 is the leftmost pin. V1 does not have A7 on the green connector
#define PIN_ACTUATOR0 A7
#define PIN_ACTUATOR1 A6
#define PIN_ACTUATOR2 A1
#define PIN_ACTUATOR3 A0
#define PIN_ALARM A2

#define RS485_TX_EN D6
#define RS485_TX TX
#define RS485_RX RX

#define PIN_TOUCH_CS D3
#define PIN_TOUCH_IRQ D2
#define PIN_LCD_CS D4
#define PIN_LCD_DC D5
#define PIN_SD_CS D7

#endif

#define MAX_ACTUATOR_COUNT (4)

#define BREWPI_INVERT_ACTUATORS 0
// Spark Core shield has no digital input pins
#ifndef USE_INTERNAL_PULL_UP_RESISTORS
#define USE_INTERNAL_PULL_UP_RESISTORS 0
#endif


uint8_t getShieldVersion();

bool shieldIsV1();

void boardInit();

#ifdef __cplusplus
}
#endif
