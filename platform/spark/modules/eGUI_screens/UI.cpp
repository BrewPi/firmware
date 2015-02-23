#include "Brewpi.h"
#include "UI.h"
#include "ConnectedDevicesManager.h"
#include "PiLink.h"

extern "C" {
#include "d4d.h"
}

#include "brewpi_boot_screen.h"


uint8_t UI::init() {
    if (!D4D_Init(&screen_boot))
        return 1;
    
    D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
    D4D_CheckTouchScreen();
    return 0;
}

uint32_t UI::showStartupPage()
{
    D4D_CalibrateTouchScreen();
    D4D_Poll();    
    return 0;
}
        
/**
 * Show the main controller page. 
 */
void UI::showControllerPage()
{
    // for now we in fact show what will be the startup page. 
    
}

void ConnectedDeviceUpdate(ConnectedDevicesManager* mgr, int index, ConnectedDevice* device, ConnectedDeviceChange change) {
    char buf[10];
    switch (change) {
        case ADDED:    
            piLink.debugMessage("sensor added at slot %d", index);
            break;
        case REMOVED:    
            piLink.debugMessage("sensor removed at slot %d", index);
            break;
        case UPDATED:
            tempToString(buf, device->value.temp, 3, 9);
            piLink.debugMessage("sensor updated at slot %d: %s", index, buf);
            break;
    }
}

ConnectedDevicesManager mgr(ConnectedDeviceUpdate);

void UI::update() 
{
    D4D_TimeTickPut();
    D4D_CheckTouchScreen();
    D4D_Poll();
    
    static uint32_t last = 0;
    
    uint32_t now = millis();
    if (now-last>=1000) {
        last = now;
        mgr.update();
    }
}


 