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
* @file      d4d_scheme.h
*
* @author     Petr Gargulak
*
* @version   0.0.28.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver color scheme header file
*
*******************************************************************************/

#ifndef __D4D_SCHEME_H
#define __D4D_SCHEME_H

/******************************************************************************
* Includes
******************************************************************************/


/******************************************************************************
* D4D SCHEME setting  constants
*
*//*! @addtogroup doxd4d_scheme_const
* @{
*******************************************************************************/

//Colors systems definition
/*!
  @defgroup doxd4d_scheme_const_colorssystem D4D SCHEME the color systems definition to define D4D_COLOR_SYSTEM.
  This section specifies the list of eGUI supported color systems.
  @ingroup doxd4d_scheme_const
*/

/**
 * @addtogroup doxd4d_scheme_const_colorssystem
 * @{
 */
#define D4D_COLOR_SYSTEM_RGB888 24888   ///< The eGUI 24-bit (888) color system
#define D4D_COLOR_SYSTEM_RGB666 18666   ///< The eGUI 18-bit (666) color system
#define D4D_COLOR_SYSTEM_RGB565 16565   ///< The eGUI 16-bit (565) color system
#define D4D_COLOR_SYSTEM_RGB555 15555   ///< The eGUI 15-bit (555) color system
#define D4D_COLOR_SYSTEM_RGB332 8332    ///< The eGUI 8-bit (332) color system
#define D4D_COLOR_SYSTEM_MONO 21        ///< The eGUI 1-bit (monochrome) color system

/**@}*/

/*! @brief The definition of color system used by eGUI in this project.
           The all options are defined in \ref doxd4d_scheme_const_colorssystem
           If not defined, it sets to D4D_COLOR_SYSTEM_RGB565 as a default.*/
#ifndef D4D_COLOR_SYSTEM
  #define D4D_COLOR_SYSTEM D4D_COLOR_SYSTEM_RGB565
#endif

//Colors definition
/*!
  @defgroup doxd4d_scheme_const_colors D4D SCHEME Defined standard colors
  This section specifies the list of standard colors.
  The list can be extend to any user colors.
  The definitions in brackets are in (R,G,B)
  @ingroup doxd4d_scheme_const
*/

/**
 * @addtogroup doxd4d_scheme_const_colors
 * @{
 */
  #define D4D_COLOR_DARK_BLUE     D4D_COLOR_RGB(0, 0, 152)      ///< Dark Blue color (0, 0, 152)
  #define D4D_COLOR_BRIGHT_BLUE   D4D_COLOR_RGB(222, 219, 255)  ///< Bright Blue color (222, 219, 255)
  #define D4D_COLOR_BLUE          D4D_COLOR_RGB(0, 0, 255)      ///< Blue color (0, 0, 255)
  #define D4D_COLOR_CYAN          D4D_COLOR_RGB(0, 255, 255)    ///< Cyan color (0, 255, 255)
  #define D4D_COLOR_BRIGHT_YELLOW D4D_COLOR_RGB(255, 220, 120)  ///< Bright Yellow color (255, 220, 120)
  #define D4D_COLOR_YELLOW        D4D_COLOR_RGB(255, 255, 0)    ///< Yellow color (255, 255, 0)
  #define D4D_COLOR_ORANGE        D4D_COLOR_RGB(255, 152, 96)   ///< Orange color (255, 152, 96)
  #define D4D_COLOR_BRIGHT_RED    D4D_COLOR_RGB(255, 28, 24)    ///< Bright Red color (255, 28, 24)
  #define D4D_COLOR_RED           D4D_COLOR_RGB(255, 0, 0)      ///< Red color (255, 0, 0)
  #define D4D_COLOR_DARK_RED      D4D_COLOR_RGB(152, 0, 0)      ///< Dark Red color (152, 0, 0)
  #define D4D_COLOR_MAGENTA       D4D_COLOR_RGB(255, 0, 255)    ///< Magenta color (255, 0, 255)
  #define D4D_COLOR_BRIGHT_GREEN  D4D_COLOR_RGB(152, 255, 152)  ///< Bright Green color (152, 255, 152)
  #define D4D_COLOR_GREEN         D4D_COLOR_RGB(0, 255, 0)      ///< Green color (0, 255, 0)
  #define D4D_COLOR_DARK_GREEN    D4D_COLOR_RGB(0, 128, 0)      ///< Dark Green color (0, 128, 0)
  #define D4D_COLOR_BRIGHT_GREY   D4D_COLOR_RGB(48, 48, 48)     ///< Bright Grey color (48, 48, 48)
  #define D4D_COLOR_LIGHT_GREY    D4D_COLOR_RGB(120, 120, 120)  ///< Light Grey color (120, 120, 120)
  #define D4D_COLOR_GREY          D4D_COLOR_RGB(24, 24, 24)     ///< Grey color (24, 24, 24)
  #define D4D_COLOR_WHITE         D4D_COLOR_RGB(255, 255, 255)  ///< White color (255, 255, 255)
  #define D4D_COLOR_BLACK         D4D_COLOR_RGB(0, 0, 0)        ///< Black color (0, 0, 0)
/**@}*/

/*! @brief This is fore object color in normal state of object.
           If not defined, it sets to \ref D4D_COLOR_BLACK as a default.*/
#ifndef D4D_COLOR_FORE_NORM
  #define D4D_COLOR_FORE_NORM  D4D_COLOR_BLACK
#endif

/*! @brief This is background object color in normal state of object.
           If not defined, it sets to \ref D4D_COLOR_WHITE as a default.*/
#ifndef D4D_COLOR_BCKG_NORM
  #define D4D_COLOR_BCKG_NORM  D4D_COLOR_WHITE
#endif

/*! @brief This is fore object color in disabled state of object.
           If not defined, it sets to \ref D4D_COLOR_GREY as a default.*/
#ifndef D4D_COLOR_FORE_DISABLED
  #define D4D_COLOR_FORE_DISABLED D4D_COLOR_GREY
#endif

/*! @brief This is background object color in disabled state of object.
           If not defined, it sets to \ref D4D_COLOR_LIGHT_GREY as a default.*/
#ifndef D4D_COLOR_BCKG_DISABLED
  #define D4D_COLOR_BCKG_DISABLED D4D_COLOR_LIGHT_GREY
#endif

/*! @brief This is fore object color in focused state of object.
           If not defined, it sets to \ref D4D_COLOR_RED as a default.*/
#ifndef D4D_COLOR_FORE_FOCUS
  #define D4D_COLOR_FORE_FOCUS D4D_COLOR_RED
#endif

/*! @brief This is background object color in focused state of object.
           If not defined, it sets to \ref D4D_COLOR_LIGHT_GREY as a default.*/
#ifndef D4D_COLOR_BCKG_FOCUS
  #define D4D_COLOR_BCKG_FOCUS D4D_COLOR_LIGHT_GREY
#endif

/*! @brief This is fore object color in captured state of object.
           If not defined, it sets to \ref D4D_COLOR_BRIGHT_RED as a default.*/
#ifndef D4D_COLOR_FORE_CAPTURE
  #define D4D_COLOR_FORE_CAPTURE D4D_COLOR_BRIGHT_RED
#endif

/*! @brief This is background object color in captured state of object.
           If not defined, it sets to \ref D4D_COLOR_GREY as a default.*/
#ifndef D4D_COLOR_BCKG_CAPTURE
  #define D4D_COLOR_BCKG_CAPTURE D4D_COLOR_GREY
#endif

/*! @brief This is desktop screen color.
           If not defined, it sets to \ref D4D_COLOR_WHITE as a default.*/
#ifndef D4D_COLOR_SCR_DESKTOP
  #define D4D_COLOR_SCR_DESKTOP  D4D_COLOR_WHITE
#endif

/*! @brief This is outline screen color.
           If not defined, it sets to \ref D4D_COLOR_LIGHT_GREY as a default.*/
#ifndef D4D_COLOR_SCR_OUTLINE
  #define D4D_COLOR_SCR_OUTLINE  D4D_COLOR_LIGHT_GREY
#endif

/*! @brief This is title bar screen color.
           If not defined, it sets to \ref D4D_COLOR_ORANGE as a default.*/
#ifndef D4D_COLOR_SCR_TITLEBAR
  #define D4D_COLOR_SCR_TITLEBAR  D4D_COLOR_ORANGE
#endif

/*! @brief This is title text screen color.
           If not defined, it sets to \ref D4D_COLOR_WHITE as a default.*/
#ifndef D4D_COLOR_SCR_TILTLETEXT
  #define D4D_COLOR_SCR_TILTLETEXT  D4D_COLOR_WHITE
#endif

/*! @brief This is screen exit button fore color.
           If not defined, it sets to \ref D4D_COLOR_WHITE as a default.*/
#ifndef D4D_COLOR_SCR_EXIT_BTN_FORE
  #define D4D_COLOR_SCR_EXIT_BTN_FORE  D4D_COLOR_WHITE
#endif

/*! @brief This is screen exit button background color.
           If not defined, it sets to \ref D4D_COLOR_BRIGHT_RED as a default.*/
#ifndef D4D_COLOR_SCR_EXIT_BTN_BCKG
  #define D4D_COLOR_SCR_EXIT_BTN_BCKG
#endif


/*! @} End of doxd4d_scheme_const                                               */
/******************************************************************************
* Private Constants
******************************************************************************/

//******************************************************************************
//      24 bit - 888 color space
/*!
  @defgroup doxd4d_scheme_macro_888 D4D SCHEME 24-bit color mode support macros.
  This section specifies help macros to handle 24-bit color format.

  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_888
 * @{
 */

#define D4D_COLOR_RGB888_GET_R(color) ((Byte)((color) >> 16))   ///< The macro gets the Red component from 24-bit RGB888 format to 8-bit component format
#define D4D_COLOR_RGB888_GET_G(color) ((Byte)((color) >> 8))    ///< The macro gets the Green component from 24-bit RGB888 format to 8-bit component format
#define D4D_COLOR_RGB888_GET_B(color) ((Byte)(color))           ///< The macro gets the Blue component from 24-bit RGB888 format to 8-bit component format

#define D4D_COLOR_BGR888_GET_B(color) ((Byte)((color) >> 16))   ///< The macro gets the Red component from 24-bit BGR888 format to 8-bit component format
#define D4D_COLOR_BGR888_GET_G(color) ((Byte)((color) >> 8))    ///< The macro gets the Green component from 24-bit BGR888 format to 8-bit component format
#define D4D_COLOR_BGR888_GET_R(color) ((Byte)(color))           ///< The macro gets the Blue component from 24-bit BGR888 format to 8-bit component format


#define D4D_COLOR888_GET_R(color) D4D_COLOR_RGB888_GET_R(color) ///< The macro gets the Red component from 24-bit standard format to 8-bit component format
#define D4D_COLOR888_GET_G(color) D4D_COLOR_RGB888_GET_G(color) ///< The macro gets the Green component from 24-bit standard format to 8-bit component format
#define D4D_COLOR888_GET_B(color) D4D_COLOR_RGB888_GET_B(color) ///< The macro gets the Blue component from 24-bit standard format to 8-bit component format

#define D4D_COLOR_RGB888(R,G,B) ((LWord)(((R) << 16) + ((G) << 8) + (B)))       ///< The macro create the color from the color component Red/Green/Blue to 24-bit RGB888 format (the input parameters are 8-bit)
#define D4D_COLOR_BGR888(R,G,B) ((LWord)(((B) << 16) + ((G) << 8) + (R)))       ///< The macro create the color from the color component Red/Green/Blue to 24-bit BGR888 format (the input parameters are 8-bit)
/**@}*/

//******************************************************************************
//      18 bit - 666 color space
/*!
  @defgroup doxd4d_scheme_macro_666 D4D SCHEME 18-bit color mode support macros.
  This section specifies help macros to handle 18-bit color format.

  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_666
 * @{
 */

#define D4D_COLOR_RGB666_GET_R(color) ((Byte)((((color) >> 12) * 65) / 16))             ///< The macro gets the Red component from 18-bit RGB666 format to 8-bit component format
#define D4D_COLOR_RGB666_GET_G(color) ((Byte)(((((color) >> 6) & 0x3f) * 65) / 16))     ///< The macro gets the Green component from 18-bit RGB666 format to 8-bit component format
#define D4D_COLOR_RGB666_GET_B(color) ((Byte)((((color) & 0x3f) * 65) / 16))            ///< The macro gets the Blue component from 18-bit RGB666 format to 8-bit component format

#define D4D_COLOR_BGR666_GET_B(color) ((Byte)((((color) >> 12) * 65) / 16))             ///< The macro gets the Red component from 18-bit BGR666 format to 8-bit component format
#define D4D_COLOR_BGR666_GET_G(color) ((Byte)(((((color) >> 6) & 0x3f) * 65) / 16))     ///< The macro gets the Green component from 18-bit BGR666 format to 8-bit component format
#define D4D_COLOR_BGR666_GET_R(color) ((Byte)((((color) & 0x3f) * 65) / 16))            ///< The macro gets the Blue component from 18-bit BGR666 format to 8-bit component format


#define D4D_COLOR666_GET_R(color) D4D_COLOR_RGB666_GET_R(color)                         ///< The macro gets the Red component from 18-bit standard format to 8-bit component format
#define D4D_COLOR666_GET_G(color) D4D_COLOR_RGB666_GET_G(color)                         ///< The macro gets the Green component from 18-bit standard format to 8-bit component format
#define D4D_COLOR666_GET_B(color) D4D_COLOR_RGB666_GET_B(color)                         ///< The macro gets the Blue component from 18-bit standard format to 8-bit component format

#define D4D_COLOR_RGB666(R,G,B) ((LWord)((((R) / 4) << 12) + (((G) / 4) << 6) + ((B) / 4)))     ///< The macro create the color from the color component Red/Green/Blue to 18-bit RGB666 format (the input parameters are 8-bit)
#define D4D_COLOR_BGR666(R,G,B) ((LWord)((((R) / 4) << 12) + (((G) / 4) << 6) + ((B) / 4)))     ///< The macro create the color from the color component Red/Green/Blue to 18-bit BGR666 format (the input parameters are 8-bit)
/**@}*/
//******************************************************************************
//      16 bit - 565 color space

/*!
  @defgroup doxd4d_scheme_macro_565 D4D SCHEME 16-bit color mode support macros.
  This section specifies help macros to handle 16-bit color format(565).

  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_565
 * @{
 */

#define D4D_COLOR_RGB565_GET_R(color) ((Byte)((((color) >> 11) * 33) / 4))              ///< The macro gets the Red component from 16-bit RGB565 format to 8-bit component format
#define D4D_COLOR_RGB565_GET_G(color) ((Byte)(((((color) >> 5) & 0x3f) * 65) / 16))     ///< The macro gets the Green component from 16-bit RGB565 format to 8-bit component format
#define D4D_COLOR_RGB565_GET_B(color) ((Byte)((((color) & 0x1f) * 33) / 4))             ///< The macro gets the Blue component from 16-bit RGB565 format to 8-bit component format

#define D4D_COLOR_BGR565_GET_B(color) ((Byte)((((color) >> 11) * 33) / 4))              ///< The macro gets the Red component from 16-bit BGR565 format to 8-bit component format
#define D4D_COLOR_BGR565_GET_G(color) ((Byte)(((((color) >> 5) & 0x3f) * 65) / 16))     ///< The macro gets the Green component from 16-bit BGR565 format to 8-bit component format
#define D4D_COLOR_BGR565_GET_R(color) ((Byte)((((color) & 0x1f) * 33) / 4))             ///< The macro gets the Blue component from 16-bit BGR565 format to 8-bit component format


#define D4D_COLOR565_GET_R(color) D4D_COLOR_RGB565_GET_R(color)                         ///< The macro gets the Red component from 16-bit standard format to 8-bit component format
#define D4D_COLOR565_GET_G(color) D4D_COLOR_RGB565_GET_G(color)                         ///< The macro gets the Green component from 16-bit standard format to 8-bit component format
#define D4D_COLOR565_GET_B(color) D4D_COLOR_RGB565_GET_B(color)                         ///< The macro gets the Blue component from 16-bit standard format to 8-bit component format

#define D4D_COLOR_RGB565(R,G,B) ((Word)((((R) / 8) << 11) + (((G) / 4) << 5) + ((B) / 8)))      ///< The macro create the color from the color component Red/Green/Blue to 16-bit RGB565 format (the input parameters are 8-bit)
#define D4D_COLOR_BGR565(R,G,B) ((Word)((((B) / 8) << 11) + (((G) / 4) << 5) + ((R) / 8)))      ///< The macro create the color from the color component Red/Green/Blue to 16-bit BGR565 format (the input parameters are 8-bit)
/**@}*/
//******************************************************************************
//      15 bit - 555 color space
/*!
  @defgroup doxd4d_scheme_macro_555 D4D SCHEME 15-bit color mode support macros.
  This section specifies help macros to handle 15-bit color format(555).

  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_555
 * @{
 */
#define D4D_COLOR_RGB555_GET_R(color) ((Byte)((((color) >> 10) * 33) / 4))              ///< The macro gets the Red component from 15-bit RGB555 format to 8-bit component format
#define D4D_COLOR_RGB555_GET_G(color) ((Byte)(((((color) >> 5) & 0x1f) * 33) / 4))      ///< The macro gets the Green component from 15-bit RGB555 format to 8-bit component format
#define D4D_COLOR_RGB555_GET_B(color) ((Byte)((((color) & 0x1f) * 33) / 4))             ///< The macro gets the Blue component from 15-bit RGB555 format to 8-bit component format

#define D4D_COLOR_BGR555_GET_B(color) ((Byte)((((color) >> 10) * 33) / 4))              ///< The macro gets the Red component from 15-bit BGR555 format to 8-bit component format
#define D4D_COLOR_BGR555_GET_G(color) ((Byte)(((((color) >> 5) & 0x1f) * 33) / 4))      ///< The macro gets the Green component from 15-bit BGR555 format to 8-bit component format
#define D4D_COLOR_BGR555_GET_R(color) ((Byte)((((color) & 0x1f) * 33) / 4))             ///< The macro gets the Blue component from 15-bit BGR555 format to 8-bit component format


#define D4D_COLOR555_GET_R(color) D4D_COLOR_RGB555_GET_R(color)                         ///< The macro gets the Red component from 15-bit standard format to 8-bit component format
#define D4D_COLOR555_GET_G(color) D4D_COLOR_RGB555_GET_G(color)                         ///< The macro gets the Green component from 15-bit standard format to 8-bit component format
#define D4D_COLOR555_GET_B(color) D4D_COLOR_RGB555_GET_B(color)                         ///< The macro gets the Blue component from 15-bit standard format to 8-bit component format

#define D4D_COLOR_RGB555(R,G,B) ((Word)((((R) / 8) << 10) + (((G) / 8) << 5) + ((B) / 8)))      ///< The macro create the color from the color component Red/Green/Blue to 15-bit RGB555 format (the input parameters are 8-bit)
#define D4D_COLOR_BGR555(R,G,B) ((Word)((((B) / 8) << 10) + (((G) / 8) << 5) + ((R) / 8)))      ///< The macro create the color from the color component Red/Green/Blue to 15-bit BGR555 format (the input parameters are 8-bit)
/**@}*/
//******************************************************************************
//      8 bit - 332 color space

/*!
  @defgroup doxd4d_scheme_macro_332 D4D SCHEME 8-bit color mode support macros.
  This section specifies help macros to handle 8-bit color format(332).

  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_332
 * @{
 */
#define D4D_COLOR_RGB332_GET_R(color) ((Byte)(((((color) >> 5) & 0x07) * 73) / 2))      ///< The macro gets the Red component from 8-bit RGB332 format to 8-bit component format
#define D4D_COLOR_RGB332_GET_G(color) ((Byte)(((((color) >> 2) & 0x07) * 73) / 2))      ///< The macro gets the Green component from 8-bit RGB332 format to 8-bit component format
#define D4D_COLOR_RGB332_GET_B(color) ((Byte)((((color) & 0x03) * 170) / 2))            ///< The macro gets the Blue component from 8-bit RGB332 format to 8-bit component format

#define D4D_COLOR_BGR332_GET_B(color) ((Byte)(((((color) >> 5) & 0x07) * 73) / 2))      ///< The macro gets the Red component from 8-bit BGR332 format to 8-bit component format
#define D4D_COLOR_BGR332_GET_G(color) ((Byte)(((((color) >> 2) & 0x07) * 73) / 2))      ///< The macro gets the Green component from 8-bit BGR332 format to 8-bit component format
#define D4D_COLOR_BGR332_GET_R(color) ((Byte)((((color) & 0x03) * 170) / 2))            ///< The macro gets the Blue component from 8-bit BGR332 format to 8-bit component format


#define D4D_COLOR332_GET_R(color) D4D_COLOR_RGB332_GET_R(color)                         ///< The macro gets the Red component from 8-bit standard format to 8-bit component format
#define D4D_COLOR332_GET_G(color) D4D_COLOR_RGB332_GET_G(color)                         ///< The macro gets the Green component from 8-bit standard format to 8-bit component format
#define D4D_COLOR332_GET_B(color) D4D_COLOR_RGB332_GET_B(color)                         ///< The macro gets the Blue component from 8-bit standard format to 8-bit component format

#define D4D_COLOR_RGB332(R,G,B) ((Byte)((((R) / 32) << 5) + (((G) / 32) << 2) + ((B) / 64)))    ///< The macro create the color from the color component Red/Green/Blue to 8-bit RGB332 format (the input parameters are 8-bit)
#define D4D_COLOR_BGR332(R,G,B) ((Byte)((((B) / 32) << 5) + (((G) / 32) << 2) + ((R) / 64)))    ///< The macro create the color from the color component Red/Green/Blue to 8-bit BGR332 format (the input parameters are 8-bit)
/**@}*/
//******************************************************************************
//      1 bit - monochrome color space

/*!
  @defgroup doxd4d_scheme_macro_monochrome D4D SCHEME 1-bit color mode support macros.
  This section specifies help macros to handle 1-bit color format(monochrome).
  This is still non tested color scheme - added just for some initial tests.
  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_monochrome
 * @{
 */
#ifndef D4D_COLORMONO_THRESHOLD
  #define D4D_COLORMONO_THRESHOLD 128           ///< Defiend treshold of color components to compute the final color (black or white)
#endif

#define D4D_COLORMONO_GET_R(color) ((Byte)((color > D4D_COLORMONO_THRESHOLD)? 0xFF:0))  ///< The macro gets the Red component from 1-bit monochrome format to 8-bit component format
#define D4D_COLORMONO_GET_G(color) ((Byte)((color > D4D_COLORMONO_THRESHOLD)? 0xFF:0))  ///< The macro gets the Green component from 1-bit monochrome format to 8-bit component format
#define D4D_COLORMONO_GET_B(color) ((Byte)((color > D4D_COLORMONO_THRESHOLD)? 0xFF:0))  ///< The macro gets the Blue component from 1-bit monochrome format to 8-bit component format

#define D4D_COLOR_RGBMONO(R,G,B) ((D4D_COLOR)(((((R) + (G) + (B)) / 3) & 0xff) > D4D_COLORMONO_THRESHOLD)? 0xff:0)      ///< The macro create the color from the color component Red/Green/Blue to 1-bit monochrome format (the input parameters are 8-bit)
/**@}*/
//******************************************************************************
//              Current color system definition

#if D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB888
// comented just 888 color system due to Doxygen requirements

/*!
  @defgroup doxd4d_scheme_macro_colors D4D SCHEME help macros to handle different color modes of eGUI.
  This section specifies help macros to handle different color modes of eGUI.

  @ingroup doxd4d_scheme_macro
*/

/**
 * @addtogroup doxd4d_scheme_macro_colors
 * @{
 */
  #define D4D_COLOR_RGB(R,G,B) D4D_COLOR_RGB888(R,G,B)  ///< The macro create the color from the color component Red/Green/Blue to configured color format by D4D_COLOR_SYSTEM constant(the input parameters are 8-bit)

  #define D4D_COLOR_GET_R(color) D4D_COLOR888_GET_R(color)      ///< The macro gets the Red component from configured color format by D4D_COLOR_SYSTEM constant to 8-bit component format
  #define D4D_COLOR_GET_G(color) D4D_COLOR888_GET_G(color)      ///< The macro gets the Green component from configured color format by D4D_COLOR_SYSTEM constant to 8-bit component format
  #define D4D_COLOR_GET_B(color) D4D_COLOR888_GET_B(color)      ///< The macro gets the Blue component from configured color format by D4D_COLOR_SYSTEM constant to 8-bit component format

  #define D4D_COLOR_FROM_888(color888) (color888)               ///< The macro convert the COLOR in format 24-bit 888 to configured color format by D4D_COLOR_SYSTEM constant
  #define D4D_COLOR_FROM_666(color666) D4D_COLOR_RGB(D4D_COLOR666_GET_R((color666)), D4D_COLOR666_GET_G((color666)), D4D_COLOR666_GET_B((color666)))    ///< The macro convert the COLOR in format 18-bit 666 to configured color format by D4D_COLOR_SYSTEM constant
  #define D4D_COLOR_FROM_565(color565) D4D_COLOR_RGB(D4D_COLOR565_GET_R((color565)), D4D_COLOR565_GET_G((color565)), D4D_COLOR565_GET_B((color565)))    ///< The macro convert the COLOR in format 16-bit 565 to configured color format by D4D_COLOR_SYSTEM constant
  #define D4D_COLOR_FROM_555(color555) D4D_COLOR_RGB(D4D_COLOR555_GET_R((color555)), D4D_COLOR555_GET_G((color555)), D4D_COLOR555_GET_B((color555)))    ///< The macro convert the COLOR in format 15-bit 555 to configured color format by D4D_COLOR_SYSTEM constant
  #define D4D_COLOR_FROM_332(color332) D4D_COLOR_RGB(D4D_COLOR332_GET_R((color332)), D4D_COLOR332_GET_G((color332)), D4D_COLOR332_GET_B((color332)))    ///< The macro convert the COLOR in format 8-bit 332 to configured color format by D4D_COLOR_SYSTEM constant
/**@}*/

#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB666

  #define D4D_COLOR_RGB(R,G,B) D4D_COLOR_RGB666(R,G,B)

  #define D4D_COLOR_GET_R(color) D4D_COLOR666_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLOR666_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLOR666_GET_B(color)

  #define D4D_COLOR_FROM_888(color888) D4D_COLOR_RGB(D4D_COLOR888_GET_R((color888)), D4D_COLOR888_GET_G((color888)), D4D_COLOR888_GET_B((color888)))
  #define D4D_COLOR_FROM_666(color666) (color666)
  #define D4D_COLOR_FROM_565(color565) D4D_COLOR_RGB(D4D_COLOR565_GET_R((color565)), D4D_COLOR565_GET_G((color565)), D4D_COLOR565_GET_B((color565)))
  #define D4D_COLOR_FROM_555(color555) D4D_COLOR_RGB(D4D_COLOR555_GET_R((color555)), D4D_COLOR555_GET_G((color555)), D4D_COLOR555_GET_B((color555)))
  #define D4D_COLOR_FROM_332(color332) D4D_COLOR_RGB(D4D_COLOR332_GET_R((color332)), D4D_COLOR332_GET_G((color332)), D4D_COLOR332_GET_B((color332)))


#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB565

  #define D4D_COLOR_RGB(R,G,B) D4D_COLOR_RGB565(R,G,B)

  #define D4D_COLOR_GET_R(color) D4D_COLOR565_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLOR565_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLOR565_GET_B(color)

  #define D4D_COLOR_FROM_888(color888) D4D_COLOR_RGB(D4D_COLOR888_GET_R((color888)), D4D_COLOR888_GET_G((color888)), D4D_COLOR888_GET_B((color888)))
  #define D4D_COLOR_FROM_666(color666) D4D_COLOR_RGB(D4D_COLOR666_GET_R((color666)), D4D_COLOR666_GET_G((color666)), D4D_COLOR666_GET_B((color666)))
  #define D4D_COLOR_FROM_565(color565) (color565)
  #define D4D_COLOR_FROM_555(color555) D4D_COLOR_RGB(D4D_COLOR555_GET_R((color555)), D4D_COLOR555_GET_G((color555)), D4D_COLOR555_GET_B((color555)))
  #define D4D_COLOR_FROM_332(color332) D4D_COLOR_RGB(D4D_COLOR332_GET_R((color332)), D4D_COLOR332_GET_G((color332)), D4D_COLOR332_GET_B((color332)))

#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB555

  #define D4D_COLOR_RGB(R,G,B) D4D_COLOR_RGB555(R,G,B)


  #define D4D_COLOR_GET_R(color) D4D_COLOR555_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLOR555_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLOR555_GET_B(color)

  #define D4D_COLOR_FROM_888(color888) D4D_COLOR_RGB(D4D_COLOR888_GET_R((color888)), D4D_COLOR888_GET_G((color888)), D4D_COLOR888_GET_B((color888)))
  #define D4D_COLOR_FROM_666(color666) D4D_COLOR_RGB(D4D_COLOR666_GET_R((color666)), D4D_COLOR666_GET_G((color666)), D4D_COLOR666_GET_B((color666)))
  #define D4D_COLOR_FROM_565(color565) D4D_COLOR_RGB(D4D_COLOR565_GET_R((color565)), D4D_COLOR565_GET_G((color565)), D4D_COLOR565_GET_B((color565)))
  #define D4D_COLOR_FROM_555(color555) (color555)
  #define D4D_COLOR_FROM_332(color332) D4D_COLOR_RGB(D4D_COLOR332_GET_R((color332)), D4D_COLOR332_GET_G((color332)), D4D_COLOR332_GET_B((color332)))

#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB332

  #define D4D_COLOR_RGB(R,G,B) D4D_COLOR_RGB332(R,G,B)


  #define D4D_COLOR_GET_R(color) D4D_COLOR332_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLOR332_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLOR332_GET_B(color)

  #define D4D_COLOR_FROM_888(color888) D4D_COLOR_RGB(D4D_COLOR888_GET_R((color888)), D4D_COLOR888_GET_G((color888)), D4D_COLOR888_GET_B((color888)))
  #define D4D_COLOR_FROM_666(color666) D4D_COLOR_RGB(D4D_COLOR666_GET_R((color666)), D4D_COLOR666_GET_G((color666)), D4D_COLOR666_GET_B((color666)))
  #define D4D_COLOR_FROM_565(color565) D4D_COLOR_RGB(D4D_COLOR565_GET_R((color565)), D4D_COLOR565_GET_G((color565)), D4D_COLOR565_GET_B((color565)))
  #define D4D_COLOR_FROM_555(color555) D4D_COLOR_RGB(D4D_COLOR555_GET_R((color555)), D4D_COLOR555_GET_G((color555)), D4D_COLOR555_GET_B((color555)))
  #define D4D_COLOR_FROM_332(color332) (color332)

#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_MONO

  #define D4D_COLOR_RGB(R,G,B) D4D_COLOR_RGBMONO(R,G,B)

  #define D4D_COLOR_GET_R(color) D4D_COLORMONO_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLORMONO_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLORMONO_GET_B(color)

  #define D4D_COLOR_FROM_888(color888) D4D_COLOR_RGB(D4D_COLOR888_GET_R((color888)), D4D_COLOR888_GET_G((color888)), D4D_COLOR888_GET_B((color888)))
  #define D4D_COLOR_FROM_666(color666) D4D_COLOR_RGB(D4D_COLOR666_GET_R((color666)), D4D_COLOR666_GET_G((color666)), D4D_COLOR666_GET_B((color666)))
  #define D4D_COLOR_FROM_565(color565) D4D_COLOR_RGB(D4D_COLOR565_GET_R((color565)), D4D_COLOR565_GET_G((color565)), D4D_COLOR565_GET_B((color565)))
  #define D4D_COLOR_FROM_555(color555) D4D_COLOR_RGB(D4D_COLOR555_GET_R((color555)), D4D_COLOR555_GET_G((color555)), D4D_COLOR555_GET_B((color555)))
  #define D4D_COLOR_FROM_332(color332) D4D_COLOR_RGB(D4D_COLOR332_GET_R((color332)), D4D_COLOR332_GET_G((color332)), D4D_COLOR332_GET_B((color332)))
  #define D4D_COLOR_FROM_MONO(colorMono) (colorMono)
#else
  #error The D4D color system (D4D_COLOR_SYSTEM) is not supported. Select supported one.
#endif




/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* D4D Scheme types
*
*//*! @addtogroup doxd4d_scheme_type
* @{
*******************************************************************************/

/*! @brief This is the structure of the color scheme for screens in the D4D. It contains all the necessary
colors to draw a screen under the common color scheme. */
/*! @note This is part of  D4D_CLR_SCHEME_S structure*/
typedef struct
{
  D4D_COLOR desktop;            ///< Color of screen desktop
  D4D_COLOR outline;            ///< Color of screen outline (if enabled)
  D4D_COLOR title_bar;          ///< Color of screen title bar (if enabled)
  D4D_COLOR title_text;         ///< Color of screen title bar text (if enabled)
  D4D_COLOR exitBtnFore;        ///< Fore color of screen exit button
  D4D_COLOR exitBtnBckg;        ///< Background color of screen exit button (if enabled)
}D4D_CLR_SCHEME_SCR;

/*! @brief This structure contains specific colors (non-standard) for the gauge object. */
/*! @note This is part of  D4D_CLR_SCHEME_OBJ structure*/
typedef struct
{
  D4D_COLOR hub;        ///< The gauge hub color.
  D4D_COLOR pointer;    ///< The gauge pointer color.
}D4D_CLR_SCHEME_GAUG;

/*! @brief This structure contains specific colors (non-standard) for the slider object. */
/*! @note This is part of  D4D_CLR_SCHEME_OBJ structure*/
typedef struct
{
  D4D_COLOR barBckg;    ///< The slider bar background color
  D4D_COLOR barFore;    ///< The slider bar fore color
  D4D_COLOR barStart;   ///< The slider bar fore start color for case that the D4D_SLDR_F_BAR_SCALECOLOR flag is enabled
  D4D_COLOR barEnd;     ///< The slider bar fore end color for case that the D4D_SLDR_F_BAR_SCALECOLOR flag is enabled
}D4D_CLR_SCHEME_SLDR;

/*! @brief This structure contains specific colors (non-standard) for the check box object. */
/*! @note This is part of  D4D_CLR_SCHEME_OBJ structure, an is also used by radio button*/
typedef struct
{
  D4D_COLOR iconBckg;   ///< The checkbox background of icon
}D4D_CLR_SCHEME_CHECKB;

/*! @brief This structure contains specific colors (non-standard) for the graph object. */
/*! @note This is part of  D4D_CLR_SCHEME_OBJ structure*/
typedef struct
{
  D4D_COLOR grid;       ///< The graph grid color
}D4D_CLR_SCHEME_GRAPH;

/*! @brief This structure contains specific colors (non-standard) for the progress bar object. */
/*! @note This is part of  D4D_CLR_SCHEME_OBJ structure*/
typedef struct
{
  D4D_COLOR barBckg;    ///< The progress bar, bar background color
  D4D_COLOR barFore;    ///< The progress bar, bar fore start color for case that the D4D_PRGRS_BAR_F_BAR_SCALECOLOR flag is enabled, if not it's used a standard fore bar color.
  D4D_COLOR barEnd;     ///< The progress bar, bar fore end color for case that the D4D_PRGRS_BAR_F_BAR_SCALECOLOR flag is enabled
}D4D_CLR_SCHEME_PRGRS_BAR;

/*! @brief This structure contains all object dependent colors of the color scheme in the D4D. It is
divided into the small objects that describe structures containing individual colors. */
/*! @note This is part of  D4D_CLR_SCHEME_S structure*/
typedef struct
{
  D4D_CLR_SCHEME_GAUG   gauge;          ///< The non standard colors of gauge object
  D4D_CLR_SCHEME_SLDR   slider;         ///< The non standard colors of slider object
  D4D_CLR_SCHEME_CHECKB checkBox;       ///< The non standard colors of check box object
  D4D_CLR_SCHEME_GRAPH   graph;         ///< The non standard colors of graph object
  D4D_CLR_SCHEME_PRGRS_BAR progressBar; ///< The non standard colors of progerss bar object
}D4D_CLR_SCHEME_OBJ;

/*! @brief This is the main structure of the color scheme in the D4D. It contains all the necessary colors
to run the whole driver under the common color scheme. The screen colors are placed in
D4D_CLR_SCHEME_SCR structure and object dependent colors are placed in D4D_CLR_SCHEME_OBJ structure. */
/*! @note This is main eGUI color scheme structure.*/
typedef struct D4D_CLR_SCHEME_S
{
  D4D_CLR_SCHEME_SCR screen;            ///< Sub structure of screen colors
  D4D_COLOR bckg;                       ///< The object background color in standard state
  D4D_COLOR bckgDis;                    ///< The object background color in disabled state
  D4D_COLOR bckgFocus;                  ///< The object background color in focused state
  D4D_COLOR bckgCapture;                ///< The object background color in captured state
  D4D_COLOR fore;                       ///< The object fore color in standard state
  D4D_COLOR foreDis;                    ///< The object fore color in disabled state
  D4D_COLOR foreFocus;                  ///< The object fore color in focused state
  D4D_COLOR foreCapture;                ///< The object fore color in captured state
  D4D_CLR_SCHEME_OBJ objectDepend;      ///< Sub structure of object non standard colors
}D4D_CLR_SCHEME;

typedef const D4D_CLR_SCHEME * D4D_CLR_SCHEME_PTR;

/*! @} End of doxd4d_scheme_type                                               */

/******************************************************************************
* D4D SCHEME setting  macros
*
*//*! @addtogroup doxd4d_scheme_macro
* @{
*******************************************************************************/


/*! ****************************************************************************
* @brief The color scheme has a simple instantiation macro that specifies all colors for screens, common object
*       colors, and object dependent colors. This is a full definition macro that allows setting of all colors in D4D color scheme.
* @param name - Name of the color scheme
* @param scrDesktop - Used as a desktop color of screen
* @param scrOutline -  Used as a color of the screen outline
* @param scrTitleBar - Used as a color of the screen title bar
* @param scrTitleText - Used as a color of the screen title text
* @param scrExitBtnFore - Used as a color of the screen exit button fore color
* @param scrExitBtnBckg - Used as a color of the screen exit button background color
* @param objBckg - Used as a background color of objects
* @param objBckgDis - Used as a background color of objects in disable state
* @param objBckgFocus - Used as a background color of objects in focus state
* @param objBckgCapture - Used as a background color of objects in capture state
* @param objFore - Used as a fore color of objects
* @param objForeDis - Used as a fore color of objects in disable state
* @param objForeFocus - Used as a fore color of objects in focus state
* @param objForeCapture - Used as a fore color of objects in capture state
* @param gaugHub - Used as a color for hub in gauge object
* @param gaugPointer - Used as a color for pointer in gauge object
* @param sldrBarBckg - Used as a background color for bar in slider object
* @param sldrBar - Used as a forecolor for bar in slider object
* @param sldrBarStart - Used as a start color for the bar in the slider object, when the auto color of the bar option is enabled
* @param sldrBarEnd - Used as an end color for a bar in slider object, when the auto color of the bar option is enabled
* @param iconBckg - Used as a background color for icon in check box object
* @param graphGrid - Used as a color of grid lines in graph object
* @param prgrsBarBarBckg - Used as a background color for bar in progress bar object
* @param prgrsBarBar - Used as a forecolor and start color for bar in progress bar object
* @param prgrsBarBarEnd - Used as an end color for a bar in progress bar object, when the auto color of the bar option is enabled
* @note This instantiation macro creates a color scheme in the non-volatile memory (ROM).
*******************************************************************************/
#define D4D_DECLARE_CLR_SCHEME(name, \
        scrDesktop, scrOutline, scrTitleBar, scrTitleText, scrExitBtnFore, scrExitBtnBckg,\
        objBckg, objBckgDis, objBckgFocus, objBckgCapture,\
        objFore, objForeDis, objForeFocus, objForeCapture,\
        gaugHub, gaugPointer,\
        sldrBarBckg, sldrBar, sldrBarStart, sldrBarEnd,\
        iconBckg,\
        graphGrid,\
        prgrsBarBarBckg, prgrsBarBar, prgrsBarBarEnd\
        )\
        static const D4D_CLR_SCHEME name = \
        { \
          { scrDesktop, scrOutline, scrTitleBar, scrTitleText, scrExitBtnFore , scrExitBtnBckg}, \
          objBckg, objBckgDis, objBckgFocus, objBckgCapture,\
          objFore, objForeDis, objForeFocus, objForeCapture,\
          {\
            {gaugHub, gaugPointer },\
            {sldrBarBckg, sldrBar, sldrBarStart, sldrBarEnd },\
            {iconBckg },\
            {graphGrid },\
            {prgrsBarBarBckg, prgrsBarBar, prgrsBarBarEnd }\
          }\
        };

/*! @brief The color scheme has a simple instantiation macro that specifies all colors for screens, common object
*       colors, and object dependent colors. This is a full definition macro that allows setting of all colors in D4D color scheme.
* @param name - Name of the color scheme
* @param scrDesktop - Used as a desktop color of screen
* @param scrOutline -  Used as a color of the screen outline
* @param scrTitleBar - Used as a color of the screen title bar
* @param scrTitleText - Used as a color of the screen title text
* @param scrExitBtnFore - Used as a color of the screen exit button fore color
* @param scrExitBtnBckg - Used as a color of the screen exit button background color
* @param objBckg - Used as a background color of objects
* @param objBckgDis - Used as a background color of objects in disable state
* @param objBckgFocus - Used as a background color of objects in focus state
* @param objBckgCapture - Used as a background color of objects in capture state
* @param objFore - Used as a fore color of objects
* @param objForeDis - Used as a fore color of objects in disable state
* @param objForeFocus - Used as a fore color of objects in focus state
* @param objForeCapture - Used as a fore color of objects in capture state
* @param gaugHub - Used as a color for hub in gauge object
* @param gaugPointer - Used as a color for pointer in gauge object
* @param sldrBarBckg - Used as a background color for bar in slider object
* @param sldrBar - Used as a forecolor for bar in slider object
* @param sldrBarStart - Used as a start color for the bar in the slider object, when the auto color of the bar option is enabled
* @param sldrBarEnd - Used as an end color for a bar in slider object, when the auto color of the bar option is enabled
* @param iconBckg - Used as a background color for icon in check box object
* @param graphGrid - Used as a color of grid lines in graph object
* @param prgrsBarBarBckg - Used as a background color for bar in progress bar object
* @param prgrsBarBar - Used as a forecolor and start color for bar in progress bar object
* @param prgrsBarBarEnd - Used as an end color for a bar in progress bar object, when the auto color of the bar option is enabled
* @note This instantiation macro creates a color scheme in the volatile memory (RAM).*/
#define D4D_DECLARE_CLR_SCHEME_INRAM(name, \
        scrDesktop, scrOutline, scrTitleBar, scrTitleText, scrExitBtnFore, scrExitBtnBckg,\
        objBckg, objBckgDis, objBckgFocus, objBckgCapture,\
        objFore, objForeDis, objForeFocus, objForeCapture,\
        gaugHub, gaugPointer,\
        sldrBarBckg, sldrBar, sldrBarStart, sldrBarEnd,\
        iconBckg,\
        graphGrid,\
        prgrsBarBarBckg, prgrsBarBar, prgrsBarBarEnd\
        )\
        D4D_CLR_SCHEME name = \
        { \
          { scrDesktop, scrOutline, scrTitleBar, scrTitleText, scrExitBtnFore , scrExitBtnBckg}, \
          objBckg, objBckgDis, objBckgFocus, objBckgCapture,\
          objFore, objForeDis, objForeFocus, objForeCapture,\
          {\
            {gaugHub, gaugPointer },\
            {sldrBarBckg, sldrBar, sldrBarStart, sldrBarEnd },\
            {iconBckg },\
            {graphGrid },\
            {prgrsBarBarBckg, prgrsBarBar, prgrsBarBarEnd }\
          }\
        };
/*! @} End of doxd4d_scheme_macro                                               */

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/


#endif  /* __D4D_SCHEME_H */
