/**************************************************************************
* 
* Copyright 2014 by Petr Gargulak. eGUI Community.
* Copyright 2009-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3 
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the eGUI project give you
* permission to link the eGUI sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your 
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based 
* on this library. 
* If you modify the eGUI sources, you may extend this exception 
* to your version of the eGUI sources, but you are not obligated 
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************//*!
*
* @file      d4d_screen_settings.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - settings of d4d typical screen source file
*
******************************************************************************/

/****************************************************************************
*       Includes header files
****************************************************************************/
#include "d4d.h"
#include "main.h"
#include "fonts.h"
#include "strings.h"

/****************************************************************************
*       Local functions declarations
***************************************************************************/

//static void RadioButnLang_OnChange(D4D_OBJECT* pThis);
static void ComboBoxLang_OnChange(D4D_OBJECT* pThis, D4D_EVENTID eventId);
static void RadioButnScheme_OnChange(D4D_OBJECT* pThis);
static void RadioButnFonts_OnChange(D4D_OBJECT* pThis);
static void CheckBxScrSvrEnable_OnChange(D4D_OBJECT* pThis);
static void SliderScrSvrTime_OnChange(D4D_OBJECT * pThis);

static void BtnOkOnClick(D4D_OBJECT* pThis);

/****************************************************************************
*       Local variables declarations
***************************************************************************/

/*************************************
*   eGUI color schemes declaration
*************************************/

#define MY_COLOR_FINE_GREEN             D4D_COLOR_RGB(229, 255, 229)
#define MY_COLOR_FINE1_GREEN            D4D_COLOR_RGB(173, 255, 173)
#define MY_COLOR_FINE2_GREEN            D4D_COLOR_RGB(229, 255, 241)

#define MY_COLOR_DARK_GREEN             D4D_COLOR_RGB(75, 140, 75)

D4D_DECLARE_CLR_SCHEME(d4d_clr_scheme_green, \
        MY_COLOR_FINE_GREEN, D4D_COLOR_BLACK, MY_COLOR_DARK_GREEN, D4D_COLOR_WHITE, MY_COLOR_DARK_GREEN, MY_COLOR_FINE_GREEN,\
        MY_COLOR_FINE1_GREEN, MY_COLOR_FINE2_GREEN, MY_COLOR_FINE_GREEN, D4D_COLOR_WHITE,\
        MY_COLOR_DARK_GREEN, D4D_COLOR_GREY, D4D_COLOR_BLACK, D4D_COLOR_GREEN,\
        D4D_COLOR_WHITE, MY_COLOR_DARK_GREEN,\
        D4D_COLOR_SLDR_BAR_BCKG, D4D_COLOR_SLDR_BAR_FORE, D4D_COLOR_SLDR_BAR_START, D4D_COLOR_SLDR_BAR_END,\
        MY_COLOR_FINE_GREEN,\
        D4D_COLOR_GRAPH_GRID,\
        D4D_COLOR_PRGRS_BAR_BAR_BCKG, D4D_COLOR_PRGRS_BAR_BAR_FORE, D4D_COLOR_PRGRS_BAR_BAR_END\
        )

#define MY_COLOR_BLUE                   D4D_COLOR_RGB(73, 170, 255) 
#define MY_COLOR_FINE_BLUE              D4D_COLOR_RGB(224, 240, 255) 
#define MY_COLOR_FINE1_BLUE             D4D_COLOR_RGB(100, 255, 255) 
#define MY_COLOR_FINE2_BLUE             D4D_COLOR_RGB(200, 255, 255) 


D4D_DECLARE_CLR_SCHEME(d4d_clr_scheme_blue, \
        MY_COLOR_BLUE, D4D_COLOR_WHITE, MY_COLOR_FINE_BLUE, D4D_COLOR_BLACK, MY_COLOR_FINE_BLUE, MY_COLOR_BLUE,\
        MY_COLOR_FINE1_BLUE, MY_COLOR_FINE2_BLUE, MY_COLOR_FINE2_BLUE, D4D_COLOR_BLUE,\
        D4D_COLOR_DARK_BLUE, D4D_COLOR_BRIGHT_BLUE, D4D_COLOR_DARK_BLUE, D4D_COLOR_GREY,\
        D4D_COLOR_GAUG_HUB, D4D_COLOR_GAUG_POINTER,\
        D4D_COLOR_SLDR_BAR_BCKG, D4D_COLOR_SLDR_BAR_FORE, D4D_COLOR_SLDR_BAR_START, D4D_COLOR_SLDR_BAR_END,\
        D4D_COLOR_DARK_BLUE,\
        D4D_COLOR_GRAPH_GRID,\
        D4D_COLOR_PRGRS_BAR_BAR_BCKG, D4D_COLOR_PRGRS_BAR_BAR_FORE, D4D_COLOR_PRGRS_BAR_BAR_END\
        )

#define MY_COLOR_DARK_RED               D4D_COLOR_RGB(122, 39, 18) 
#define MY_COLOR_RED                    D4D_COLOR_RGB(255, 81, 38)
#define MY_COLOR_RED1                   D4D_COLOR_RGB(255, 106, 0)
#define MY_COLOR_FINE_RED               D4D_COLOR_RGB(255, 178, 127)
#define MY_COLOR_FINE_RED1              D4D_COLOR_RGB(255, 235, 221)

D4D_DECLARE_CLR_SCHEME(d4d_clr_scheme_red, \
        MY_COLOR_DARK_RED, D4D_COLOR_WHITE, MY_COLOR_RED, D4D_COLOR_WHITE, MY_COLOR_RED, D4D_COLOR_WHITE,\
        MY_COLOR_RED1, MY_COLOR_FINE_RED, MY_COLOR_RED, MY_COLOR_DARK_RED,\
        MY_COLOR_FINE_RED, MY_COLOR_FINE_RED1, MY_COLOR_FINE_RED, MY_COLOR_FINE_RED,\
        D4D_COLOR_GAUG_HUB, D4D_COLOR_GAUG_POINTER,\
        D4D_COLOR_SLDR_BAR_BCKG, D4D_COLOR_SLDR_BAR_FORE, D4D_COLOR_SLDR_BAR_START, D4D_COLOR_SLDR_BAR_END,\
        MY_COLOR_DARK_RED,\
        D4D_COLOR_GRAPH_GRID,\
        D4D_COLOR_PRGRS_BAR_BAR_BCKG, D4D_COLOR_PRGRS_BAR_BAR_FORE, D4D_COLOR_PRGRS_BAR_BAR_END\
        )

/****************************************************************************
*       D4D widgets (graphic objects) declarations
****************************************************************************/

#define SET_SCREEN_SIZE_X       432
#define SET_SCREEN_SIZE_Y       220

#define SET_OFFSET_X            4
#define SET_OFFSET_Y            4

#define SET_SIZE_GB_X           (SET_SCREEN_SIZE_X - 3*SET_OFFSET_X) / 2
#define SET_SIZE_GB_Y           120
#define SET_SIZE_GB_SMALL_Y     (SET_SCREEN_SIZE_Y - SET_POS_GB_Y(1) - SET_OFFSET_Y)
#define SET_SIZE_GB_SMALL_X     110

#define SET_POS_GB_X(x)         (SET_OFFSET_X + ((SET_SIZE_GB_X + SET_OFFSET_X) * (x)))
#define SET_POS_GB_Y(y)         (SET_OFFSET_Y + ((SET_SIZE_GB_Y + SET_OFFSET_Y) * (y))) 


#define SET_SIZE_RB_X           140
#define SET_SIZE_RB_SMALL_X     98
#define SET_SIZE_RB_Y           24

#define SET_SIZE_CB_X           180
#define SET_SIZE_CB_Y           30

#define SET_RB_POS_Y(y)         (SET_OFFSET_Y + (y) * SET_SIZE_RB_Y)

#define SET_BTN_SIZE_X          (SET_SCREEN_SIZE_X - SET_POS_GB_X(1) - SET_SIZE_GB_SMALL_X - 2*SET_OFFSET_X)
#define SET_BTN_SIZE_Y          SET_SIZE_GB_SMALL_Y

#define SET_BTN_POS_X           (SET_POS_GB_X(1) + SET_SIZE_GB_SMALL_X + SET_OFFSET_X)
#define SET_BTN_POS_Y           SET_POS_GB_Y(1)                   

#define SET_RADIUS              6

#define SET_BUTTON_FLAGS        (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_BEVEL_RAISED | D4D_BTN_F_3D)
#define D4D_SCRSET_F            (D4D_SCR_F_OUTLINE | D4D_SCR_F_TITLEBAR | D4D_SCR_F_BCKG | D4D_SCR_FINT_MOUSE_NORMAL)
#define SET_CHECKBOX_F          (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_BEVEL_DENTED | D4D_CHECKBOX_F_ICON_RECTANGLE | D4D_OBJECT_F_MOUSE_NORMAL)
#define SET_SLIDER_F            (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_BEVEL_DENTED | D4D_SLDR_F_AUTOTOUCH_OFF | D4D_OBJECT_F_MOUSE_NORMAL)

// Extern the parent group box objects
D4D_EXTERN_OBJECT(scrSett_grpBxLang)
D4D_EXTERN_OBJECT(scrSett_grpBxScheme)
D4D_EXTERN_OBJECT(scrSett_grpBxFonts)
D4D_EXTERN_OBJECT(scrSett_grpBxScrSvr)

// Declare the relations structures of group boxes
//D4D_DECLARE_OBJECT_RELATIONS_BEGIN(grpBxLang_Rel, &scrSett_grpBxLang)
//D4D_DECLARE_OBJECT_RELATIONS_END

D4D_DECLARE_OBJECT_RELATIONS_BEGIN(grpBxScheme_Rel, &scrSett_grpBxScheme)
D4D_DECLARE_OBJECT_RELATIONS_END

D4D_DECLARE_OBJECT_RELATIONS_BEGIN(grpBxFonts_Rel, &scrSett_grpBxFonts)
D4D_DECLARE_OBJECT_RELATIONS_END

D4D_DECLARE_OBJECT_RELATIONS_BEGIN(grpBxScrSvr_Rel, &scrSett_grpBxScrSvr)
D4D_DECLARE_OBJECT_RELATIONS_END

/*******************************************
* Language settings Group box 
*******************************************/
_D4D_DECLARE_COMBO_BOX_BEGIN(D4D_CONST, grpBxLang_CmbBx, SET_OFFSET_X, SET_RB_POS_Y(0), SET_SIZE_CB_X, SET_SIZE_CB_Y, 68, SET_RADIUS, NULL, &scrSett_grpBxLang, D4D_COMBO_BOX_F_DEFAULT, NULL, FONT_SYSTEM, 20, 0, 0, NULL, ComboBoxLang_OnChange, NULL)
  D4D_DECLARE_COMBO_BOX_ITEM(D4D_DEFSTR("English"), STRING_LANG_ENGLISH)
  D4D_DECLARE_COMBO_BOX_ITEM(D4D_DEFSTR("Cesky"), STRING_LANG_CZECH)
  D4D_DECLARE_COMBO_BOX_ITEM((D4D_TCHAR*)string_chinese_big5_D4D_STR_LANGCHI, STRING_LANG_CHINESE_BIG5)
D4D_DECLARE_COMBO_BOX_END()    

// Declare language Settings Group Box
D4D_DECLARE_STD_GROUP_BOX_BEGIN(scrSett_grpBxLang, SET_POS_GB_X(0), SET_POS_GB_Y(0), SET_SIZE_GB_X, SET_SIZE_GB_Y, SET_RADIUS, NULL, D4D_STR_LANG, FONT_SYSTEM_SMALL)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxLang_CmbBx) 
D4D_DECLARE_GROUP_BOX_OBJECT_END

/*******************************************
* Color scheme settings Group box 
*******************************************/
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxScheme_Grey, D4D_STR_SCHEME_GREY, SET_OFFSET_X, SET_RB_POS_Y(0), SET_SIZE_RB_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxScheme_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, NULL, RadioButnScheme_OnChange, NULL)
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxScheme_Green, D4D_STR_SCHEME_GREEN, SET_OFFSET_X, SET_RB_POS_Y(1), SET_SIZE_RB_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxScheme_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, &d4d_clr_scheme_green, RadioButnScheme_OnChange, NULL)
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxScheme_Blue, D4D_STR_SCHEME_BLUE, SET_OFFSET_X, SET_RB_POS_Y(2), SET_SIZE_RB_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxScheme_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, &d4d_clr_scheme_blue, RadioButnScheme_OnChange, NULL)
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxScheme_Red, D4D_STR_SCHEME_RED, SET_OFFSET_X, SET_RB_POS_Y(3), SET_SIZE_RB_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxScheme_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, &d4d_clr_scheme_red, RadioButnScheme_OnChange, NULL)


// Declare scheme Settings Group Box
D4D_DECLARE_STD_GROUP_BOX_BEGIN(scrSett_grpBxScheme, SET_POS_GB_X(1), SET_POS_GB_Y(0), SET_SIZE_GB_X, SET_SIZE_GB_Y, SET_RADIUS, NULL, D4D_STR_SCHEME, FONT_SYSTEM_SMALL)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxScheme_Grey)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxScheme_Green)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxScheme_Blue)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxScheme_Red)  
D4D_DECLARE_GROUP_BOX_OBJECT_END

/*******************************************
* Fonts settings Group box 
*******************************************/ 
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxFonts_Courier, D4D_STR_FONTS_COURIER, SET_OFFSET_X, SET_RB_POS_Y(0), SET_SIZE_RB_SMALL_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxFonts_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, NULL, RadioButnFonts_OnChange, NULL)
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxFonts_Arial, D4D_STR_FONTS_ARIAL, SET_OFFSET_X, SET_RB_POS_Y(1), SET_SIZE_RB_SMALL_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxFonts_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, &fontTable_Arial, RadioButnFonts_OnChange, NULL)

// Declare font Settings Group Box
D4D_DECLARE_STD_GROUP_BOX_BEGIN(scrSett_grpBxFonts, SET_POS_GB_X(1), SET_POS_GB_Y(1), SET_SIZE_GB_SMALL_X, SET_SIZE_GB_SMALL_Y, SET_RADIUS, NULL, D4D_STR_FONTS, FONT_SYSTEM_SMALL)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxFonts_Courier)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxFonts_Arial)  
D4D_DECLARE_GROUP_BOX_OBJECT_END

/*******************************************
* Screen saver settings Group box 
*******************************************/                      
_D4D_DECLARE_CHECKBOX(D4D_CONST, grpBxScrsvr_enable, D4D_STR_ENABLE, SET_OFFSET_X, SET_RB_POS_Y(0), SET_SIZE_RB_X, SET_SIZE_RB_Y, SET_RADIUS, NULL, grpBxScrSvr_Rel, SET_CHECKBOX_F, NULL, NULL, NULL, FONT_SYSTEM, NULL, CheckBxScrSvrEnable_OnChange, NULL)

D4D_TCHAR grpBxScrsvr_timeStr[16] = D4D_DEFSTR("30s");
_D4D_DECLARE_SLIDER(D4D_CONST, grpBxScrsvr_time, grpBxScrsvr_timeStr, SET_OFFSET_X, SET_RB_POS_Y(1) + SET_OFFSET_Y, (SET_SIZE_GB_X - 20), (SET_SIZE_RB_Y + 6), SET_RADIUS, NULL, grpBxScrSvr_Rel, 0, 0, 0, 0, 0, 0, SET_SLIDER_F, NULL, NULL, FONT_SYSTEM, NULL, SliderScrSvrTime_OnChange, NULL)
// Declare screen saver Settings Group Box
D4D_DECLARE_STD_GROUP_BOX_BEGIN(scrSett_grpBxScrSvr, SET_POS_GB_X(0), SET_POS_GB_Y(1), SET_SIZE_GB_X, SET_SIZE_GB_SMALL_Y, SET_RADIUS, NULL, D4D_STR_SCRSVR, FONT_SYSTEM_SMALL)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxScrsvr_enable)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxScrsvr_time)  
D4D_DECLARE_GROUP_BOX_OBJECT_END

// Declare OK button
D4D_DECLARE_RBUTTON(sSett_btnOk, D4D_STR_OK, SET_BTN_POS_X, SET_BTN_POS_Y, SET_BTN_SIZE_X, SET_BTN_SIZE_Y, SET_RADIUS, SET_BUTTON_FLAGS, NULL, NULL, NULL, FONT_SYSTEM_BIG, NULL, BtnOkOnClick, NULL)


/****************************************************************************
*       D4D screen declarations
****************************************************************************/
D4D_DECLARE_SCREEN_BEGIN(screen_settings, ScreenSettings_, 20 ,20, (SET_SCREEN_SIZE_X + 2), (SET_SCREEN_SIZE_Y + 20), D4D_STR_SETTINGS, FONT_SYSTEM, NULL, D4D_SCRSET_F, NULL)
  D4D_DECLARE_SCREEN_OBJECT(scrSett_grpBxLang)    
  D4D_DECLARE_SCREEN_OBJECT(scrSett_grpBxScheme)    
  D4D_DECLARE_SCREEN_OBJECT(scrSett_grpBxScrSvr)
  D4D_DECLARE_SCREEN_OBJECT(scrSett_grpBxFonts)
  D4D_DECLARE_SCREEN_OBJECT(sSett_btnOk)    
D4D_DECLARE_SCREEN_END()    


/****************************************************************************
*       D4D widgets static events functions definitions
****************************************************************************/
static void ComboBoxLang_OnChange(D4D_OBJECT* pThis, D4D_EVENTID eventId)
{
  if(eventId == D4D_EVENT_ONCLICK)
  {  
    //Change used language
    D4D_ChangeStringTable((LWord)D4D_ComboBoxGetItemUserData(pThis));
    
    //Invalidate screen to show change on screen
    D4D_InvalidateScreen(pThis->pData->pScreen, D4D_TRUE);
  }
}

static void RadioButnScheme_OnChange(D4D_OBJECT* pThis)
{
  // Change used color scheme
  D4D_SetDefaultScheme((D4D_CLR_SCHEME*)D4D_GetUserPointer(pThis));
  
  //Invalidate screen to show change on screen
  D4D_InvalidateScreen(pThis->pData->pScreen, D4D_TRUE);
}

static void RadioButnFonts_OnChange(D4D_OBJECT* pThis)
{
  // Change used font
  D4D_SetFontTable((D4D_FONT_TYPE*)D4D_GetUserPointer(pThis));
  
  //Invalidate screen to show change on screen
  D4D_InvalidateScreen(pThis->pData->pScreen, D4D_TRUE);
}


// Example of switching busy/normal cursor on slider - just as example
static void CheckBxScrSvrEnable_OnChange(D4D_OBJECT* pThis)
{
  if(D4D_CheckBoxGetValue(pThis))
    D4D_ObjectSetMouseCursor(&grpBxScrsvr_time, D4D_MOUSE_CURSOR_TYPE_NORMAL);
  else
    D4D_ObjectSetMouseCursor(&grpBxScrsvr_time, D4D_MOUSE_CURSOR_TYPE_BUSY);  
}

static void SliderScrSvrTime_OnChange(D4D_OBJECT * pThis)
{
  //Chenge the timeout for screen saver
  // there is 4 step 
  switch(D4D_SldrGetValue(pThis))
  {
    case 1:
      D4D_SetText(pThis, D4D_DEFSTR("10s"));
      screenSaverTimeoutPreset = 100;
      break;
    
    case 2:
      D4D_SetText(pThis, D4D_DEFSTR("30s"));
      screenSaverTimeoutPreset = 300;
      break;
    
    case 3:
      D4D_SetText(pThis, D4D_DEFSTR("1m"));
      screenSaverTimeoutPreset = 600;
      break;
    
    case 4:
      D4D_SetText(pThis, D4D_DEFSTR("10m"));
      screenSaverTimeoutPreset = 6000;
      break;
      
  }
  
}

// The OK button action
static void BtnOkOnClick(D4D_OBJECT* pThis)
{
  D4D_EscapeScreen();
}

/****************************************************************************
*       D4D standard screen functions definitions
****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenSettings_OnInit()
{
  const D4D_SLIDER_LIMITS sliderLimits = {1, 4, 1, 1};
  
  //D4D_RadioButtonSetValue(&grpBxLang_Eng, D4D_TRUE);
  D4D_RadioButtonSetValue(&grpBxScheme_Grey, D4D_TRUE);
  D4D_RadioButtonSetValue(&grpBxFonts_Courier, D4D_TRUE);
  D4D_CheckBoxSetValue(&grpBxScrsvr_enable, D4D_TRUE);
  
  D4D_SldrSetLimits(&grpBxScrsvr_time, &sliderLimits);
  D4D_SldrSetValue(&grpBxScrsvr_time, 2);
}



// Screen on Activate function called with each screen activation
static void ScreenSettings_OnActivate()
{
  
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenSettings_OnMain()
{
    
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenSettings_OnDeactivate()
{
    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenSettings_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}