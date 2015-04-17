#pragma once

#include "Arduino.h"
#undef byte // Please just use uint8_t
#undef boolean // Please just use bool

#define WIRING 1

#define arraySize(x) (sizeof(x)/sizeof(x[0]))

#define PRINTF_PROGMEM "%S"             // on arduino, use the special format symbol

#define ONEWIRE_PIN