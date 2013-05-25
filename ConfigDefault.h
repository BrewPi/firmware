/*
 * config_default.h
 *
 * Created: 06/05/2013 00:23:26
 *  Author: mat
 */ 

#pragma once

/**
 * Do not change this file directly - rather edit ConfigLocal.h
 */

// Set verbosity of debug messages 0-3
#ifndef BREWPI_DEBUG
#define BREWPI_DEBUG 1
#endif

#ifndef BREWPI_ESTIMATOR_MESSAGES
#define BREWPI_ESTIMATOR_MESSAGES 0
#endif

#ifndef BREWPI_STATIC_CONFIG
#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_REV_C
#endif

/**
 * This flag virtualizes as much of the hardware as possible, so the code can be run in the AvrStudio simulator, which
 * only emulates the microcontroller, not any attached peripherals.
 */
#ifndef BREWPI_EMULATE
#define BREWPI_EMULATE 0
#endif

#ifndef TEMP_SENSOR_CASCADED_FILTER
#define TEMP_SENSOR_CASCADED_FILTER 1
#endif

#ifndef TEMP_CONTROL_STATIC
#define TEMP_CONTROL_STATIC 1
#endif

#ifndef FAST_DIGITAL_PIN 
#define FAST_DIGITAL_PIN 0
#endif

/**
 * Enable the simulator. Real sensors/actuators are replaced with simulated versions. In particular, the values reported by
 * temp sensors are based on a model of the fridge/beer.
 */
#ifndef BREWPI_SIMULATE
#define BREWPI_SIMULATE 0
#endif

/**
 * Enable DS2413 Actuators. 
 */
#ifndef BREWPI_DS2413
#define BREWPI_DS2413 0
#endif

/**
 * Enable the LCD menu.
 */
#ifndef BREWPI_MENU
#define BREWPI_MENU 1
#endif

/**
 * Enable the LCD display. Without this, a NullDisplay is used
 */
#ifndef BREWPI_LCD
#define BREWPI_LCD 1
#endif

#ifndef BREWPI_BUZZER
#define BREWPI_BUZZER 1
#endif

#ifndef ENABLE_ROTARY_ENCODER
#define ENABLE_ROTARY_ENCODER 1
#endif

#ifndef BREWPI_EEPROM_HELPER_COMMANDS
#define BREWPI_EEPROM_HELPER_COMMANDS BREWPI_DEBUG || BREWPI_SIMULATE
#endif

// BREWPI_SENSOR_PINS - can be disabled if only using onewire devices
#ifndef BREWPI_SENSOR_PINS
#define BREWPI_SENSOR_PINS 1
#endif

// BREWPI_ACTUATOR_PINS - can be disabled if only using onewire devices
#ifndef BREWPI_ACTUATOR_PINS
#define BREWPI_ACTUATOR_PINS 1
#endif

/** 
 * Enable debug messages from the temp control estimator.
 */
#if BREWPI_ESTIMATOR_MESSAGES
	#define ESTIMATOR_MSG(msg, ...) piLink.debugMessage(PSTR(msg), __VA_ARGS__)
#else
	#define ESTIMATOR_MSG(msg,...)  {}
#endif

#ifndef BREWPI_BOARD

#if defined(USBCON)
	#define BREWPI_BOARD "leonardo"
#else
	#define BREWPI_BOARD "standard"
#endif

#ifndef OPTIMIZE_GLOBAL
#define OPTIMIZE_GLOBAL 1
#endif


#endif


