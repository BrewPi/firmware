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
* @file      d4d_screen_about.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - about of d4d typical screen source file
*
******************************************************************************/

#include "screens.h"
#include "brewpi-logo.h"

#define XMARGIN 10
#define WINDOW_SIZE_X 220
#define WINDOW_SIZE_Y 140

#define HELLO_STR_POSY(ix) (45 + (ix) * 13)

/*****************************************************************************
 *
 * Graphic object declarations
 *
 *
 *****************************************************************************/

// Body strings

D4D_DECLARE_STD_LABEL(scrAbout_lblTitle, "BrewPi says Hello", XMARGIN, 2, WINDOW_SIZE_X-2*XMARGIN, 20, FONT_BERLIN_SANS_FBDEMI12)
D4D_DECLARE_STD_LABEL(scrAbout_lblTitle1, "a first message on eGUI ", XMARGIN, 20, WINDOW_SIZE_X-2*XMARGIN, 20, FONT_BERLIN_SANS_FBDEMI12)

D4D_DECLARE_STD_LABEL(scrAbout_lblApp, "Example application: \"Hello World\".", XMARGIN, HELLO_STR_POSY(0), WINDOW_SIZE_X-2*XMARGIN, 15, FONT_ARIAL7)
D4D_DECLARE_STD_LABEL(scrAbout_lblApp1, "Hi!", XMARGIN, HELLO_STR_POSY(1), WINDOW_SIZE_X-2*XMARGIN, 15, FONT_ARIAL7)

D4D_DECLARE_STD_LABEL(scrAbout_lblElco, "Elco Jacobs, (2015)", XMARGIN, HELLO_STR_POSY(2), WINDOW_SIZE_X-2*XMARGIN, 18, FONT_ARIAL7)

//D4D_DECLARE_STD_LABEL(scrAbout_lblVer, D4D_VERSION, HELLO_STR_POSX, HELLO_STR_POSY(3) + 16, 205, 18, FONT_BERLIN_SANS_FBDEMI12)
D4D_DECLARE_RLABEL(scrAbout_lblVer, D4D_VERSION, WINDOW_SIZE_X/2-35, HELLO_STR_POSY(2) + 20, 70, 20, 8, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT), NULL, FONT_BERLIN_SANS_FBDEMI12, NULL, NULL)


/*****************************************************************************
 *
 * eGUI/D4D screen declaration
 *
 *
 *****************************************************************************/

// Standard screen declaration
D4D_DECLARE_RSCREEN_BEGIN(screen_helloworld, ScreenHelloWorld_, 50, 50, WINDOW_SIZE_X, WINDOW_SIZE_Y, 8, "eGUI/D4D Hello", FONT_BERLIN_SANS_FBDEMI12, &bmp_brewpi_logo_22_16, (D4D_SCR_F_BCKG | D4D_SCR_F_EXIT | D4D_SCR_F_TITLEBAR | D4D_SCR_F_BEVEL | D4D_SCR_F_TOUCHENABLE), NULL)
D4D_DECLARE_SCREEN_OBJECT(scrAbout_lblTitle)
D4D_DECLARE_SCREEN_OBJECT(scrAbout_lblTitle1)
D4D_DECLARE_SCREEN_OBJECT(scrAbout_lblApp)
D4D_DECLARE_SCREEN_OBJECT(scrAbout_lblApp1)
D4D_DECLARE_SCREEN_OBJECT(scrAbout_lblElco)
D4D_DECLARE_SCREEN_OBJECT(scrAbout_lblVer)
D4D_DECLARE_SCREEN_END()


/*****************************************************************************
 *
 * Screen system functions 
 *
 *
 *****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenHelloWorld_OnInit() {
//    Log_AddScreenEvent("About", "OnInit");
}

// Screen on Activate function called with each screen activation

static void ScreenHelloWorld_OnActivate() {
//    Log_AddScreenEvent("About", "OnActivate");
}

// Screen "Main" function called periodically in each D4D_poll runs

static void ScreenHelloWorld_OnMain() {

}


// Screen on DeActivate function called with each screen deactivation

static void ScreenHelloWorld_OnDeactivate() {
//    Log_AddScreenEvent("About", "OnDeActivate");
}

// Screen on message function called with each internal massage for this screen

static Byte ScreenHelloWorld_OnObjectMsg(D4D_MESSAGE* pMsg) {
    D4D_UNUSED(pMsg);

    return 0;
}



