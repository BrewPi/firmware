/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan
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

/**
 * Do not change this file directly - rather edit Config.h
 */

#ifndef BREWPI_STATIC_CONFIG
#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_REV_C
#endif

/*
 * LCD Display using a shift register.
 * For diy-shields prior to the revA shield, this should be set to 0.
 */
#ifndef BREWPI_SHIFT_LCD	
#if BREWPI_STATIC_CONFIG != BREWPI_SHIELD_DIY
	#define BREWPI_SHIFT_LCD 1
#else
	#define BREWPI_SHIFT_LCD 0
#endif
#endif


#ifndef FAST_DIGITAL_PIN 
#define FAST_DIGITAL_PIN 0
#endif

/**
 * Enable DS2413 Actuators. 
 */
#ifndef BREWPI_DS2413
#define BREWPI_DS2413 0
#endif

/**
 * Enable the LCD display. Without this, a NullDisplay is used
 */
#ifndef BREWPI_LCD
#define BREWPI_LCD 1
#endif

#ifndef BREWPI_BUZZER
	#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_DIY
		#define BREWPI_BUZZER 0
	#else
		#define BREWPI_BUZZER 1
	#endif
#endif

#ifndef BREWPI_ROTARY_ENCODER
#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_DIY
	#define BREWPI_ROTARY_ENCODER 0
#else
	#define BREWPI_ROTARY_ENCODER 1
#endif	
#endif

// BREWPI_SENSOR_PINS - can be disabled if only using onewire devices
#ifndef BREWPI_SENSOR_PINS
#define BREWPI_SENSOR_PINS 1
#endif

// BREWPI_ACTUATOR_PINS - can be disabled if only using onewire devices
#ifndef BREWPI_ACTUATOR_PINS
#define BREWPI_ACTUATOR_PINS 1
#endif


#ifndef BREWPI_BOARD

#if !ARDUINO
        #define BREWPI_BOARD BREWPI_BOARD_UNKNOWN
#elif defined(__AVR_ATmega32U4__)
        #define BREWPI_BOARD BREWPI_BOARD_LEONARDO
#elif defined(__AVR_ATmega328P__)
        #define BREWPI_BOARD BREWPI_BOARD_STANDARD
#elif defined(__AVR_ATmega2560__)
        #define BREWPI_BOARD BREWPI_BOARD_MEGA
#else
        #error Unknown processor type!
        #define BREWPI_BOARD BREWPI_BOARD_UNKNOWN
#endif

#endif // ifndef BREWPI_BOARD

/*
 * Disable onewire crc table - it takes up 256 bytes of progmem.
 */
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 0
#endif

#ifndef ONEWIRE_PARASITE_SUPPORT
#define ONEWIRE_PARASITE_SUPPORT 0
#endif

#ifndef DS2413_SUPPORT_SENSE
#define DS2413_SUPPORT_SENSE 0
#endif


