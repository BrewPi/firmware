/*
 * Copyright 2015 BrewPi/Elco Jacobs/Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Brewpi.h"
#include "fixstl.h"
#include "UI.h"
#include "Buzzer.h"
#include "eGuiSettings.h"
#include "ConnectedDevicesManager.h"
#include "PiLink.h"

#include "brewpi_boot_screen.h"
#include "ConnectedDevicesView.h"

extern "C" {
#include "d4d.h"
}

eGuiSettingsClass eGuiSettings;

uint8_t UI::init() {
    if (!D4D_Init(NULL))
        return 1;
    
    D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
    #if BREWPI_BUZZER
	buzzer.init();
	buzzer.beep(2, 100);
    #endif
        
    return 0;
}

const D4D_OBJECT* views[] = { &scrTest_devices00, &scrTest_devices10, &scrTest_devices20, &scrTest_devices01, &scrTest_devices11, &scrTest_devices21 };
ConnectedDevicesManager mgr;
ConnectedDevicesPresenter presenter(&mgr, views, 6);

extern "C" void ActuatorClicked(D4D_OBJECT* pThis)
{
    int idx = -1;
    if (pThis==&scrTest_actuator1)
        idx = 0;
    else if (pThis==&scrTest_actuator2)
        idx = 1;
    if (pThis==&scrTest_actuator3)
        idx = 2;
    
    if (idx>=0) {
        Actuator* actuator = mgr.actuator(idx);
        bool active = !actuator->isActive();
        actuator->setActive(active);
        SetActuatorButtonState(pThis, active);
    }
}

uint32_t UI::showStartupPage()
{
    // Check if touch screen has been calibrated
    if (!eGuiSettings.loadTouchCalib()) {
        // could not load valid settings from flash memory
        calibrateTouchScreen();
    }
            
    D4D_ActivateScreen(&screen_boot, D4D_TRUE);
    D4D_Poll();
    return 0;
}

/**
 * Show the main controller page. 
 */
void UI::showControllerPage() {
    // for now we in fact show what will be the startup page.     
}

void ConnectedDeviceUpdate(ConnectedDevicesManager* mgr, int index, ConnectedDevice* device, ConnectedDeviceChange change) 
{
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

void UI::ticks()
{
    D4D_TimeTickPut();
    D4D_CheckTouchScreen();
    D4D_Poll();
    
    #if BREWPI_BUZZER
	buzzer.setActive(alarm.isActive() && !buzzer.isActive());
    #endif    
}    

void UI::update() 
{    
    static uint32_t last = 0;    
    uint32_t now = millis();
    if (now-last>=800) {
        last = now;
        mgr.update();
    }
}

/**
 * Show touch screen calibration screen store settings afterwards
 */
void UI::calibrateTouchScreen() {
    D4D_CalibrateTouchScreen();
    eGuiSettings.storeTouchCalib();
}