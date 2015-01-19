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
* @file      d4d_list_box.h
*
* @author     Petr Gargulak
*
* @version   0.0.8.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver listBox object header file
*
*******************************************************************************/

#ifndef __D4D_LIST_BOX_H
#define __D4D_LIST_BOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_listBoxSysFunc;
void D4D_ListBoxScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);
/******************************************************************************
* D4D LIST BOX setting  constants
*
*//*! @addtogroup doxd4d_list_box_const
* @{
*******************************************************************************/

//********************* List Box flags for button specific flags *****************
/*!
  @defgroup doxd4d_list_box_const_flags D4D LIST BOX Defines masks of list box specific behaviour flags
  This section specifies masks of list box behaviour flags.
  @ingroup doxd4d_list_box_const
*/

/**
 * @addtogroup doxd4d_list_box_const_flags
 * @{
 */
#define D4D_LIST_BOX_F_SIDEBAR    (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< Force to used always side bar scroll bar
/**@}*/

/*! @brief This is list box init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_DENTED) as a default.*/
#ifndef D4D_LIST_BOX_F_DEFAULT
  #define D4D_LIST_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_DENTED)
#endif

/*! @brief This is list box embedded scroll bar init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_OBJECT_F_MOUSE_NORMAL) as a default.*/
#ifndef D4D_LIST_BOX_F_SCRLBRS_DEFAULT
  #define D4D_LIST_BOX_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_BEVEL_RAISED | D4D_OBJECT_F_MOUSE_NORMAL)
#endif

/*! @brief This is list box embedded scroll bar width
           If not defined, it sets to 20 pixels as a default.*/
#ifndef D4D_LIST_BOX_SCRLBR_WIDTH
  #define D4D_LIST_BOX_SCRLBR_WIDTH  ( 20 )
#endif

/*! @brief This is list box embedded scroll bar change step
           If not defined, it sets to 1 step as a default.*/
#ifndef D4D_LIST_BOX_SCRLBR_STEP
  #define D4D_LIST_BOX_SCRLBR_STEP  ( 1 )
#endif

/*! @brief This is list box init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_LEFT_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_LIST_BOX_ITEM_TXT_PRTY_DEFAULT
  #define D4D_LIST_BOX_ITEM_TXT_PRTY_DEFAULT  ( D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_LEFT_MASK )
#endif

/*! @brief This is list box init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_LIST_BOX_ITEM_FNT_PRTY_DEFAULT
  #define D4D_LIST_BOX_ITEM_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @} End of doxd4d_list_box_const                                             */

/******************************************************************************
* D4D LIST BOX setting  public types
*
*//*! @addtogroup doxd4d_list_box_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of list box on event callback function
* @param   pThis - pointer to list box instance that call the function
* @param   eventId - event type that caused this call back function
* @return  none
* @note    This is list box call back function used to notify user application about
*          list box events (\ref D4D_EVENT_ONCLICK, \ref D4D_EVENT_ONCHANGE, \ref D4D_EVENT_ONRELOAD).
*******************************************************************************/
typedef void (*D4D_LSTBX_ON_EVENT)(D4D_OBJECT* pThis, D4D_EVENTID eventId);

/*! @brief This is list box item index variable type.*/
typedef sByte D4D_LIST_BOX_INDEX;

/*! @brief This is list box item variable type.*/
typedef struct
{
  D4D_TCHAR* pText;     ///<    pointer to item text
  void* pUser;          ///<    pointer to void (it can  be retype to any other user data in size or less that is pointer size)
}D4D_LIST_BOX_ITEM;

/*! @} End of doxd4d_list_box_type                                            */

/******************************************************************************
* Private Types
******************************************************************************/

typedef struct
{
   D4D_LIST_BOX_INDEX ix;
   D4D_LIST_BOX_INDEX page_ix;
   const D4D_LIST_BOX_ITEM*  pItems;
} D4D_LIST_BOX_DATA;

typedef struct
{
    D4D_FONT  itemsFontId;
    D4D_LIST_BOX_INDEX posCnt; // Number of visiable position
    D4D_COOR textOff; // text offset between individual item on screen
    D4D_LIST_BOX_DATA* pData;     // ListBox volatile data struct
    D4D_LSTBX_ON_EVENT OnEvent;
} D4D_LIST_BOX;


#define D4D_LIST_BOX_CHILD_SCROLL_BAR_IX 1

/******************************************************************************
* Macros
******************************************************************************/

// getting the LIST_BOX structure from general OBJECT
#define D4D_GET_LIST_BOX(pObj) ((D4D_LIST_BOX*)((pObj)->pParam))

#define D4D_GET_LIST_BOX_SCROLL_BAR(pObj) ((D4D_OBJECT*)((pObj)->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX]))

/******************************************************************************
* D4D LIST BOX setting  public macros
*
*//*! @addtogroup doxd4d_list_box_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the List box object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of list box object
* @param   x - coordination of list box in X axis
* @param   y - coordination of list box in Y axis
* @param   cx - size of list box in X axis (width)
* @param   cy - size of list box in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw list box
* @param   itemsFontId - identification number of the used item text font
* @param   posCnt - count of visible positions
* @param   listBoxItemsOff - offset between the individual items in pixels
* @param   pUser - user data of  combo box
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_LSTBX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_LIST_BOX structure, including the object data sub structure. Is used to define all properties of list box.
*               It must be followed by \ref D4D_DECLARE_LIST_BOX_ITEM or at least \ref D4D_DECLARE_LIST_BOX_END.
*******************************************************************************/
#define _D4D_DECLARE_LIST_BOX_BEGIN(type, name, x, y, cx, cy, radius, pMargin, pParent, flags, pScheme,\
    itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
    extern type D4D_LIST_BOX_ITEM name##_items[];\
    extern type D4D_OBJECT name##_scrollBarVer;\
    static D4D_LIST_BOX_DATA name##_data = { 0, 0, (const D4D_LIST_BOX_ITEM*)&(name##_items[0])};\
    static const D4D_OBJECT * const name##_relations[] = {pParent, &name##_scrollBarVer, NULL};\
    static type D4D_LIST_BOX name##_params = \
    { \
        itemsFontId,        \
        posCnt, \
        listBoxItemsOff, \
        &(name##_data), \
        pOnEvent \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, name##_relations, pOnUsrMsg, &d4d_listBoxSysFunc, &(name##_params), flags, pUser, pScheme)\
    \
    static const D4D_OBJECT* const name##_scrollBarVerRelations[] = {&name, NULL};\
    _D4D_DECLARE_SCROLL_BAR(type, name##_scrollBarVer,  (D4D_COOR)(cx - D4D_LIST_BOX_SCRLBR_WIDTH - 2*D4D_BEVEL_WIDTH), 0, D4D_LIST_BOX_SCRLBR_WIDTH, (D4D_COOR)(cy - 2*D4D_BEVEL_WIDTH),\
     radius, NULL, name##_scrollBarVerRelations, D4D_LIST_BOX_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_ListBoxScrollBarsFeedBack, NULL) \
    \
    type D4D_LIST_BOX_ITEM name##_items[] = { \


/**************************************************************************/ /*!
* @brief   Macro that is used to add item to list box definition
* @param   pText - text of item
* @param   pUser - user data of item (*void)
* @note    This macro is used after the list box declaration to add new item
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_LIST_BOX_BEGIN(my_listBox, 10, 10, 100, 20, NULL, MY_FONT, My_ListBoxEventsCB)
*       D4D_DECLARE_LIST_BOX_ITEM(D4D_DEFSTR("My Item1"), 0)
*       D4D_DECLARE_LIST_BOX_ITEM(D4D_DEFSTR("My Item2"), 1)
*       D4D_DECLARE_LIST_BOX_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_LIST_BOX_ITEM(pText, pUser) { pText, (void*)pUser},

/**************************************************************************/ /*!
* @brief   Macro that is used to close the item list for list box definition
* @note    This macro is used after the list box items declaration to close the list of items
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_LIST_BOX_BEGIN(my_listBox, 10, 10, 100, 20, NULL, MY_FONT, My_ListBoxEventsCB)
*       D4D_DECLARE_LIST_BOX_ITEM(D4D_DEFSTR("My Item1"), 0)
*       D4D_DECLARE_LIST_BOX_ITEM(D4D_DEFSTR("My Item2"), 1)
*       D4D_DECLARE_LIST_BOX_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_LIST_BOX_END() { NULL, NULL} };

/**************************************************************************/ /*!
* @brief   Macro that create the List box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of list box object
* @param   x - coordination of list box in X axis
* @param   y - coordination of list box in Y axis
* @param   cx - size of list box in X axis (width)
* @param   cy - size of list box in Y axis (height) (with hidden list box)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pParent - pointer to parent object (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw list box
* @param   itemsFontId - identification number of the used item text font
* @param   posCnt - count of visible positions
* @param   listBoxItemsOff - offset between the individual items in pixels
* @param   pUser - user data of  combo box
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_LSTBX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_LIST_BOX structure, including the object data sub structure. Is used to define all properties of list box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LIST_BOX_BEGIN instead of this one.
*               It must be followed by \ref D4D_DECLARE_LIST_BOX_ITEM or at least \ref D4D_DECLARE_LIST_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_LIST_BOX_BEGIN(name, x, y, cx, cy, flags, pParent, pScheme, itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_LIST_BOX_BEGIN(D4D_CONST, name, x, y, cx, cy, 0, (D4D_MARGIN*)&listBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the List box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of list box object
* @param   x - coordination of list box in X axis
* @param   y - coordination of list box in Y axis
* @param   cx - size of list box in X axis (width)
* @param   cy - size of list box in Y axis (height) (with hidden list box)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   itemsFontId - identification number of the used item text font
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_LSTBX_ON_EVENT
*
* @note    This macro create complete D4D_LIST_BOX structure, including the object data sub structure. Is used to define all properties of list box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LIST_BOX_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*               It must be followed by \ref D4D_DECLARE_LIST_BOX_ITEM or at least \ref D4D_DECLARE_LIST_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_STD_LIST_BOX_BEGIN(name, x, y, cx, cy, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_LIST_BOX_BEGIN(name, x, y, cx, cy, (D4D_LIST_BOX_F_DEFAULT), pParent, NULL, itemsFontId, 0, 0, NULL, pOnEvent, NULL)

// Rounded listBox definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded List box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of list box object
* @param   x - coordination of list box in X axis
* @param   y - coordination of list box in Y axis
* @param   cx - size of list box in X axis (width)
* @param   cy - size of list box in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pParent - pointer to parent object (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw list box
* @param   itemsFontId - identification number of the used item text font
* @param   posCnt - count of visible positions
* @param   listBoxItemsOff - offset between the individual items in pixels
* @param   pUser - user data of  combo box
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_LSTBX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_LIST_BOX structure, including the object data sub structure. Is used to define all properties of list box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LIST_BOX_BEGIN instead of this one.
*               It must be followed by \ref D4D_DECLARE_LIST_BOX_ITEM or at least \ref D4D_DECLARE_LIST_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_RLIST_BOX_BEGIN(name, x, y, cx, cy, radius, flags, pParent, pScheme, itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_LIST_BOX_BEGIN(D4D_CONST, name, x, y, cx, cy, radius, (D4D_MARGIN*)&listBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded List box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of list box object
* @param   x - coordination of list box in X axis
* @param   y - coordination of list box in Y axis
* @param   cx - size of list box in X axis (width)
* @param   cy - size of list box in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   pParent - pointer to parent object (Could be NULL)
* @param   itemsFontId - identification number of the used item text font
* @param   pOnEvent - Pointer to an on-evnt user callback function \ref D4D_LSTBX_ON_EVENT
*
* @note    This macro create complete D4D_LIST_BOX structure, including the object data sub structure. Is used to define all properties of list box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LIST_BOX_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*               It must be followed by \ref D4D_DECLARE_LIST_BOX_ITEM or at least \ref D4D_DECLARE_LIST_BOX_END.
*******************************************************************************/
#define D4D_DECLARE_STD_RLIST_BOX_BEGIN(name, x, y, cx, cy, radius, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_RLIST_BOX_BEGIN(name, x, y, cx, cy, radius, (D4D_LIST_BOX_F_DEFAULT), pParent, NULL, itemsFontId, 0, 0, NULL, pOnEvent, NULL)


//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_LIST_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_LIST_BOX_BEGIN_INRAM(name, x, y, cx, cy, flags, pParent, pScheme, itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_LIST_BOX_BEGIN(D4D_NO_CONST, name, x, y, cx, cy, 0, (D4D_MARGIN*)&listBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_LIST_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_LIST_BOX_BEGIN_INRAM(name, x, y, cx, cy, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_LIST_BOX_BEGIN_INRAM(name, x, y, cx, cy, (D4D_LIST_BOX_F_DEFAULT), pParent, NULL, itemsFontId, 0, 0, NULL, pOnEvent, NULL)


// Rounded listBox definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RLIST_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RLIST_BOX_BEGIN_INRAM(name, x, y, cx, cy, radius, flags, pParent, pScheme, itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_LIST_BOX_BEGIN(D4D_NO_CONST, name, x, y, cx, cy, radius, (D4D_MARGIN*)&listBox_marginDefault, pParent, flags, pScheme,\
    itemsFontId, posCnt, listBoxItemsOff, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RLIST_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RLIST_BOX_BEGIN_INRAM(name, x, y, cx, cy, radius, pParent, itemsFontId, pOnEvent) \
    D4D_DECLARE_RLIST_BOX_BEGIN_INRAM(name, x, y, cx, cy, radius, (D4D_LIST_BOX_F_DEFAULT), pParent, NULL, itemsFontId, 0, 0, NULL, pOnEvent, NULL)

/*! @} End of doxd4d_list_box_macro                                          */

/******************************************************************************
* Global variables
******************************************************************************/
extern const D4D_MARGIN listBox_marginDefault;

/******************************************************************************
* Global functions
******************************************************************************/
D4D_LIST_BOX_INDEX D4D_ListBoxGetIndex(D4D_OBJECT* pThis);

void  D4D_ListBoxSetIndex(D4D_OBJECT* pThis, D4D_LIST_BOX_INDEX ix);
D4D_LIST_BOX_INDEX D4D_ListBoxGetItemCount(D4D_OBJECT_PTR pThis);
D4D_LIST_BOX_INDEX D4D_ListBoxFindUserDataItem(D4D_OBJECT_PTR pThis, void* pUser);
void* D4D_ListBoxGetItemUserData(D4D_OBJECT_PTR pThis);
D4D_TCHAR* D4D_ListBoxGetItemText(D4D_OBJECT_PTR pThis);
D4D_TCHAR* D4D_ListBoxGetItemTextIx(D4D_OBJECT_PTR pThis, D4D_LIST_BOX_INDEX ix);
void D4D_ListBoxEnsureVisible(D4D_OBJECT* pThis);

void D4D_ListBoxChangeItemList(D4D_OBJECT_PTR pThis, const D4D_LIST_BOX_ITEM* pItems);
D4D_LIST_BOX_ITEM* D4D_ListBoxGetItemList(D4D_OBJECT_PTR pThis);
#endif /* __D4D_LIST_BOX_H  */




