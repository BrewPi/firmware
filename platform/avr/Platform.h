#pragma once

#include "Arduino.h"
#undef byte // Please just use uint8_t
#undef boolean // Please just use bool

#define WIRING 1

#define arraySize(x) (sizeof(x)/sizeof(x[0]))

#define PRINTF_PROGMEM "%S"             // on arduino, use the special format symbol

#define ONEWIRE_PIN

typedef uint16_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint16_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;

bool platform_init();
