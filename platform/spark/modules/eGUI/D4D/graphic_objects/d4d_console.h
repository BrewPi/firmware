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
* @file      d4d_console.h
*
* @author     Petr Gargulak
*
* @version   0.0.20.0
*
* @date      Oct-14-2013
*
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_CONSOLE_H
#define __D4D_CONSOLE_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_consoleSysFunc;
void D4D_CnslScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);


/******************************************************************************
* D4D CONSOLE setting  constants
*
*//*! @addtogroup doxd4d_console_const
* @{
*******************************************************************************/

/*! @brief This is console init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_DENTED) as a default.*/
#ifndef D4D_CNSL_F_DEFAULT
  #define D4D_CNSL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_DENTED)
#endif

/*! @brief This is console init flags for embedded scroll bars.
           If not defined, it sets to (\ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED) as a default.*/
#ifndef D4D_CNSL_F_SCRLBRS_DEFAULT
  #define D4D_CNSL_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED)
#endif

/*! @brief This is console init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_LEFT_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_CNSL_TXT_PRTY_DEFAULT
  #define D4D_CNSL_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_LEFT_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is console init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_CNSL_FNT_PRTY_DEFAULT
  #define D4D_CNSL_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is definition of console cursor height .
           If not defined, it sets to ( 1 ) as a default.*/
#ifndef D4D_CNSL_CURSOR_HEIGHT
  #define D4D_CNSL_CURSOR_HEIGHT  ( 1 )
#endif

/*! @brief This is definition of console tab size in pixels.
           If not defined, it sets to ( 8 ) as a default.*/
#ifndef D4D_CNSL_STD_TAB_SIZE
  #define D4D_CNSL_STD_TAB_SIZE  ( 8 )
#endif

/*! @brief This is definition of console scroll bars height .
           If not defined, it sets to ( 20 ) as a default.*/
#ifndef D4D_CNSL_SCRLBR_WIDTH
  #define D4D_CNSL_SCRLBR_WIDTH  ( 20 )
#endif

/*! @brief This is definition of console vertical scroll bar step (in lines on text).
           If not defined, it sets to ( 2 ) as a default.*/
#ifndef D4D_CNSL_SCRLBR_STEP_V
  #define D4D_CNSL_SCRLBR_STEP_V  ( 2 )
#endif

/*! @brief This is definition of console horizontal scroll bar step (in characters).
           If not defined, it sets to ( 8 ) as a default.*/
#ifndef D4D_CNSL_SCRLBR_STEP_H
  #define D4D_CNSL_SCRLBR_STEP_H  ( 8 )
#endif

/*! @brief This is definition of console cursor blink time (in ticks of eGUI time ticks - \ref D4D_TimeTickPut).
           If not defined, it sets to ( 1 ) as a default.*/
#ifndef D4D_CNSL_CURSOR_BLINK_TICK_COUNTER
  #define D4D_CNSL_CURSOR_BLINK_TICK_COUNTER  ( 10 )
#endif

/*! @} End of doxd4d_console_const                                             */

typedef Byte D4D_CNSL_FLAGS;

#define D4D_CNSL_FLAGS_CHECKLINE      (0x01)
#define D4D_CNSL_FLAGS_REDRAWALL      (0x02)
#define D4D_CNSL_FLAGS_REDRAWLINE     (0x04)
#define D4D_CNSL_FLAGS_REDRAWCURSOR   (0x10)
#define D4D_CNSL_FLAGS_CURSORSTATE    (0x20)

typedef struct D4D_CNSL_DATA_S
{
	D4D_COOR lastLineIx;								// index of last line
	D4D_POINT cursorPos;
	D4D_CNSL_FLAGS flags;
	D4D_INDEX tickCounter;
	D4D_FONT_PROPERTIES fnt_properties;	// propertis of used font for object strings
}D4D_CNSL_DATA;

// label configuration (goes to ROM by default)

typedef struct
{
    D4D_TCHAR* pTxtArr;			 // Pointer on text array that will use Console to store all lines
    D4D_SIZE txtArrSize;     // Size of two dimensional array for text
    D4D_FONT textFontId; 		 // Used font for whole text
    D4D_INDEX tabSize;       // TAb Size of console object
    D4D_CNSL_DATA* pData;		 // Run time data needed for Console
} D4D_CONSOLE;


#define D4D_LIST_BOX_CHILD_SCROLL_BAR_HOR_IX 1
#define D4D_LIST_BOX_CHILD_SCROLL_BAR_VER_IX 2

/******************************************************************************
* Macros
******************************************************************************/

// getting the CONSOLE structure from general OBJECT
#define D4D_GET_CONSOLE(pObj) ((D4D_CONSOLE*)((pObj)->pParam))
#define D4D_GET_CONSOLE_DATA(pObj) (D4D_GET_CONSOLE(pObj)->pData)
#define D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pObj) ((D4D_OBJECT*)((pObj)->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_HOR_IX]))
#define D4D_GET_LIST_BOX_SCROLL_BAR_VER(pObj) ((D4D_OBJECT*)((pObj)->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_VER_IX]))

/******************************************************************************
* D4D CONSOLE setting  public macros
*
*//*! @addtogroup doxd4d_console_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Console object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of console object
* @param   x - coordination of console in X axis
* @param   y - coordination of console in Y axis
* @param   cx - size of console in X axis (width)
* @param   cy - size of console in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   line_cnt - count of lines to allocate memory array
* @param   char_cnt - count of characters per line to allocate memory array
* @param   tabSize - size of tabulator in characteres (to meet standard behaviour of console)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw console
* @param   fontId - Identification number of the used text font
* @param   pUser - user data of  console
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_CONSOLE structure, including the object data sub structure. Is used to define all properties of console.
*******************************************************************************/
#define _D4D_DECLARE_CONSOLE(type, name, x, y, cx, cy, radius, pMargin, pParent, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg) \
    static D4D_CNSL_DATA name##_data = { 0, {0, 0}, 0, 0, 0};\
    static D4D_TCHAR name##_txtArray[line_cnt][char_cnt + 1];\
    extern type D4D_OBJECT name##_scrollBarHor;\
    extern type D4D_OBJECT name##_scrollBarVer;\
    static const D4D_OBJECT * const name##_relations[] = {pParent, &name##_scrollBarHor, &name##_scrollBarVer, NULL};\
    static type D4D_CONSOLE name##_params = \
    { \
        (D4D_TCHAR*)&name##_txtArray[0][0],	/* pTextArr */ \
        { char_cnt, line_cnt }, /* txtArrSize */ \
        fontId,			/* textFontId */ \
        tabSize,    /* tabSize */ \
        &name##_data, /* pData */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, name##_relations, pOnUsrMsg, &d4d_consoleSysFunc, &(name##_params), ((flags) | D4D_OBJECT_F_TIMETICK), pUser, pScheme)\
    \
    static const D4D_OBJECT* const name##_scrollBarsRelations[] = {&name, NULL};\
    \
    _D4D_DECLARE_SCROLL_BAR(type, name##_scrollBarHor,  D4D_BORDER_WIDTH(flags), (D4D_COOR)(cy - 2*D4D_BORDER_WIDTH(flags) - D4D_CNSL_SCRLBR_WIDTH + 1), (D4D_COOR)(cx - D4D_CNSL_SCRLBR_WIDTH - 2 * D4D_BORDER_WIDTH(flags)), D4D_CNSL_SCRLBR_WIDTH, \
     radius, NULL, name##_scrollBarsRelations, D4D_CNSL_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_CnslScrollBarsFeedBack, NULL) \
    \
    _D4D_DECLARE_SCROLL_BAR(type, name##_scrollBarVer,  (D4D_COOR)(cx - D4D_CNSL_SCRLBR_WIDTH - 2 * D4D_BORDER_WIDTH(flags) + 1), D4D_BORDER_WIDTH(flags), D4D_LIST_BOX_SCRLBR_WIDTH, (D4D_COOR)(cy - D4D_LIST_BOX_SCRLBR_WIDTH - 2*D4D_BEVEL_WIDTH),\
     radius, NULL, name##_scrollBarsRelations, D4D_CNSL_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_CnslScrollBarsFeedBack, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the Console object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of console object
* @param   x - coordination of console in X axis
* @param   y - coordination of console in Y axis
* @param   cx - size of console in X axis (width)
* @param   cy - size of console in Y axis (height)
* @param   line_cnt - count of lines to allocate memory array
* @param   char_cnt - count of characters per line to allocate memory array
* @param   tabSize - size of tabulator in characteres (to meet standard behaviour of console)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw console
* @param   fontId - Identification number of the used text font
* @param   pUser - user data of  console
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_CONSOLE structure, including the object data sub structure. Is used to define all properties of console. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CONSOLE instead of this one.
*******************************************************************************/
#define D4D_DECLARE_CONSOLE(name, x, y, cx, cy, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_CONST, name, x, y, cx, cy, 0, (D4D_MARGIN*)&d4d_marginDefault, NULL, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Console object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of console object
* @param   x - coordination of console in X axis
* @param   y - coordination of console in Y axis
* @param   cx - size of console in X axis (width)
* @param   cy - size of console in Y axis (height)
* @param   line_cnt - count of lines to allocate memory array
* @param   char_cnt - count of characters per line to allocate memory array
* @param   fontId - Identification number of the used text font

* @note    This macro create complete D4D_CONSOLE structure, including the object data sub structure. Is used to define all properties of console. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CONSOLE instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_CONSOLE(name, x, y, cx, cy, line_cnt, char_cnt, fontId) \
  D4D_DECLARE_CONSOLE(name, x, y, cx, cy, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, D4D_CNSL_F_DEFAULT, NULL, fontId, NULL, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Console object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of console object
* @param   x - coordination of console in X axis
* @param   y - coordination of console in Y axis
* @param   cx - size of console in X axis (width)
* @param   cy - size of console in Y axis (height)
* @param   radius - radius of corners
* @param   line_cnt - count of lines to allocate memory array
* @param   char_cnt - count of characters per line to allocate memory array
* @param   tabSize - size of tabulator in characteres (to meet standard behaviour of console)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw console
* @param   fontId - Identification number of the used text font
* @param   pUser - user data of  console
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_CONSOLE structure, including the object data sub structure. Is used to define all properties of console. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CONSOLE instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RCONSOLE(name, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_CONST, name, x, y, cx, cy, radius, (D4D_MARGIN*)&d4d_marginDefault, NULL, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Console object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of console object
* @param   x - coordination of console in X axis
* @param   y - coordination of console in Y axis
* @param   cx - size of console in X axis (width)
* @param   cy - size of console in Y axis (height)
* @param   radius - radius of corners
* @param   line_cnt - count of lines to allocate memory array
* @param   char_cnt - count of characters per line to allocate memory array
* @param   fontId - Identification number of the used text font

* @note    This macro create complete D4D_CONSOLE structure, including the object data sub structure. Is used to define all properties of console. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CONSOLE instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RCONSOLE(name, x, y, cx, cy, radius, line_cnt, char_cnt, fontId) \
  D4D_DECLARE_RCONSOLE(name, x, y, cx, cy, radius, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, D4D_CNSL_F_DEFAULT, NULL, fontId, NULL, NULL)


// IN RAM instantions macro
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_CONSOLE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_CONSOLE_INRAM(name, x, y, cx, cy, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_NO_CONST, name, x, y, cx, cy, 0, (D4D_MARGIN*)&d4d_marginDefault, NULL, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_CONSOLE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_CONSOLE_INRAM(name, x, y, cx, cy, line_cnt, char_cnt, fontId) \
  D4D_DECLARE_CONSOLE_INRAM(name, x, y, cx, cy, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, D4D_CNSL_F_DEFAULT, NULL, fontId, NULL, NULL)

// Rounded console definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RCONSOLE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RCONSOLE_INRAM(name, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_NO_CONST, name, x, y, cx, cy, radius, (D4D_MARGIN*)&d4d_marginDefault, NULL, line_cnt, char_cnt, tabSize, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RCONSOLE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RCONSOLE_INRAM(name, x, y, cx, cy, radius, line_cnt, char_cnt, fontId) \
  D4D_DECLARE_RCONSOLE_INRAM(name, x, y, cx, cy, radius, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, D4D_CNSL_F_DEFAULT, NULL, fontId, NULL, NULL)

/*! @} End of doxd4d_console_macro                                             */
/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

D4D_BOOL D4D_CnslPutChar(D4D_OBJECT_PTR pObj, D4D_TCHAR ch);
D4D_BOOL D4D_CnslPutString(D4D_OBJECT_PTR pObj, D4D_TCHAR* pText);
void D4D_CnslClearLine(D4D_OBJECT_PTR pObj, D4D_INDEX line);
void D4D_CnslGoToXY(D4D_OBJECT_PTR pObj, D4D_POINT newPosition);
void D4D_CnslEnsureVisible(D4D_OBJECT_PTR pObj);
void D4D_CnslClearAll(D4D_OBJECT_PTR pObj);
D4D_POINT D4D_CnslGetCursor(D4D_OBJECT_PTR pObj);
void D4D_CnslSetScrollBarPosition(D4D_OBJECT_PTR pObj, D4D_INDEX hor, D4D_INDEX ver);
D4D_POINT D4D_CnslGetScrollBarPosition(D4D_OBJECT_PTR pObj);
/*********************************************************
*
* Object API
*
*********************************************************/


#endif /* __D4D_CONSOLE_H */
