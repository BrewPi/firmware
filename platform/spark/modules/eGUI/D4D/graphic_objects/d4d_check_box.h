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
* @file      d4d_check_box.h
*
* @author     Petr Gargulak
*
* @version   0.0.33.0
*
* @date      Oct-14-2013
*
* @brief     D4D Driver check box object header file
*
*******************************************************************************/

#ifndef __D4D_CHECKBOX_H
#define __D4D_CHECKBOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_checkBoxSysFunc;
/******************************************************************************
* D4D CHECK BOX setting  constants
*
*//*! @addtogroup doxd4d_check_box_const
* @{
*******************************************************************************/

/*! @brief This is pixel count of check box bitmap border offset.
           If not defined, it sets to 3 pixel as a default.*/
#ifndef D4D_CHECKBOX_BORDER_OFFSET
  #define D4D_CHECKBOX_BORDER_OFFSET 3
#endif

/*! @brief This is pixel count of check box text border offset.
           If not defined, it sets to 4 pixel as a default.*/
#ifndef D4D_CHECKBOX_TEXT_OFFSET
  #define D4D_CHECKBOX_TEXT_OFFSET 4
#endif

/*! @brief This is check box icon background color definition. Is used for example if non-check icon is not defined.
           If not defined, it sets to \ref D4D_COLOR_WHITE as a default.*/
#ifndef D4D_COLOR_CHECKBOX_ICON_BCKG
  #define D4D_COLOR_CHECKBOX_ICON_BCKG  D4D_COLOR_WHITE
#endif


//********************* Check Box flags for check box specific flags *****************
/*!
  @defgroup doxd4d_check_box_const_flags D4D CHECK BOX Defines masks of check box specific behaviour flags
  This section specifies masks of check box behaviour flags.
  @ingroup doxd4d_check_box_const
*/

/**
 * @addtogroup doxd4d_check_box_const_flags
 * @{
 */
#define D4D_CHECKBOX_F_ICON_RECTANGLE   (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)     ///< Enable the check box icon rectangle

/**@}*/

//******************************************************************************

/*! @brief This is check box init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE
            | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_CHECKBOX_F_ICON_RECTANGLE ) as a default.*/
#ifndef D4D_CHECKBOX_F_DEFAULT
  #define D4D_CHECKBOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE\
                                  | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED | D4D_CHECKBOX_F_ICON_RECTANGLE )
#endif

/*! @brief This is check box init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_CHECKBOX_TXT_PRTY_DEFAULT
  #define D4D_CHECKBOX_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_LEFT_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is check box init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_CHECKBOX_FNT_PRTY_DEFAULT
  #define D4D_CHECKBOX_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @} End of doxd4d_check_box_const                                             */

/******************************************************************************
* D4D CHECK BOX setting  public types
*
*//*! @addtogroup doxd4d_check_box_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of check box on change callback function
* @param   pThis - pointer to check box instance that call the function
* @return  none
* @note    This is check box call back function used to notify user application about
*          On Change check box event.
*******************************************************************************/
typedef void (*D4D_CHCKBX_ON_CHANGE)(D4D_OBJECT* pThis);

/*! @} End of doxd4d_check_box_type                                             */

/******************************************************************************
* Private Types
******************************************************************************/

// Check box configuration (goes to ROM by default)


typedef Byte D4D_CHECKBOX_STATUS;

#define D4D_CHECKBOX_STATUS_PRESSED_MASK    (0x01)
#define D4D_CHECKBOX_STATUS_CHECKED_MASK    (0x02)

typedef struct
{
    D4D_STRING textBuff;    // check box text
    const D4D_BMP*  pBmpChecked;    // Checked state bitmap
    const D4D_BMP*  pBmpUnChecked;    // Checked state bitmap
    D4D_CHECKBOX_STATUS*  pStatus;
    D4D_CHCKBX_ON_CHANGE OnChange;
} D4D_CHECKBOX;


/******************************************************************************
* Macros
******************************************************************************/

// getting the CHECK BOX structure from general OBJECT
#define D4D_GET_CHECKBOX(pObj) ((D4D_CHECKBOX*)((pObj)->pParam))
#define D4D_GET_CHECKBOX_STATUS(pObj) ((D4D_CHECKBOX_STATUS*)(D4D_GET_CHECKBOX(pObj)->pStatus))



/******************************************************************************
* D4D CHECK BOX setting  public macros
*
*//*! @addtogroup doxd4d_check_box_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Check box object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of check box object
* @param   text - title text of check box
* @param   x - coordination of check box in X axis
* @param   y - coordination of check box in Y axis
* @param   cx - size of check box in X axis (width)
* @param   cy - size of check box in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_check_box_const_flags
* @param   pbmpChecked - pointer to a bitmap that is shown in a checked state of the check box (Could be NULL)
* @param   pbmpUnChecked - pointer to a bitmap that is shown in a unchecked state of the check box (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw check box
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  check box
* @param   onchange - Pointer to an on-change user callback function \ref D4D_CHCKBX_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_CHECK_BOX structure, including the object data sub structure. Is used to define all properties of check box.
*******************************************************************************/
#define _D4D_DECLARE_CHECKBOX(type, name, text, x, y, cx, cy, radius, pMargin, pRelations, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
    static D4D_CHECKBOX_STATUS name##_status = { 0 }; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_CHECKBOX_FNT_PRTY_DEFAULT, D4D_CHECKBOX_TXT_PRTY_DEFAULT}; \
    static type D4D_CHECKBOX name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0}, /* textBuff */ \
        pbmpChecked,      /* pBmpChecked */ \
        pbmpUnChecked,      /* pBmpUnChecked */ \
        &(name##_status), /* Status clear */ \
        onchange     /* Click event */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_checkBoxSysFunc, &(name##_params), flags, pUser, pScheme)

/**************************************************************************/ /*!
* @brief   Macro that create the Check box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of check box object
* @param   text - title text of check box
* @param   x - coordination of check box in X axis
* @param   y - coordination of check box in Y axis
* @param   cx - size of check box in X axis (width)
* @param   cy - size of check box in Y axis (height)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_check_box_const_flags
* @param   pbmpChecked - pointer to a bitmap that is shown in a checked state of the check box (Could be NULL)
* @param   pbmpUnChecked - pointer to a bitmap that is shown in a unchecked state of the check box (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw check box
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  check box
* @param   onchange - Pointer to an on-change user callback function \ref D4D_CHCKBX_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_CHECK_BOX structure, including the object data sub structure. Is used to define all properties of check box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*******************************************************************************/
#define D4D_DECLARE_CHECKBOX(name, text, x, y, cx, cy, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_CONST, name, text, x, y, cx, cy, 0, (D4D_MARGIN*)&d4d_marginDefault, NULL, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Check box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of check box object
* @param   text - title text of check box
* @param   x - coordination of check box in X axis
* @param   y - coordination of check box in Y axis
* @param   cx - size of check box in X axis (width)
* @param   cy - size of check box in Y axis (height)
* @param   pbmpChecked - pointer to a bitmap that is shown in a checked state of the check box (Could be NULL)
* @param   pbmpUnChecked - pointer to a bitmap that is shown in a unchecked state of the check box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onchange - Pointer to an on-change user callback function \ref D4D_CHCKBX_ON_CHANGE

* @note    This macro create complete D4D_CHECK_BOX structure, including the object data sub structure. Is used to define all properties of check box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_CHECKBOX(name, text, x, y, cx, cy, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_CHECKBOX(name, text, x, y, cx, cy, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)

// Rounded check box definition

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Check box object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of check box object
* @param   text - title text of check box
* @param   x - coordination of check box in X axis
* @param   y - coordination of check box in Y axis
* @param   cx - size of check box in X axis (width)
* @param   cy - size of check box in Y axis (height)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_check_box_const_flags
* @param   pbmpChecked - pointer to a bitmap that is shown in a checked state of the check box (Could be NULL)
* @param   pbmpUnChecked - pointer to a bitmap that is shown in a unchecked state of the check box (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw check box
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  check box
* @param   onchange - Pointer to an on-change user callback function \ref D4D_CHCKBX_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_CHECK_BOX structure, including the object data sub structure. Is used to define all properties of check box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RCHECKBOX(name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_CONST, name, text, x, y, cx, cy, radius, (D4D_MARGIN*)&d4d_marginDefault, NULL, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Check box object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of check box object
* @param   text - title text of check box
* @param   x - coordination of check box in X axis
* @param   y - coordination of check box in Y axis
* @param   cx - size of check box in X axis (width)
* @param   cy - size of check box in Y axis (height)
* @param   radius - radius of corners
* @param   pbmpChecked - pointer to a bitmap that is shown in a checked state of the check box (Could be NULL)
* @param   pbmpUnChecked - pointer to a bitmap that is shown in a unchecked state of the check box (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onchange - Pointer to an on-change user callback function \ref D4D_CHCKBX_ON_CHANGE

* @note    This macro create complete D4D_CHECK_BOX structure, including the object data sub structure. Is used to define all properties of check box. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_CHECKBOX instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RCHECKBOX(name, text, x, y, cx, cy, radius, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_RCHECKBOX(name, text, x, y, cx, cy, radius, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_CHECKBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_CHECKBOX_INRAM(name, text, x, y, cx, cy, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_NO_CONST, name, text, x, y, cx, cy, 0, (D4D_MARGIN*)&d4d_marginDefault, NULL, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_CHECKBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_CHECKBOX_INRAM(name, text, x, y, cx, cy, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_CHECKBOX_INRAM(name, text, x, y, cx, cy, 0, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)


// Rounded check box definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RCHECKBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RCHECKBOX_INRAM(name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_NO_CONST, name, text, x, y, cx, cy, radius, (D4D_MARGIN*)&d4d_marginDefault, NULL, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RCHECKBOX, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RCHECKBOX_INRAM(name, text, x, y, cx, cy, radius, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_RCHECKBOX_INRAM(name, text, x, y, cx, cy, radius, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)

/*! @} End of doxd4d_check_box_macro                                          */

/******************************************************************************
* Global functions
******************************************************************************/

void D4D_CheckBoxSetValue(D4D_OBJECT_PTR pThis, D4D_BOOL value);
D4D_BOOL D4D_CheckBoxGetValue(D4D_OBJECT_PTR pThis);

// Obsolete functions, replaced by any general
#define D4D_CheckBoxSetText D4D_SetText



#endif /*__D4D_CHECKBOX_H*/












