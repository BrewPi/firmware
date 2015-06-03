
#include "SparkEepromRegions.h"
#include "EepromAccessImpl.h"

void SparkEepromAccess::init() 
{
#if PLATFORM_ID==0
    flash = Flashee::Devices::createAddressErase(4096*EEPROM_CONTROLLER_START_BLOCK, 4096*EEPROM_CONTROLLER_END_BLOCK);       
#elif PLATFORM_ID==6
    flash = Flashee::Devices::createEepromDevice(EEPROM_CONTROLLER_START_BLOCK, EEPROM_CONTROLLER_END_BLOCK);
#else
#error Unknown Platform ID        
#endif        
}

