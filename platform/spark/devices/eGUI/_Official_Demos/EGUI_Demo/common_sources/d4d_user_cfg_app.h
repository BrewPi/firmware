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
* @file      d4d_user_cfg_app.h
*
* @author    Petr Gargulak
* 
* @version   0.0.4.0
* 
* @date      Jan-15-2014
* 
* @brief     D4D Driver user configuration file for Kinetis TWR-K60N512
*
*******************************************************************************/

#ifndef __D4D_USER_CFG_APP_H
#define __D4D_USER_CFG_APP_H

/******************************************************************************
* Desciption : This file allows user depends modification of D4D/eGUI 
*   graphical driver. 
*
*
******************************************************************************/


// Variable of coordinations of display
// in simple - if display has resolution bigger than 256, than you have to specify 
// bigger type than unsigned char (recommended is unsigned short) 
#define D4D_COOR_TYPE Word

#define D4D_COLOR_MY_GREY         D4D_COLOR_RGB(229, 229, 188)
#define D4D_COLOR_LIGHT2_GREY     D4D_COLOR_RGB(180, 180, 180) 
#define D4D_COLOR_LIGHT_BLACK     D4D_COLOR_RGB(50, 50, 50) 
/******************************************************************************
*
*   User definition of input KEYS format 
*     To apply user definition uncomment the accurate line
*
******************************************************************************/

#define D4D_KEYS_BUFF_LENGTH 8

// Here is place for any change of standard keys codes

#define D4D_KEY_SCANCODE_UP     0x52
#define D4D_KEY_SCANCODE_DOWN   0x51
#define D4D_KEY_SCANCODE_LEFT   0x50
#define D4D_KEY_SCANCODE_RIGHT  0x4F
#define D4D_KEY_SCANCODE_ENTER  0x28
#define D4D_KEY_SCANCODE_ESC    0x29

#define D4D_KEY_FUNC_FOCUS_NEXT    D4D_KEY_SCANCODE_RIGHT
#define D4D_KEY_FUNC_FOCUS_PREV    D4D_KEY_SCANCODE_LEFT

// Backward compatibility section
  //#define D4D_KEY_UP     0x01
  //#define D4D_KEY_DOWN   0x02
  //#define D4D_KEY_LEFT   0x04 
  //#define D4D_KEY_RIGHT  0x08
  //#define D4D_KEY_ENTER  0x10
  //#define D4D_KEY_ESC    0x20

/******************************************************************************
* Constants
******************************************************************************/

#define D4D_OS_MQX		                // if this is uncommented the eGUI is using some MQX features (example: memAlloc)			

#define D4D_FONT_TABLE_DISABLED D4D_FALSE       // this option enable /disable font table for whole driver
                                                // when this option is sets to true all strings are disabled

#define D4D_ROUND_CORNER_ENABLE D4D_TRUE  // this option disable/enable round corners support of D4D

#define D4D_FNT_WORK_PATH_DEFAULT "A:\\Fonts\\"

// Supported color systems
//  D4D_COLOR_SYSTEM_RGB888 
//  D4D_COLOR_SYSTEM_RGB666 
//  D4D_COLOR_SYSTEM_RGB565 
//  D4D_COLOR_SYSTEM_RGB332 

#define D4D_COLOR_SYSTEM D4D_COLOR_SYSTEM_RGB888  // this select the type of D4D system colors (also depends size of type D4D_COLOR on this settings)

//#define D4D_COLOR_TRANSPARENT D4D_COLOR_GREEN // this is definition of transparent color - comment it in case that transparent color won't be used in project

#define D4D_EXTSRC_TEXT_ENABLE D4D_FALSE		// External strings support
 
#define D4D_STRINGTABLE_PREFIX '@'

#define D4D_EXTSRC_FILE_ENABLE  D4D_TRUE

#define D4D_FNT_EXTSRC_SUPPORT D4D_TRUE

#define D4D_EXTSRC_BUFF_SIZE    4096

#define D4D_UNICODE                   // Enables Unicode support

//#define D4D_SCREEN_HISTORY 10         // This option define depth of history of screens

#define D4D_SCREEN_SIZE_LONGER_SIDE  480  // The maximum resolution of longer longer side of physical LCD

#define D4D_SCREEN_SIZE_SHORTER_SIDE  272  // The maximum resolution of  shorter longer side of physical LCD

#define D4D_COLOR_SYSTEM_FORE D4D_COLOR_YELLOW // System fore color (for example for calibration screen)

#define D4D_COLOR_SYSTEM_BCKG D4D_COLOR_GREY // System background color (for example for calibration screen)

#define D4D_FONT_SYSTEM_DEFAULT 0 // System default font (for example for calibration screen)

#define D4D_LLD_FLUSH_ELEMENT D4D_TRUE

/******************************************************************************
* CallBack function definition
******************************************************************************/


#define D4D_INPUT_EVENT_CALLBACK D4D_InputEventCB // This name of callback function 
                                                  // that is called with all input events
                                                  // from keyboard or touchscreen. 
                                                  // Prototype is void D4D_INPUT_EVENT_CALLBACK(void)
                                                  // Uncomment to used it.           

/******************************************************************************
*
*        ==================== D4D BITMAPS ======================              
*
*                 List of supported bitmap types 
*     To enable bitmap decoding algorithm change D4D_FALSE option to D4D_TRUE
*
******************************************************************************/
// 16m colors bitmap without pallete
#define D4D_BMP_16MNOPAL_ENABLE D4D_TRUE

// 65k colors bitmap without pallete 
#define D4D_BMP_65536NOPAL_ENABLE D4D_TRUE

// 256 colors bitmap without pallete
#define D4D_BMP_256NOPAL_ENABLE D4D_TRUE

// 256 colors bitmap with pallete
#define D4D_BMP_PAL_256_ENABLE D4D_TRUE

// 16 colors bitmap with pallete
#define D4D_BMP_PAL_16_ENABLE D4D_FALSE

// 2 colors bitmap with pallete
#define D4D_BMP_PAL_2_ENABLE D4D_FALSE

/******************************************************************************
* External files bitmpas decoders
******************************************************************************/

#define D4D_BMP_EXTSRC_SUPPORT_D4D D4D_TRUE

#define D4D_BMP_EXTSRC_SUPPORT_BMP D4D_TRUE

/******************************************************************************
*
*        ==================== D4D_MOUSE ======================              
*
*               User definition of defaults of D4D mouse
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/

#define D4D_MOUSE_NORMAL_BMP            bmp_blue_normal_key_3f0000
#define D4D_MOUSE_BUSY_BMP              bmp_blue_busy_key_3f0000
#define D4D_MOUSE_UNAVAILABLE_BMP       bmp_blue_unavailable_key_3f0000
#define D4D_MOUSE_BMP_KEY_COLOR         0x003F0000


/******************************************************************************
*
*        ==================== D4D_SCREEN ======================              
*
*               User definition of defaults of D4D screen
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/

// Define a default screen outline color
#define D4D_COLOR_SCR_OUTLINE  D4D_COLOR_LIGHT_GREY

// Define a default screen header color
#define D4D_COLOR_SCR_TITLEBAR  D4D_COLOR_BLUE

// Define a default screen header text color
#define D4D_COLOR_SCR_TILTLETEXT  D4D_COLOR_WHITE

// Define a default screen header exit button fore color
#define D4D_COLOR_SCR_EXIT_BTN_FORE  D4D_COLOR_BLACK

// Define a default screen header exit button background color
#define D4D_COLOR_SCR_EXIT_BTN_BCKG  D4D_COLOR_MY_GREY

// Define a default screen background color
#define D4D_COLOR_SCR_DESKTOP  D4D_COLOR_MY_GREY



// Define a default title offset in axis X (is used already for title icon offset)
//#define D4D_SCR_TITLE_OFF_X  30

// Define a default title offset in axis Y (is used already for title icon offset)
//#define D4D_SCR_TITLE_OFF_Y  1

// Define a minimum size of screen header in case that header is enabled
//#define D4D_SCR_HEADER_SIZE_MIN_SIZE  13

// Define a minimum size of EXIT button(cross) in screen header
//#define D4D_SCR_TITLE_EXITBTN_MIN_SIZE  6

// Define a default leasing of cross in EXIT button, opposite to size of button
//#define D4D_SCR_EXITBTN_CROSS_SIZE 2

// Define a default exit button offset
//#define D4D_SCR_TITLE_EXITBTN_OFFSET  4


// Define a default flags 
#define D4D_SCR_F_DEFAULT        (D4D_SCR_F_BCKG | D4D_SCR_FINT_MOUSE_NORMAL)

/******************************************************************************
*
*        ==================== D4D_OBJECT ======================              
*
*          User definition of defaults  of individual objects
*          To apply user definition uncomment the accurate line
*
******************************************************************************/

/******************************************************************************
* General object colors
******************************************************************************/

// standard normal color fore
#define D4D_COLOR_FORE_NORM  D4D_COLOR_GREY
// standard normal color background
#define D4D_COLOR_BCKG_NORM  D4D_COLOR_LIGHT_GREY

// standard disabled color fore
#define D4D_COLOR_FORE_DISABLED D4D_COLOR_WHITE
// standard disabled color background
#define D4D_COLOR_BCKG_DISABLED D4D_COLOR_BLACK

// standard focus color fore
#define D4D_COLOR_FORE_FOCUS D4D_COLOR_LIGHT_BLACK
// standard focus color background
#define D4D_COLOR_BCKG_FOCUS D4D_COLOR_MY_GREY

// standard capturing color fore
#define D4D_COLOR_FORE_CAPTURE D4D_COLOR_DARK_RED
// standard capturing color background
#define D4D_COLOR_BCKG_CAPTURE D4D_COLOR_WHITE


/******************************************************************************
* Button Object
******************************************************************************/

/***********************
* Features
***********************/
#define D4D_BTN_3D_ENABLE D4D_TRUE

/***********************
* Properties
***********************/

#define D4D_BTN_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL | D4D_BTN_F_3D | D4D_BTN_F_CONT_RECT)

//#define D4D_BTN_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_BTN_FNT_PRTY_DEFAULT  ( 0 )
/***********************
* Sizes constants
***********************/

//#define D4D_BTN_BORDER_OFFSET 3

//#define D4D_BTN_3D_FRAME_WIDTH 2

//#define D4D_BTN_3D_SHIFT 1


/******************************************************************************
* Gauge Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_GAUGE_F_DEFAULT     (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | \
//                                  D4D_OBJECT_F_FOCUSRECT | D4D_GAUGE_F_REDRAW_TEXT | D4D_GAUGE_F_HUB)

//#define D4D_GAUGE_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_GAUGE_FNT_PRTY_DEFAULT  ( 0 )

/***********************
* Sizes constants
***********************/

//#define D4D_GAUGE_HUB_RADIUS 6

/***********************
* Colors
***********************/
// standard gauge colors
//#define D4D_COLOR_GAUG_HUB        D4D_COLOR_DARK_RED
//#define D4D_COLOR_GAUG_POINTER    D4D_COLOR_DARK_BLUE

/******************************************************************************
* Slider Object
******************************************************************************/

/***********************
* Properties
***********************/


//#define D4D_SLDR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TRANSP_TEXT)

//#define D4D_SLDR_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_SLDR_FNT_PRTY_DEFAULT  ( D4D_FNT_PRTY_TRANSPARENT_YES_MASK )
/***********************
* Sizes constants
***********************/
//#define D4D_SLDR_BAR_OFF_LENGTH  2
//#define D4D_SLDR_BAR_OFF_WIDTH  4

/***********************
* Colors
***********************/
// standard slider colors
#define D4D_COLOR_SLDR_BAR_FORE   D4D_COLOR_RGB(0, 0, 80) // very dark blue
#define D4D_COLOR_SLDR_BAR_BCKG   D4D_COLOR_WHITE
//#define D4D_COLOR_SLDR_BAR_START  D4D_COLOR_WHITE
//#define D4D_COLOR_SLDR_BAR_END    D4D_COLOR_BLACK

/******************************************************************************
* Icon Object
******************************************************************************/
 
/***********************
* Features
***********************/
//#define D4D_ICON_ENABLE_ANIMATION D4D_TRUE
//#define D4D_ICON_ANIMATION_TICK_COUNTER  ( 4 )

/***********************
* Properties
***********************/

//#define D4D_ICON_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)

//#define D4D_ICON_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_ICON_FNT_PRTY_DEFAULT  ( 0 )

/******************************************************************************
* Label Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_LBL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT)

//#define D4D_LBL_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_LBL_FNT_PRTY_DEFAULT  ( 0 )

/******************************************************************************
* Menu Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_MENU_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_MENU_F_INDEX | D4D_MENU_F_SIDEBAR)

//#define D4D_MENU_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_MENU_FNT_PRTY_DEFAULT  ( 0 )

//#define D4D_MENU_IX_TXT_PRTY_DEFAULT  ( D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK )
//#define D4D_MENU_IX_FNT_PRTY_DEFAULT  ( 0 )

//#define D4D_MENU_ITEM_TXT_PRTY_DEFAULT  ( D4D_TXT_PRTY_ALIGN_V_CENTER_MASK | D4D_TXT_PRTY_ALIGN_H_LEFT_MASK )
//#define D4D_MENU_ITEM_FNT_PRTY_DEFAULT  ( 0 )


/******************************************************************************
* CheckBox  Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_CHECKBOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE\
//                                  | D4D_OBJECT_F_FOCUSRECT | D4D_CHECKBOX_F_ICON_RECTANGLE )


// #define D4D_CHECKBOX_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
// #define D4D_CHECKBOX_FNT_PRTY_DEFAULT  ( 0 )
/***********************
* Sizes constants
***********************/

//#define D4D_CHECKBOX_BORDER_OFFSET 3
//#define D4D_CHECKBOX_TEXT_OFFSET 4

/***********************
* Colors
***********************/
// standard check box colors
//#define D4D_COLOR_CHECKBOX_ICON_BCKG  D4D_COLOR_WHITE


/******************************************************************************
* Radio button  Object
******************************************************************************/

/***********************
* Properties
***********************/

#define D4D_RADIOBUTTON_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH\
                                  | D4D_RADIOBUTTON_F_ICON_RING | D4D_OBJECT_F_MOUSE_NORMAL )

// #define D4D_RADIOBUTTON_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
// #define D4D_RADIOBUTTON_FNT_PRTY_DEFAULT  ( 0 )
/***********************
* Sizes constants
***********************/

//#define D4D_RADIOBUTTON_BORDER_OFFSET 3
//#define D4D_RADIOBUTTON_TEXT_OFFSET 4

//#define D4D_RADIOBUTTON_ICON_RING 2
//#define D4D_RADIOBUTTON_MIN_SIZE 10

/***********************
* Colors
***********************/
// standard check box colors
//#define D4D_COLOR_RADIOBUTTON_ICON_BCKG  D4D_COLOR_WHITE

/******************************************************************************
* Graph Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_GRAPH_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_GRAPH_F_MODE_ROLLOVER | D4D_GRAPH_F_VALUE_X_BOTT | D4D_GRAPH_F_VALUE_Y_RIGHT  )

//#define D4D_GRAPH_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_GRAPH_FNT_PRTY_DEFAULT  ( 0 )
//#define D4D_GRAPH_LBL_FNT_PRTY_DEFAULT  ( 0 ) 

/***********************
* Sizes constants
***********************/
//#define D4D_GRAPH_BORDER_OFF        5
//#define D4D_GRAPH_VALUE_OFF        2

 
/***********************
* Colors
***********************/
// standard graph grid color
//#define D4D_COLOR_GRAPH_GRID  D4D_COLOR_GREY

/******************************************************************************
* Picture Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_PIC_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)


/******************************************************************************
* Scroll bar Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_SCRLBR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT)

/***********************
* Sizes constants
***********************/
//#define #define D4D_SCRLBAR_MIN_TAB_SIZE  ( 4 )


/******************************************************************************
* Console Object
******************************************************************************/

/***********************
* Properties
***********************/
//Console default properties
//#define D4D_CNSL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE)

// Console scroll bars default properties
//#define D4D_CNSL_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT) 

//#define D4D_CNSL_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_CNSL_FNT_PRTY_DEFAULT  ( 0 ) 

/***********************
* Sizes constants
***********************/
//#define D4D_CNSL_TAB_SIZE  ( 8 )
//#define D4D_CNSL_SCRLBR_WIDTH  ( 16 )
//#define D4D_CNSL_SCRLBR_STEP_V  ( 2 )
//#define D4D_CNSL_SCRLBR_STEP_H  ( 8 )

/******************************************************************************
* Text Box Object
******************************************************************************/

/***********************
* Properties
***********************/
//Text Box default properties
//#define D4D_TXTBX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_TABSTOP)

// Text Box scroll bar default properties
//#define D4D_TXTBX_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)

//#define D4D_TXTBX_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_TXTBX_FNT_PRTY_DEFAULT  ( 0 ) 

/***********************
* Sizes constants
***********************/
#define D4D_TXTBX_SCRLBR_WIDTH  ( 20 )
#define D4D_TXTBX_SCRLBR_STEP  ( 4 )

/******************************************************************************
* Progress Bar Object
******************************************************************************/

/***********************
* Properties
***********************/


//#define D4D_PRGRS_BAR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED  | D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_BAR_SCALECOLOR)

/***********************
* Sizes constants
***********************/
//#define D4D_PRGRS_BAR_BAR_OFF  5

/***********************
* Colors
***********************/
// standard slider colors
//#define D4D_COLOR_PRGRS_BAR_BAR_FORE    D4D_COLOR_DARK_GREEN
//#define D4D_COLOR_PRGRS_BAR_BAR_END  D4D_COLOR_YELLOW
//#define D4D_COLOR_PRGRS_BAR_BAR_BCKG   D4D_COLOR_BRIGHT_GREY

/***********************
* Limits
***********************/
#define D4D_COLOR_PRGRS_BAR_MAX_VAL  100

/******************************************************************************
* Group Box Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_GROUP_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_GROUP_BOX_F_BEVEL)

/***********************
* Sizes constants
***********************/
//#define D4D_GROUP_BOX_TEXT_OFFSET 6

#define D4D_GROUP_BOX_HEADER_MARGIN 18

//#define D4D_GROUP_BOX_FRAME_MARGIN 4

/******************************************************************************
* List Box Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_LIST_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_LIST_BOX_F_BEVEL)

//#define D4D_LIST_BOX_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)

// #define D4D_LIST_BOX_ITEM_FNT_PRTY_DEFAULT  ( 0 )
// #define D4D_LIST_BOX_ITEM_TXT_PRTY_DEFAULT  ( D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_LEFT_MASK )

/***********************
* Sizes constants
***********************/
#define D4D_LIST_BOX_SCRLBR_WIDTH  ( 24 )

#define D4D_LIST_BOX_SCRLBR_STEP  ( 2 )


/******************************************************************************
* Edit Box Object
******************************************************************************/

/***********************
* Properties
***********************/

// #define D4D_EDIT_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE /*| D4D_OBJECT_F_FOCUSRECT*/ | D4D_EDIT_BOX_F_BEVEL)

// #define D4D_EDIT_BOX_ITEM_FNT_PRTY_DEFAULT  ( 0 )

// #define D4D_EDIT_BOX_ITEM_TXT_PRTY_DEFAULT  ( D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_LEFT_MASK )

// #define D4D_EDIT_BOX_CURSOR_BLINK_TICK_COUNTER  ( 10 )

#endif /* __D4D_USER_CFG_H */