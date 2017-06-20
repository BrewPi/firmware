#include "Brewpi.h"
#include "Platform.h"
#include "application.h"
#include "deviceid_hal.h"
#if PLATFORM_ID==0
#include "Ymodem/Ymodem.h"
#endif
#include "EepromAccess.h"

void handleReset(bool exit)
{ 
	if(exit)
		System.reset();
}

void flashFirmware()
{
    System.firmwareUpdate(&Serial);
}


#define EEPROM_MAGIC1 (0xD0)
#define EEPROM_MAGIC2 (0x9E)

void eraseExternalFlash()
{
#if PLATFORM_ID==PLATFORM_SPARK_CORE
    Flashee::Devices::userFlash().eraseAll();    
#endif    
}


#if PLATFORM_ID==3
static uint8_t device_id[12];
#endif

bool platform_init()
{            
#if PLATFORM_ID==3
	HAL_device_ID(device_id, 12);
#endif

	bool initialize = (EEPROM.read(0)!=EEPROM_MAGIC1 || EEPROM.read(1)!=EEPROM_MAGIC2);
    if (initialize) {
        
        eraseExternalFlash();
        
        EEPROM.write(0, EEPROM_MAGIC1);
        EEPROM.write(1, EEPROM_MAGIC2);
    }
    eepromAccess.init();
#if PLATFORM_ID==3
    WiFi.connect();
    waitUntil(WiFi.ready);
#endif
    return initialize;
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

