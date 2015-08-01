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
#include "Display.h"
#include "UIController.h"
#include "Actuator.h"
#include "Board.h"

#include "../eGUI_screens/devicetest/device_test_screen.h"
#include "../eGUI_screens/controller/controller_screen.h"
#include "../eGUI_screens/startup/startup_screen.h"
#include "../eGUI_screens/screen_model.h"

extern "C" {
#include "d4d.h"
}

DisplayType realDisplay;
DisplayType DISPLAY_REF display = realDisplay;


eGuiSettingsClass eGuiSettings;

uint8_t UI::init() {
    eGuiSettings.init();
    display.init();

    if (!D4D_Init(NULL))
        return 1;

    D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
    #if BREWPI_BUZZER
	buzzer.init(!shieldIsV2());
	buzzer.beep(2, 100);
    #endif

    return 0;
}

uint32_t UI::showStartupPage()
{
    // Check if touch screen has been calibrated
    if (!eGuiSettings.loadTouchCalib()) {
        // could not load valid settings from flash memory
        calibrateTouchScreen();
    }

    uiController.beginStartup();
    return 0;
}

/**
 * Since the startup page waits for the user, it has variable duration. This allows
 * the main loop to continue running while the startup screen is displayed.
 */
void UI::showControllerPage() {
    display.printStationaryText();
    display.printState();
}

extern ValueActuator alarm;
void UI::ticks()
{
    D4D_TimeTickPut();
    D4D_CheckTouchScreen();
    D4D_Poll();

    #if BREWPI_BUZZER
	buzzer.setActive(alarm.isActive() && !buzzer.isActive());
    #endif
}

UIController uiController;

void UI::update()
{
    uiController.updateScreen();

    display.printState();
    display.printAllTemperatures();
    display.printMode();
    display.updateBacklight();
}

/**
 * Show touch screen calibration screen store settings afterwards
 */
void calibrateTouchScreen() {
    D4D_CalibrateTouchScreen();
    eGuiSettings.storeTouchCalib();
}

bool UI::inStartup() {
    return uiController.inStartup();
}
