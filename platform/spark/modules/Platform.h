#pragma once

#include <stdint.h>
#include "application.h"
#include "math.h"

#define WIRING 1

#define strcpy_P strcpy
#define strlcpy_P strncpy
#define sprintf_P sprintf
#define strlen_P strlen
#define strcmp_P strcmp
#define memcpy_P memcpy
#define vsnprintf_P vsnprintf
#define PROGMEM
#define PSTR(x) (x)
#define pgm_read_byte(x)  (*(x))

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


#ifdef PLATFORM_THREADING
extern ApplicationWatchdog appWatchdog;
#else
// define dummy watchdog checkin for when the watchdog is not available
#define appWatchdog.checkin()
#endif


/**
 * Retrieves a pointer to the device id.
 */
void platform_device_id(data_block_ref& id);
