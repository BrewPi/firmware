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
* @file      d4d_combo_box.h
*
* @author     Petr Gargulak
*
* @version   0.0.5.0
*
* @date      Oct-14-2013
*
* @brief     D4D Driver comboBox object header file
*
*******************************************************************************/

#ifndef __D4D_COMBO_BOX_H
#define __D4D_COMBO_BOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_comboBoxSysFunc;
void D4D_ComboBoxListFeedBack(D4D_OBJECT* pThis, D4D_EVENTID eventId);
void D4D_ComboBoxEditFeedBack(D4D_OBJECT* pThis, D4D_EVENTID eventId);
void D4D_ComboBoxButtonFeedBack(D4D_OBJECT* pThis);
Byte D4D_ComboBoxButtonMsgFeedBack(D4D_MESSAGE* pMsg);
Byte D4D_ComboBoxListBoxMsgFeedBack(D4D_MESSAGE* pMsg);
/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* D4D COMBO BOX setting  constants
*
*//*! @addtogroup doxd4d_combo_box_const
* @{
*******************************************************************************/

/*! @brief This is combo box init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED) as a default.*/
#ifndef D4D_COMBO_BOX_F_DEFAULT
  #define D4D_COMBO_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED)
#endif

/*! @brief This is combo box init flags for embedded edit box .
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE  | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_DENTED) as a default.*/
#ifndef D4D_COMBO_BOX_F_EDITBOX_DEFAULT
  #define D4D_COMBO_BOX_F_EDITBOX_DEFAULT       (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE  | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_DENTED)
#endif

/*! @brief This is combo box init flags for embedded button.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_BTN_F_3D | \ref D4D_BTN_F_CONT_RECT) as a default.*/
#ifndef D4D_COMBO_BOX_F_BTN_DEFAULT
  #define D4D_COMBO_BOX_F_BTN_DEFAULT       (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED | D4D_BTN_F_3D | D4D_BTN_F_CONT_RECT)
#endif

/*! @brief This is combo box init flags for embedded list box .
           If not defined, it sets to (\ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED) as a default.*/
#ifndef D4D_COMBO_BOX_F_LISTBOX_DEFAULT
  #define D4D_COMBO_BOX_F_LISTBOX_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED)
#endif

/*! @brief This is combo box init items font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_COMBO_BOX_ITEM_FNT_PRTY_DEFAULT
  #define D4D_COMBO_BOX_ITEM_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is combo box maximal length of item strings used by default declaration macros.
           If not defined, it sets to 20 as a default.*/
#ifndef D4D_COMBO_BOX_STR_MAX_LEN
  #define D4D_COMBO_BOX_STR_MAX_LEN  (20)
#endif

/*! @} End of doxd4d_combo_box_const                                             */

/******************************************************************************
* D4D COMBO BOX setting  public types
*
*//*! @addtogroup doxd4d_combo_box_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of combo box on event callback function
* @param   pThis - pointer to combo box instance that call the function
* @param   eventId - event type that caused this call back function
* @return  none
* @note    This is combo box call back function used to notify user application about
*          combo box events (\ref D4D_EVENT_ONCLICK, \ref D4D_EVENT_ONCHANGE).
*******************************************************************************/
typedef void (*D4D_CMBBX_ON_EVENT)(D4D_OBJECT* pThis, D4D_EVENTID eventId);

/*! @} End of doxd4d_combo_box_type                                             */

// ComboBox configuration (goes to ROM by default)



/******************************************************************************
* Private Types
******************************************************************************/
typedef D4D_LIST_BOX_INDEX D4D_COMBO_BOX_INDEX;
typedef D4D_LIST_BOX_ITEM D4D_COMBO_BOX_ITEM;

typedef struct D4D_COMBO_BOX_DATA_S
{
  D4D_LIST_BOX_INDEX real_ix;
}D4D_COMBO_BOX_DATA;

typedef struct D4D_COMBO_BOX_S
{
  D4D_COOR              list_size_y;
  D4D_COMBO_BOX_DATA*   pData;
  D4D_CMBBX_ON_EVENT    OnEvent;
}D4D_COMBO_BOX;


#define D4D_COMBO_BOX_CHILD_EDIT_BOX_IX 1
#define D4D_COMBO_BOX_CHILD_BUTTON_IX   2
#define D4D_COMBO_BOX_CHILD_LIST_BOX_IX 3

D4D_EXTERN_BMP(bmp_arrow);

/******************************************************************************
* Macros
******************************************************************************/

// getting the COMBO_BOX structure from general OBJECT
#define D4D_GET_COMBO_BOX(pObj) ((D4D_COMBO_BOX*)((pObj)->pParam))
#define D4D_GET_COMBO_BOX_DATA(pObj) ((D4D_COMBO_BOX_DATA*)((D4D_GET_COMBO_BOX(pObj))->pData))

#define D4D_GET_COMBO_BOX_EDIT_BOX(pObj)        ((D4D_OBJECT*)((pObj)->pRelations[D4D_COMBO_BOX_CHILD_EDIT_BOX_IX]))
#define D4D_GET_COMBO_BOX_BUTTON(pObj)          ((D4D_OBJECT*)((pObj)->pRelations[D4D_COMBO_BOX_CHILD_BUTTON_IX]))
#define D4D_GET_COMBO_BOX_LIST_BOX(pObj)        ((D4D_OBJECT*)((pObj)->pRelations[D4D_COMBO_BOX_CHILD_LIST_BOX_IX]))

/******************************************************************************
* D4D COMBO BOX setting  public macros
*
*//*! @addtogroup doxd4d_combo_box_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Combo box object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of combo box object
* @param   x - coordination of combo box in X axis
* @param   y - coordination of combo box in Y axis
* @param   cx - size of combo box in X axis (width)
* @param   cy - size of combo box in Y axis (height) (with hidden list box)
* @param   cy_list - size of combo box list in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw combo box
* @param   itemsFontId - identification number of the used item text font
* @param   maxStrLen - max lenght of the item texts
* @param   posCnt - count of visible positions
* @param   comboBoxItemsOff - offset between the individual items in pixels
* @param   pUser - user data of  combo box
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_CMBBX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_COMBO_BOX structure, including the object data sub structure. Is used to define all properties of combo box.
*               In fact the combo box is created fromthe three chilren object \ref D4D_BUTTON, \ref D4D_EDIT_BOX and \ref D4D_LIST_BOX handled in one combo box.
*               It must be followed by \ref D4D_DECLARE_COMBO_BOX_ITEM or at least \ref D4D_DECLARE_COMBO_BOX_END.
*******************************************************************************/
#define _D4D_DECLARE_COMBO_BOX_BEGIN(type, name, x, y, cx, cy, cy_list, radius, pMargin, pParent, flags, pScheme,\
    itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
    extern type D4D_OBJECT name##_EditBox;\
    extern type D4D_OBJECT name##_Button;\
    extern type D4D_OBJECT name##_ListBox;\
    static D4D_COMBO_BOX_DATA name##_data = {-1};\
    static const D4D_OBJECT * type name##_relations[] = {pParent, &name##_EditBox, &name##_Button, &name##_ListBox, NULL};\
    static type D4D_COMBO_BOX name##_params = \
    { \
        cy_list,      \
        &(name##_data), \
        pOnEvent      \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, (cx), (D4D_COOR)((cy) + cy_list), radius, pMargin, name##_relations, pOnUsrMsg, &d4d_comboBoxSysFunc, &(name##_params), (flags/* & ~(D4D_OBJECT_F_BEVEL_MASK | D4D_OBJECT_F_FOCUSRECT)*/), pUser, pScheme)\
    \
    static const D4D_OBJECT* const name##_EditBoxRelations[] = {&name, NULL};\
    _D4D_DECLARE_EDIT_BOX(type, name##_EditBox, 0, 0, (D4D_COOR)((cx) - (cy)), (cy), radius, (D4D_MARGIN*)&d4d_marginDefault, name##_EditBoxRelations, D4D_COMBO_BOX_F_EDITBOX_DEFAULT, pScheme, NULL, itemsFontId, maxStrLen, (void*)&name, D4D_ComboBoxEditFeedBack, NULL) \
    static const D4D_OBJECT* const name##_ButtonsRelations[] = {&name, NULL};\
    _D4D_DECLARE_BUTTON(type, name##_Button, NULL, (D4D_COOR)((cx) - (cy)), 0, (cy), (cy), radius, NULL, name##_ButtonsRelations, D4D_COMBO_BOX_F_BTN_DEFAULT, &bmp_arrow, &bmp_arrow, pScheme, 0, (void*)&name, D4D_ComboBoxButtonFeedBack, D4D_ComboBoxButtonMsgFeedBack) \
    _D4D_DECLARE_LIST_BOX_BEGIN(type, name##_ListBox, 0, (cy), (cx), cy_list, radius, (D4D_MARGIN*)&d4d_marginDefault, &name, D4D_COMBO_BOX_F_LISTBOX_DEFAULT, pScheme, itemsFontId, posCnt, comboBoxItemsOff, (void*)&name, D4D_ComboBoxListFeedBack, D4D_ComboBoxListBoxMsgFeedBack)

/**************************************************************************/ /*!
* @brief   Macro that is used to add item to combo box definition
* @param   pText - text of item
* @param   pUser - user data of item (*void)
* @note    This macro is used after the combo box declaration to add new item
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_COMBO_BOX_BEGIN(my_comboBox, 10, 10, 100, 20, 100, NULL, MY_FONT, My_ComboBoxEventsCB)
*       D4D_DECLARE_COMBO_BOX_ITEM(D4D_DEFSTR("My Item1"), 0)
*       D4D_DECLARE_COMBO_BOX_ITEM(D4D_DEFSTR("My Item2"), 1)
*       D4D_DECLARE_COMBO_BOX_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_COMBO_BOX_ITEM      D4D_DECLARE_LIST_BOX_ITEM

/**************************************************************************/ /*!
* @brief   Macro that is used to close the item list for combo box definition
* @note    This macro is used after the combo box items declaration to close the list of items
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_COMBO_BOX_BEGIN(my_comboBox, 10, 10, 100, 20, 100, NULL, MY_FONT, My_ComboBoxEventsCB)
*       D4D_DECLARE_COMBO_BOX_ITEM(D4D_DEFSTR("My Item1"), 0)
*       D4D_DECLARE_COMBO_BOX_ITEM(D4D_DEFSTR("My Item2"), 1)
*       D4D_DECLARE_COMBO_BOX_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_COMBO_BOX_END       D4D_DECLARE_LIST_BOX_END


/**************************************************************************/ /*!
* @brief   Macro that create the Combo box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of combo box object
* @param   x - coordination of combo box in X axis
* @param   y - coordination of combo box in Y axis
* @param   cx - size of combo box in X axis (width)
* @param   cy - size of combo box in Y axis (height)
* @param   cy_list - size of combo box list in Y axis (height)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pParent - pointer to parent object (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw combo box
* @param   itemsFontId - identification number of the used item text font
* @param   maxStrLen - max lenght of the item texts
* @param   posCnt - count of visible positions
* @param   comboBoxItemsOff - offset between the individual items in pixels
* @param   pUser - user data of  combo box
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_CMBBX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_COMBO_BOX structure, including the object data sub structure. Is used to define all properties of combo box.
*               In fact the combo box is created fromthe three chilren object \ref D4D_BUTTON, \ref D4D_EDIT_BOX and \ref D4D_LIST_BOX handled in one combo box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*               It must be followed by \ref D4D_DECLARE_COMBO_BOX_ITEM or at least \ref D4D_DECLARE_COMBO_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_COMBO_BOX_BEGIN(name, x, y, cx, cy, cy_list, flags, pParent, pScheme, itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_COMBO_BOX_BEGIN(D4D_CONST, name, x, y, cx, cy, cy_list, 0, (D4D_MARGIN*)&comboBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Combo box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of combo box object
* @param   x - coordination of combo box in X axis
* @param   y - coordination of combo box in Y axis
* @param   cx - size of combo box in X axis (width)
* @param   cy - size of combo box in Y axis (height)
* @param   cy_list - size of combo box list in Y axis (height)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   itemsFontId - identification number of the used item text font
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_CMBBX_ON_EVENT

* @note    This macro create complete D4D_COMBO_BOX structure, including the object data sub structure. Is used to define all properties of combo box.
*               In fact the combo box is created fromthe three chilren object \ref D4D_BUTTON, \ref D4D_EDIT_BOX and \ref D4D_LIST_BOX handled in one combo box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*               It must be followed by \ref D4D_DECLARE_COMBO_BOX_ITEM or at least \ref D4D_DECLARE_COMBO_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_STD_COMBO_BOX_BEGIN(name, x, y, cx, cy, cy_list, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_COMBO_BOX_BEGIN(name, x, y, cx, cy, cy_list, (D4D_COMBO_BOX_F_DEFAULT), pParent, NULL, itemsFontId, D4D_COMBO_BOX_STR_MAX_LEN, 0, 0, NULL, pOnEvent, NULL)

// Rounded comboBox definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Combo box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of combo box object
* @param   x - coordination of combo box in X axis
* @param   y - coordination of combo box in Y axis
* @param   cx - size of combo box in X axis (width)
* @param   cy - size of combo box in Y axis (height)
* @param   cy_list - size of combo box list in Y axis (height)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pParent - pointer to parent object (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw combo box
* @param   itemsFontId - identification number of the used item text font
* @param   maxStrLen - max lenght of the item texts
* @param   posCnt - count of visible positions
* @param   comboBoxItemsOff - offset between the individual items in pixels
* @param   pUser - user data of  combo box
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_CMBBX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_COMBO_BOX structure, including the object data sub structure. Is used to define all properties of combo box.
*               In fact the combo box is created fromthe three chilren object \ref D4D_BUTTON, \ref D4D_EDIT_BOX and \ref D4D_LIST_BOX handled in one combo box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*               It must be followed by \ref D4D_DECLARE_COMBO_BOX_ITEM or at least \ref D4D_DECLARE_COMBO_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_RCOMBO_BOX_BEGIN(name, x, y, cx, cy, cy_list, radius, flags, pParent, pScheme, itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_COMBO_BOX_BEGIN(D4D_CONST, name, x, y, cx, cy, cy_list, radius, (D4D_MARGIN*)&comboBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Combo box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of combo box object
* @param   x - coordination of combo box in X axis
* @param   y - coordination of combo box in Y axis
* @param   cx - size of combo box in X axis (width)
* @param   cy - size of combo box in Y axis (height)
* @param   cy_list - size of combo box list in Y axis (height)
* @param   radius - radius of corners
* @param   pParent - pointer to parent object (Could be NULL)
* @param   itemsFontId - identification number of the used item text font
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_CMBBX_ON_EVENT

* @note    This macro create complete D4D_COMBO_BOX structure, including the object data sub structure. Is used to define all properties of combo box.
*               In fact the combo box is created fromthe three chilren object \ref D4D_BUTTON, \ref D4D_EDIT_BOX and \ref D4D_LIST_BOX handled in one combo box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*               It must be followed by \ref D4D_DECLARE_COMBO_BOX_ITEM or at least \ref D4D_DECLARE_COMBO_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_STD_RCOMBO_BOX_BEGIN(name, x, y, cx, cy, cy_list, radius, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_RCOMBO_BOX_BEGIN(name, x, y, cx, cy, cy_list, radius, (D4D_COMBO_BOX_F_DEFAULT), pParent, NULL, itemsFontId, D4D_COMBO_BOX_STR_MAX_LEN, 0, 0, NULL, pOnEvent, NULL)


//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_COMBO_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_COMBO_BOX_BEGIN_INRAM(name, x, y, cx, cy, cy_list, flags, pParent, pScheme, itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_COMBO_BOX_BEGIN(D4D_NO_CONST, name, x, y, cx, cy, cy_list, 0, (D4D_MARGIN*)&comboBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_COMBO_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_COMBO_BOX_BEGIN_INRAM(name, x, y, cx, cy, cy_list, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_COMBO_BOX_BEGIN_INRAM(name, x, y, cx, cy, cy_list, (D4D_COMBO_BOX_F_DEFAULT), pParent, NULL, itemsFontId, D4D_COMBO_BOX_STR_MAX_LEN, 0, 0, NULL, pOnEvent, NULL)


// Rounded comboBox definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RCOMBO_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RCOMBO_BOX_BEGIN_INRAM(name, x, y, cx, cy, cy_list, radius, flags, pParent, pScheme, itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_COMBO_BOX_BEGIN(D4D_NO_CONST, name, x, y, cx, cy, cy_list, radius, (D4D_MARGIN*)&comboBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, maxStrLen, posCnt, comboBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RCOMBO_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RCOMBO_BOX_BEGIN_INRAM(name, x, y, cx, cy, cy_list, radius, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_RCOMBO_BOX_BEGIN_INRAM(name, x, y, cx, cy, cy_list, radius, (D4D_COMBO_BOX_F_DEFAULT), pParent, NULL, itemsFontId, D4D_COMBO_BOX_STR_MAX_LEN, 0, 0, NULL, pOnEvent, NULL)

/*! @} End of doxd4d_combo_box_macro                                          */

/******************************************************************************
* Global variables
******************************************************************************/
extern const D4D_MARGIN comboBox_marginDefault;

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* Begin of D4D_COMBO_BOX public functions
*//*! @addtogroup doxd4d_combo_box_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function gets the count of combo box items
* @param   pThis - pointer to the combo box object
* @return  count of combo box items.
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxGetItemCount(pThis) D4D_ListBoxGetItemCount(D4D_GET_COMBO_BOX_LIST_BOX(pThis))

/**************************************************************************/ /*!
* @brief   Function finds the index of items with specified user data
* @param   pThis - pointer to the combo box object
* @param   pUser - user data that is used to look for the item index (must match)
* @return  index of item if it's found, if not, it returns -1.
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxFindUserDataItem(pThis, pUser) D4D_ListBoxFindUserDataItem(D4D_GET_COMBO_BOX_LIST_BOX(pThis), pUser)

/**************************************************************************/ /*!
* @brief   Function gets the selected item user data
* @param   pThis - pointer to the combo box object
* @return  user data of selected item.
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxGetItemUserData(pThis) D4D_ListBoxGetItemUserData(D4D_GET_COMBO_BOX_LIST_BOX(pThis))

/**************************************************************************/ /*!
* @brief   Function gets the selected item text
* @param   pThis - pointer to the combo box object
* @return  pointer to slected  item text
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxGetItemText(pThis) D4D_ListBoxGetItemText(D4D_GET_COMBO_BOX_LIST_BOX(pThis))

/**************************************************************************/ /*!
* @brief   Function gets the text of item choosed by item index
* @param   pThis - pointer to the combo box object
* @param   ix - index to item array
* @return  pointer to item text
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxGetItemTextIx(pThis, ix) D4D_ListBoxGetItemTextIx(D4D_GET_COMBO_BOX_LIST_BOX(pThis), ix)

/**************************************************************************/ /*!
* @brief   Function ensure that the selected item in list will be scrolled to visible area
* @param   pThis - pointer to the combo box object
* @return  noen
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxEnsureVisible(pThis) D4D_ListBoxEnsureVisible(D4D_GET_COMBO_BOX_LIST_BOX(pThis))

/**************************************************************************/ /*!
* @brief   Function change the items list using by combo box
* @param   pThis - pointer to the combo box object
* @param   pItems - pointer to the new item list
* @return  none
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxChangeItemList(pThis, pItems) D4D_ListBoxChangeItemList(D4D_GET_COMBO_BOX_LIST_BOX(pThis), pItems)

/**************************************************************************/ /*!
* @brief   Function gets the current using item list
* @param   pThis - pointer to the combo box object
* @return  pointer to current using item list
* @note    Because the combo box is based on the list box, this calls the list box API.
*******************************************************************************/
#define D4D_ComboBoxGetItemList(pThis) D4D_ListBoxGetItemList(D4D_GET_COMBO_BOX_LIST_BOX(pThis))

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_combo_box_func                                              */
/******************************************************************************/
#endif /* __D4D_COMBO_BOX_H  */




