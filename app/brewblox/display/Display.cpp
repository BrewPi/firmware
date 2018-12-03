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

#include "Display.h"

#include "startup/startup_screen.h"

extern "C" {
#include "d4d.h"
}

D4D_SCREEN* DisplayClass::onTimeoutScreen = nullptr;
D4D_SCREEN* DisplayClass::nextScreen = nullptr;
ticks_millis_t DisplayClass::screenStartTime = 0;
D4D_EXTERN_SCREEN(screen_startup);

DisplayClass::DisplayClass()
{
    D4D_Init(&screen_startup);
}

uint32_t
DisplayClass::showStartupPage()
{
    // Check if touch screen has been calibrated
    /*
    if (!eGuiSettings.loadTouchCalib()) {
        // could not load valid settings from flash memory
        calibrateTouchScreen();
    }

    uiController.beginStartup();
    ticks(); // render
    */
    return 5000;
}

void
DisplayClass::tick()
{
    D4D_TimeTickPut();
    D4D_CheckTouchScreen();
    D4D_Poll();
    D4D_FlushOutput();
}

void
DisplayClass::update(const ticks_millis_t& now)
{

    D4D_SCREEN* currentScreen = D4D_GetActiveScreen();
    D4D_SCREEN* newScreen = currentScreen; // (tempControl.getMode() == MODE_TEST) ? &screen_devicetest : &screen_controller;
    if (currentScreen != newScreen) {
        D4D_ActivateScreen(newScreen, D4D_TRUE);
    }
}
/*
#if PLATFORM_ID == 3
#define FREERTOS 0
#endif

#ifndef FREERTOS
#define FREERTOS 1
#endif

#if FREERTOS
Timer* timer = nullptr;
#endif

void
cancelCalibration()
{
#if FREERTOS
    D4D_InterruptCalibrationScreen();
#endif
}

void
createTimer()
{
#if FREERTOS
    timer = new Timer(60 * 1000, cancelCalibration);
    if (timer)
        timer->start();
#endif
}

void
destroyTimer()
{
#if FREERTOS
    delete timer;
    timer = NULL;
#endif
}

void
calibrateTouchScreen()
{

    createTimer();
    D4D_CalibrateTouchScreen();
    destroyTimer();
    eGuiSettings.storeTouchCalib();
}

bool
DisplayClass::inStartup()
{
    return uiController.inStartup();
}
*/