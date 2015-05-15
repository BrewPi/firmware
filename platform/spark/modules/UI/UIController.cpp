
#include "UIController.h"
#include "TempControl.h"

extern "C" {
#include "d4d.h"
}
    
D4D_EXTERN_SCREEN(screen_startup);
D4D_EXTERN_SCREEN(screen_devicetest);
D4D_EXTERN_SCREEN(screen_controller);

void UIController::beginStartup()
{
    startupComplete = false;
    D4D_ActivateScreen(&screen_startup, D4D_TRUE);    
}

void UIController::updateScreen()
{
    if (!startupComplete)
        return;
    
    D4D_SCREEN* currentScreen = D4D_GetActiveScreen();
    D4D_SCREEN* newScreen = (tempControl.getMode()==MODE_TEST) ? &screen_devicetest : &screen_controller;
    if (currentScreen!=newScreen) {
        D4D_ActivateScreen(newScreen, D4D_TRUE);
    }
}   


