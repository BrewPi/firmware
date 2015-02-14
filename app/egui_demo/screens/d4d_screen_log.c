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
* @file      d4d_screen_log.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - log screen source file
*
******************************************************************************/

#include "d4d_screen_log.h"

static void Log_PrintTime(void);
static void Log_PrintObjectName(D4D_OBJECT* pObj);

static D4D_TCHAR* newLine = "\n\r";
/*****************************************************************************
*
* Graphic object declarations
*
*
*****************************************************************************/
D4D_DECLARE_STD_RCONSOLE(scrLog_CnslLog, 0, 0, 276, 171, 8, 50, 40, FONT_ARIAL7)


/*****************************************************************************
*
* eGUI/D4D screen declaration
*
*
*****************************************************************************/
// Standard screen declaration
D4D_DECLARE_RSCREEN_BEGIN(screen_log, ScreenLog_, 20 ,20, 280, 200, 8, "Log Window", FONT_BERLIN_SANS_FBDEMI12, &bmpFreescale, (D4D_SCR_F_BCKG | D4D_SCR_F_EXIT | D4D_SCR_F_TITLEBAR | D4D_SCR_F_BEVEL | D4D_SCR_F_TOUCHENABLE), NULL)
  D4D_DECLARE_SCREEN_OBJECT(scrLog_CnslLog)    
D4D_DECLARE_SCREEN_END()    


void Log_GetTimeString(char charArrray[32]);
static void Log_PrintTime(void)
{
    char timeString[32];
    Log_GetTimeString(timeString);
    (void)D4D_CnslPutString(&scrLog_CnslLog, timeString);
}

static void Log_PrintObjectName(D4D_OBJECT* pObj)
{
  D4D_CnslPutString(&scrLog_CnslLog, pObj->pObjFunc->strName);
}

#if D4D_MCU_TYPE != D4D_HCS08
void Log_AddObjectEvent(D4D_OBJECT_PTR pObj, D4D_TCHAR* pEvent)
{
  Log_PrintTime();
  (void)D4D_CnslPutChar(&scrLog_CnslLog, ' ');
  Log_PrintObjectName((D4D_OBJECT*)pObj);
  (void)D4D_CnslPutString(&scrLog_CnslLog, pEvent);
  (void)D4D_CnslPutString(&scrLog_CnslLog, newLine);
}

void Log_AddScreenEvent(D4D_TCHAR* pScrText, D4D_TCHAR* pEvent)
{
  Log_PrintTime();
  (void)D4D_CnslPutString(&scrLog_CnslLog, ":Screen ");
  (void)D4D_CnslPutString(&scrLog_CnslLog, pScrText);
  (void)D4D_CnslPutChar(&scrLog_CnslLog, '-');
  (void)D4D_CnslPutString(&scrLog_CnslLog, pEvent);
  (void)D4D_CnslPutString(&scrLog_CnslLog, newLine);
}
#endif

/*****************************************************************************
*
* Screen system functions 
*
*
*****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenLog_OnInit()
{
  Log_AddScreenEvent("Log", "OnInit");
}

// Screen on Activate function called with each screen activation
static void ScreenLog_OnActivate()
{
  Log_AddScreenEvent("Log", "OnActivate");  
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenLog_OnMain()
{
  
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenLog_OnDeactivate()
{
  Log_AddScreenEvent("Log", "OnDeActivate");    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenLog_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}



