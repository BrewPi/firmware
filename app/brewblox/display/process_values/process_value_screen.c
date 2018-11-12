/*
 * Copyright 2018 BrewBlox / BrewPi B.V.
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

#include "../fonts.h"
#include "../widget_color_scheme.h"
#include "d4d.h"
#include "process_value_widget.h"

#define SCR_PV_X0 65
#define SCR_PV_Y0 65
#define SCR_PV_CX 104
#define SCR_PV_CX_GAP 4
#define SCR_PV_CY 120
#define SCR_PV_CY_GAP 4
#define TEST_TEXT_X 108
#define TEST_TEXT_CY 12

D4D_DECLARE_PV(scrProcessValues00, SCR_PV_CX + 0 * (SCR_PV_CX + SCR_PV_CX_GAP), SCR_PV_CX, SCR_PV_Y0 + 0 * (SCR_PV_CY + SCR_PV_CY_GAP), SCR_PV_CY);
D4D_DECLARE_PV(scrProcessValues01, SCR_PV_CX + 1 * (SCR_PV_CX + SCR_PV_CX_GAP), SCR_PV_CX, SCR_PV_Y0 + 1 * (SCR_PV_CY + SCR_PV_CY_GAP), SCR_PV_CY);
D4D_DECLARE_PV(scrProcessValues02, SCR_PV_CX + 2 * (SCR_PV_CX + SCR_PV_CX_GAP), SCR_PV_CX, SCR_PV_Y0 + 0 * (SCR_PV_CY + SCR_PV_CY_GAP), SCR_PV_CY);
D4D_DECLARE_PV(scrProcessValues10, SCR_PV_CX + 0 * (SCR_PV_CX + SCR_PV_CX_GAP), SCR_PV_CX, SCR_PV_Y0 + 1 * (SCR_PV_CY + SCR_PV_CY_GAP), SCR_PV_CY);
D4D_DECLARE_PV(scrProcessValues11, SCR_PV_CX + 1 * (SCR_PV_CX + SCR_PV_CX_GAP), SCR_PV_CX, SCR_PV_Y0 + 0 * (SCR_PV_CY + SCR_PV_CY_GAP), SCR_PV_CY);
D4D_DECLARE_PV(scrProcessValues12, SCR_PV_CX + 2 * (SCR_PV_CX + SCR_PV_CX_GAP), SCR_PV_CX, SCR_PV_Y0 + 1 * (SCR_PV_CY + SCR_PV_CY_GAP), SCR_PV_CY);

extern char controller_wifi_ip[16];
D4D_DECLARE_COLOR_LABEL(scrProcessValues_wifi_state, controller_wifi_ip, 30, 53, 80, TEST_TEXT_CY, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, D4D_COLOR_GREY);
D4D_DECLARE_COLOR_LABEL(scrProcessValues_usb_state, "USB", 0, 53, 30, TEST_TEXT_CY, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, D4D_COLOR_GREY);

D4D_DECLARE_STD_LABEL(scrProcessValues_title, "BrewBlox Process Values", TEST_TEXT_X, 0, 320 - TEST_TEXT_X, 2 * TEST_TEXT_CY, FONT_ARIAL7_BIG);
D4D_DECLARE_STD_LABEL(scrProcessValues_text0, "Shows all process values on the controller.", TEST_TEXT_X, TEST_TEXT_CY * 2, 320 - TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrProcessValues_text1, "Add more rename and re-order", TEST_TEXT_X, TEST_TEXT_CY * 3, 320 - TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrProcessValues_text2, "with the BrewBlox UI", TEST_TEXT_X, TEST_TEXT_CY * 4, 320 - TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);

D4D_DECLARE_STD_SCREEN_BEGIN(screen_process_values, screenPv_)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues00)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues01)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues02)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues10)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues11)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues12)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues_wifi_state)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues_usb_state)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues_title)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues_text0)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues_text1)
D4D_DECLARE_SCREEN_OBJECT(scrProcessValues_text2)
D4D_DECLARE_SCREEN_END()
