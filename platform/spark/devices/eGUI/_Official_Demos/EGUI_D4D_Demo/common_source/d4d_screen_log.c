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


/*****************************************************************************
*
* Local function 
*
*
*****************************************************************************/
static void Log_Print2Digit(Byte num, D4D_TCHAR* pText)
{
  if(num > 99)
      num = 99;
  
  if(num > 9)
  {
    pText[0] = (D4D_TCHAR)('0' + (Byte)(num / 10));
    num %= 10;
  }
  
  pText[1] = (D4D_TCHAR)('0' + num);
  
}


static void Log_PrintTime(void)
{
  D4D_TCHAR tmp_str[] = "00:00.00,0";
  LWord time_tick = time100sm_cnt;          //123456
  
  if(time_tick > 36000) // more than 1 hour
  {                                                           
    Log_Print2Digit((Byte)(time_tick / 36000), &tmp_str[0]);   //3
    time_tick %= 36000;                                        //15456
  }
  
  if(time_tick > 600) // more than 1 minute
  {
    Log_Print2Digit((Byte)(time_tick / 600), &tmp_str[3]);  //25
    time_tick %= 600;                                       //456
  }
  
  if(time_tick > 10) // more than 1 second
  {
    Log_Print2Digit((Byte)(time_tick / 10), &tmp_str[6]);  //45
    time_tick %= 10;                                       //6
  }
  
  tmp_str[9] = (D4D_TCHAR)('0' + time_tick);                //6
  
  (void)D4D_CnslPutString(&scrLog_CnslLog, tmp_str);

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



