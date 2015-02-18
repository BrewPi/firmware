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
* @file      d4d_screen_winmenu.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - winmenu of d4d typical screen source file
*
******************************************************************************/

#include "screen_common.h"

D4D_EXTERN_SCREEN(screen_log) // log screen
D4D_EXTERN_SCREEN(screen_about) // about screen
D4D_EXTERN_SCREEN(screen_entry) // entry screen
D4D_EXTERN_SCREEN(screen_touchtest) // touch test screen

/*****************************************************************************
*
* Graphic objects callback/events functions declaration
*
*
*****************************************************************************/
             
static void ScrWinMenu_OnClickMenu(D4D_OBJECT* pThis, D4D_MENU_INDEX ix);


/*****************************************************************************
*
* Graphic object declarations
*
*
*****************************************************************************/
D4D_DECLARE_STD_RMENU_BEGIN(scrWinMenu_menu, "Select any" , FONT_BERLIN_SANS_FBDEMI12, 0, 0, 190, 140, 8, FONT_ARIAL7, FONT_BERLIN_SANS_FBDEMI12, NULL, ScrWinMenu_OnClickMenu)
#if D4D_MCU_TYPE != D4D_HCS08
  D4D_DECLARE_MENU_ITEM_FULL("Log Screen", NULL, &screen_log)
#endif  
  D4D_DECLARE_MENU_ITEM_FULL("About", NULL, &screen_about)
  D4D_DECLARE_MENU_ITEM_FULL("Entry Screen",NULL, &screen_entry)
  D4D_DECLARE_MENU_ITEM_FULL("Touch Test", NULL, &screen_touchtest)
  D4D_DECLARE_MENU_ITEM("Dummy Item2", NULL)
  D4D_DECLARE_MENU_ITEM("Dummy Item3", NULL)
  D4D_DECLARE_MENU_ITEM("Dummy Item4", NULL)
  D4D_DECLARE_MENU_ITEM("Dummy Item5", NULL)
  D4D_DECLARE_MENU_ITEM_FULL("Exit", NULL, (void*)-1)
D4D_DECLARE_MENU_END(scrWinMenu_menu)

/*****************************************************************************
*
* eGUI/D4D screen declaration
*
*
*****************************************************************************/
// Screen declaration
D4D_DECLARE_RSCREEN_BEGIN(screen_winmenu, ScreenWinMenu_, 60, 40, 194, 169, 10, "Menu", FONT_BERLIN_SANS_FBDEMI12, &bmpFreescale, (D4D_SCR_F_BCKG | D4D_SCR_F_TITLEBAR | D4D_SCR_F_EXIT | D4D_SCR_F_BEVEL | D4D_SCR_F_TOUCHENABLE), NULL)
  D4D_DECLARE_SCREEN_OBJECT(scrWinMenu_menu)    
D4D_DECLARE_SCREEN_END()    

/*****************************************************************************
*
* Local function 
*
*
*****************************************************************************/
static void ScrWinMenu_OnClickMenu(D4D_OBJECT* pThis, D4D_MENU_INDEX ix)
{
  D4D_UNUSED(ix);
  
  if(D4D_MenuGetItemUserData(pThis))
  {
    if(D4D_MenuGetItemUserData(pThis) == (void*)-1)
      D4D_EscapeScreen();
    else
      D4D_ActivateScreen((D4D_SCREEN*)D4D_MenuGetItemUserData(pThis), D4D_FALSE);
  }
  
}

/*****************************************************************************
*
* Screen system functions 
*
*
*****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenWinMenu_OnInit()
{
  Log_AddScreenEvent("WinMenu", "OnInit");
}

// Screen on Activate function called with each screen activation
static void ScreenWinMenu_OnActivate()
{
  D4D_CaptureKeys(&scrWinMenu_menu);  
  Log_AddScreenEvent("WinMenu", "OnActivate");
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenWinMenu_OnMain()
{
    
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenWinMenu_OnDeactivate()
{
  Log_AddScreenEvent("WinMenu", "OnDeActivate");    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenWinMenu_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}



