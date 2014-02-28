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
* @file      d4d_types.h
*
* @author     Petr Gargulak
*
* @version   0.0.37.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver global types header file
*
*******************************************************************************/


#ifndef __D4D_TYPES_H
#define __D4D_TYPES_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Doxygen Documentation settings
******************************************************************************/
#ifdef __EGUI_DOXYGEN__

  #undef D4D_COLOR_SYSTEM
  #define D4D_COLOR_SYSTEM D4D_COLOR_SYSTEM_RGB888

  #undef D4D_ROUND_CORNER_ENABLE
  #define D4D_ROUND_CORNER_ENABLE D4D_TRUE

  #undef D4D_MCU_TYPE
  #define D4D_MCU_TYPE D4D_MK

  #define D4D_UNICODE

  #define d4dlcd_template_ID 1

  #undef D4D_LLD_LCD
  #define D4D_LLD_LCD d4dlcd_template_ID

  #undef D4D_LLD_LCD_HW
  #define D4D_LLD_LCD_HW d4dlcdhw_template_ID

  #undef D4D_LLD_TCH
  #define D4D_LLD_TCH d4dtch_template_ID

  #undef D4D_LLD_TCH_HW
  #define D4D_LLD_TCH_HW d4dtchhw_template_ID

  #undef D4D_LLD_MOUSE
  #define D4D_LLD_MOUSE nothing

  #undef D4D_STRINGTABLE_PREFIX
  #define D4D_STRINGTABLE_PREFIX '@'

  #

#endif



/******************************************************************************
* D4D TYPES setting  constants
*
*//*! @addtogroup doxd4d_types_const
* @{
*******************************************************************************/

/*! @brief This is definition of boolean operation value in eGUI - FALSE.*/
#define D4D_FALSE 0
/*! @brief This is definition of boolean operation value in eGUI - TRUE.*/
#define D4D_TRUE  1

// The MCU type list
/*!
  @defgroup doxd4d_types_const_mcuTypes D4D TYPES Defines MCU types constants
  This section specifies MCU types constants.
  @ingroup doxd4d_types_const
*/

/**
 * @addtogroup doxd4d_types_const_mcuTypes
 * @{
 */
  #define  D4D_HC08       1       ///< The group of MCU with CORE HC08
  #define  D4D_HCS08      2       ///< The group of MCU with CORE HCS08
  #define  D4D_HC12       10      ///< The group of MCU with CORE HC12
  #define  D4D_HCS12      11      ///< The group of MCU with CORE HCS12
  #define  D4D_HCS12X     12      ///< The group of MCU with CORE HCS12X
  #define  D4D_MCF51      50      ///< The group of MCU with CORE MCF51 (ColdFire 1)
  #define  D4D_MCF52      51      ///< The group of MCU with CORE MCF52 (ColdFire 2)
  #define  D4D_MCF53      52      ///< The group of MCU with CORE MCF53 (ColdFire 3)
  #define  D4D_MCF54      53      ///< The group of MCU with CORE MCF54 (ColdFire 4)
  #define  D4D_MPC51      60      ///< The group of MCU with CORE MCP51 (Power PC)
  #define  D4D_MK         80      ///< The group of MCU with CORE MK    (Kinetis family)
/**@}*/


/*! @brief Standard variable types disable eGUI declaration.
           If not defined, it sets to (D4D_FALSE) as a default. So it's enable type definition like Byte, Word, etc...*/
#ifndef D4D_USE_STANDARD_TYPES
  #define D4D_USE_STANDARD_TYPES D4D_FALSE
  #warning "The D4D is using own standard types Byte, Word etc. , because the D4D_USE_STANDARD_TYPES is not defined!"
#endif

/*! @} End of doxd4d_types_const                                              */

/******************************************************************************
* D4D Standard types
*
*//*! @addtogroup doxd4d_types_type
* @{
*******************************************************************************/
#if (D4D_USE_STANDARD_TYPES != D4D_TRUE)
  #ifndef Byte
    /*! @brief Type definition of Byte (unsigned 8-bit). */
    typedef unsigned char       Byte;
  #endif
  #ifndef sByte
    /*! @brief Type definition of sByte (signed 8-bit). */
    typedef signed char         sByte;
  #endif
  #ifndef Word
    /*! @brief Type definition of Word (unsigned 16-bit). */
    typedef unsigned short      Word;
  #endif
  #ifndef sWord
    /*! @brief Type definition of sWord (signed 16-bit). */
    typedef signed short        sWord;
  #endif
  #ifndef LWord
    /*! @brief Type definition of LWord (unsigned 32-bit). */
    typedef unsigned long       LWord;
  #endif
  #ifndef sLWord
    /*! @brief Type definition of sLWord (signed 32-bit). */
    typedef signed long         sLWord;
  #endif
  #ifndef DLWord
    /*! @brief Type definition of DLWord (unsigned 64-bit). */
    typedef unsigned long long  DLWord;
  #endif
  #ifndef sDLWord
    /*! @brief Type definition of sDLWord (signed 64-bit). */
    typedef signed long long    sDLWord;
  #endif

  #ifndef NULL
    /*! @brief Type definition of null pointer. */
    #define NULL (void*)0
  #endif

#else
  #if D4D_MCU_TYPE == D4D_HCS08
    #include <hidef.h>
  #endif
#endif


#if (D4D_MCU_TYPE == D4D_HC08) || (D4D_MCU_TYPE == D4D_HCS08)
  typedef Byte D4D_BOOL;
  typedef Word D4D_INDEX;
  typedef sWord D4D_INDEX_DELTA;
#elif (D4D_MCU_TYPE == D4D_HC12) || (D4D_MCU_TYPE == D4D_HCS12) || (D4D_MCU_TYPE == D4D_HCS12X)
  typedef Word D4D_BOOL;
  typedef Word D4D_INDEX;
  typedef sWord D4D_INDEX_DELTA;
#elif (D4D_MCU_TYPE == D4D_MCF51) || (D4D_MCU_TYPE == D4D_MCF52) || (D4D_MCU_TYPE == D4D_MPC51) || (D4D_MCU_TYPE == D4D_MK)
  /*! @brief Type definition of eGUI boolean. */
  typedef LWord D4D_BOOL;
  /*! @brief Type definition of eGUI general index variables.*/
  typedef LWord D4D_INDEX;
  /*! @brief Type definition of eGUI general index variables for signed offsets.*/
  typedef sLWord D4D_INDEX_DELTA;
#else
  #error "Unsupported MCU type for D4D_BOOL type!"
#endif


#ifdef D4D_COOR_TYPE
  /*! @brief Type definition of eGUI coordination variables.*/
  typedef D4D_COOR_TYPE D4D_COOR;
#else
  /*! @brief Type definition of eGUI coordination variables.*/
  typedef Byte D4D_COOR;
#endif

/*! @brief Type definition of eGUI point structure.*/
typedef struct
{
    D4D_COOR x; ///< Coordination in axis X
    D4D_COOR y; ///< Coordination in axis Y
} D4D_POINT;

/*! @brief Type definition of eGUI size structure.*/
typedef struct
{
    D4D_COOR cx;        ///< Size in axis X (width)
    D4D_COOR cy;        ///< Size in axis Y (height)
} D4D_SIZE;

/*! @brief Type definition of eGUI geometry structure.*/
typedef struct
{
  D4D_POINT pnt;        ///< Left Top corner point
  D4D_SIZE sz;          ///< Size of object
}D4D_GEOMETRY;

/*! @brief Type definition of eGUI client area margin structure.*/
typedef struct
{
  D4D_COOR left;        ///< Margin of left side
  D4D_COOR top;         ///< Margin of top side
  D4D_COOR right;       ///< Margin of right side
  D4D_COOR bottom;      ///< Margin of bottom side
}D4D_MARGIN, *D4D_MARGIN_PTR;

/*! @brief Type definition of eGUI tabulation structure.*/
typedef struct
{
  D4D_COOR* pTabTable;  ///< Pointer to tabulator table
  D4D_COOR tabOffset;   ///< Start offset of tabulator
}D4D_TAB;

// Color type definition
#if ((D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB888) || (D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB666))
  /*! @brief Type definition of eGUI color variables.*/
  typedef LWord D4D_COLOR;
#elif ((D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB565) || (D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB555))
  typedef Word D4D_COLOR;
#elif ((D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB332) || (D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_MONO))
  typedef Byte D4D_COLOR;
#else
  #error D4D_COLOR not defined due to unsupported D4D system color.
#endif

#ifndef D4D_WCHAR
  /*! @brief Type definition of eGUI wide char.*/
  typedef unsigned short D4D_WCHAR;
#endif

/*! @brief Type definition of eGUI ASCII character.*/
typedef char D4D_CHAR;

#ifdef D4D_UNICODE
  /*! @brief Type definition of eGUI character (it depends on UNICODE setting if this is \ref D4D_CHAR or \ref D4D_WCHAR).*/
  typedef D4D_WCHAR D4D_TCHAR;
#else
  /*! @brief Type definition of eGUI character (it depends on UNICODE setting if this is \ref D4D_CHAR or \ref D4D_WCHAR).*/
  typedef D4D_CHAR D4D_TCHAR;
#endif

/*! @brief Type definition of eGUI line type.*/
typedef enum
{
  D4D_LINE_THIN = 0x00, ///< Line type thin
  D4D_LINE_THICK        ///< Line type thick
}D4D_LINETYPE;

/*! @brief Type definition of eGUI file pointer.*/
typedef void* D4D_FILEPTR;

/*! @brief Type definition of eGUI keys - this is obsolete type that is kept just only for backward compatibility.*/
typedef Byte D4D_KEYS;

/*! @brief Type definition of eGUI keys scan code.*/
typedef Byte D4D_KEY_SCANCODE;

/*! @brief Type definition of eGUI direction type.*/
typedef enum
{
  D4D_DIR_UP,           ///< Direction UP
  D4D_DIR_DOWN,         ///< Direction DOWN
  D4D_DIR_RIGHT,        ///< Direction RIGHT
  D4D_DIR_LEFT          ///< Direction LEFT
}D4D_DIRECTION;

/*! @brief Type definition of eGUI trend type.*/
typedef enum
{
  D4D_CLOCK_WISE = 0,   ///< Trend Clock Wise
  D4D_ANTI_CLOCK_WISE   ///< Trend Anti Clock Wise
}D4D_TREND;

/*! @brief Type definition of eGUI circle quadrant.*/
typedef enum
{
  D4D_QUAD_1,   ///< First quadrant <0-90)
  D4D_QUAD_2,   ///< Second quadrant <90-180)
  D4D_QUAD_3,   ///< Third quadrant <180-270)
  D4D_QUAD_4    ///< Fourth quadrant <270-360)
}D4D_QUADRANT;

/*! @brief Type definition of eGUI bevel type.*/
typedef enum
{
  D4D_BEVEL_NONE = 0,           ///< None Bevel is used
  D4D_BEVEL_RAISED = 1,         ///< Raised bevel type
  D4D_BEVEL_DENTED = 2,         ///< Dented bevel type
  D4D_BEVEL_RESERVED = 3        ///< Reserved for future use
}D4D_BEVEL;

/*! @brief Type definition of eGUI display orientation.*/
typedef enum
{
  D4D_ORIENT_PORTRAIT,	        ///< Portrait orientation
  D4D_ORIENT_PORTRAIT180,	///< Portrait up side down orientation
  D4D_ORIENT_LANDSCAPE,	        ///< Landscape orientation
  D4D_ORIENT_LANDSCAPE180       ///< Landscape up side down orientation
} D4D_ORIENTATION;

/*! @brief Type definition of eGUI touch screen calibration structure.*/
typedef struct
{
  unsigned char ScreenCalibrated;               ///< Boolean if the values are valid
  unsigned short TouchScreenXoffset;            ///< Input value offset in axis X
  unsigned short TouchScreenYoffset;            ///< Input value offset in axis Y
  unsigned short TouchScreenXBitsPerPixelx16;   ///< Input value gain per 16 pixels in axis X
  unsigned short TouchScreenYBitsPerPixelx16;   ///< Input value gain per 16 pixels in axis Y
}D4D_TOUCHSCREEN_CALIB;

/*! @brief Type definition of eGUI touch screen event structure.*/
typedef struct
{
  D4D_BOOL touched;     ///< Boolean of touch state (touched or non touched)
  D4D_POINT point;      ///< Point of touch event or lost of touch event
}D4D_TOUCHSCREEN_EVENT;

/*! @} End of doxd4d_types_type                                               */
/******************************************************************************
* Macros
******************************************************************************/



#endif /* __D4D_TYPES_H */
