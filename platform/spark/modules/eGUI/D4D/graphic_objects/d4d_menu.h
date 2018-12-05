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
* @file      d4d_menu.h
*
* @author     Petr Gargulak
*
* @version   0.0.47.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver menu object header file
*
*******************************************************************************/

#ifndef __D4D_MENU_H
#define __D4D_MENU_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_menuSysFunc;
void D4D_MenuScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);
/******************************************************************************
* D4D MENU setting  constants
*
*//*! @addtogroup doxd4d_menu_const
* @{
*******************************************************************************/

//********************* Menu flags for button specific flags *****************
/*!
  @defgroup doxd4d_menu_const_flags D4D MENU Defines masks of menu specific behaviour flags
  This section specifies masks of menu behaviour flags.
  @ingroup doxd4d_menu_const
*/

/**
 * @addtogroup doxd4d_menu_const_flags
 * @{
 */
  #define D4D_MENU_F_INDEX    (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Enable show of index of items in header
  #define D4D_MENU_F_SIDEBAR  (0x02 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Forca always show of side bar scroll bar
/**@}*/

/*! @brief This is menu init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_MENU_F_INDEX | \ref D4D_MENU_F_SIDEBAR) as a default.*/
#ifndef D4D_MENU_F_DEFAULT
  #define D4D_MENU_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED | D4D_MENU_F_INDEX | D4D_MENU_F_SIDEBAR)
#endif

/*! @brief This is menu embedded scroll bar init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FASTTOUCH | \ref D4D_OBJECT_F_MOUSE_NORMAL) as a default.*/
#ifndef D4D_MENU_F_SCRLBRS_DEFAULT
  #define D4D_MENU_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_MOUSE_NORMAL)
#endif

/*! @brief This is menu embedded scroll bar width
           If not defined, it sets to 20 pixels as a default.*/
#ifndef D4D_MENU_SCRLBR_WIDTH
  #define D4D_MENU_SCRLBR_WIDTH  ( 20 )
#endif

/*! @brief This is menu embedded scroll bar change step
           If not defined, it sets to 1 step as a default.*/
#ifndef D4D_MENU_SCRLBR_STEP
  #define D4D_MENU_SCRLBR_STEP  ( 1 )
#endif

/*! @brief This is menu init title text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_MENU_TXT_PRTY_DEFAULT
  #define D4D_MENU_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is menu init title font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_MENU_FNT_PRTY_DEFAULT
  #define D4D_MENU_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is menu init index font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_MENU_IX_FNT_PRTY_DEFAULT
  #define D4D_MENU_IX_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is menu init index text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_MENU_IX_TXT_PRTY_DEFAULT
  #define D4D_MENU_IX_TXT_PRTY_DEFAULT  ( D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK )
#endif

/*! @brief This is menu init items font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_MENU_ITEM_FNT_PRTY_DEFAULT
  #define D4D_MENU_ITEM_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is menu init items text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_LEFT_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_MENU_ITEM_TXT_PRTY_DEFAULT
  #define D4D_MENU_ITEM_TXT_PRTY_DEFAULT  ( D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_LEFT_MASK )
#endif

/*! @} End of doxd4d_menu_const                                             */

/******************************************************************************
* D4D MENU setting  public types
*
*//*! @addtogroup doxd4d_menu_type
* @{
*******************************************************************************/

/*! @brief This is menu item index variable type.*/
typedef sByte D4D_MENU_INDEX;

/**************************************************************************/ /*!
* @brief   Type definition of menu on click callback function
* @param   pThis - pointer to menu instance that call the function
* @param   ix - index of item that has been clicked
* @return  none
* @note    This is menu call back function used to notify user application about on click event on handled index of item.
*******************************************************************************/
typedef void (*D4D_MENU_ON_CLICK)(D4D_OBJECT* pThis, D4D_MENU_INDEX ix);


/*! @} End of doxd4d_list_box_type                                            */

/******************************************************************************
* Private Types
******************************************************************************/

// Menu configuration (goes to ROM by default)
typedef struct
{
  D4D_STRING text;
  const D4D_BMP*  pIcon;    // Item Icon
  void* pUser;
}D4D_MENU_ITEM;

typedef struct
{
   D4D_MENU_INDEX ix;
   D4D_MENU_INDEX page_ix;
} D4D_MENU_DATA;

typedef struct
{
    D4D_STRING title_text;    // menu title text
    D4D_FONT  indexFontId;
    D4D_FONT  itemsFontId;
    D4D_MENU_INDEX posCnt; // Number of visiable position
    D4D_COOR textOff; // text offset between individual item on screen
    const D4D_BMP*  pIcon;    // Title Icon
    D4D_MENU_DATA* pData;     // Menu volatile data struct
    const D4D_MENU_ITEM*  pItems;
    D4D_MENU_ON_CLICK OnClicked;
} D4D_MENU;


/******************************************************************************
* Macros
******************************************************************************/
#define D4D_MENU_CHILD_SCROLL_BAR_VER_IX 1

// getting the MENU structure from general OBJECT
#define D4D_GET_MENU(pObj) ((D4D_MENU*)((pObj)->pParam))

#define D4D_GET_MENU_SCROLL_BAR_HOR(pObj) ((D4D_OBJECT*)((pObj)->pRelations[D4D_MENU_CHILD_SCROLL_BAR_VER_IX]))

/******************************************************************************
* D4D MENU setting  public macros
*
*//*! @addtogroup doxd4d_menu_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Menu object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of menu object
* @param   title_text - title text
* @param   title_font - title text font id
* @param   x - coordination of menu in X axis
* @param   y - coordination of menu in Y axis
* @param   cx - size of menu in X axis (width)
* @param   cy - size of menu in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pParent - pointer to parent object (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_menu_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw menu
* @param   indexFontId - identification number of the used index text font
* @param   itemsFontId - identification number of the used item text font
* @param   posCnt - count of visible positions
* @param   menuItemsOff - offset between the individual items in pixels
* @param   pIcon - pointer to \ref D4D_BMP structure to title
* @param   pUser - user data of  menu
* @param   pOnClick - Pointer to an on-click user callback function \ref D4D_MENU_ON_CLICK
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_MENU structure, including the object data sub structure. Is used to define all properties of menu.
*               It must be followed by \ref D4D_DECLARE_MENU_ITEM or \ref D4D_DECLARE_MENU_ITEM_FULL or at least \ref D4D_DECLARE_MENU_END.
*******************************************************************************/
#define _D4D_DECLARE_MENU_BEGIN(type, name, title_text, title_font, x, y, cx, cy, radius, pMargin, pParent, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
    extern type D4D_MENU_ITEM name##_items[];\
    extern D4D_OBJECT name##_scrollBarVer;\
    static const D4D_OBJECT * name##_relations[] = {pParent, &name##_scrollBarVer, NULL};\
    static D4D_MENU_DATA name##_data = { 0, 0};\
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_MENU_FNT_PRTY_DEFAULT , D4D_MENU_TXT_PRTY_DEFAULT }; \
    static type D4D_MENU name##_params = \
    { \
        { title_text, D4D_TEXT_LEN(title_text), title_font, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(title_text), 0},  \
        indexFontId,        \
		    itemsFontId,        \
		    posCnt, \
		    menuItemsOff, \
		    pIcon, \
		    &(name##_data), \
		    (const D4D_MENU_ITEM*)&(name##_items[0]),\
		    pOnClick, \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, name##_relations, pOnUsrMsg, &d4d_menuSysFunc, &(name##_params), flags, pUser, pScheme)\
    \
    static const D4D_OBJECT* const name##_scrollBarRelations[] = {&name, NULL};\
    \
    _D4D_DECLARE_SCROLL_BAR(D4D_NO_CONST, name##_scrollBarVer,  20, 100, 100, 20, radius, NULL, name##_scrollBarRelations, D4D_MENU_F_SCRLBRS_DEFAULT, pScheme, NULL, D4D_MenuScrollBarsFeedBack, NULL) \
    \
    type D4D_MENU_ITEM name##_items[] = {

/**************************************************************************/ /*!
* @brief   Macro that is used to add item to menu definition
* @param   pText - text of item
* @param   pIcon - pointer to item icon bitmap
* @note    This macro is used after the menu declaration to add new item
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_RMENU_AUTOSIZE_BEGIN(my_menu, D4D_DEFSTR("My Menu"), MU_BIG_FONT, 10, 10, 100, 120, 6, MY_SMALL_FONT, MY_FONT, &my_icon, NULL)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item1"), &myIcon1)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item2"), &myIcon2)
*       D4D_DECLARE_MENU_END(my_menu)
* @endcode
*******************************************************************************/
#define D4D_DECLARE_MENU_ITEM(pText, pIcon) { {(pText), D4D_TEXT_LEN(pText), 0, NULL, D4D_OBJECT_MAX_TEXT_LEN(pText), 0}, pIcon, NULL},

/**************************************************************************/ /*!
* @brief   Macro that is used to add item with user data menu definition
* @param   pText - text of item
* @param   pIcon - pointer to item icon bitmap
* @param   pUser - pointer to user data
* @note    This macro is used after the menu declaration to add new item with user data
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_RMENU_AUTOSIZE_BEGIN(my_menu, D4D_DEFSTR("My Menu"), MU_BIG_FONT, 10, 10, 100, 120, 6, MY_SMALL_FONT, MY_FONT, &my_icon, NULL)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item1"), &myIcon1, 0)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item2"), &myIcon2, 1)
*       D4D_DECLARE_MENU_END(my_menu)
* @endcode
*******************************************************************************/
#define D4D_DECLARE_MENU_ITEM_FULL(pText, pIcon, pUser) { {(pText), D4D_TEXT_LEN(pText), 0, NULL, D4D_OBJECT_MAX_TEXT_LEN(pText), 0}, pIcon, (void*)pUser},

/**************************************************************************/ /*!
* @brief   Macro that is used to close the item list for menu definition
* @note    This macro is used after the menu items declaration to close the list of items
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_RMENU_AUTOSIZE_BEGIN(my_menu, D4D_DEFSTR("My Menu"), MU_BIG_FONT, 10, 10, 100, 120, 6, MY_SMALL_FONT, MY_FONT, &my_icon, NULL)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item1"), &myIcon1, 0)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item2"), &myIcon2, 1)
*       _D4D_DECLARE_MENU_END()
* @endcode
*******************************************************************************/
#define _D4D_DECLARE_MENU_END() { {NULL, 0, 0, NULL, 0, 0}, NULL, NULL} };

/**************************************************************************/ /*!
* @brief   Macro that is used to close the item list for menu definition
* @note    This macro is used after the menu items declaration to close the list of items
* Here is an example demonstrating how to used it.
*               This is obsolete macro, the parameter is not needed. It can be used \ref _D4D_DECLARE_MENU_END() instead of.
* @code
*       D4D_DECLARE_STD_RMENU_AUTOSIZE_BEGIN(my_menu, D4D_DEFSTR("My Menu"), MU_BIG_FONT, 10, 10, 100, 120, 6, MY_SMALL_FONT, MY_FONT, &my_icon, NULL)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item1"), &myIcon1, 0)
*       D4D_DECLARE_MENU_ITEM(D4D_DEFSTR("My Item2"), &myIcon2, 1)
*       _D4D_DECLARE_MENU_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_MENU_END(name)  _D4D_DECLARE_MENU_END()


/**************************************************************************/ /*!
* @brief   Macro that create the Menu object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of menu object
* @param   title_text - title text
* @param   title_font - title text font id
* @param   x - coordination of menu in X axis
* @param   y - coordination of menu in Y axis
* @param   cx - size of menu in X axis (width)
* @param   cy - size of menu in Y axis (height) (with hidden list box)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_menu_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw menu
* @param   indexFontId - identification number of the used index text font
* @param   itemsFontId - identification number of the used item text font
* @param   posCnt - count of visible positions
* @param   menuItemsOff - offset between the individual items in pixels
* @param   pIcon - pointer to \ref D4D_BMP structure to title
* @param   pUser - user data of  menu
* @param   pOnClick - Pointer to an on-click user callback function \ref D4D_MENU_ON_CLICK
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_MENU structure, including the object data sub structure. Is used to define all properties of menu. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_MENU_BEGIN instead of this one.
*               It must be followed by \ref D4D_DECLARE_MENU_ITEM or \ref D4D_DECLARE_MENU_ITEM_FULL or at least \ref D4D_DECLARE_MENU_END.
*******************************************************************************/
#define D4D_DECLARE_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_CONST, name, title_text, title_font, x, y, cx, cy, 0, NULL, NULL, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Menu object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of menu object
* @param   title_text - title text
* @param   title_font - title text font id
* @param   x - coordination of menu in X axis
* @param   y - coordination of menu in Y axis
* @param   cx - size of menu in X axis (width)
* @param   cy - size of menu in Y axis (height) (with hidden list box)
* @param   indexFontId - identification number of the used index text font
* @param   itemsFontId - identification number of the used item text font
* @param   pIcon - pointer to \ref D4D_BMP structure to title
* @param   pOnClick - Pointer to an on-click user callback function \ref D4D_MENU_ON_CLICK

* @note    This macro create complete D4D_MENU structure, including the object data sub structure. Is used to define all properties of menu. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_MENU_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*               It must be followed by \ref D4D_DECLARE_MENU_ITEM or \ref D4D_DECLARE_MENU_ITEM_FULL or at least \ref D4D_DECLARE_MENU_END.
*******************************************************************************/
#define D4D_DECLARE_STD_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, indexFontId, itemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, 0, 0, pIcon, NULL, pOnClick, NULL)

// Rounded menu definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Menu object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of menu object
* @param   title_text - title text
* @param   title_font - title text font id
* @param   x - coordination of menu in X axis
* @param   y - coordination of menu in Y axis
* @param   cx - size of menu in X axis (width)
* @param   cy - size of menu in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_menu_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw menu
* @param   indexFontId - identification number of the used index text font
* @param   itemsFontId - identification number of the used item text font
* @param   posCnt - count of visible positions
* @param   menuItemsOff - offset between the individual items in pixels
* @param   pIcon - pointer to \ref D4D_BMP structure to title
* @param   pUser - user data of  menu
* @param   pOnClick - Pointer to an on-click user callback function \ref D4D_MENU_ON_CLICK
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_MENU structure, including the object data sub structure. Is used to define all properties of menu. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_MENU_BEGIN instead of this one.
*               It must be followed by \ref D4D_DECLARE_MENU_ITEM or \ref D4D_DECLARE_MENU_ITEM_FULL or at least \ref D4D_DECLARE_MENU_END.
*******************************************************************************/
#define D4D_DECLARE_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_CONST, name, title_text, title_font, x, y, cx, cy, radius, NULL, NULL, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)



/**************************************************************************/ /*!
* @brief   Macro that create the rounded Menu object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of menu object
* @param   title_text - title text
* @param   title_font - title text font id
* @param   x - coordination of menu in X axis
* @param   y - coordination of menu in Y axis
* @param   cx - size of menu in X axis (width)
* @param   cy - size of menu in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   indexFontId - identification number of the used index text font
* @param   itemsFontId - identification number of the used item text font
* @param   pIcon - pointer to \ref D4D_BMP structure to title
* @param   pOnClick - Pointer to an on-click user callback function \ref D4D_MENU_ON_CLICK

* @note    This macro create complete D4D_MENU structure, including the object data sub structure. Is used to define all properties of menu. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_MENU_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*               It must be followed by \ref D4D_DECLARE_MENU_ITEM or \ref D4D_DECLARE_MENU_ITEM_FULL or at least \ref D4D_DECLARE_MENU_END.
*******************************************************************************/
#define D4D_DECLARE_STD_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, indexFontId, itemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, (D4D_MENU_F_DEFAULT), NULL,\
    indexFontId, itemsFontId, 0, 0, pIcon, NULL, pOnClick, NULL)


//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_MENU_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_NO_CONST, name, title_text, title_font, x, y, cx, cy, 0, NULL, NULL, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)


/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_MENU_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, 0, 0, pIcon, NULL, pOnClick, NULL)


// Rounded menu definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RMENU_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_NO_CONST, name, title_text, title_font, x, y, cx, cy, radius, NULL, NULL, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)



/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RMENU_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, 0, 0, pIcon, NULL, pOnClick, NULL)

/*! @} End of doxd4d_menu_macro                                          */

      /* Obsolete macros */

      #define D4D_DECLARE_RMENU_END(name)  D4D_DECLARE_MENU_END(name)
      #define D4D_DECLARE_MENU_END_INRAM(name)  _D4D_DECLARE_MENU_END()
      #define D4D_DECLARE_RMENU_END_INRAM(name)  D4D_DECLARE_MENU_END_INRAM(name)
/******************************************************************************
* Global functions
******************************************************************************/
D4D_MENU_INDEX D4D_MenuGetIndex(D4D_OBJECT* pThis);

void  D4D_MenuSetIndex(D4D_OBJECT* pThis, D4D_MENU_INDEX ix);
D4D_MENU_INDEX D4D_MenuGetItemCount(D4D_OBJECT_PTR pThis);
D4D_MENU_INDEX D4D_MenuFindUserDataItem(D4D_OBJECT_PTR pThis, void* pUser);
void* D4D_MenuGetItemUserData(D4D_OBJECT_PTR pThis);
D4D_STRING* D4D_MenuGetItemText(D4D_OBJECT_PTR pThis);


#endif /* __D4D_MENU_H  */




