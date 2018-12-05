/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewBlox.
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

#include "BrewPiTouch.h"
#include "screen.h"
#include "spark_wiring_timer.h"
#include <algorithm>

extern BrewPiTouch touch;

#define xstr(s) str(s)
#define str(s) #s

char str_version[] = "BrewBlox " xstr(GIT_VERSION);
char str_text[] = "Tap screen to re-calibrate touch";

D4D_DECLARE_STD_LABEL(scrStartup_version, str_version, 140, 320, 32 * 7, 15, FONT_SMALL)
D4D_DECLARE_STD_LABEL(scrStartup_text, str_text, 160 - 16 * 7, 200, 32 * 7, 15, FONT_SMALL)

D4D_DECLARE_SCREEN_BEGIN(screen_startup, ScrStartup_, 0, 0, (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), nullptr, 0, nullptr, (D4D_SCR_F_DEFAULT | D4D_SCR_F_TOUCHENABLE), nullptr)
D4D_DECLARE_SCREEN_OBJECT(scrStartup_version)
D4D_DECLARE_SCREEN_OBJECT(scrStartup_text)
D4D_DECLARE_SCREEN_END()

class StartupScreen {
private:
    static uint8_t fade_color;

public:
    static void start()
    {
        fade_color = 0;
        D4D_ActivateScreen(&screen_startup, true);
    }

    static void end()
    {
    }

    static void update()
    {
        if (fade_color < 255) {
            ++fade_color;
            scrStartup_version.clrScheme->fore = D4D_COLOR_RGB(fade_color, fade_color, fade_color);
            D4D_InvalidateObject(&scrStartup_version, D4D_FALSE);
        }
    }

    static void calibrateTouchIfNeeded()
    {
        D4D_TOUCHSCREEN_CALIB calib;
        // TODO load calibration
        calib.ScreenCalibrated = 0; // temporary

        if (calib.ScreenCalibrated != 1) {
            calibrateTouch();
            // store calibration
            D4D_TCH_SetCalibration(calib);
        }
    }

    static void calibrateTouch()
    {
#if PLATFORM_ID != 3
        touch.setStabilityThreshold(5); // require extra stable reading
        auto timeoutTimer = Timer(20000, D4D_InterruptCalibrationScreen, true);
#endif
        D4D_CalibrateTouchScreen();
#if PLATFORM_ID != 3
        touch.setStabilityThreshold(); // reset to default
#endif
    }

    static uint8_t
    onMessage(D4D_MESSAGE* pMsg)
    {
        if (pMsg->nMsgId == D4D_MSG_TOUCHED) {
            calibrateTouch();
        }
        return 0;
    }
};

uint8_t StartupScreen::fade_color;

void
ScrStartup_OnInit()
{
}

void
ScrStartup_OnMain()
{
}

void
ScrStartup_OnActivate()
{
    StartupScreen::start();
}

void
ScrStartup_OnDeactivate()
{
}

uint8_t
ScrStartup_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    return StartupScreen::onMessage(pMsg);
}
