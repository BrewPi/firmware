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
* @file      d4d_label.h
*
* @author     Petr Gargulak
*
* @version   0.0.40.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_LABEL_H
#define __D4D_LABEL_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_labelSysFunc;

/******************************************************************************
* D4D LABEL setting  constants
*
*//*! @addtogroup doxd4d_label_const
* @{
*******************************************************************************/

/*! @brief This is label init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_FOCUSRECT) as a default.*/
#ifndef D4D_LBL_F_DEFAULT
  #define D4D_LBL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT)
#endif

/*! @brief This is label init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_LBL_TXT_PRTY_DEFAULT
  #define D4D_LBL_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is label init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_LBL_FNT_PRTY_DEFAULT
  #define D4D_LBL_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @} End of doxd4d_label_const                                             */


/******************************************************************************
* Types
******************************************************************************/


// label configuration (goes to ROM by default)

typedef struct
{
    D4D_STRING textBuff;    // label text
} D4D_LABEL;


/******************************************************************************
* Macros
******************************************************************************/

// getting the LABEL structure from general OBJECT
#define D4D_GET_LABEL(pObj) ((D4D_LABEL*)((pObj)->pParam))

/******************************************************************************
* D4D LABEL setting  public macros
*
*//*! @addtogroup doxd4d_label_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Label object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of label object
* @param   text - title text of label
* @param   x - coordination of label in X axis
* @param   y - coordination of label in Y axis
* @param   cx - size of label in X axis (width)
* @param   cy - size of label in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_LABEL structure, including the object data sub structure. Is used to define all properties of label.
The code example:
      @code
      _D4D_DECLARE_LABEL(D4D_CONST, my_label, D4D_DEFSTR("My label"), 10, 10, 120, 120, 6, NULL, NULL, (D4D_LABEL_F_DEFAULT), NULL, MY_FONT, NULL, NULL)
      @endcode
*******************************************************************************/
#define _D4D_DECLARE_LABEL(type, name, text, x, y, cx, cy, radius, pMargin, pRelations, flags, pScheme, fontId, pUser, pOnUsrMsg) \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_LBL_FNT_PRTY_DEFAULT, D4D_LBL_TXT_PRTY_DEFAULT}; \
    static type D4D_LABEL name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0} /* textBuff */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_labelSysFunc, &(name##_params), flags, pUser, pScheme)


/**************************************************************************/ /*!
* @brief   Macro that create the Label object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of label object
* @param   text - title text of label
* @param   x - coordination of label in X axis
* @param   y - coordination of label in Y axis
* @param   cx - size of label in X axis (width)
* @param   cy - size of label in Y axis (height)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_LABEL structure, including the object data sub structure. Is used to define all properties of label.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LABEL instead of this one.
The code example:
      @code
      D4D_DECLARE_LABEL(my_label, D4D_DEFSTR("My label"), 10, 10, 120, 120, (D4D_LABEL_F_DEFAULT), NULL, MY_FONT, NULL, NULL)
      @endcode
*******************************************************************************/
#define D4D_DECLARE_LABEL(name, text, x, y, cx, cy, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Label object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of label object
* @param   text - title text of label
* @param   x - coordination of label in X axis
* @param   y - coordination of label in Y axis
* @param   cx - size of label in X axis (width)
* @param   cy - size of label in Y axis (height)
* @param   fontId - Identification number of the used title text font

* @note    This macro create complete D4D_LABEL structure, including the object data sub structure. Is used to define all properties of label.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LABEL instead of this one.
*               The main advantage is less parameters of this macro against the full version.
The code example:
      @code
      D4D_DECLARE_STD_LABEL(my_label, D4D_DEFSTR("My label"), 10, 10, 120, 120, MY_FONT)
      @endcode
*******************************************************************************/
#define D4D_DECLARE_STD_LABEL(name, text, x, y, cx, cy, fontId) \
    D4D_DECLARE_LABEL(name, text, x, y, cx, cy, (D4D_LBL_F_DEFAULT) , NULL, fontId, NULL, NULL)

// Rounded button definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Label object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of label object
* @param   text - title text of label
* @param   x - coordination of label in X axis
* @param   y - coordination of label in Y axis
* @param   cx - size of label in X axis (width)
* @param   cy - size of label in Y axis (height)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_LABEL structure, including the object data sub structure. Is used to define all properties of label.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LABEL instead of this one.
The code example:
      @code
      D4D_DECLARE_LABEL(my_label, D4D_DEFSTR("My label"), 10, 10, 120, 120, (D4D_LABEL_F_DEFAULT), NULL, MY_FONT, NULL, NULL)
      @endcode
*******************************************************************************/
#define D4D_DECLARE_RLABEL(name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Label object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of label object
* @param   text - title text of label
* @param   x - coordination of label in X axis
* @param   y - coordination of label in Y axis
* @param   cx - size of label in X axis (width)
* @param   cy - size of label in Y axis (height)
* @param   radius - radius of corners
* @param   fontId - Identification number of the used title text font

* @note    This macro create complete D4D_LABEL structure, including the object data sub structure. Is used to define all properties of label.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_LABEL instead of this one.
*               The main advantage is less parameters of this macro against the full version.
The code example:
      @code
      D4D_DECLARE_STD_LABEL(my_label, D4D_DEFSTR("My label"), 10, 10, 120, 120, MY_FONT)
      @endcode
*******************************************************************************/
#define D4D_DECLARE_STD_RLABEL(name, text, x, y, cx, cy, radius, fontId) \
    D4D_DECLARE_RLABEL(name, text, x, y, cx, cy, radius, (D4D_LBL_F_DEFAULT) , NULL, fontId, NULL, NULL)

// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_LABEL, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_LABEL_INRAM(name, text, x, y, cx, cy, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_NO_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_LABEL, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_LABEL_INRAM(name, text, x, y, cx, cy, font) \
    D4D_DECLARE_LABEL_INRAM(name, text, x, y, cx, cy, (D4D_LBL_F_DEFAULT) , NULL, font, NULL, NULL)

// Rounded button definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RLABEL, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RLABEL_INRAM(name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_NO_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, flags, pScheme, fontId, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RLABEL, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RLABEL_INRAM(name, text, x, y, cx, cy, radius, font) \
    D4D_DECLARE_RLABEL_INRAM(name, text, x, y, cx, cy, radius, (D4D_LBL_F_DEFAULT) , NULL, font, NULL, NULL)

/*! @} End of doxd4d_label_macro                                              */
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



// Obsolete functions, replaced by any general
#define D4D_LabelSetText D4D_SetText




#endif /* __D4D_LABEL_H */
