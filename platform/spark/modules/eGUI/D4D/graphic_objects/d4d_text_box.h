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
* @file      d4d_text_box.h
*
* @author     Petr Gargulak
*
* @version   0.0.15.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_TEXTBOX_H
#define __D4D_TEXTBOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_textboxSysFunc;
void D4D_TextBoxScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);

/******************************************************************************
* D4D TEXT BOX setting  constants
*
*//*! @addtogroup doxd4d_text_box_const
* @{
*******************************************************************************/

/*! @brief This is text box init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_MOUSE_NORMAL) as a default.*/
#ifndef D4D_TXTBX_F_DEFAULT
  #define D4D_TXTBX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_MOUSE_NORMAL)
#endif

/*! @brief This is text box embedded scroll bar init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_MOUSE_NORMAL) as a default.*/
#ifndef D4D_TXTBX_F_SCRLBRS_DEFAULT
  #define D4D_TXTBX_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL)
#endif

/*! @brief This is text box init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_LEFT_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_TXTBX_TXT_PRTY_DEFAULT
  #define D4D_TXTBX_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_LEFT_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is text box init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_TXTBX_FNT_PRTY_DEFAULT
  #define D4D_TXTBX_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is text box embedded scroll bar width
           If not defined, it sets to 20 pixels as a default.*/
#ifndef D4D_TXTBX_SCRLBR_WIDTH
  #define D4D_TXTBX_SCRLBR_WIDTH  ( 20 )
#endif

/*! @brief This is text box embedded scroll bar change step
           If not defined, it sets to 2 step as a default.*/
#ifndef D4D_TXTBX_SCRLBR_STEP
  #define D4D_TXTBX_SCRLBR_STEP  ( 2 )
#endif

/*! @} End of doxd4d_text_box_const                                           */

/******************************************************************************
* Types
******************************************************************************/







typedef struct D4D_TXTBX_DATA_S
{
	D4D_INDEX firstShowedCharIx;
	D4D_TCHAR* pTxtArr;			 // Pointer on text array that will use TextBox to store all lines
	D4D_BOOL redrawText;
}D4D_TXTBX_DATA;

// label configuration (goes to ROM by default)

typedef struct
{
    D4D_COOR* pTabTable;
    D4D_FONT textFontId; 		 // Used font for whole text
    D4D_TXTBX_DATA* pData;		 // Run time data needed for TextBox
} D4D_TEXTBOX;


/******************************************************************************
* Macros
******************************************************************************/
#define D4D_TEXTBOX_CHILD_SCROLL_BAR_VER_IX 1


// getting the TEXTBOX structure from general OBJECT
#define D4D_GET_TEXTBOX(pObj) ((D4D_TEXTBOX*)((pObj)->pParam))
#define D4D_GET_TEXTBOX_DATA(pObj) (D4D_GET_TEXTBOX(pObj)->pData)

#define D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pObj) ((D4D_OBJECT*)((pObj)->pRelations[D4D_TEXTBOX_CHILD_SCROLL_BAR_VER_IX]))

/******************************************************************************
* D4D TEXT BOX setting  public macros
*
*//*! @addtogroup doxd4d_text_box_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Text box object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of text box object
* @param   x - coordination of text box in X axis
* @param   y - coordination of text box in Y axis
* @param   cx - size of text box in X axis (width)
* @param   cy - size of text box in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   pTextArray - pointer to text array that will be showed in text box
* @param   pTabTable - pointer to tabulator table
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw text box
* @param   fontId - identification number of the used text font
* @param   pUser - user data of  text box
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_TEXT_BOX structure, including the object data sub structure. Is used to define all properties of text box.
*******************************************************************************/
#define _D4D_DECLARE_TEXTBOX(type, name, x, y, cx, cy, radius, pMargin, pParent, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg) \
    static D4D_TXTBX_DATA name##_data = { 0, (D4D_TCHAR*)pTextArray, D4D_TRUE};\
    extern type D4D_OBJECT name##_scrollBarVer;\
    static const D4D_OBJECT * const name##_relations[] = {pParent, &name##_scrollBarVer, NULL};\
    static type D4D_TEXTBOX name##_params = \
    { \
        (D4D_COOR*)pTabTable, /* pTabTable */ \
        fontId,			/* textFontId */ \
        &name##_data, /* pData */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, name##_relations, pOnUsrMsg, &d4d_textboxSysFunc, &(name##_params), flags, pUser, pScheme)\
    \
    static const D4D_OBJECT* const name##_scrollBarRelations[] = {&name, NULL};\
    \
    _D4D_DECLARE_SCROLL_BAR(type, name##_scrollBarVer, (D4D_COOR)(cx - D4D_BORDER_WIDTH(flags) - D4D_TXTBX_SCRLBR_WIDTH), D4D_BORDER_WIDTH(flags), D4D_TXTBX_SCRLBR_WIDTH, (D4D_COOR)(cy - 2*D4D_BORDER_WIDTH(flags)),\
      ((radius > D4D_TXTBX_SCRLBR_WIDTH / 2)? (D4D_COOR)(D4D_TXTBX_SCRLBR_WIDTH / 2) : radius), NULL, name##_scrollBarRelations, D4D_TXTBX_F_SCRLBRS_DEFAULT, pScheme, NULL, D4D_TextBoxScrollBarsFeedBack, NULL)


/**************************************************************************/ /*!
* @brief   Macro that create the Text box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of text box object
* @param   x - coordination of text box in X axis
* @param   y - coordination of text box in Y axis
* @param   cx - size of text box in X axis (width)
* @param   cy - size of text box in Y axis (height)
* @param   pTextArray - pointer to text array that will be showed in text box
* @param   pTabTable - pointer to tabulator table
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw text box
* @param   fontId - identification number of the used text font
* @param   pUser - user data of  text box
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_TEXT_BOX structure, including the object data sub structure. Is used to define all properties of text box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_TEXTBOX instead of this one.
*******************************************************************************/
#define D4D_DECLARE_TEXTBOX(name, x, y, cx, cy, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_CONST, name, x, y, cx, cy, 0, NULL, NULL, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Text box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of text box object
* @param   x - coordination of text box in X axis
* @param   y - coordination of text box in Y axis
* @param   cx - size of text box in X axis (width)
* @param   cy - size of text box in Y axis (height)
* @param   pTextArray - pointer to text array that will be showed in text box
* @param   pTabTable - pointer to tabulator table
* @param   fontId - identification number of the used text font

* @note    This macro create complete D4D_TEXT_BOX structure, including the object data sub structure. Is used to define all properties of text box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_TEXTBOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_TEXTBOX(name, x, y, cx, cy, pTextArray, pTabTable, fontId) \
  D4D_DECLARE_TEXTBOX(name, x, y, cx, cy, pTextArray, pTabTable, D4D_TXTBX_F_DEFAULT, NULL, fontId, NULL, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Text box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of text box object
* @param   x - coordination of text box in X axis
* @param   y - coordination of text box in Y axis
* @param   cx - size of text box in X axis (width)
* @param   cy - size of text box in Y axis (height)
* @param   radius - radius of corners
* @param   pTextArray - pointer to text array that will be showed in text box
* @param   pTabTable - pointer to tabulator table
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw text box
* @param   fontId - identification number of the used text font
* @param   pUser - user data of  text box
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_TEXT_BOX structure, including the object data sub structure. Is used to define all properties of text box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_TEXTBOX instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RTEXTBOX(name, x, y, cx, cy, radius, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_CONST, name, x, y, cx, cy, radius, NULL, NULL, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Text box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of text box object
* @param   x - coordination of text box in X axis
* @param   y - coordination of text box in Y axis
* @param   cx - size of text box in X axis (width)
* @param   cy - size of text box in Y axis (height)
* @param   radius - radius of corners
* @param   pTextArray - pointer to text array that will be showed in text box
* @param   pTabTable - pointer to tabulator table
* @param   fontId - identification number of the used text font

* @note    This macro create complete D4D_TEXT_BOX structure, including the object data sub structure. Is used to define all properties of text box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_TEXTBOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RTEXTBOX(name, x, y, cx, cy, radius, pTextArray, pTabTable,  fontId) \
  D4D_DECLARE_RTEXTBOX(name, x, y, cx, cy, radius, pTextArray, pTabTable, D4D_TXTBX_F_DEFAULT, NULL, fontId, NULL, NULL)

//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_TEXTBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_TEXTBOX_INRAM(name, x, y, cx, cy, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_NO_CONST, name, x, y, cx, cy, 0, NULL, NULL, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_TEXTBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_TEXTBOX_INRAM(name, x, y, cx, cy, pTextArray, pTabTable,  fontId) \
  D4D_DECLARE_TEXTBOX_INRAM(name, x, y, cx, cy, pTextArray, pTabTable, D4D_TXTBX_F_DEFAULT, NULL, fontId, NULL, NULL)

// Rounded text box definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RTEXTBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RTEXTBOX_INRAM(name, x, y, cx, cy, radius, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_NO_CONST, name, x, y, cx, cy, radius, NULL, NULL, pTextArray, pTabTable, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RTEXTBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RTEXTBOX_INRAM(name, x, y, cx, cy, radius, pTextArray, pTabTable,  fontId) \
  D4D_DECLARE_RTEXTBOX_INRAM(name, x, y, cx, cy, radius, pTextArray, pTabTable, D4D_TXTBX_F_DEFAULT, NULL, fontId, NULL, NULL)

/*! @} End of doxd4d_text_box_macro                                          */

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/
void D4D_TextBoxRefreshAll(D4D_OBJECT* pObject);
void D4D_TextBoxChangeText(D4D_OBJECT* pObject, D4D_TCHAR* pText);

#endif /* __D4D_TEXTBOX_H */
