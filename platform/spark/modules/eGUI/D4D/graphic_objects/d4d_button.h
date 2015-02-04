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
* @file      d4d_button.h
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.47.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver button object header file
*
*******************************************************************************/

#ifndef __D4D_BUTTON_H
#define __D4D_BUTTON_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_btnSysFunc;

/******************************************************************************
* D4D BUTTON setting  constants
*
*//*! @addtogroup doxd4d_button_const
* @{
*******************************************************************************/

/*! @brief This is pixel count of 3D appearence button content shift.
           If not defined, it sets to 1 pixel as a default.*/
#ifndef D4D_BTN_3D_SHIFT
  #define D4D_BTN_3D_SHIFT 1
#endif

//********************* Button flags for button specific flags *****************
/*!
  @defgroup doxd4d_button_const_flags D4D BUTTON Defines masks of button specific behaviour flags
  This section specifies masks of button behaviour flags.
  @ingroup doxd4d_button_const
*/

/**
 * @addtogroup doxd4d_button_const_flags
 * @{
 */
  #define D4D_BTN_F_3D            (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< Enable 3D appearence for button
  #define D4D_BTN_F_CONT_RECT     (0x02 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< Enable content rectangle for button
  #define D4D_BTN_F_INVERT        (0x04 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< Enabel inverting button colours on keypress
/**@}*/

//******************************************************************************

// button configuration (goes to ROM by default)

/*! @brief This is button init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_BTN_F_3D) as a default.*/
#ifndef D4D_BTN_F_DEFAULT
  #define D4D_BTN_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_BEVEL_RAISED | D4D_BTN_F_3D)
#endif

/*! @brief This is button init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_BTN_TXT_PRTY_DEFAULT
  #define D4D_BTN_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is button init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_BTN_FNT_PRTY_DEFAULT
  #define D4D_BTN_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is button intesity of fore color in 3D appearence mode.
           If not defined, it sets to ( \ref D4D_BEVEL_FORE_INTENSITY ) as a default.*/
#ifndef D4D_BUTTON_3D_FORE_INTENSITY
  #define D4D_BUTTON_3D_FORE_INTENSITY    D4D_BEVEL_FORE_INTENSITY
#endif

/*! @brief This is button intesity of back color in 3D appearence mode.
           If not defined, it sets to ( \ref D4D_BEVEL_BCKG_INTENSITY ) as a default.*/
#ifndef D4D_BUTTON_3D_BCKG_INTENSITY
  #define D4D_BUTTON_3D_BCKG_INTENSITY    D4D_BEVEL_BCKG_INTENSITY
#endif

/*! @} End of doxd4d_button_const                                             */



/******************************************************************************
* D4D BUTTON setting  public types
*
*//*! @addtogroup doxd4d_button_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of button on click callback function
* @param   pThis - pointer to button instance that call the function
* @return  none
* @note    This is button call back function used to notify user application about
*          On Click button event.
*******************************************************************************/
typedef void (*D4D_BTN_ON_CLICK)(D4D_OBJECT* pThis);

/*! @} End of doxd4d_button_type                                             */

/******************************************************************************
* Private Types
******************************************************************************/

typedef Byte D4D_BUTTON_STATUS;

#define D4D_BUTTON_STATUS_PRESSED_MASK          (0x01)
#define D4D_BUTTON_STATUS_1ST_TOUCH_MASK        (0x02)
#define D4D_BUTTON_STATUS_MOUSE_CLICK_MASK      (0x04)


typedef struct
{
    D4D_STRING textBuff;    // button text
    const D4D_BMP*  pBmpNormal;    // normal state bitmap
    const D4D_BMP*  pBmpFocus;     // focused state bitmap
    D4D_BUTTON_STATUS*  pStatus;
    D4D_BTN_ON_CLICK OnClicked;
} D4D_BUTTON;


/******************************************************************************
* Macros
******************************************************************************/

// getting the BUTTON structure from general OBJECT
#define D4D_GET_BUTTON(pObj) ((D4D_BUTTON*)((pObj)->pParam))
#define D4D_GET_BUTTON_STATUS(pObj) ((D4D_BUTTON_STATUS*)(D4D_GET_BUTTON(pObj)->pStatus))


/******************************************************************************
* D4D BUTTON setting  public macros
*
*//*! @addtogroup doxd4d_button_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Button object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_button_const_flags
* @param   pBmpN - pointer to a bitmap that is shown in a normal state of the button (Could be NULL)
* @param   pBmpF - pointer to a bitmap that is shown in a focus state of the button (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  button
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. Is used to define all properties of Button.
*******************************************************************************/
#define _D4D_DECLARE_BUTTON(type, name, text, x, y, cx, cy, radius, pMargin, pRelations, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
    static D4D_BUTTON_STATUS  name##_status = { 0 }; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_BTN_FNT_PRTY_DEFAULT, D4D_BTN_TXT_PRTY_DEFAULT}; \
    static type D4D_BUTTON name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0}, /* textBuff */ \
        pBmpN,      /* pBmpNormal */ \
        pBmpF,      /* pBmpFocus */ \
        &(name##_status), /* Status clear */ \
        onclick     /* Click event */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg,  &d4d_btnSysFunc, &(name##_params), flags, pUser, pScheme)

/**************************************************************************/ /*!
* @brief   Macro that create the Button object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_button_const_flags
* @param   pBmpN - pointer to a bitmap that is shown in a normal state of the button (Could be NULL)
* @param   pBmpF - pointer to a bitmap that is shown in a focus state of the button (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  button
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. Is used to define all properties of Button. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*******************************************************************************/
#define D4D_DECLARE_BUTTON(name, text, x, y, cx, cy, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Button object structure in memory including all substructures with restricted count of parameters to simplify definition.
*               The missing parameters are replaced by default values.
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   pBmpN - pointer to a bitmap that is shown in a normal state of the button (Could be NULL)
* @param   pBmpF - pointer to a bitmap that is shown in a focus state of the button (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_BUTTON(name, text, x, y, cx, cy, pBmpN, pBmpF, fontId, onclick) \
    D4D_DECLARE_BUTTON(name, text, x, y, cx, cy, (D4D_BTN_F_DEFAULT), pBmpN, pBmpF, NULL, fontId, NULL, onclick, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the only text Button object structure in memory including all substructures with restricted count of parameters to simplify definition.
*               The missing parameters are replaced by default values.
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   fontId - Identification number of the used title text font
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_TXT_BUTTON(name, text, x, y, cx, cy, fontId, onclick) \
    D4D_DECLARE_STD_BUTTON(name, text, x, y, cx, cy,  NULL, NULL, \
        fontId, onclick)


// Rounded button definition
/**************************************************************************/ /*!
* @brief   Macro that create the round Button object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_button_const_flags
* @param   pBmpN - pointer to a bitmap that is shown in a normal state of the button (Could be NULL)
* @param   pBmpF - pointer to a bitmap that is shown in a focus state of the button (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  button
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the D4D_MSG_SKIP
*               return value, in a normal case the return value must be D4D_MSG_NOSKIP

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. Is used to define all properties of Button. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RBUTTON(name, text, x, y, cx, cy, radius, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the round Button object structure in memory including all substructures with restricted count of parameters to simplify definition.
*               The missing parameters are replaced by default values.
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   radius - radius of corners
* @param   pBmpN - pointer to a bitmap that is shown in a normal state of the button (Could be NULL)
* @param   pBmpF - pointer to a bitmap that is shown in a focus state of the button (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RBUTTON(name, text, x, y, cx, cy, radius, pBmpN, pBmpF, fontId, onclick) \
    D4D_DECLARE_RBUTTON(name, text, x, y, cx, cy, radius, (D4D_BTN_F_DEFAULT), pBmpN, pBmpF, NULL, fontId, NULL, onclick, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the only text round Button object structure in memory including all substructures with restricted count of parameters to simplify definition.
*               The missing parameters are replaced by default values.
* @param   name - name of button object
* @param   text - title text of button
* @param   x - coordination of button in X axis
* @param   y - coordination of button in Y axis
* @param   cx - size of button in X axis (width)
* @param   cy - size of button in Y axis (height)
* @param   radius - radius of corners
* @param   fontId - Identification number of the used title text font
* @param   onclick - Pointer to an on-click user callback function \ref D4D_BTN_ON_CLICK

* @note    This macro create complete D4D_BUTTON structure, including the object data sub structure. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_TXT_RBUTTON(name, text, x, y, cx, cy, radius, fontId, onclick) \
    D4D_DECLARE_STD_RBUTTON(name, text, x, y, cx, cy,  radius, NULL, NULL, \
        fontId, onclick)

// IN RAM instantion macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_BUTTON, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_BUTTON_INRAM(name, text, x, y, cx, cy, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_NO_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_BUTTON, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_BUTTON_INRAM(name, text, x, y, cx, cy, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_BUTTON_INRAM(name, text, x, y, cx, cy, (D4D_BTN_F_DEFAULT), bmpN, bmpF, NULL, fontId, NULL, onclick, NULL)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_TXT_BUTTON, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_TXT_BUTTON_INRAM(name, text, x, y, cx, cy, fontId, onclick) \
    D4D_DECLARE_STD_BUTTON_INRAM(name, text, x, y, cx, cy,  NULL, NULL, \
        fontId, onclick)

// Rounded button definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RBUTTON, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_NO_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, flags, pBmpN, pBmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RBUTTON, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, (D4D_BTN_F_DEFAULT), bmpN, bmpF, NULL, fontId, NULL, onclick, NULL)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_TXT_RBUTTON, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_TXT_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, fontId, onclick) \
    D4D_DECLARE_STD_RBUTTON_INRAM(name, text, x, y, cx, cy,  radius, NULL, NULL, \
        fontId, onclick)

/*! @} End of doxd4d_button_macro                                             */
/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/

// Obsolete functions, replaced by any general
#define  D4D_BtnSetText D4D_SetText

#endif /*__D4D_BUTTON_H*/












