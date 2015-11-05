/* 
 * File:   Platform.h
 * Author: Elco
 *
 * Created on 17 april 2015, 0:30
 */

#ifndef PLATFORM_H
#define	PLATFORM_H

#define WIRING 0

#define strcpy_P strcpy
#define strlcpy_P strncpy
#define sprintf_P sprintf
#define strcmp_P strcmp
#define memcpy_P memcpy
#define vsnprintf_P vsnprintf
#define PROGMEM
#define PSTR(x) x
#define pgm_read_byte(x)  (*(x))

#define TWO_PI 6.283185307179586476925286766559

#define PRINTF_PROGMEM "%s"             // devices with unified address space

#define ONEWIRE_NULL

#include <stdio.h> // for vsnprintf
#include <stdint.h>

typedef uint32_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint32_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;

#include "ConfigDefault.h"

#endif	/* PLATFORM_H */

