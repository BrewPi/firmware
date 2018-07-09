#include "Brewpi.h"
#include "Platform.h"
#include "application.h"
#include "deviceid_hal.h"
#include "EepromAccess.h"

void handleReset(bool exit)
{ 
	if(exit)
		System.reset();
}

#if PLATFORM_ID==PLATFORM_GCC
static uint8_t device_id[12];
#endif

bool platform_init()
{            
#if PLATFORM_ID==PLATFORM_GCC
	HAL_device_ID(device_id, 12);
#endif
	bool wasInitialized = eepromAccess.init();

#if PLATFORM_THREADING
    System.on(setup_update, watchdogCheckin);
#endif

    return wasInitialized;
}

#if PLATFORM_THREADING
// Reset the system after 60 seconds if the application is unresponsive
// The timeout of 60 seconds will reset automatically each time loop() is called, or manually by calling wd.checkin()
ApplicationWatchdog appWatchdog(60000, System.reset);
#endif

/**
 * In the cbox app, this is called as part of global construction, which is
 * too early for the gcc device to have feched the device id, so it's initialized
 * properly in platform_init() after the gcc command args have been parsed.
 */
void platform_device_id(data_block_ref& id)
{
#if PLATFORM_ID!=3
	id.data = (void*)ID1;
	id.size = 12;
#else
	id.data = device_id;
	id.size = 12;
#endif
}

