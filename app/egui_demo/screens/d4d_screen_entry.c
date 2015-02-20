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
* @file      d4d_screen_entry.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - Entry screen of eGUI/D4D demo application
*
******************************************************************************/
#include "screen_common.h"

#define SCR_ENTRY_TIMEOUT_TICKS 100

static D4D_BOOL firstTime = D4D_TRUE;

/*****************************************************************************
*
* Graphic objects coordination specifications
*
*
*****************************************************************************/
D4D_EXTERN_SCREEN(screen_main)
D4D_EXTERN_SCREEN(screen_log)

// Declare tabulator table
D4D_DECLARE_TAB_TABLE_BEGIN(scrEntry_txtBxTabTable)
D4D_DECLARE_TAB(20)
D4D_DECLARE_TAB(50)
D4D_DECLARE_TAB_TABLE_END


const D4D_TCHAR scrEntry_txtBxText[] = 
{
"Features:\n"
"\t> Supports graphical color LCD displays of various sizes\n"
"\t> Very small RAM (volatile) memory footprint\n"
"\t> Multiple platform support\n"
"\t> Object style of driver\n"
"\t> Very smart-support-screen-oriented-structure-of-user code\n"
"\t> Custom screen sizes, position, and header like a window\n"
"\t> Objects:\n"
"\t\t>> Button\n"
"\t\t>> Check Box\n"
"\t\t>> Gauge\n"
"\t\t>> Icon\n"
"\t\t>> Label\n"
"\t\t>> Menu\n"
"\t\t>> Picture\n"
"\t\t>> Slider\n"
"\t\t>> Graph\n"
"\t\t>> Scroll Bar\n"
"\t\t>> Console\n"
"\t\t>> Text Box\n"
"\t\t>> Progress Bar\n"
"\t> Touch screen support\n"
"\t> Multiple font support\n"
"\t> Buffer for input keys\n"
};


/*****************************************************************************
*
* Graphic object declarations
*
*
*****************************************************************************/

#define SPACE		        2

#define LBLVER_SIZE_X		70
#define LBLVER_SIZE_Y		24

#define PRGRS_SIZE_X		(D4D_SCREEN_SIZE_LONGER_SIDE - LBLVER_SIZE_X - (3 * SPACE))

#define TEXT_BOX_SIZE_X		D4D_SCREEN_SIZE_LONGER_SIDE
#define TEXT_BOX_SIZE_Y 	(D4D_SCREEN_SIZE_SHORTER_SIDE - SPACE - LBLVER_SIZE_Y - 30)

#define PRGRS_POX_X	        SPACE
#define PRGRS_POX_Y	        (30 + TEXT_BOX_SIZE_Y + SPACE)

#define LBLVER_POS_X		(PRGRS_POX_X + PRGRS_SIZE_X + SPACE)
#define LBLVER_POS_Y		PRGRS_POX_Y

// Version string
D4D_DECLARE_RLABEL(scrEntry_lblTitle, "eGUI - D4D Graphic Driver", 0, 0, TEXT_BOX_SIZE_X, 30, 8, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED  | D4D_OBJECT_F_BEVEL_RAISED ), NULL, FONT_BERLIN_SANS_FBDEMI12, NULL, NULL)

// declare timout slider
D4D_DECLARE_STD_PROGRESS_BAR(scrEntry_prgrsBarTimeout,  PRGRS_POX_X, PRGRS_POX_Y, PRGRS_SIZE_X, LBLVER_SIZE_Y, 8)

// declare Features text box
D4D_DECLARE_RTEXTBOX(scrEntry_TxtBx, 0, 30, TEXT_BOX_SIZE_X, TEXT_BOX_SIZE_Y, 8, scrEntry_txtBxText, scrEntry_txtBxTabTable, D4D_TXTBX_F_DEFAULT, NULL, FONT_ARIAL7, NULL, NULL)

// Version string
D4D_DECLARE_RLABEL(scrEntry_lblVersion, D4D_VERSION, LBLVER_POS_X, LBLVER_POS_Y, LBLVER_SIZE_X, LBLVER_SIZE_Y, 8, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT), NULL, FONT_BERLIN_SANS_FBDEMI12, NULL, NULL)

/*****************************************************************************
*
* eGUI/D4D screen declaration
*
*
*****************************************************************************/
// Standard screen declaration

D4D_DECLARE_STD_SCREEN_BEGIN(screen_entry, ScreenEntry_)
  D4D_DECLARE_SCREEN_OBJECT(scrEntry_lblTitle)
  D4D_DECLARE_SCREEN_OBJECT(scrEntry_TxtBx)
  D4D_DECLARE_SCREEN_OBJECT(scrEntry_prgrsBarTimeout)
  D4D_DECLARE_SCREEN_OBJECT(scrEntry_lblVersion)
D4D_DECLARE_SCREEN_END()    


/*****************************************************************************
*
* Screen system functions 
*
*
*****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenEntry_OnInit()
{  
#if D4D_MCU_TYPE != D4D_HCS08
  D4D_InitScreen(&screen_log);
#endif
  
  D4D_SetTextProperties(&scrEntry_lblTitle, (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK));
  
  Log_AddScreenEvent("Entry", "OnInit");
}

// Screen on Activate function called with each screen activation
static void ScreenEntry_OnActivate()
{  
  //D4D_SLIDER_LIMITS tmp_limits = { 0, SCR_ENTRY_TIMEOUT_TICKS, 1 , 0};
  // Set up timout slider limits
  //D4D_SldrSetLimits(&scrEntry_sldrTimeout,&tmp_limits);
  //D4D_SldrSetValue(&scrEntry_sldrTimeout, 0);
  D4D_PrgrsBarSetValue(&scrEntry_prgrsBarTimeout, 0);
  Log_AddScreenEvent("Entry", "OnActivate");
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenEntry_OnMain()
{
  static unsigned long lastMillis = 0;
  if(millisC() - lastMillis >=  100)
  {
    lastMillis = millisC();
    // Check the end of count down
    //if(D4D_SldrGetValue(&scrEntry_sldrTimeout) >= SCR_ENTRY_TIMEOUT_TICKS)
    if(D4D_PrgrsBarGetValue(&scrEntry_prgrsBarTimeout) >= SCR_ENTRY_TIMEOUT_TICKS)
    {
      //Log_AddObjectEvent(&scrEntry_sldrTimeout, "Timeout overflow.");
      
      // On time overflow activate main screen and replace this one in screen history
      if(firstTime == D4D_TRUE)
      {        
        D4D_ActivateScreen(&screen_main, D4D_TRUE);
        firstTime = D4D_FALSE;
      }else
        D4D_EscapeScreen();
    }else
      // Do time count
      //D4D_SldrChangeValue(&scrEntry_sldrTimeout, 1);
      D4D_PrgrsBarChangeValue(&scrEntry_prgrsBarTimeout, 1);
    
  }
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenEntry_OnDeactivate()
{
  Log_AddScreenEvent("Entry", "OnDeActivate");    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenEntry_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}



