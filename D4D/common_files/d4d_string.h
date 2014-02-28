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
* @file      d4d_string.h
*
* @author     Petr Gargulak
*
* @version   0.0.3.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver strings header file
*
*******************************************************************************/

#ifndef __D4D_STRING_H
#define __D4D_STRING_H

/******************************************************************************
* Includes
******************************************************************************/
#include "common_files/d4d_font.h"
/******************************************************************************
* D4D STRING setting  constants
*
*//*! @addtogroup doxd4d_string_const
* @{
*******************************************************************************/

/*! @brief This is maximal count of string table.
           If not defined, it sets to 4 string tables (languages) as a default.*/
#ifndef D4D_STR_TABLE_MAX_CNT
  #define D4D_STR_TABLE_MAX_CNT 4
#endif

/*! @} End of doxd4d_string_const                                             */

/******************************************************************************
* Private constants - obsolete
******************************************************************************/
// just for backward compatibility
#define D4D_TXT_PRTY_ALIGN_H_MASK                D4D_ALIGN_H_MASK
#define D4D_TXT_PRTY_ALIGN_H_LEFT_MASK           D4D_ALIGN_H_LEFT_MASK
#define D4D_TXT_PRTY_ALIGN_H_RIGHT_MASK          D4D_ALIGN_H_RIGHT_MASK
#define D4D_TXT_PRTY_ALIGN_H_CENTER_MASK         D4D_ALIGN_H_CENTER_MASK

#define D4D_TXT_PRTY_ALIGN_V_MASK                D4D_ALIGN_V_MASK
#define D4D_TXT_PRTY_ALIGN_V_TOP_MASK            D4D_ALIGN_V_TOP_MASK
#define D4D_TXT_PRTY_ALIGN_V_BOTTOM_MASK         D4D_ALIGN_V_BOTTOM_MASK
#define D4D_TXT_PRTY_ALIGN_V_CENTER_MASK         D4D_ALIGN_V_CENTER_MASK

/******************************************************************************
* D4D STRING setting  types
*
*//*! @addtogroup doxd4d_string_type
* @{
*******************************************************************************/

/*! @brief The string text properties type. The masks are described here \ref doxd4d_base_const_alignmasks.*/
typedef Byte D4D_TEXT_PROPERTIES;

/*! @brief The string properties type. This structure contains as Font as Text properties.*/
typedef struct D4D_STR_PROPERTIES_S
{
  D4D_FONT_PROPERTIES font_properties;  ///< Font properties structure
  D4D_TEXT_PROPERTIES text_properties;  ///< Text properties structure
}D4D_STR_PROPERTIES;

/*! @brief The string type. This structure contains all properties about string in eGUI.*/
typedef struct D4D_STRING_S
{
  D4D_TCHAR *pText;                     ///< pointer to text array
  D4D_INDEX buffSize;                   ///< size of text buffer array
  D4D_FONT fontId;                      ///< index of used font
  D4D_STR_PROPERTIES *str_properties;   ///< pointer to string properties
  D4D_INDEX printLen;                   ///< Length of string that should be used (printed).
  D4D_INDEX printOff;                   ///< Offset of string that should be used (printed).
}D4D_STRING;

/*! @} End of doxd4d_string_type                                             */

/******************************************************************************
* Private types
******************************************************************************/

typedef struct
{
  D4D_INDEX     stringId;
  D4D_TCHAR**   stringTable;
  D4D_INDEX     stringCount;
}D4D_STRING_TABLE;

/******************************************************************************
* D4D Object macros
*
*//*! @addtogroup doxd4d_string_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that starts of default string table declaration.
* @note    This macro is used  with \ref D4D_DECLARE_STRING & \ref D4D_DECLARE_STRING_TABLE_END to create the string (language) table in eGUI
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STRING_TABLE_BEGIN
*       D4D_DECLARE_STRING(1, myStrings1)
*       D4D_DECLARE_STRING(2, myStrings2)
*       D4D_DECLARE_STRING_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_STRING_TABLE_BEGIN  D4D_DECLARE_USR_STRING_TABLE_BEGIN(d4d_StringTable)

/**************************************************************************/ /*!
* @brief   Macro that starts of user string table declaration.
* @param   name - Name of user string table
* @note    This macro is used  with \ref D4D_DECLARE_STRING & \ref D4D_DECLARE_STRING_TABLE_END to create the string (language) table in eGUI
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_USR_STRING_TABLE_BEGIN(myUserStringTable)
*       D4D_DECLARE_STRING(1, myStrings1)
*       D4D_DECLARE_STRING(2, myStrings2)
*       D4D_DECLARE_STRING_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_USR_STRING_TABLE_BEGIN(name)  const D4D_STRING_TABLE name[] = {

/**************************************************************************/ /*!
* @brief   Macro that adds one language to strings table declaration.
* @param   stringId - unique id of string(language) table
* @param   string_table - pointer to string table
* @note    None
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_USR_STRING_TABLE_BEGIN(myUserStringTable)
*       D4D_DECLARE_STRING(1, myStrings1)
*       D4D_DECLARE_STRING(2, myStrings2)
*       D4D_DECLARE_STRING_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_STRING(stringId, string_table) \
  {stringId, (D4D_TCHAR**)string_table, (sizeof(string_table) / sizeof(D4D_TCHAR*))}, // string info will be replaced by string descriptor

/**************************************************************************/ /*!
* @brief   Macro that ends the strings table declaration.
* @note    None
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_USR_STRING_TABLE_BEGIN(myUserStringTable)
*       D4D_DECLARE_STRING(1, myStrings1)
*       D4D_DECLARE_STRING(2, myStrings2)
*       D4D_DECLARE_STRING_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_STRING_TABLE_END {0, NULL} };

/**************************************************************************/ /*!
* @brief   Macro that starts of tabulator table declaration.
* @param   name - Name of tabulator table
* @note    This macro is used  with \ref D4D_DECLARE_TAB & \ref D4D_DECLARE_TAB_TABLE_END to create the tabulator table in eGUI
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_TAB_TABLE_BEGIN(myTabTable)
*       D4D_DECLARE_TAB(20)     // 20 Pixels
*       D4D_DECLARE_TAB(50)     // 50 Pixels
*       D4D_DECLARE_TAB_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_TAB_TABLE_BEGIN(name)  const D4D_COOR name[] = {

/**************************************************************************/ /*!
* @brief   Macro that adds one tabulator to tabulator table declaration.
* @param   tab - tabulator in pixels
* @note    This macro is used  with \ref D4D_DECLARE_TAB_TABLE_BEGIN & \ref D4D_DECLARE_TAB_TABLE_END to create the tabulator table in eGUI
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_TAB_TABLE_BEGIN(myTabTable)
*       D4D_DECLARE_TAB(20)     // 20 Pixels
*       D4D_DECLARE_TAB(50)     // 50 Pixels
*       D4D_DECLARE_TAB_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_TAB(tab) tab, // tab space in pixels

/**************************************************************************/ /*!
* @brief   Macro that close table declaration.
* @note    This macro is used  with \ref D4D_DECLARE_TAB_TABLE_BEGIN & \ref D4D_DECLARE_TAB to create the tabulator table in eGUI
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_TAB_TABLE_BEGIN(myTabTable)
*       D4D_DECLARE_TAB(20)     // 20 Pixels
*       D4D_DECLARE_TAB(50)     // 50 Pixels
*       D4D_DECLARE_TAB_TABLE_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_TAB_TABLE_END 0 };

#ifdef D4D_UNICODE
  /**************************************************************************/ /*!
  * @brief   Macro that helps declare the strings in eGUI
  * @note    It handles UNICODE wide strings declaration if UNICODE is enabled.
  * Here is an example demonstrating how to used it.
  * @code
  *       D4D_DEFSTR("My test string.")
  * @endcode
  *******************************************************************************/
  #define D4D_DEFSTR(str) L##str
#else
  /**************************************************************************/ /*!
  * @brief   Macro that helps declare the strings in eGUI
  * @note    It handles UNICODE wide strings declaration if UNICODE is enabled.
  * Here is an example demonstrating how to used it.
  * @code
  *       D4D_DEFSTR("My test string.")
  * @endcode
  *******************************************************************************/
  #define D4D_DEFSTR(str) str
#endif


/**************************************************************************/ /*!
* @brief   Macro that returns if the input cahr is digit or not.
* Here is an example demonstrating how to used it.
* @code
*       D4D_CHAR my_char;
*       my_char = '6'; // Just for example fill up the variable
*       if(D4D_IsDigit(my_char))
*       {
*        // do code for digit char
*       }else
*       {
*        // do code for non digit char
*       {
* @endcode
*******************************************************************************/
#define D4D_IsDigit(x) (((x)>= '0') && ((x)<= '9'))

/*! @} End of doxd4d_string_macro                                             */
/******************************************************************************
* Private macros
******************************************************************************/
#define D4D_StringTable_GetCount(x) (sizeof(x) / sizeof(D4D_TCHAR*))

/******************************************************************************
* Global variables
******************************************************************************/
extern const D4D_STRING_TABLE d4d_StringTable[];
/******************************************************************************
* Global functions
******************************************************************************/
D4D_TCHAR* D4D_GetInternalStringPointer(const D4D_TCHAR* originTxt);
void D4D_ChangeStringTable(LWord stringId);
LWord D4D_GetCurrentStringId(void);
void D4D_LCD_PrintStr(D4D_PRINT_DESC* p_StrDes);
#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE

#endif

#endif  /* __D4D_STRING_H */


