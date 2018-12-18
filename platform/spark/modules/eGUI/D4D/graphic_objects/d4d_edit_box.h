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
* @file      d4d_edit_box.h
*
* @author     Petr Gargulak
*
* @version   0.0.5.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver editBox object header file
*
*******************************************************************************/

#ifndef __D4D_EDIT_BOX_H
#define __D4D_EDIT_BOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_editBoxSysFunc;
/******************************************************************************
* D4D EDIT BOX setting  constants
*
*//*! @addtogroup doxd4d_edit_box_const
* @{
*******************************************************************************/

/*! @brief This is edit box init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_DENTED) as a default.*/
#ifndef D4D_EDIT_BOX_F_DEFAULT
  #define D4D_EDIT_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_DENTED)
#endif

/*! @brief This is edit box init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_EDIT_BOX_FNT_PRTY_DEFAULT
  #define D4D_EDIT_BOX_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is edit box init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_LEFT_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_EDIT_BOX_TXT_PRTY_DEFAULT
  #define D4D_EDIT_BOX_TXT_PRTY_DEFAULT  ( D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_LEFT_MASK )
#endif

/*! @brief This is definition of edit box cursor blink time (in ticks of eGUI time ticks - \ref D4D_TimeTickPut).
           If not defined, it sets to ( 10 ) as a default.*/
#ifndef D4D_EDIT_BOX_CURSOR_BLINK_TICK_COUNTER
  #define D4D_EDIT_BOX_CURSOR_BLINK_TICK_COUNTER  ( 10 )
#endif

/*! @} End of doxd4d_edit_box_const                                             */

/******************************************************************************
* D4D EDIT BOX setting  public types
*
*//*! @addtogroup doxd4d_edit_box_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of edit box on event callback function
* @param   pThis - pointer to edit box instance that call the function
* @param   eventId - event type that caused this call back function
* @return  none
* @note    This is edit box call back function used to notify user application about
*          edit box events (\ref D4D_EVENT_ONCLEAR, \ref D4D_EVENT_ONCHANGE, \ref D4D_EVENT_ONCHANGEDONE).
*******************************************************************************/
typedef void (*D4D_EDIT_BOX_ON_EVENT)(D4D_OBJECT* pThis, D4D_EVENTID eventId);

/*! @} End of doxd4d_edit_box_type                                             */

/******************************************************************************
* Private Types
******************************************************************************/

typedef sByte D4D_EDIT_BOX_INDEX;

typedef Byte D4D_EDIT_BOX_FLAGS;

#define D4D_EDIT_BOX_FLAGS_REDRAWTEXT     (0x04)
#define D4D_EDIT_BOX_FLAGS_MOVECURSOR     (0x08)
#define D4D_EDIT_BOX_FLAGS_REDRAWCURSOR   (0x10)
#define D4D_EDIT_BOX_FLAGS_CURSORSTATE    (0x20)
#define D4D_EDIT_BOX_FLAGS_FULL           (0x40)
#define D4D_EDIT_BOX_FLAGS_CHANGETEXT     (0x80)




typedef struct
{
   D4D_STRING string;
   D4D_INDEX curPos;
   D4D_INDEX tickCounter;
   D4D_EDIT_BOX_FLAGS flags;
} D4D_EDIT_BOX_DATA;

// EditBox configuration (goes to ROM by default)
typedef struct
{
   D4D_TCHAR* pInitText;
   D4D_EDIT_BOX_DATA* pData;     // EditBox volatile data struct
   D4D_EDIT_BOX_ON_EVENT pOnEvent;
} D4D_EDIT_BOX;

/******************************************************************************
* Macros
******************************************************************************/

// getting the EDIT_BOX structure from general OBJECT
#define D4D_GET_EDIT_BOX(pObj) ((D4D_EDIT_BOX*)((pObj)->pParam))
#define D4D_GET_EDIT_BOX_DATA(pEditBox) ((pEditBox)->pData)

/******************************************************************************
* D4D EDIT BOX setting  public macros
*
*//*! @addtogroup doxd4d_edit_box_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Edit Box object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of edit box object
* @param   x - coordination of edit box in X axis
* @param   y - coordination of edit box in Y axis
* @param   cx - size of edit box in X axis (width)
* @param   cy - size of edit box in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   pInitText - the initialization text of edit box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   maxStr - maximal length of string handled by edit box (it allocates the memory)
* @param   pUser - user data of  button
* @param   pOnEvent - Pointer to an on-event user callback function \ref D4D_EDIT_BOX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_EDIT_BOX structure, including the object data sub structure. Is used to define all properties of Edit Box.
*******************************************************************************/
#define _D4D_DECLARE_EDIT_BOX(type, name, x, y, cx, cy, radius, pMargin, pRelations, flags, pScheme, pInitText, fontId, maxStr, pUser, pOnEvent, pOnUsrMsg) \
    static D4D_STR_PROPERTIES name##_strPrties = {D4D_EDIT_BOX_FNT_PRTY_DEFAULT, D4D_EDIT_BOX_TXT_PRTY_DEFAULT};\
    static D4D_TCHAR name##_txtArray[maxStr + 1];\
    static D4D_EDIT_BOX_DATA name##_data = { {name##_txtArray, maxStr, fontId, &name##_strPrties, 0, 0}, 0, 0, 0};\
    static type D4D_EDIT_BOX name##_params = \
    { \
        pInitText,\
        &(name##_data), \
        pOnEvent \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_editBoxSysFunc, &(name##_params), flags, pUser, pScheme)


/**************************************************************************/ /*!
* @brief   Macro that create the Edit Box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of edit box object
* @param   x - coordination of edit box in X axis
* @param   y - coordination of edit box in Y axis
* @param   cx - size of edit box in X axis (width)
* @param   cy - size of edit box in Y axis (height)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   pInitText - the initialization text of edit box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   maxStr - maximal length of string handled by edit box (it allocates the memory)
* @param   pUser - user data of  button
* @param   pOnEvent - Pointer to an on-event user callback function \ref D4D_EDIT_BOX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_EDIT_BOX structure, including the object data sub structure. Is used to define all properties of Edit Box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_EDIT_BOX instead of this one.
*******************************************************************************/
#define D4D_DECLARE_EDIT_BOX(name, x, y, cx, cy, pRelations, flags, pScheme, pInitText, fontId, maxStr, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_EDIT_BOX(D4D_CONST, name, x, y, cx, cy, 0, (D4D_MARGIN*)&d4d_marginDefault, pRelations, flags, pScheme, pInitText,\
    fontId, maxStr, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Edit Box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of edit box object
* @param   x - coordination of edit box in X axis
* @param   y - coordination of edit box in Y axis
* @param   cx - size of edit box in X axis (width)
* @param   cy - size of edit box in Y axis (height)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   pInitText - the initialization text of edit box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   maxStr - maximal length of string handled by edit box (it allocates the memory)
* @param   pOnEvent - Pointer to an on-event user callback function \ref D4D_EDIT_BOX_ON_EVENT

* @note    This macro create complete D4D_EDIT_BOX structure, including the object data sub structure. Is used to define all properties of Edit Box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_EDIT_BOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_EDIT_BOX(name, x, y, cx, cy, pRelations, pInitText, fontId, maxStr, pOnEvent) \
    D4D_DECLARE_EDIT_BOX(name, x, y, cx, cy, pRelations, (D4D_EDIT_BOX_F_DEFAULT), NULL, pInitText, fontId, maxStr, NULL, pOnEvent, NULL)

// Rounded editBox definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Edit Box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of edit box object
* @param   x - coordination of edit box in X axis
* @param   y - coordination of edit box in Y axis
* @param   cx - size of edit box in X axis (width)
* @param   cy - size of edit box in Y axis (height)
* @param   radius - radius of corners
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   pInitText - the initialization text of edit box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   maxStr - maximal length of string handled by edit box (it allocates the memory)
* @param   pUser - user data of  button
* @param   pOnEvent - Pointer to an on-event user callback function \ref D4D_EDIT_BOX_ON_EVENT
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_EDIT_BOX structure, including the object data sub structure. Is used to define all properties of Edit Box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_EDIT_BOX instead of this one.
*******************************************************************************/
#define D4D_DECLARE_REDIT_BOX(name, x, y, cx, cy, radius, pRelations, flags, pScheme, pInitText, fontId, maxStr, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_EDIT_BOX(D4D_CONST, name, x, y, cx, cy, radius, (D4D_MARGIN*)&d4d_marginDefault, pRelations, flags, pScheme, pInitText,\
    fontId, maxStr, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Edit Box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of edit box object
* @param   x - coordination of edit box in X axis
* @param   y - coordination of edit box in Y axis
* @param   cx - size of edit box in X axis (width)
* @param   cy - size of edit box in Y axis (height)
* @param   radius - radius of corners
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   pInitText - the initialization text of edit box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   maxStr - maximal length of string handled by edit box (it allocates the memory)
* @param   pOnEvent - Pointer to an on-event user callback function \ref D4D_EDIT_BOX_ON_EVENT

* @note    This macro create complete D4D_EDIT_BOX structure, including the object data sub structure. Is used to define all properties of Edit Box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_EDIT_BOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_REDIT_BOX(name, x, y, cx, cy, radius, pRelations, pInitText, fontId, maxStr, pOnEvent) \
    D4D_DECLARE_REDIT_BOX(name, x, y, cx, cy, radius, pRelations, (D4D_EDIT_BOX_F_DEFAULT), NULL, pInitText, fontId, maxStr, NULL, pOnEvent, NULL)


//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_EDIT_BOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_EDIT_BOX_INRAM(name, x, y, cx, cy, pRelations, flags, pScheme, pInitText, fontId, maxStr, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_EDIT_BOX(D4D_NO_CONST, name, x, y, cx, cy, 0, (D4D_MARGIN*)&d4d_marginDefault, pRelations, flags, pScheme, pInitText,\
    fontId, maxStr, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_EDIT_BOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_EDIT_BOX_INRAM(name, x, y, cx, cy, pRelations, pInitText, fontId, maxStr, pOnEvent) \
    D4D_DECLARE_EDIT_BOX_INRAM(name, x, y, cx, cy, pRelations, (D4D_EDIT_BOX_F_DEFAULT), NULL, pInitText, fontId, maxStr, NULL, pOnEvent, NULL)

// Rounded editBox definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_REDIT_BOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_REDIT_BOX_INRAM(name, x, y, cx, cy, radius, pRelations, flags, pScheme, pInitText, fontId, maxStr, pUser, pOnEvent, pOnUsrMsg) \
 _D4D_DECLARE_EDIT_BOX(D4D_NO_CONST, name, x, y, cx, cy, radius, (D4D_MARGIN*)&d4d_marginDefault, pRelations, flags, pScheme, pInitText,\
    fontId, maxStr, pUser, pOnEvent, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_REDIT_BOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_REDIT_BOX_INRAM(name, x, y, cx, cy, radius, pRelations, pInitText, fontId, maxStr, pOnEvent) \
    D4D_DECLARE_REDIT_BOX_INRAM(name, x, y, cx, cy, radius, pRelations, (D4D_EDIT_BOX_F_DEFAULT), NULL, pInitText, fontId, maxStr, NULL, pOnEvent, NULL)

/*! @} End of doxd4d_edit_box_macro                                             */
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
D4D_BOOL D4D_EditBoxPutChar(D4D_OBJECT_PTR pObj, D4D_TCHAR ch);
D4D_BOOL D4D_EditBoxPutString(D4D_OBJECT_PTR pObj, D4D_TCHAR* pText);
D4D_INDEX D4D_EditBoxSetCursor(D4D_OBJECT_PTR pObj, D4D_INDEX newPos);
D4D_INDEX D4D_EditBoxGetCursor(D4D_OBJECT_PTR pObj);
void D4D_EditBoxClearAll(D4D_OBJECT_PTR pObj);
D4D_TCHAR* D4D_EditBoxGetText(D4D_OBJECT_PTR pObj);

#endif /* __D4D_EDIT_BOX_H  */




