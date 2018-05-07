#pragma once

#define WIRING 0

#define TWO_PI 6.283185307179586476925286766559

#define ONEWIRE_NULL

#include <stdio.h> // for vsnprintf
#include <stdint.h>

#define BREWPI_EMULATE 1
#define BREWPI_DS2413 0
#define BREWPI_DS2408 0
#define BREWPI_USE_WIFI 0

bool platform_init();

struct data_block_ref {
    void* data;
    size_t size;
};

void platform_device_id(data_block_ref& id);

inline void handleReset(bool){};
