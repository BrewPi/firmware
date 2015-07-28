/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2015 Matthew McGowan
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
#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_SPARK_V1
#endif

#ifndef FAST_DIGITAL_PIN 
#define FAST_DIGITAL_PIN 0
#endif

/**
 * Enable DS2413 Actuators. 
 */
#ifndef BREWPI_DS2413
#define BREWPI_DS2413 0 // TODO
#endif

#ifndef BREWPI_BUZZER
    #define BREWPI_BUZZER 1
#endif

// BREWPI_SENSOR_PINS - Only OneWire devices and digital outputs on the spark shield
#ifndef BREWPI_SENSOR_PINS
#define BREWPI_SENSOR_PINS 0
#endif

// BREWPI_ACTUATOR_PINS - can be disabled if only using onewire devices
#ifndef BREWPI_ACTUATOR_PINS
#define BREWPI_ACTUATOR_PINS 1
#endif


#ifndef BREWPI_BOARD
#if PLATFORM_ID==0
    #define BREWPI_BOARD BREWPI_BOARD_SPARKCORE
#elif PLATFORM_ID==6
    #define BREWPI_BOARD BREWPI_BOARD_PHOTON
#else
#error Unknown Platform ID
#endif
#endif

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


