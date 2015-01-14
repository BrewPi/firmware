#pragma once

#define ONEWIRE_DS2482

#if defined(ONEWIRE_DS2482)

#include "DS2482.h"

typedef DS2482 OneWireImpl;

#elif defined(ONEWIRE_PIN)

#include "OneWirePin.h"

typedef OneWirePin OneWireImpl;

#endif