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

#include "ProcessValuesScreen.h"
#include "../screen.h"
#include "../widget_color_scheme.h"
#include <algorithm>

#define str(s) #s
#define xstr(s) str(s)

char scrPV_wifi_ip[32] = "0.0.0.0";

extern char controller_wifi_ip[16];

#define SCR_PV_X0 65
#define SCR_PV_Y0 65
#define SCR_PV_CX 104
#define SCR_PV_CX_GAP 4
#define SCR_PV_CY 120
#define SCR_PV_CY_GAP 4
#define TITLE_X 60
#define TITLE_Y 108
#define TITLE_CX 200
#define TITLE_CY 40
#define TEST_TEXT_CY 12

D4D_DECLARE_COLOR_LABEL(scrPV_wifi_state, controller_wifi_ip, 30, 53, 80, TEST_TEXT_CY, FONT_SMALL, D4D_NO_CONST, D4D_COLOR_BLACK, D4D_COLOR_GREY);
D4D_DECLARE_COLOR_LABEL(scrPV_usb_state, "USB", 0, 53, 30, TEST_TEXT_CY, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, D4D_COLOR_GREY);

D4D_DECLARE_STD_LABEL(scrPV_title, "BrewBlox Process Values", TITLE_X, TITLE_Y, TITLE_CX, TITLE_CY, FONT_04B03);
D4D_DECLARE_STD_LABEL(scrPV_title2, "BrewBlox Process Values", TITLE_X, TITLE_Y + 50, TITLE_CX, TITLE_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrPV_title3, "BrewBlox Process Values", TITLE_X, TITLE_Y + 100, TITLE_CX, TITLE_CY, FONT_TEWI);
D4D_DECLARE_STD_LABEL(scrPV_title4, "BrewBlox", TITLE_X, TITLE_Y - 50, TITLE_CX, TITLE_CY, FONT_TEWI_BIG);
/*D4D_DECLARE_STD_LABEL(scrPV_text0, "Shows all process values on the controller.", TEST_TEXT_X, TEST_TEXT_CY * 2, 320 - TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrPV_text1, "Add more rename and re-order", TEST_TEXT_X, TEST_TEXT_CY * 3, 320 - TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrPV_text2, "with the BrewBlox UI", TEST_TEXT_X, TEST_TEXT_CY * 4, 320 - TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
*/
D4D_DECLARE_STD_SCREEN_BEGIN(screen_process_values, scrPV_)
/*D4D_DECLARE_SCREEN_OBJECT(scrPV00)
D4D_DECLARE_SCREEN_OBJECT(scrPV01)
D4D_DECLARE_SCREEN_OBJECT(scrPV02)
D4D_DECLARE_SCREEN_OBJECT(scrPV10)
D4D_DECLARE_SCREEN_OBJECT(scrPV11)
D4D_DECLARE_SCREEN_OBJECT(scrPV12)*/
D4D_DECLARE_SCREEN_OBJECT(scrPV_wifi_state)
D4D_DECLARE_SCREEN_OBJECT(scrPV_usb_state)
D4D_DECLARE_SCREEN_OBJECT(scrPV_title)
D4D_DECLARE_SCREEN_OBJECT(scrPV_title2)
D4D_DECLARE_SCREEN_OBJECT(scrPV_title3)
D4D_DECLARE_SCREEN_OBJECT(scrPV_title4)
//D4D_DECLARE_SCREEN_OBJECT(scrPV_text0)
//D4D_DECLARE_SCREEN_OBJECT(scrPV_text1)
//D4D_DECLARE_SCREEN_OBJECT(scrPV_text2)
D4D_DECLARE_SCREEN_END()

void
ProcessValuesScreen::activate()
{
    D4D_ActivateScreen(&screen_process_values, D4D_TRUE);
}

void
scrPV_OnInit()
{
}

void
scrPV_OnMain()
{
}

void
scrPV_OnActivate()
{
}

void
scrPV_OnDeactivate()
{
}

uint8_t
scrPV_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    return D4D_FALSE; // don't block further processing
}
