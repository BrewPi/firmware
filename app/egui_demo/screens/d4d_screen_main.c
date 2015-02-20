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
* @file      d4d_screen_main.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - main screen source file of eGUI/D4D demo application
*
******************************************************************************/

#include "screen_common.h"


D4D_EXTERN_SCREEN(screen_winmenu)

/*****************************************************************************
*
* Graphic objects coordination specifications
*
*
*****************************************************************************/


// Graph object "Input audio Signal"

#define CENTER_SPACE	6
#define EDGE_SPACE	2

#define GRAPH_POSX EDGE_SPACE
#define GRAPH_POSY 56

#define GRAPH_SIZEX (D4D_SCREEN_SIZE_LONGER_SIDE - (2 * EDGE_SPACE) - CENTER_SPACE - LBL_VOL_SIZEX)
#define GRAPH_SIZEY (D4D_SCREEN_SIZE_SHORTER_SIDE - (2 * EDGE_SPACE) - BTN_SIZEY - GRAPH_POSY)

#define GRAPH_DATA_HISTORY 20

// Label object - "Audio" demo

#define LBL_TIT_POSX GRAPH_POSX
#define LBL_TIT_POSY 2

#define LBL_TIT_SIZEX GRAPH_SIZEX
#define LBL_TIT_SIZEY 40

// Label object - Freescale eGUI(D4D)

#define LBL_TIT1_POSX GRAPH_POSX
#define LBL_TIT1_POSY 35

#define LBL_TIT1_SIZEX GRAPH_SIZEX
#define LBL_TIT1_SIZEY 18


// Label object "Volume"

#define LBL_VOL_POSX (GRAPH_POSX + GRAPH_SIZEX + CENTER_SPACE)
#define LBL_VOL_POSY 2

#define LBL_VOL_SIZEX 130
#define LBL_VOL_SIZEY 20

// Slider object "Volume"

#define SLDR_VOL_POSX LBL_VOL_POSX
#define SLDR_VOL_POSY (LBL_VOL_POSY + LBL_VOL_SIZEY + EDGE_SPACE)

#define SLDR_VOL_SIZEX LBL_VOL_SIZEX
#define SLDR_VOL_SIZEY 30

// Label object "Balance"

#define LBL_BAL_POSX LBL_VOL_POSX
#define LBL_BAL_POSY (SLDR_VOL_POSY + SLDR_VOL_SIZEY + 5)

#define LBL_BAL_SIZEX LBL_VOL_SIZEX
#define LBL_BAL_SIZEY LBL_VOL_SIZEY

// Slider object "Balance"

#define SLDR_BAL_POSX SLDR_VOL_POSX
#define SLDR_BAL_POSY (LBL_BAL_POSY + LBL_BAL_SIZEY + 2)

#define SLDR_BAL_SIZEX LBL_BAL_SIZEX
#define SLDR_BAL_SIZEY SLDR_VOL_SIZEY

// Gauge objects "Output audio Signal"

#define GAUGE_SIZEX 64
#define GAUGE_SIZEY GAUGE_SIZEX

#define GAUGE_POSX SLDR_VOL_POSX
#define GAUGE_POSY (SLDR_BAL_POSY + SLDR_BAL_SIZEY + 4)

#define GAUGES_POSX(x) (GAUGE_POSX + (GAUGE_SIZEX * (x)))
#define GAUGES_POSY GAUGE_POSY

// Check box objects "Output audio Signal"

#define CHECKB_POSX(ix) GAUGES_POSX(ix)
#define CHECKB_POSY (GAUGES_POSY + GAUGE_SIZEY + 2)

#define CHECKB_SIZE_X GAUGE_SIZEX
#define CHECKB_SIZE_Y 16

// Button objects "Menu/Play/Stop"

#define BTN_SIZEX 54
#define BTN_SIZEY 26


#define BTN_POSX(ix) GRAPH_POSX + ((ix) * (BTN_SIZEX + 10))
#define BTN_POSY (D4D_SCREEN_SIZE_SHORTER_SIDE - BTN_SIZEY - EDGE_SPACE)

#define BTN_RADIUS 10

// Icon object "animation of equalizer"

#define ICON_EQU_POSX (D4D_SCREEN_SIZE_LONGER_SIDE - 2 - ICON_EQU_SIZEX)
#define ICON_EQU_POSY (D4D_SCREEN_SIZE_SHORTER_SIDE - 2 - ICON_EQU_SIZEY)

#define ICON_EQU_SIZEX 50
#define ICON_EQU_SIZEY 35

/*****************************************************************************
*
* Graphic objects callback/events functions declaration
*
*
*****************************************************************************/

static void ScrMain_OnClickBtnMenu(D4D_OBJECT* pThis);
static void ScrMain_OnClickBtnPlay(D4D_OBJECT* pThis);
static void ScrMain_OnClickBtnStop(D4D_OBJECT* pThis);
static void ScrMain_OnChangeCheckBLeft(D4D_OBJECT* pThis);
static void ScrMain_OnChangeCheckBRight(D4D_OBJECT* pThis);
static void ScrMain_OnChangeSldr(D4D_OBJECT* pThis);


/*****************************************************************************
*
* Graphic object declarations
*
*
*****************************************************************************/

// Gauges
D4D_DECLARE_STD_RGAUGE(scrMain_gaugeLeft, "L", GAUGES_POSX(0), GAUGES_POSY, GAUGE_SIZEX-2, GAUGE_SIZEY-2, 8, 3, 3, GAUGE_SIZEX - 12, GAUGE_SIZEY - 12, GAUGE_SIZEX - 25, &bmpGauge, FONT_ARIAL7_WIDE, NULL)
D4D_DECLARE_STD_RGAUGE(scrMain_gaugeRight, "R", GAUGES_POSX(1), GAUGES_POSY, GAUGE_SIZEX-2, GAUGE_SIZEY-2, 8, 3, 3, GAUGE_SIZEX - 12, GAUGE_SIZEY - 12, GAUGE_SIZEX - 25, &bmpGauge, FONT_ARIAL7_WIDE, NULL)

// Check Boxes
D4D_DECLARE_STD_CHECKBOX(scrMain_checkBxLeft, "Left", CHECKB_POSX(0), CHECKB_POSY, CHECKB_SIZE_X, CHECKB_SIZE_Y, &bmpCheckBCheck, NULL, FONT_ARIAL7, ScrMain_OnChangeCheckBLeft)
D4D_DECLARE_STD_CHECKBOX(scrMain_checkBxRight, "Right", CHECKB_POSX(1), CHECKB_POSY, CHECKB_SIZE_X, CHECKB_SIZE_Y, &bmpCheckBCheck, NULL, FONT_ARIAL7, ScrMain_OnChangeCheckBRight)


// Label
D4D_DECLARE_STD_LABEL(scrMain_lblVol, "Volume", LBL_VOL_POSX, LBL_VOL_POSY, LBL_VOL_SIZEX, LBL_VOL_SIZEY, FONT_BERLIN_SANS_FBDEMI12)
// Slider
static D4D_TCHAR scrMain_sldrVol_str[] = " 0";
D4D_DECLARE_RSLIDER_AUTOSIZE(scrMain_sldrVol, scrMain_sldrVol_str, SLDR_VOL_POSX, SLDR_VOL_POSY, SLDR_VOL_SIZEX, SLDR_VOL_SIZEY, 8, (D4D_SLDR_F_DEFAULT | D4D_SLDR_F_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR), NULL, NULL, FONT_BERLIN_SANS_FBDEMI12, NULL, ScrMain_OnChangeSldr, NULL)
// Label
D4D_DECLARE_STD_LABEL(scrMain_lblBal, "Balance", LBL_BAL_POSX, LBL_BAL_POSY, LBL_BAL_SIZEX, LBL_BAL_SIZEY, FONT_BERLIN_SANS_FBDEMI12)
// Slider
static D4D_TCHAR scrMain_sldrBalance_str[] = " 0 ";
D4D_DECLARE_RSLIDER_AUTOSIZE(scrMain_sldrBalance, scrMain_sldrBalance_str, SLDR_BAL_POSX, SLDR_BAL_POSY, SLDR_BAL_SIZEX, SLDR_BAL_SIZEY, 8, (D4D_SLDR_F_DEFAULT | D4D_SLDR_F_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR), NULL, NULL, FONT_BERLIN_SANS_FBDEMI12, NULL, ScrMain_OnChangeSldr, NULL)



// Graph
Byte dataTraceLeft[GRAPH_DATA_HISTORY];
Byte dataTraceRight[GRAPH_DATA_HISTORY];

D4D_DECLARE_STD_RGRAPH_BEGIN(scrMain_graph, "Input Audio Signal", GRAPH_POSX, GRAPH_POSY, GRAPH_SIZEX, GRAPH_SIZEY, 8, 4, 2, 20, FONT_ARIAL7, FONT_ARIAL7)
  D4D_DECLARE_GRAPH_TRACE(dataTraceLeft, D4D_COLOR_RED, D4D_LINE_THIN, D4D_GRAPH_TRACE_TYPE_LINE)
  D4D_DECLARE_GRAPH_TRACE(dataTraceRight, D4D_COLOR_GREEN, D4D_LINE_THIN, D4D_GRAPH_TRACE_TYPE_LINE)
D4D_DECLARE_GRAPH_END()  

// Buttons
D4D_DECLARE_TXT_RBUTTON(scrMain_btnPlay, "Play", BTN_POSX(0), BTN_POSY, BTN_SIZEX, BTN_SIZEY, BTN_RADIUS, FONT_BERLIN_SANS_FBDEMI12, ScrMain_OnClickBtnPlay)
D4D_DECLARE_TXT_RBUTTON(scrMain_btnStop, "Stop", BTN_POSX(1), BTN_POSY, BTN_SIZEX, BTN_SIZEY, BTN_RADIUS, FONT_BERLIN_SANS_FBDEMI12, ScrMain_OnClickBtnStop)
D4D_DECLARE_TXT_BUTTON(scrMain_btnMenu, "Menu", BTN_POSX(2), BTN_POSY, BTN_SIZEX, BTN_SIZEY, FONT_BERLIN_SANS_FBDEMI12, ScrMain_OnClickBtnMenu)

// Label Title
D4D_DECLARE_STD_LABEL(scrMain_lblTitle, "\"Audio\" demo", LBL_TIT_POSX, LBL_TIT_POSY, LBL_TIT_SIZEX, LBL_TIT_SIZEY, FONT_BERLIN_SANS_FBDEMI12_HIGH)
D4D_DECLARE_STD_LABEL(scrMain_lblTitle1, "Freescale eGUI (D4D)", LBL_TIT1_POSX, LBL_TIT1_POSY, LBL_TIT1_SIZEX, LBL_TIT1_SIZEY, FONT_ARIAL7)


// Icon
D4D_DECLARE_STD_ICON_BEGIN(scrMain_icnAnimEqu, NULL, ICON_EQU_POSX, ICON_EQU_POSY, 50, 35, 0, 0, FONT_ARIAL7, NULL)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu1)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu2)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu3)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu4)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu5)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu6)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu7)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu8)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu9)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu10)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu11)
  D4D_DECLARE_ICON_BMP(&bmpAnimEqu12)
D4D_DECLARE_ICON_END()

// declare FSL logo picture
D4D_DECLARE_STD_PICTURE(scrMain_picFsl, BTN_POSX(3), BTN_POSY, 24, 24, &bmpFreescale)


/*****************************************************************************
*
* eGUI/D4D screen declaration
*
*
*****************************************************************************/

// Standard screen declaration
D4D_DECLARE_STD_SCREEN_BEGIN(screen_main, ScreenMain_)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_lblTitle)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_lblTitle1)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_graph)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_lblVol)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_sldrVol)  
  D4D_DECLARE_SCREEN_OBJECT(scrMain_lblBal)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_sldrBalance)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_gaugeLeft)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_gaugeRight)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_checkBxLeft)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_checkBxRight)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_btnPlay)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_btnStop)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_picFsl)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_icnAnimEqu)
  D4D_DECLARE_SCREEN_OBJECT(scrMain_btnMenu)
D4D_DECLARE_SCREEN_END()    

/*****************************************************************************
*
* Local variables 
*
*
*****************************************************************************/

static int musicEnable = D4D_TRUE;
static Byte* pDummyMusic;


/*****************************************************************************
*
* Local function 
*
*
*****************************************************************************/

//Button Play OnClick CallBack
static void ScrMain_OnClickBtnPlay(D4D_OBJECT* pThis)
{
  musicEnable = D4D_TRUE;
  D4D_IconAnimationStart(&scrMain_icnAnimEqu);
  Log_AddObjectEvent(pThis, "Play click.");
}

//Button Stop OnClick CallBack
static void ScrMain_OnClickBtnStop(D4D_OBJECT* pThis)
{
  musicEnable = D4D_FALSE;
  D4D_IconAnimationStop(&scrMain_icnAnimEqu);
  pDummyMusic = 0;
  D4D_GraphClearAll(&scrMain_graph);
  Log_AddObjectEvent(pThis, "Stop click.");
}

//Button Menu OnClick CallBack
static void ScrMain_OnClickBtnMenu(D4D_OBJECT* pThis)
{
  D4D_ActivateScreen(&screen_winmenu, D4D_FALSE);
  Log_AddObjectEvent(pThis, "Menu click.");
}

//Slider OnChange CallBack
static void ScrMain_OnChangeSldr(D4D_OBJECT* pThis)
{
  D4D_TCHAR tmp_txt[4];     
  tmp_txt[D4D_SprintDecS8(D4D_SldrGetValue(pThis), &tmp_txt[0], 0)] = 0;
  D4D_SldrSetText(pThis, tmp_txt);
  
  if(pThis == &scrMain_sldrBalance)
  {
    Log_AddObjectEvent(pThis, "Balance Changed.");
  }
  else
  {
    Log_AddObjectEvent(pThis, "Volume Changed.");
  }
}

//CheckBox Left OnChange CallBack
static void ScrMain_OnChangeCheckBLeft(D4D_OBJECT* pThis)
{
  if(!D4D_CheckBoxGetValue(pThis))
    D4D_GaugSetValue(&scrMain_gaugeLeft, 0);
  Log_AddObjectEvent(pThis, "Left output changed.");
}

//CheckBox Right OnChange CallBack
static void ScrMain_OnChangeCheckBRight(D4D_OBJECT* pThis)
{
  if(!D4D_CheckBoxGetValue(pThis))
    D4D_GaugSetValue(&scrMain_gaugeRight, 0);
  Log_AddObjectEvent(pThis, "Right output changed.");
}

/*****************************************************************************
*
* Screen system functions 
*
*
*****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenMain_OnInit()
{
  D4D_TEXT_PROPERTIES tmp_txtProperties;
  D4D_SLIDER_LIMITS tmp_limits = { -16, 16, 1 , 0};
  D4D_GAUGE_LIMITS tmp_glimits = { 0, 127, 0, 63};

  // Gauges initialization
  D4D_GaugSetLimits(&scrMain_gaugeLeft, &tmp_glimits);
  D4D_GaugSetLimits(&scrMain_gaugeRight, &tmp_glimits);
  
  // Balance slider initialization
  D4D_SldrSetLimits(&scrMain_sldrBalance, &tmp_limits);
  
  // Volume slider initialization
  tmp_limits.valueMax = 32;
  tmp_limits.valueMin = 0;
  tmp_limits.step = 1;
  D4D_SldrSetLimits(&scrMain_sldrVol, &tmp_limits);

  // Volume slider value initialization
  D4D_SldrSetValue(&scrMain_sldrVol, 24);

  // Labels text propeties initialization
  tmp_txtProperties = D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK;
  
  D4D_SetTextProperties(&scrMain_lblVol, tmp_txtProperties);  
  D4D_SetTextProperties(&scrMain_lblBal, tmp_txtProperties);
  D4D_SetTextProperties(&scrMain_lblTitle, tmp_txtProperties);
  D4D_SetTextProperties(&scrMain_lblTitle1, tmp_txtProperties);
  
  // Check boxes initialization
  D4D_CheckBoxSetValue(&scrMain_checkBxLeft, D4D_TRUE);
  D4D_CheckBoxSetValue(&scrMain_checkBxRight, D4D_TRUE);
  
  D4D_IconAnimationSetCounts(&scrMain_icnAnimEqu, 4);
  D4D_IconAnimationStart(&scrMain_icnAnimEqu);
  
  Log_AddScreenEvent("Main", "OnInit");
  
}

// Screen on Activate function called with each screen activation
static void ScreenMain_OnActivate()
{
  pDummyMusic = 0;
  Log_AddScreenEvent("Main", "OnActivate");  
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenMain_OnMain()
{

  // if music is enabled
  if(musicEnable)
  {
    // each 100ms
    static unsigned long lastMillis = 0;
    if(millisC() - lastMillis >=  100)
    {
      Byte tmp_output;
      
      lastMillis = millisC(); 
      
      // add new data into "input audio signal" graph
      D4D_GraphAddTracesData(&scrMain_graph, pDummyMusic);
            
      // if left output signal is enable (check the check box)
      if(D4D_CheckBoxGetValue(&scrMain_checkBxLeft))
      {        
        // modify the input signal to fit to the gauge
        tmp_output = (Byte)(255 - pDummyMusic[0]);
        
        tmp_output /= 2;
        
        // Compute with volume of signal
        tmp_output = D4D_MulDivUU8(tmp_output, (Byte)(D4D_SldrGetValue(&scrMain_sldrVol)), 32);
        
        // Compute with balance of signal
        if(D4D_SldrGetValue(&scrMain_sldrBalance) > 0)
          tmp_output = D4D_MulDivUU8(tmp_output, (Byte)(17 - D4D_SldrGetValue(&scrMain_sldrBalance)), 16);          

        
        D4D_GaugSetValue(&scrMain_gaugeLeft, (D4D_GAUGE_VALUE)(tmp_output));
      }
      
      // if left output signal is enable (check the check box)
      if(D4D_CheckBoxGetValue(&scrMain_checkBxRight))
      {
        // modify the input signal to fit to the gauge
        tmp_output = (Byte)(255 - pDummyMusic[1]);
        
        tmp_output /= 2;
        
        // Compute with volume of signa
        tmp_output = D4D_MulDivUU8(tmp_output, (Byte)(D4D_SldrGetValue(&scrMain_sldrVol)), 32);
        
        // Compute with balance of signal
        if(D4D_SldrGetValue(&scrMain_sldrBalance) < 0)
          tmp_output = D4D_MulDivUU8(tmp_output, (Byte)(17 + D4D_SldrGetValue(&scrMain_sldrBalance)), 16);

        
        D4D_GaugSetValue(&scrMain_gaugeRight, (D4D_GAUGE_VALUE)(tmp_output));
      }
      
      // update pointer to dummy data in memory of MCU
      pDummyMusic += 2;
      
    }
  }
    
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenMain_OnDeactivate()
{
  Log_AddScreenEvent("Main", "OnDeActivate");    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenMain_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}



