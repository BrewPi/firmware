/* 
 * File:   Platform.h
 * Author: Elco
 *
 * Created on 17 april 2015, 0:30
 */

#ifndef PLATFORM_H
#define	PLATFORM_H

#define WIRING 0

#define TWO_PI 6.283185307179586476925286766559

#define ONEWIRE_NULL

#include <stdio.h> // for vsnprintf
#include <stdint.h>

typedef uint32_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint32_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;

#define BREWPI_EMULATE 1
#define BREWPI_DS2413 0
#define BREWPI_DS2408 0
#define BREWPI_USE_WIFI 0

#define CONTROLBOX_WIRING 0

#endif	/* PLATFORM_H */

