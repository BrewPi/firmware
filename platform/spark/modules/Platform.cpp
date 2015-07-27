#include "Brewpi.h"
#include "application.h"
#if PLATFORM_ID==0
#include "ymodem/ymodem.h"
#endif
#include "flashee-eeprom.h"
#include "EepromManager.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

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

bool platform_init()
{            
    bool initialize = (EEPROM.read(0)!=EEPROM_MAGIC1 || EEPROM.read(1)!=EEPROM_MAGIC2);
    if (initialize) {
        
        eraseExternalFlash();
        
        EEPROM.write(0, EEPROM_MAGIC1);
        EEPROM.write(1, EEPROM_MAGIC2);
    }
    eepromAccess.init();
    return initialize;
}
