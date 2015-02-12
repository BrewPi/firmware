#include "Brewpi.h"
#include "PiLinkHandlers.h"
#include "ymodem/ymodem.h"

void handleReset() 
{ 
	System.reset();
}

void flashFirmware()
{
    System.serialFirmwareUpdate(&Serial);
}

