
#include "d4d.h"
#include "fonts.h"
#include "pictures.h"
#include "brewpi-logo.h"
#include "connected_device_widget.h"
#include "widget_color_scheme.h"
#include "spark_macros.h"
#include "ModeControl.h"
#include <string.h>

#define INACTIVE_BG_COLOR D4D_COLOR_RGB(24,24,24)
#define ACTIVE_BG_COLOR D4D_COLOR_RGB(44,90,160)
#define INACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(64,64,64)
#define ACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(160,160,160)
#define INACTIVE_FS_COLOR D4D_COLOR_RGB(128,128,128)
#define ACTIVE_FG_COLOR D4D_COLOR_RGB(255,255,255)


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
void SetActuatorButtonState(D4D_OBJECT* pThis, D4D_BOOL state)
{
    D4D_SetText(pThis, state ? "ON" : "OFF");
    D4D_COLOR bg = state ? color_scheme_device.bckg : color_scheme_device.bckgDis;
    D4D_COLOR fg = state ? color_scheme_device.fore : color_scheme_device.foreDis;
    
    pThis->clrScheme->bckg = bg;
    pThis->clrScheme->bckgDis = bg;
    pThis->clrScheme->bckgCapture = bg;
    pThis->clrScheme->bckgFocus = bg;
    pThis->clrScheme->fore = fg;
    pThis->clrScheme->foreDis = fg;
    pThis->clrScheme->foreCapture = fg;
    pThis->clrScheme->foreFocus = fg;

    D4D_InvalidateObject(pThis, D4D_TRUE);
}

D4D_DECLARE_STD_LABEL(scrBoot_lblTitle, "Brewpi", 10, 10, 50, 20, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrBoot_lblVersion, "0.2.6", 10, 30, 50, 20, FONT_ARIAL7);
D4D_DECLARE_STD_PICTURE(scrBoot_bmpLogo, 10, 50, 50, 50, &bmp_brewpi_logo_48_30);

#define D4D_DECLARE_ACTUATOR(idx)\
     _D4D_DECLARE_ACTUATOR(scrBoot_actuator##idx, idx, 25+((2-(idx-1))*90), 320-50, 86, 50, FONT_ARIAL7, FONT_ARIAL7_BIG)

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


D4D_DECLARE_ACTUATOR(1);
D4D_DECLARE_ACTUATOR(2);
D4D_DECLARE_ACTUATOR(3);

#define SCRBOOT_DEVICES_Y 0
#define SCRBOOT_DEVICES_CX 104
#define SCRBOOT_DEVICES_CY 150 
#define SCRBOOT_DEVICES_CX_GAP 4
D4D_DECLARE_CDV_LIST(D4D_CONST, scrBoot_devices0, 0, SCRBOOT_DEVICES_Y, SCRBOOT_DEVICES_CX, SCRBOOT_DEVICES_CY, FONT_ARIAL7_BIG, FONT_ARIAL7, &color_scheme_device, &color_scheme_connection);
D4D_DECLARE_CDV_LIST(D4D_CONST, scrBoot_devices1, SCRBOOT_DEVICES_CX+SCRBOOT_DEVICES_CX_GAP, SCRBOOT_DEVICES_Y, SCRBOOT_DEVICES_CX, SCRBOOT_DEVICES_CY, FONT_ARIAL7_BIG, FONT_ARIAL7, &color_scheme_device, &color_scheme_connection);
D4D_DECLARE_CDV_LIST(D4D_CONST, scrBoot_devices2, 2*(SCRBOOT_DEVICES_CX+SCRBOOT_DEVICES_CX_GAP), SCRBOOT_DEVICES_Y, SCRBOOT_DEVICES_CX, SCRBOOT_DEVICES_CY, FONT_ARIAL7_BIG, FONT_ARIAL7, &color_scheme_device, &color_scheme_connection);

D4D_DECLARE_STD_SCREEN_BEGIN(screen_boot, ScreenBoot_)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_actuator1)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_actuator2)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_actuator3)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices00)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices01)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices10)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices11)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices20)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices21)
D4D_DECLARE_SCREEN_END()    

static void ScreenBoot_OnInit()
{
/*
    for (int l=0; l<arraySize(devices_colors); l++) {       // for each list
        for (int c=0; c<arraySize(devices_colors[0]); c++) {    // for each column            
            memcpy(&devices_colors[l][c][0], &color_scheme_device, sizeof(color_scheme_device));
            memcpy(&devices_colors[l][c][1], &color_scheme_device, sizeof(color_scheme_device));
            memcpy(&devices_colors[l][c][2], &color_scheme_device, sizeof(color_scheme_connection));
        }
    }        
*/    
    SetActuatorButtonState((D4D_OBJECT*)&scrBoot_actuator1, D4D_FALSE);
    SetActuatorButtonState((D4D_OBJECT*)&scrBoot_actuator2, D4D_FALSE);
    SetActuatorButtonState((D4D_OBJECT*)&scrBoot_actuator3, D4D_FALSE);
}

static void ScreenBoot_OnMain()
{
    
}

control_mode_t prev_mode;
static void ScreenBoot_OnActivate()
{
    prev_mode = ModeControl_SetMode(MODE_TEST);
}

static void ScreenBoot_OnDeactivate()
{
    if (ModeControl_GetMode()==MODE_TEST)
        ModeControl_SetMode(prev_mode);
}

static Byte ScreenBoot_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    return 0;
}
