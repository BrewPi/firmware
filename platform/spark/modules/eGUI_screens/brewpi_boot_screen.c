
#include "d4d.h"
#include "fonts.h"
#include "brewpi-logo.h"


//D4D_DECLARE_TXT_BUTTON(scrBoot_btnMenu, "Menu", BTN_POSX(0), BTN_POSY, BTN_SIZEX, BTN_SIZEY, FONT_BERLIN_SANS_FBDEMI12, ScrTouchtest_OnClickBtnMenu)

D4D_DECLARE_STD_LABEL(scrBoot_lblTitle, "Brewpi", 10, 10, 50, 20, FONT_ARIAL7);
D4D_DECLARE_STD_LABEL(scrBoot_lblVersion, "0.2.6", 10, 30, 50, 20, FONT_ARIAL7);
D4D_DECLARE_STD_PICTURE(scrBoot_bmpLogo, 10, 50, 50, 50, &bmp_brewpi_logo_48_30);

D4D_DECLARE_STD_SCREEN_BEGIN(screen_boot, ScreenBoot_)
  D4D_DECLARE_SCREEN_OBJECT(scrBoot_lblTitle)
  D4D_DECLARE_SCREEN_OBJECT(scrBoot_lblVersion)          
  D4D_DECLARE_SCREEN_OBJECT(scrBoot_bmpLogo)
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
