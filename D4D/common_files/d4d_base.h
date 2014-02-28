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
* @file      d4d_base.h
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.107.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver core and base functions header file
*
*******************************************************************************/

#ifndef __D4D_BASE_H
#define __D4D_BASE_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* D4D BASE setting  constants
*
*//*! @addtogroup doxd4d_base_const
* @{
*******************************************************************************/
/*! @brief There is stored version number of eGUI in string format - DO NOT CHANGE
           To check release history - read the release_notes.txt file */
#define D4D_VERSION "v3.00"

/*! @brief This macro is used to select the running platform of eGUI code.
           If not defined, the Kinetis platform is used as a default.*/
#ifndef D4D_MCU_TYPE
  #define D4D_MCU_TYPE D4D_MK
  #warning "There is no definition for D4D_MCU_TYPE in configuration file. The driver is using as default D4D_MK."
#endif

/*! @brief This macro is used to set the screen buffer depth.
           If not defined, the 5 is used as a default.*/
#ifndef D4D_SCREEN_HISTORY
  #define D4D_SCREEN_HISTORY 5
#endif


#ifdef D4D_ENABLE_AUTOSIZE
/*! @brief This macro is checking just absolete eGUI function - AUTOSIZE.
           In case that somebody during update to new version of eGUI was using AUTOSIZR capability the compilation error is shown.*/
  #if D4D_ENABLE_AUTOSIZE != D4D_FALSE
    #error The D4D_ENABLE_AUTOSIZE capability had removed from eGUI since version 3.00. For update your eGUI project just define size for all objects.
  #endif
#endif

/*! @brief This macro is used to enable the round corners feature.
           If not defined, the round corners are disabled as a default.*/
#ifndef D4D_ROUND_CORNER_ENABLE
  #define D4D_ROUND_CORNER_ENABLE D4D_FALSE
#endif

/*! @brief This macro is used to enable the drawing fonts feature.
           If not defined, the fonts are disabled as a default.*/
#ifndef D4D_FONT_TABLE_DISABLED
  #define D4D_FONT_TABLE_DISABLED D4D_FALSE
  #warning "Font table is disabled by default, set the D4D_FONT_TABLE_DISABLED to D4D_TRUE in config file."
#endif

/*! @brief This macro is used to define the size of longer side of LCD.
           If not defined, the longer side is set to 320 as a default.*/
#ifndef D4D_SCREEN_SIZE_LONGER_SIDE
	#define D4D_SCREEN_SIZE_LONGER_SIDE 320
	#warning "The D4D configuration file doesn't contains the resolution information for longer axis. The D4D sets this value to default: 320 pixels."
#endif

/*! @brief This macro is used to define the size of shorter side of LCD.
           If not defined, the shorter side is set to 240 as a default.*/
#ifndef D4D_SCREEN_SIZE_SHORTER_SIDE
	#define D4D_SCREEN_SIZE_SHORTER_SIDE 240
	#warning "The D4D configuration file doesn't contains the resolution information for shorter axis. The D4D sets this value to default: 240 pixels."
#endif

/*! @brief This macro is used to define the bus clock of MCU that's eGUI runs.
           If not defined, the 24MHz as a default.*/
#ifndef D4D_MCU_BUS_CLOCK
  #define D4D_MCU_BUS_CLOCK 24000000L
#endif

/*! @brief This macro is used to define the system fore color (For example used in touch screen calibration).
           If not defined, the system fore color is sets D4D_COLOR_YELLOW as a default.*/
#ifndef D4D_COLOR_SYSTEM_FORE
  #define D4D_COLOR_SYSTEM_FORE D4D_COLOR_YELLOW
#endif

/*! @brief This macro is used to define the system background color (For example used in touch screen calibration).
           If not defined, the system background color is sets D4D_COLOR_BLACK as a default.*/
#ifndef D4D_COLOR_SYSTEM_BCKG
  #define D4D_COLOR_SYSTEM_BCKG D4D_COLOR_BLACK
#endif

/*! @brief This macro is used to define the system font index (For example used in touch screen calibration).
           If not defined, the system font index is sets 0 as a default.*/
#ifndef D4D_FONT_SYSTEM_DEFAULT
  #define D4D_FONT_SYSTEM_DEFAULT 0
#endif

/*! @brief This macro is used to enable external D4D bitmap support.
           If not defined, the external D4D bitmap support is disabled as a default.*/
#ifndef D4D_BMP_EXTSRC_SUPPORT_D4D
  #define D4D_BMP_EXTSRC_SUPPORT_D4D D4D_FALSE
#endif

/*! @brief This macro is used to enable external MicroSoft BMP bitmap support.
           If not defined, the external MicroSoft BMP bitmap support is disabled as a default.*/
#ifndef D4D_BMP_EXTSRC_SUPPORT_BMP
  #define D4D_BMP_EXTSRC_SUPPORT_BMP D4D_FALSE
#endif

/*! @brief This macro is used to enable external texts support.
           If not defined, the external texts support is disabled as a default.*/
#ifndef D4D_EXTSRC_TEXT_ENABLE
	#define D4D_EXTSRC_TEXT_ENABLE D4D_FALSE
#endif

/*! @brief This macro is used to enable external files support.
           If not defined, the external external file support is enabled based on external texts and bitmap support.*/
#ifndef D4D_EXTSRC_FILE_ENABLE
	#if (D4D_BMP_EXTSRC_SUPPORT_BMP != D4D_FALSE) || (D4D_BMP_EXTSRC_SUPPORT_D4D != D4D_FALSE) || (D4D_FNT_EXTSRC_SUPPORT != D4D_FALSE)
		#define D4D_EXTSRC_FILE_ENABLE D4D_TRUE
	#else
		#define D4D_EXTSRC_FILE_ENABLE D4D_FALSE
	#endif
#endif

/*! @brief This macro is used to specify size of buffer that is using when external file support is enabled.
           If not defined, the external buffer size is set 64 as a default.*/
#ifndef D4D_EXTSRC_BUFF_SIZE
  #define D4D_EXTSRC_BUFF_SIZE 64
#endif

/*! @brief This macro is used to enable calling flush output with drawing each elementary graphic element.
           If not defined, the flushing after each elementary part is disabled as a default.*/
#ifndef D4D_LLD_FLUSH_ELEMENT
  #define D4D_LLD_FLUSH_ELEMENT D4D_FALSE
#endif

/*! @brief This macro is used to enable some extra debug functionality.
           If not defined, the debug functionality is disabled as a default.*/
#ifndef D4D_DEBUG
  #define D4D_DEBUG D4D_FALSE
#endif

/*! @brief Constant for Big Endianism. */
#define D4D_ENDIAN_BIG 0
/*! @brief Constant for Little Endianism. */
#define D4D_ENDIAN_LITTLE 1

/*! @brief This macro is used to specify current Endianim of CPU architecture.
           If not defined, the macro is defined by used CPU type as a default.*/
#ifndef D4D_ENDIAN
  #if (D4D_MCU_TYPE == D4D_MK)
    #define D4D_ENDIAN D4D_ENDIAN_LITTLE
  #else  // for other cases (this should not work for every cases)
    #define D4D_ENDIAN D4D_ENDIAN_BIG
  #endif
#endif

/*********************************************************
*
* D4D align constants
*
*********************************************************/


/*!
  @defgroup doxd4d_base_const_alignmasks D4D BASE Defines masks of aligment properties
  This section specifies masks of aligment properties.
  @ingroup doxd4d_base_const
*/

/**
 * @addtogroup doxd4d_base_const_alignmasks
 * @{
 */

  /*! @brief The aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_MASK                  (0x0F)

  /*! @brief The horizontal aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_H_MASK                (0x03)
  /*! @brief The horizontal left aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_H_LEFT_MASK           (0x00)
  /*! @brief The horizontal right aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_H_RIGHT_MASK          (0x01)
  /*! @brief The horizontal center aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_H_CENTER_MASK         (0x02)

  /*! @brief The vertical aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_V_MASK                (0x0C)
  /*! @brief The vertical top aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_V_TOP_MASK            (0x00)
  /*! @brief The vertical bottom aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_V_BOTTOM_MASK         (0x04)
  /*! @brief The vertical center aligment option mask (for texts and also bitmaps).   */
  #define D4D_ALIGN_V_CENTER_MASK         (0x08)

/**@}*/

/*********************************************************
*
* keys setting  constants
*
*********************************************************/



/*! @brief This macro is used to specify Key UP in mask mode (obsolete).
           If not defined, it sets to 0x01 as a default.*/
#ifndef D4D_KEY_UP
  #define D4D_KEY_UP     0x01
#endif
/*! @brief This macro is used to specify Key DOWN in mask mode (obsolete).
           If not defined, it sets to 0x02 as a default.*/
#ifndef D4D_KEY_DOWN
  #define D4D_KEY_DOWN   0x02
#endif
/*! @brief This macro is used to specify Key LEFT in mask mode (obsolete).
           If not defined, it sets to 0x04 as a default.*/
#ifndef D4D_KEY_LEFT
  #define D4D_KEY_LEFT   0x04
#endif
/*! @brief This macro is used to specify Key RIGHT in mask mode (obsolete).
           If not defined, it sets to 0x08 as a default.*/
#ifndef D4D_KEY_RIGHT
  #define D4D_KEY_RIGHT  0x08
#endif
/*! @brief This macro is used to specify Key ENTER in mask mode (obsolete).
           If not defined, it sets to 0x10 as a default.*/
#ifndef D4D_KEY_ENTER
  #define D4D_KEY_ENTER  0x10
#endif
/*! @brief This macro is used to specify Key ESC in mask mode (obsolete).
           If not defined, it sets to 0x20 as a default.*/
#ifndef D4D_KEY_ESC
  #define D4D_KEY_ESC    0x20
#endif

/*! @brief This macro is used to specify Key Scan Code UP.
           If not defined, it sets to 0x51 as a default.*/
#ifndef D4D_KEY_SCANCODE_UP
  #define D4D_KEY_SCANCODE_UP     0x51
#endif
/*! @brief This macro is used to specify Key Scan Code DOWN.
           If not defined, it sets to 0x50 as a default.*/
#ifndef D4D_KEY_SCANCODE_DOWN
  #define D4D_KEY_SCANCODE_DOWN   0x50
#endif
/*! @brief This macro is used to specify Key Scan Code LEFT.
           If not defined, it sets to 0x4B as a default.*/
#ifndef D4D_KEY_SCANCODE_LEFT
  #define D4D_KEY_SCANCODE_LEFT   0x4B
#endif
/*! @brief This macro is used to specify Key Scan Code RIGHT.
           If not defined, it sets to 0x4D as a default.*/
#ifndef D4D_KEY_SCANCODE_RIGHT
  #define D4D_KEY_SCANCODE_RIGHT  0x4D
#endif
/*! @brief This macro is used to specify Key Scan Code ENTER.
           If not defined, it sets to 0x1C as a default.*/
#ifndef D4D_KEY_SCANCODE_ENTER
  #define D4D_KEY_SCANCODE_ENTER  0x1C
#endif
/*! @brief This macro is used to specify Key Scan Code ESC.
           If not defined, it sets to 0x01 as a default.*/
#ifndef D4D_KEY_SCANCODE_ESC
  #define D4D_KEY_SCANCODE_ESC    0x01
#endif

/*! @brief Mask of scan code for key index. Lower 7 bits of Byte.*/
#define D4D_KEY_SCANCODE_KEYMASK  0x7F
/*! @brief Mask of scan code for key release. MSB bit of Byte. If this bit is clear, the goes down, if sets the key is released.*/
#define D4D_KEY_SCANCODE_RELEASEMASK  0x80

/*! @brief This macro is used to specify standard system function to to set focus to next object.
           If not defined, it sets to D4D_KEY_SCANCODE_DOWN as a default.*/
#ifndef D4D_KEY_FUNC_FOCUS_NEXT
  #define D4D_KEY_FUNC_FOCUS_NEXT    D4D_KEY_SCANCODE_DOWN
#endif

/*! @brief This macro is used to specify standard system function to to set focus to previous object.
           If not defined, it sets to D4D_KEY_SCANCODE_UP as a default.*/
#ifndef D4D_KEY_FUNC_FOCUS_PREV
  #define D4D_KEY_FUNC_FOCUS_PREV    D4D_KEY_SCANCODE_UP
#endif

/*! @brief This macro is used to specify leght of keybord events buffer.
           If not defined, it sets to 4 events as a default.*/
#ifndef D4D_KEYS_BUFF_LENGTH
  #define D4D_KEYS_BUFF_LENGTH 4
#endif

/*********************************************************
*
* touch screen constants
*
*********************************************************/

/*! @brief This macro is used in touch screen calibration to define offset of calibration points from edge of LCD.
           If not defined, it sets to 30 pixels as a default.*/
#ifndef D4DTCH_CALIB_CROSS_OFFSET
  #define D4DTCH_CALIB_CROSS_OFFSET 30
#endif

/*! @} End of doxd4d_base_const                                               */

/******************************************************************************
* Internal constants
******************************************************************************/
#if D4D_SCREEN_SIZE_SHORTER_SIDE > D4D_SCREEN_SIZE_LONGER_SIDE
  #warning "The D4D_SCREEN_SIZE_SHORTER_SIDE macro has greater value then D4D_SCREEN_SIZE_LONGER_SIDE in d4d_cfg.h file. Please check!"
#endif

/******************************************************************************
* D4D Base types
*
*//*! @addtogroup doxd4d_base_type
* @{
*******************************************************************************/
/*! @brief Drawing object flags type, handled to object in D4D_MSG_DRAW events.*/
typedef Byte D4D_OBJECT_DRAWFLAGS;

#define D4D_OBJECT_DRAWFLAGS_COMPLETE   (0x01)  ///< Draw complete flag
#define D4D_OBJECT_DRAWFLAGS_FOCUSED    (0x02)  ///< Draw fosused state flag
#define D4D_OBJECT_DRAWFLAGS_CAPTURING  (0x04)  ///< Draw capturing state flag
#define D4D_OBJECT_DRAWFLAGS_STATE      (0x08)  ///< Draw just change of state flag

/*! @brief D4D object messages types enumeration.*/
typedef enum
{
    D4D_MSG_NONE,               ///< Dummy message - could be strached.
    D4D_MSG_ONINIT,             ///< On Init message - is send for first time when the object is inicialized.
    D4D_MSG_DRAW,               ///< Draw message - is send when the object should be redrawed.
    D4D_MSG_DRAWDONE,           ///< Draw Done message - is send after the object is redrawed.
    D4D_MSG_SETFOCUS,           ///< Set Focus message - is send when the object is getting focus.
    D4D_MSG_KILLFOCUS,          ///< Kill Focus message - is send when the object is losing focus.
    D4D_MSG_SETCAPTURE,         ///< Set Capture message - is send when the object is getting capture keys status.
    D4D_MSG_KILLCAPTURE,        ///< Kill Capture message - is send when the object is losing capture keys status.
    D4D_MSG_KEYUP,              ///< Key Up message - is send when the object get new Key Up event.
    D4D_MSG_KEYDOWN,            ///< Key Down message - is send when the object get new Key Down event.
    D4D_MSG_TOUCHED,            ///< Touched message - is send when the object is touched by touch screen driver.
    D4D_MSG_TOUCH_AUTO,         ///< Auto Touched message - is send when the object is still keep touch by touch screen driver .
    D4D_MSG_TOUCH_LOST,         ///< Lost Touch message - is send when the screen is still touch but the coordination leaves the original touched object.
    D4D_MSG_UNTOUCHED,          ///< UnTouched message - is send when the screen is untouched.
    D4D_MSG_MOUSE_BTN_LEFT_DOWN,        ///< Mouse Left Button Click  message - is send in case that mouse left click is detected on this object.
    D4D_MSG_MOUSE_BTN_LEFT_UP,          ///< Mouse Left Button Release  message - is send in case that mouse left release is detected on this object.
    D4D_MSG_MOUSE_BTN_RIGHT_DOWN,       ///< Mouse Right Button Click  message - is send in case that mouse right click is detected on this object.
    D4D_MSG_MOUSE_BTN_RIGHT_UP,         ///< Mouse Right Button Release  message - is send in case that mouse right release is detected on this object.
    D4D_MSG_MOUSE_BTN_MIDDLE_DOWN,      ///< Mouse Middle Button Click  message - is send in case that mouse middle click is detected on this object.
    D4D_MSG_MOUSE_BTN_MIDDLE_UP,        ///< Mouse Middle Button Release  message - is send in case that mouse middle release is detected on this object.
    D4D_MSG_MOUSE_BTN_WHEEL_UP,         ///< Mouse Whell Move Up  message - is send in case that mouse whell move up is detected on this object.
    D4D_MSG_MOUSE_BTN_WHEEL_DOWN,       ///< Mouse Whell Move Down message - is send in case that mouse whell move down is detected on this object.
    D4D_MSG_MOUSE_ENTER,        ///< Mouse Cursor Enter message - is send in case that mouse cursor enter to object area.
    D4D_MSG_MOUSE_LEAVE,        ///< Mouse Cursor Leave message - is send in case that mouse cursor leave the object area.
    D4D_MSG_TIMETICK            ///< Time Tick Occur message - is send with each time tick to object.
} D4D_MSGID;

/* the message object */
struct D4D_OBJECT_S;

/*! @brief D4D object messages structure.*/
typedef struct D4D_MESSAGE_S
{
    struct D4D_OBJECT_S* pObject;       ///< Pointer to object who is receiver of this message. If the receiver is just screen this field must be NULL.
    struct D4D_SCREEN_S* pScreen;       ///< Pointer to screen who is receiver of this message.
    D4D_MSGID   nMsgId;                 ///< Type of message

    union
    {
        D4D_OBJECT_DRAWFLAGS draw;      ///< Contains draw flags - is valid with D4D_MSG_DRAW and D4D_MSG_DRAWDONE message.
        D4D_KEY_SCANCODE key;           ///< There will be stored only code of key without release / press information - is valid with D4D_MSG_KEYUP and D4D_MSG_KEYDOWN message.

    } prm;                              ///< Additional data for some type of messages

} D4D_MESSAGE;


typedef enum
{
  D4D_MSG_NOSKIP = 0,                ///< The return value of user/application message function - continue handling of message by eGUI
  D4D_MSG_SKIP = 1                  ///< The return value of user/application message function - skip handling of message by eGUI
}D4D_MSG_HANDLER_ACTION;

/*********************************************************
*
* global events
*
*********************************************************/

/*! @brief D4D events that is handled by object callback functions.*/
typedef enum
{
    D4D_EVENT_ONCLICK,          ///< On Click Event.
    D4D_EVENT_ONDOUBLECLICK,    ///< On Double Click Event.
    D4D_EVENT_ONCHANGE,         ///< On Change Event.
    D4D_EVENT_ONCHANGEDONE,     ///< On Change done (complete change is finished.
    D4D_EVENT_ONRELOAD,         ///< On Reload.
    D4D_EVENT_ONCLEAR,          ////< On Clear.

    D4D_EVENT_CNT               ///< Keep it on end of list (there is stored count of events)
} D4D_EVENTID;

/*! @} End of doxd4d_base_type                                               */
/******************************************************************************
* Internal types
******************************************************************************/

// Internal keys buffer structure
typedef struct
{
  D4D_KEY_SCANCODE buff[D4D_KEYS_BUFF_LENGTH];
  Byte readPos;
  Byte writePos;
} D4D_KEYS_BUFFER;

// Internal touch screen status
typedef Byte D4D_TOUCHSCREEN_STATUS;

#define D4D_TCHSCR_STATUS_TOUCHED       (0x01) // The new touch has to be 0x01
#define D4D_TCHSCR_STATUS_RESERVED      (0x02)
#define D4D_TCHSCR_STATUS_NEWTOUCH      (0x04)
#define D4D_TCHSCR_STATUS_RESERVED1     (0x08)
#define D4D_TCHSCR_STATUS_LOSTTOUCH     (0x10)
#define D4D_TCHSCR_STATUS_NEWRAWEVENT   (0x20)
#define D4D_TCHSCR_STATUS_NEWEVENT      (0x40)
#define D4D_TCHSCR_STATUS_CHECKTOUCH    (0x80)

#define D4D_TCHSCR_STATUS_EVENTS        (0xE0)

// System flags type
typedef Byte D4D_SYSTEM_FLAGS;

// System flag - TimeTick
#define D4D_SYSTEM_F_TIMETICK           (0x01)

/******************************************************************************
* D4D Base macros
*
*//*! @addtogroup doxd4d_base_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that swap bytes into 16-bit variable
* @param   x - Word to Swap
* @return  swapped word
* @note    This is help macro to swap bytes in word (obsolete macro replaced by D4D_SwapWord())
*******************************************************************************/
#define D4D_SWAP_WORD(x) D4D_SwapWord(x)

/**************************************************************************/ /*!
* @brief   Macro that do logical EX-OR on the input variables
* @param   a - input A
* @param   b - input B
* @return  the result of the logical EX-OR operation
* @note    This is help macro to do logical (NOT binary) EX-OR operation.
*******************************************************************************/
#define D4D_LOG_EXOR(a,b) (!!(a) ^ !!(b))

/**************************************************************************/ /*!
* @brief   Macro used just for notify compiler that the input parameter is not used
* @param   x - unused function input parameter
* @return  none
*******************************************************************************/
#define D4D_UNUSED(x) (void)(x);

/**************************************************************************/ /*!
* @brief   Macro used to get count of chars in constant strings
* @param   txt - constant string
* @return  lenght of string
*******************************************************************************/
#define D4D_TEXT_LEN(txt) (sizeof((txt)) / sizeof(D4D_TCHAR))

/*! @} End of doxd4d_base_macro                                               */

/*********************************************************
*
* internal macros
*
*********************************************************/
#define D4D_CONST const
#define D4D_NO_CONST

#define D4D_GetKeys() 0 // Just for backward compatibility
/*********************************************************
*
* external macros
*
*********************************************************/

/******************************************************************************
* Begin of D4D_BASE public functions
*//*! @addtogroup doxd4d_base_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function gets the string width in pixels and counts with tabulator table.
* @param   text_buffer  - pointer to string structure
* @param   pTab - pointer to tabulator structure
* @return  width in pixels of text and function counts with tabulator table
* @note    Obsolete function name -keeped here just for backward compatibility.
*               Replaced by D4D_GetTextBuffWidthTab(text_buffer, pTab).
*******************************************************************************/
#define D4D_GetTextBuffWidthTabAdj(text_buffer, pTab) D4D_GetTextBuffWidthTab(text_buffer, pTab)

/**************************************************************************/ /*!
* @brief   Function gets the string width in pixels.
* @param   text_buffer  - pointer to string structure
* @return  width in pixels of text
* @note    Obsolete function name -keeped here just for backward compatibility.
*               Replaced by D4D_GetTextBuffWidthTab(text_buffer, pTab).
*******************************************************************************/
#define D4D_GetTextBuffWidth(text_buffer) D4D_GetTextBuffWidthTab(text_buffer, NULL)

/******************************************************************************
* Global call back functions
******************************************************************************/

/**************************************************************************/ /*!
* @brief   Call back function raised by any new input event (touch, mouse, keys).
* @return  none
* @note    Call back function that notify user application about any new user input event.
*******************************************************************************/
#ifdef D4D_INPUT_EVENT_CALLBACK
   extern void D4D_INPUT_EVENT_CALLBACK(void);
#endif


/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_base_func                                               */
/******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

// global shared memory used by multiple modules as temporary storage
#define D4D_SCRATCHPAD_SIZE 48
extern Byte d4d_scratchPad[D4D_SCRATCHPAD_SIZE];

// assert macro
#define D4D_ASSERT(cond) if(!(cond)) for(;;)

extern const D4D_MARGIN d4d_marginDefault;

#endif /* __D4D_BASE_H */










