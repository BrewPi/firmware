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
* @file      d4d_screen_touchtest.c
*
* @author    Elco Jacobs
* 
* @version   0.0.1.0
* 
* @date      Feb 2015
* 
* @brief     D4D driver - touch screen test drawing and coordinate printing
*
******************************************************************************/

#include "screen_common.h"
#include "common_files/d4d_base.h"

D4D_EXTERN_SCREEN(screen_winmenu)

#define CENTER_SPACE	6
#define EDGE_SPACE	2
#define BTN_SIZEX 54
#define BTN_SIZEY 26
#define BTN_POSX(ix) ((ix) * (BTN_SIZEX + 10))
#define BTN_POSY (D4D_SCREEN_SIZE_SHORTER_SIDE - BTN_SIZEY - EDGE_SPACE)

/*****************************************************************************
 *
 * Graphic objects callback/events functions declaration
 *
 *
 *****************************************************************************/

static void ScrTouchtest_OnClickBtnMenu(D4D_OBJECT* pThis);
static void ScrTouchtest_OnClickBtnClear(D4D_OBJECT* pThis);

/*****************************************************************************
 *
 * Graphic object declarations
 *
 *
 *****************************************************************************/

D4D_DECLARE_TXT_BUTTON(scrTouchtest_btnMenu, "Menu", BTN_POSX(0), BTN_POSY, BTN_SIZEX, BTN_SIZEY, FONT_BERLIN_SANS_FBDEMI12, ScrTouchtest_OnClickBtnMenu)
D4D_DECLARE_TXT_BUTTON(scrTouchtest_btnClear, "Clear", BTN_POSX(1), BTN_POSY, BTN_SIZEX, BTN_SIZEY, FONT_BERLIN_SANS_FBDEMI12, ScrTouchtest_OnClickBtnClear)

/*****************************************************************************
 *
 * eGUI/D4D screen declaration
 *
 *
 *****************************************************************************/


// Standard screen declaration
D4D_DECLARE_STD_SCREEN_BEGIN(screen_touchtest, ScreenTouchtest_)
  D4D_DECLARE_SCREEN_OBJECT(scrTouchtest_btnMenu)
  D4D_DECLARE_SCREEN_OBJECT(scrTouchtest_btnClear)
D4D_DECLARE_SCREEN_END()

/*****************************************************************************
 *
 * Screen system functions 
 *
 *
 *****************************************************************************/

static void ScreenTouchtest_printCoordinates(D4D_COOR x, D4D_COOR y, D4D_COOR screen_x, D4D_COOR screen_y) {
    D4D_COLOR fore = D4D_COLOR_WHITE;
    D4D_COLOR bckg = D4D_COLOR_BLACK;
    
    D4D_STRING tmp_txtbuff;
    D4D_STR_PROPERTIES tmp_str_prty;
    D4D_TCHAR str_touch[8] = D4D_DEFSTR("X:00000");
    tmp_txtbuff.fontId = D4D_FONT_SYSTEM_DEFAULT;
    tmp_txtbuff.str_properties = &tmp_str_prty;
    tmp_str_prty.font_properties = 0;  
    
    D4D_UNUSED(fore);
    D4D_UNUSED(bckg);
    D4D_UNUSED(tmp_txtbuff);
    
    if(D4D_GetFont(D4D_FONT_SYSTEM_DEFAULT) != NULL) {
        tmp_txtbuff.buffSize = 0;
        tmp_txtbuff.printLen = 0;
        tmp_txtbuff.printOff = 0;
        tmp_txtbuff.pText = str_touch;
        str_touch[0] = 'X';
        D4D_SprintDecU16((Word) x, &str_touch[2], ' ');
        D4D_DrawTextXY(screen_x, screen_y, &tmp_txtbuff, fore, bckg);
        str_touch[0] = 'Y';
        D4D_SprintDecU16((Word) y, &str_touch[2], ' ');
        D4D_DrawTextXY(screen_x + D4D_GetFontWidth(D4D_FONT_SYSTEM_DEFAULT) * 6, screen_y, &tmp_txtbuff, fore, bckg);
    }    
}

//Button Menu OnClick CallBack
static void ScrTouchtest_OnClickBtnMenu(D4D_OBJECT* pThis) {
    D4D_ActivateScreen(&screen_winmenu, D4D_FALSE);
    Log_AddObjectEvent(pThis, "Menu click.");
}

//Button Clear OnClick CallBack
static void ScrTouchtest_OnClickBtnClear(D4D_OBJECT* pThis) {
    D4D_InvalidateScreen((D4D_SCREEN*) &screen_touchtest, D4D_TRUE); // Do a complete redraw
    Log_AddObjectEvent(pThis, "Clear click.");
}

// One time called screen function in screen initialization process

static void ScreenTouchtest_OnInit() {
}

// Screen on Activate function called with each screen activation

static void ScreenTouchtest_OnActivate() {
    Log_AddScreenEvent("Touchtest", "OnActivate");
}

// Screen "Main" function called periodically in each D4D_poll runs

static void ScreenTouchtest_OnMain() {
    D4D_COOR xRaw, yRaw, x, y;
    if (D4D_TCH_GetRawPosition(&xRaw, &yRaw)) {
        x = xRaw;
        y = yRaw;
        D4D_TCH_GetCalibratedPosition(&x, &y);
        D4D_PutPixelXY(x, y, D4D_LINE_THIN, D4D_COLOR_RED);
        ScreenTouchtest_printCoordinates(x,y, 10, 10);
    }
}

// Screen on DeActivate function called with each screen deactivation

static void ScreenTouchtest_OnDeactivate() {
    Log_AddScreenEvent("Touchtest", "OnDeActivate");
}

// Screen on message function called with each internal massage for this screen

static Byte ScreenTouchtest_OnObjectMsg(D4D_MESSAGE* pMsg) {
    D4D_UNUSED(pMsg);

    return 0;
}


