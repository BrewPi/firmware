/*
 * config_default.h
 *
 * Created: 06/05/2013 00:23:26
 *  Author: mat
 */ 


#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H


#ifndef BREWPI_ESTIMATOR_MESSAGES
#define BREWPI_ESTIMATOR_MESSAGES 0
#endif

#if BREWPI_ESTIMATOR_MESSAGES
	#define ESTIMATOR_MSG(msg, ...) piLink.debugMessage(PSTR(msg), __VA_ARGS__)
#else
	#define ESTIMATOR_MSG(msg,...)  {}
#endif

#ifndef BREWPI_STATIC_CONFIG
#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_REV_A
#endif


#ifdef BREWPI_EMULATE
#define BREWPI_EMULATE 0
#endif

#ifndef MULTICHAMBER 
#define MULTICHAMBER 0
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

#ifndef BREWPI_SIMULATE
#define BREWPI_SIMULATE 0
#endif

#ifndef BREWPI_DS2413
#define BREWPI_DS2413 0
#endif

#ifndef BREWPI_MENU
#define BREWPI_MENU 1
#endif

#ifndef BREWPI_LCD
#define BREWPI_LCD 1
#endif

#ifndef BREWPI_DISPLAY
#define BREWPI_DISPLAY 1
#endif

#ifndef BREWPI_BUZZER
#define BREWPI_BUZZER 1
#endif

#ifndef ENABLE_ROTARY_ENCODER
#define ENABLE_ROTARY_ENCODER 1
#endif

#endif