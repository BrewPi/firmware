/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
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

#include "d4d.h"
#include "../fonts.h"
#include "../pictures.h"
#include "../brewpi-logo.h"
#include "../widget_color_scheme.h"
#include "connected_device_widget.h"
#include "spark_macros.h"
#include "ModeControl.h"
#include <string.h>
#include <stdbool.h>
#include "Board.h"

#define INACTIVE_BG_COLOR D4D_COLOR_RGB(24,24,24)
#define ACTIVE_BG_COLOR D4D_COLOR_RGB(140,0,25)
#define INACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(64,64,64)
#define ACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(160,160,160)
#define INACTIVE_FS_COLOR D4D_COLOR_RGB(128,128,128)
#define ACTIVE_FG_COLOR D4D_COLOR_RGB(255,255,255)

static D4D_BOOL actuator_views_state[MAX_ACTUATOR_COUNT];

const WIDGET_COLOR_SCHEME color_scheme_device = {
    ACTIVE_BG_COLOR,           // bkg active
    INACTIVE_BG_COLOR,            // bkg disable
    ACTIVE_BG_COLOR,           // focus
    ACTIVE_BG_COLOR,           // capture
    D4D_COLOR_RGB(192,192,192), // fg
    D4D_COLOR_RGB(128,128,128), // fg inactive
    D4D_COLOR_RGB(255,255,255),
    D4D_COLOR_RGB(255,255,255),
};

const WIDGET_COLOR_SCHEME color_scheme_connection = {
    ACTIVE_BG_COLOR,           // bkg active
    INACTIVE_BG_COLOR,            // bkg disable
    ACTIVE_BG_COLOR,           // focus
    ACTIVE_BG_COLOR,           // capture
    ACTIVE_FG_LOW_COLOR,         // fg
    INACTIVE_FG_LOW_COLOR,            // fg disabled
    ACTIVE_FG_LOW_COLOR,         // fg focus
    ACTIVE_FG_LOW_COLOR,         // fg capture
};

void ActuatorClicked(D4D_OBJECT* pThis);

void SetActuatorButtonState(const D4D_OBJECT* pThis, D4D_BOOL state, uint8_t idx)
{
    if(actuator_views_state[idx]==state){
        return; // already up to date
    }

    D4D_SetText(pThis, state ? "ON" : "OFF");

    D4D_COLOR bg, fg;
    if (D4D_IsEnabled(pThis)) {
        bg = state ? color_scheme_device.bckg : color_scheme_device.bckgDis;
        fg = state ? color_scheme_device.fore : color_scheme_device.foreDis;
    }
    else {
        bg = INACTIVE_BG_COLOR;
        fg = INACTIVE_FG_LOW_COLOR;
    }

    pThis->clrScheme->bckg = bg;
    pThis->clrScheme->bckgDis = bg;
    pThis->clrScheme->bckgCapture = bg;
    pThis->clrScheme->bckgFocus = bg;
    pThis->clrScheme->fore = fg;
    pThis->clrScheme->foreDis = fg;
    pThis->clrScheme->foreCapture = fg;
    pThis->clrScheme->foreFocus = fg;

    actuator_views_state[idx] = state;
    D4D_InvalidateObject(pThis, D4D_TRUE);
}

#define SCRBOOT_ACTUATOR_COUNT MAX_ACTUATOR_COUNT
#define SCRBOOT_ACTUATOR_CX_GAP 4
#define SCRBOOT_ACTUATOR_CX ((320-(SCRBOOT_ACTUATOR_COUNT*SCRBOOT_ACTUATOR_CX_GAP))/SCRBOOT_ACTUATOR_COUNT)

#define D4D_DECLARE_ACTUATOR(idx)\
     _D4D_DECLARE_ACTUATOR(scrDeviceTest_actuator##idx, idx, (idx*(SCRBOOT_ACTUATOR_CX+SCRBOOT_ACTUATOR_CX_GAP)), 190, SCRBOOT_ACTUATOR_CX, 50, FONT_ARIAL7, FONT_ARIAL7_BIG)

#define D4D_ACTUATOR_FLAGS (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_BTN_F_3D | D4D_OBJECT_F_BEVEL_RAISED | D4D_BTN_F_CONT_RECT )

#define _D4D_DECLARE_ACTUATOR2(name, idx, x, y, cx, cy, font_label, font_item) \
    D4D_EXTERN_OBJECT(name); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_label_relations, &name, NULL); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_item_relations, &name, NULL); \
    _D4D_DECLARE_LABEL(D4D_CONST, name##_label, "output", 0, 0, cx, cy/3, 0, NULL, name##_label_relations, D4D_LBL_F_DEFAULT, NULL, font_label, NULL, NULL); \
    _D4D_DECLARE_LABEL(D4D_CONST, name##_item, #idx, 0, cy/3, cx, cy*2/3, 0, NULL, name##_item_relations, D4D_LBL_F_DEFAULT, NULL, font_item, NULL, NULL); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_relations, NULL, &name##_label, &name##_item); \
    _D4D_DECLARE_BUTTON(D4D_CONST, name, NULL, x, y, cx, cy, 0, NULL, name##_relations, D4D_ACTUATOR_FLAGS, NULL, NULL, NULL, FONT_ARIAL7, NULL, ActuatorClicked, NULL);


#define _D4D_DECLARE_ACTUATOR(name, idx, x, y, cx, cy, font_label, font_item) \
    char name##text[5] = ""; \
    D4D_CLR_SCHEME name##scheme; \
    _D4D_DECLARE_BUTTON(D4D_CONST, name, name##text, x, y, cx, cy, 0, NULL, NULL, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH ), NULL, NULL, &name##scheme, FONT_ARIAL7_BIG, NULL, ActuatorClicked, NULL);


D4D_DECLARE_ACTUATOR(0);
D4D_DECLARE_ACTUATOR(1);
D4D_DECLARE_ACTUATOR(2);
D4D_DECLARE_ACTUATOR(3);

#define SCRBOOT_DEVICES_Y 65
#define SCRBOOT_DEVICES_CX 104
#define SCRBOOT_DEVICES_CX_GAP 4
#define SCRBOOT_DEVICES_CY 120
D4D_DECLARE_CDV_LIST(D4D_CONST, scrDeviceTest_devices0, 0, SCRBOOT_DEVICES_Y, SCRBOOT_DEVICES_CX, SCRBOOT_DEVICES_CY, FONT_ARIAL7_BIG, FONT_ARIAL7, &color_scheme_device, &color_scheme_connection);
D4D_DECLARE_CDV_LIST(D4D_CONST, scrDeviceTest_devices1, SCRBOOT_DEVICES_CX+SCRBOOT_DEVICES_CX_GAP, SCRBOOT_DEVICES_Y, SCRBOOT_DEVICES_CX, SCRBOOT_DEVICES_CY, FONT_ARIAL7_BIG, FONT_ARIAL7, &color_scheme_device, &color_scheme_connection);
D4D_DECLARE_CDV_LIST(D4D_CONST, scrDeviceTest_devices2, 2*(SCRBOOT_DEVICES_CX+SCRBOOT_DEVICES_CX_GAP), SCRBOOT_DEVICES_Y, SCRBOOT_DEVICES_CX, SCRBOOT_DEVICES_CY, FONT_ARIAL7_BIG, FONT_ARIAL7, &color_scheme_device, &color_scheme_connection);

// D4D_DECLARE_PICTURE(scrDeviceTest_bmpLogo, 0, 0, 68, 4, &bmp_brewpi_logo_black_68_48, D4D_OBJECT_F_ENABLED|D4D_OBJECT_F_VISIBLE, NULL, NULL);
D4D_DECLARE_STD_PICTURE(scrDeviceTest_bmpLogo, 16, 4, 72, 48, &bmp_brewpi_logo_black_72_48);

#define TEST_TEXT_X  108
#define TEST_TEXT_CY  12
D4D_DECLARE_STD_LABEL(scrDeviceTest_title, "BrewPi Hardware Test", TEST_TEXT_X, 0, 320-TEST_TEXT_X, 2*TEST_TEXT_CY, FONT_ARIAL7_BIG);
D4D_DECLARE_STD_LABEL(scrDeviceTest_text0, "Shows connected sensors and toggles outputs.", TEST_TEXT_X, TEST_TEXT_CY*2, 320-TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrDeviceTest_text1, "For instructions go to", TEST_TEXT_X, TEST_TEXT_CY*3, 320-TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrDeviceTest_text2, "http://brewpi.com/spark-getting-started", TEST_TEXT_X, TEST_TEXT_CY*4, 320-TEST_TEXT_X, TEST_TEXT_CY, FONT_ARIAL7);

D4D_DECLARE_STD_SCREEN_BEGIN(screen_devicetest, ScreenDeviceTest_)
	D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_actuator0)
	D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_actuator1)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_actuator2)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_actuator3)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_devices00)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_devices01)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_devices10)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_devices11)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_devices20)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_devices21)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_bmpLogo)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_title)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_text0)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_text1)
    D4D_DECLARE_SCREEN_OBJECT(scrDeviceTest_text2)
D4D_DECLARE_SCREEN_END()

uint8_t ActuatorCount()
{
    return shieldIsV2() ? 4 : 3;
}

void ScreenDeviceTest_OnInit()
{
    D4D_EnableObject((D4D_OBJECT*)&scrDeviceTest_actuator0, ActuatorCount()>3);

    actuator_views_state[0] = D4D_TRUE; // mismatch with below to force update on init
    actuator_views_state[1] = D4D_TRUE;
    actuator_views_state[2] = D4D_TRUE;
    actuator_views_state[3] = D4D_TRUE;
    SetActuatorButtonState((D4D_OBJECT*)&scrDeviceTest_actuator0, D4D_FALSE, 0);
    SetActuatorButtonState((D4D_OBJECT*)&scrDeviceTest_actuator1, D4D_FALSE, 1);
    SetActuatorButtonState((D4D_OBJECT*)&scrDeviceTest_actuator2, D4D_FALSE, 2);
    SetActuatorButtonState((D4D_OBJECT*)&scrDeviceTest_actuator3, D4D_FALSE, 3);


}
