
#include "d4d.h"
#include "fonts.h"
#include "brewpi-logo.h"
#include "connected_device_widget.h"

void ActuatorClicked(D4D_OBJECT* pThis);

//D4D_DECLARE_TXT_BUTTON(scrBoot_btnMenu, "Menu", BTN_POSX(0), BTN_POSY, BTN_SIZEX, BTN_SIZEY, FONT_BERLIN_SANS_FBDEMI12, ScrTouchtest_OnClickBtnMenu)

D4D_DECLARE_STD_LABEL(scrBoot_lblTitle, "Brewpi", 10, 10, 50, 20, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrBoot_lblVersion, "0.2.6", 10, 30, 50, 20, FONT_ARIAL7);
D4D_DECLARE_STD_PICTURE(scrBoot_bmpLogo, 10, 50, 50, 50, &bmp_brewpi_logo_48_30);


#define D4D_DECLARE_ACTUATOR(idx, font) \
    D4D_DECLARE_STD_CHECKBOX(scrBoot_actuator##idx, "pin " #idx, (idx*64), 0, 60, 56, NULL, NULL, font, ActuatorClicked);

D4D_DECLARE_ACTUATOR(1, FONT_ARIAL7_HIGH);
D4D_DECLARE_ACTUATOR(2, FONT_ARIAL7_HIGH);
D4D_DECLARE_ACTUATOR(3, FONT_ARIAL7_HIGH);

D4D_DECLARE_CDV_LIST(5, D4D_CONST, scrBoot_devices, 0, 60, 320, 180, FONT_ARIAL7_BIG, FONT_ARIAL7_BIG);

D4D_DECLARE_STD_SCREEN_BEGIN(screen_boot, ScreenBoot_)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_actuator1)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_actuator2)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_actuator3)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices0)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices1)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices2)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices3)
    D4D_DECLARE_SCREEN_OBJECT(scrBoot_devices4)
D4D_DECLARE_SCREEN_END()    

static void ScreenBoot_OnInit()
{
    
}

static void ScreenBoot_OnMain()
{
    
}

static void ScreenBoot_OnActivate()
{
    
}

static void ScreenBoot_OnDeactivate()
{
    
}

static Byte ScreenBoot_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    return 0;
}
