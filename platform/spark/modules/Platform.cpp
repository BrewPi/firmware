#include "Brewpi.h"
#include "PiLinkHandlers.h"
#include "ymodem/ymodem.h"

SYSTEM_MODE(SEMI_AUTOMATIC)

void handleReset() 
{ 
	System.reset();
}

void flashFirmware()
{
    System.serialFirmwareUpdate(&Serial);
}

