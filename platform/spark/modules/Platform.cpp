#include "Brewpi.h"
#include "Platform.h"
#include "application.h"
#include "deviceid_hal.h"
#include "EepromAccess.h"

void handleReset(bool exitFlag)
{ 
	if(exitFlag){
#if PLATFORM_ID==PLATFORM_GCC
		exit(0);
#else
		System.reset();
#endif
	}
}

#if PLATFORM_ID==PLATFORM_GCC
static uint8_t device_id[12];
#endif

void platform_init()
{            
#if PLATFORM_ID==PLATFORM_GCC
	HAL_device_ID(device_id, 12);
#endif

#if PLATFORM_THREADING
    System.on(setup_update, watchdogCheckin);
#endif
}

#if PLATFORM_THREADING
// Reset the system after 60 seconds if the application is unresponsive
// The timeout of 60 seconds will reset automatically each time loop() is called, or manually by calling wd.checkin()
ApplicationWatchdog appWatchdog(60000, System.reset);
#endif


