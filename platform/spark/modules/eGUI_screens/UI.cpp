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
#include "../BrewPiTouch/BrewPiTouch.h"

#include "devicetest/device_test_screen.h"
#include "controller/controller_screen.h"
#include "startup/startup_screen.h"
#include "screen_model.h"

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

D4D_EXTERN_SCREEN(screen_controller);
D4D_EXTERN_SCREEN(screen_startup);


class StartupScreenModel {
    
    uint32_t timer;
    bool touched_flag;
    
public:    
    void start() {
        timer = millis();
        touched_flag = false;
    }
    
    uint32_t elapsed() {
        return millis()-timer;
    }
    
    bool timeout() {
        return elapsed()>4500;
    }
    
    bool touched() {
        return touched_flag;
    }
    
    void flagTouched() {
        touched_flag = true;
    }
    
};

void* screenModel = NULL;

void startup_screen_touched() {
    ((StartupScreenModel*)screenModel)->flagTouched();
}


void* active_screen_model()
{
    return screenModel;
}

extern BrewPiTouch touch;

uint32_t UI::showStartupPage()
{    
    // Check if touch screen has been calibrated
    if (!eGuiSettings.loadTouchCalib()) {
        // could not load valid settings from flash memory
        calibrateTouchScreen();
    }
    else {                
        StartupScreenModel model;
        screenModel = &model;
        model.start();
        touch.setStabilityThreshold(16000); // set to high Threshold to disable filter
        D4D_ActivateScreen(&screen_startup, D4D_TRUE);
        uint8_t c = 0;
        while (!model.touched() && !model.timeout()) {         
            ticks();
            uint32_t elapsed = model.elapsed();
            if (elapsed>2000) {
                uint8_t c2 = std::min(255lu, ((elapsed-2000)*255)/(3000));
                if (c!=c2) {
                    scrStartup_version.clrScheme->fore = D4D_COLOR_RGB(c2,c2,c2);
                    c = c2;
                    D4D_InvalidateObject(&scrStartup_version, D4D_FALSE);
                }
            }                
        }
        if (model.touched()){
            touch.setStabilityThreshold(5); // require extra stable reading
            calibrateTouchScreen();
        }
        touch.setStabilityThreshold(); // reset to default
        screenModel = NULL;
    }
    return 0;
}

/**
 * Show the main controller page. 
 */
void UI::showControllerPage() {
    D4D_ActivateScreen(&screen_controller, D4D_TRUE);
    D4D_Poll();
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
    // todo - how to forward the update to the right screen

}

/**
 * Show touch screen calibration screen store settings afterwards
 */
void UI::calibrateTouchScreen() {
    D4D_CalibrateTouchScreen();
    eGuiSettings.storeTouchCalib();
}