#pragma once

#include <stdint.h>
#include "application.h"
#include "math.h"

#define WIRING 1

#define TWO_PI 6.283185307179586476925286766559

#define PRINTF_PROGMEM "%s"             // devices with unified address space

#define ONEWIRE_DS248X

typedef uint32_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint32_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;

bool platform_init();

struct data_block_ref {
	void* data;
	size_t size;
};


#if PLATFORM_THREADING
extern ApplicationWatchdog appWatchdog;
inline void watchdogCheckin(){
	appWatchdog.checkin();
}
#else
// define dummy watchdog checkin for when the watchdog is not available
inline void watchdogCheckin(){}
#endif

#define BREWPI_DS2413 1
#define BREWPI_DS2408 (PLATFORM_ID != 0)
#define BREWPI_USE_WIFI (PLATFORM_ID != 0)

/**
 * Retrieves a pointer to the device id.
 */
void platform_device_id(data_block_ref& id);

/**
 * Called to invoke a system reset.
 */
void handleReset(bool exit=true);
