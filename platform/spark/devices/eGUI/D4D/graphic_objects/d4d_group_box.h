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
* @file      d4d_group_box.h
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.9.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver groupBox object header file
*
*******************************************************************************/

#ifndef __D4D_GROUP_BOX_H
#define __D4D_GROUP_BOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_groupBoxSysFunc;

/******************************************************************************
* D4D GROUP BOX setting  constants
*
*//*! @addtogroup doxd4d_group_box_const
* @{
*******************************************************************************/

//********************* Group Box flags for its specific flags *****************
/*!
  @defgroup doxd4d_group_box_const_flags D4D GRUOP BOX Defines masks of its specific behaviour flags
  This section specifies masks of group box behaviour flags.
  @ingroup doxd4d_group_box_const
*/

/**
 * @addtogroup doxd4d_group_box_const_flags
 * @{
 */
  #define D4D_GROUP_BOX_F_CONTENT_OUTLINE            (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)        ///< Enable group box content outline

/**@}*/


/*! @brief This is group box init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_MOUSE_NORMAL |  \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_GROUP_BOX_F_CONTENT_OUTLINE) as a default.*/
#ifndef D4D_GROUP_BOX_F_DEFAULT
  #define D4D_GROUP_BOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_MOUSE_NORMAL |  D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_BEVEL_RAISED | D4D_GROUP_BOX_F_CONTENT_OUTLINE)
#endif

/*! @brief This is offset of title text in horizontal line
           If not defined, it sets to 6 pixel as a default.*/
#ifndef D4D_GROUP_BOX_TEXT_OFFSET
  #define D4D_GROUP_BOX_TEXT_OFFSET 6
#endif

/*! @brief This is margin of group box header (to keep place for title text)
           If not defined, it sets to 16 pixel as a default.*/
#ifndef D4D_GROUP_BOX_HEADER_MARGIN
  #define D4D_GROUP_BOX_HEADER_MARGIN 16
#endif

/*! @brief This is margin of group box rest frame (left, right and bottom)
           If not defined, it sets to 4 pixel as a default.*/
#ifndef D4D_GROUP_BOX_FRAME_MARGIN
  #define D4D_GROUP_BOX_FRAME_MARGIN 4
#endif

/*! @} End of doxd4d_group_box_const                                          */

/******************************************************************************
* Types
******************************************************************************/

// groupBox configuration (goes to ROM by default)

typedef struct
{
    D4D_STRING textBuff;    // group box text
} D4D_GROUP_BOX;


/******************************************************************************
* Macros
******************************************************************************/

// getting the GROUP_BOX structure from general OBJECT
#define D4D_GET_GROUP_BOX(pObj) ((D4D_GROUP_BOX*)((pObj)->pParam))

/******************************************************************************
* D4D GROUP BOX setting  public macros
*
*//*! @addtogroup doxd4d_group_box_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Group box object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of group box object
* @param   x - coordination of group box in X axis
* @param   y - coordination of group box in Y axis
* @param   cx - size of group box in X axis (width)
* @param   cy - size of group box in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   text - title text of group box
* @param   fontId - Identification number of the used title text font
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_group_box_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GROUP_BOX structure, including the object data sub structure. Is used to define all properties of group box.
*******************************************************************************/
#define _D4D_DECLARE_GROUP_BOX(type, name, x, y, cx, cy, radius, pMargin, pRelations, text, fontId, flags, pScheme, pUser, pOnUsrMsg) \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_FNT_PRTY_TRANSPARENT_NO_MASK, (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)}; \
    static type D4D_GROUP_BOX name##_params = \
    { \
        { text, sizeof(text), fontId,  &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0}, /* textBuff */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_groupBoxSysFunc, &(name##_params), flags, pUser, pScheme)



/**************************************************************************/ /*!
* @brief   Macro that create the Group box object structure in memory including all substructures with start of relation object list
* @param   name - name of group box object
* @param   x - coordination of group box in X axis
* @param   y - coordination of group box in Y axis
* @param   cx - size of group box in X axis (width)
* @param   cy - size of group box in Y axis (height)
* @param   radius - radius of corners
* @param   pParent - pointer to parent object(Could be NULL)
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   text - title text of group box
* @param   fontId - Identification number of the used title text font
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_group_box_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GROUP_BOX structure, including the object data sub structure. Is used to define all properties of group box.
           This macro also opens the object relations array for chidren obejct definition.
      The code example:
      @code

      D4D_DECLARE_GROUP_BOX_BEGIN(my_group_box, 10, 10, 120, 120, 6, &my_parent_object, &groupBox_marginDefault, D4D_DEFSTR("My Group Box"), MY_FONT, (D4D_GROUP_BOX_F_DEFAULT), NULL, NULL, NULL)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object1)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object2)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object3)
      D4D_DECLARE_GROUP_BOX_OBJECT_END
      @endcode
*******************************************************************************/
#define D4D_DECLARE_GROUP_BOX_BEGIN(name, x, y, cx, cy, radius, pParent, pMargin, text, fontId, flags, pScheme, pUser, pOnUsrMsg)\
    extern const D4D_OBJECT* const name##_pRelations[];\
    _D4D_DECLARE_GROUP_BOX(D4D_CONST, name, x, y, cx, cy, radius, pMargin, name##_pRelations, text, fontId, flags, pScheme, pUser, pOnUsrMsg)\
    D4D_DECLARE_OBJECT_RELATIONS_BEGIN(name##_pRelations, pParent)

/**************************************************************************/ /*!
* @brief   Macro that adds one child object to group box object relations table declaration.
* @param   name - pointer to the child object
* @note    This macro is used  with \ref D4D_DECLARE_GROUP_BOX_BEGIN & \ref D4D_DECLARE_GROUP_BOX_OBJECT_END to create the goup box children relations table in eGUI
* Here is an example demonstrating how to used it.
* @code
      D4D_DECLARE_GROUP_BOX_BEGIN(my_group_box, 10, 10, 120, 120, 6, &my_parent_object, &groupBox_marginDefault, D4D_DEFSTR("My Group Box"), MY_FONT, (D4D_GROUP_BOX_F_DEFAULT), NULL, NULL, NULL)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object1)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object2)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object3)
      D4D_DECLARE_GROUP_BOX_OBJECT_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_GROUP_BOX_OBJECT(name)  D4D_DECLARE_OBJECT_RELATIONS_CHILD(name)

/**************************************************************************/ /*!
* @brief   Macro that ends children group box object relations table declaration.
* @note    This macro is used  with \ref D4D_DECLARE_GROUP_BOX_BEGIN & \ref D4D_DECLARE_GROUP_BOX_OBJECT to create the goup box children relations table in eGUI
* Here is an example demonstrating how to used it.
* @code
      D4D_DECLARE_GROUP_BOX_BEGIN(my_group_box, 10, 10, 120, 120, 6, &my_parent_object, &groupBox_marginDefault, D4D_DEFSTR("My Group Box"), MY_FONT, (D4D_GROUP_BOX_F_DEFAULT), NULL, NULL, NULL)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object1)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object2)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object3)
      D4D_DECLARE_GROUP_BOX_OBJECT_END
* @endcode
*******************************************************************************/
#define D4D_DECLARE_GROUP_BOX_OBJECT_END    D4D_DECLARE_OBJECT_RELATIONS_END

/**************************************************************************/ /*!
* @brief   Macro that create the Group box object structure in memory including all substructures with start of relation object list
*               The missing parameters are replaced by default values.
* @param   name - name of group box object
* @param   x - coordination of group box in X axis
* @param   y - coordination of group box in Y axis
* @param   cx - size of group box in X axis (width)
* @param   cy - size of group box in Y axis (height)
* @param   radius - radius of corners
* @param   pParent - pointer to parent object(Could be NULL)
* @param   text - title text of group box
* @param   fontId - Identification number of the used title text font

* @note    This macro create complete D4D_GROUP_BOX structure, including the object data sub structure. Is used to define all properties of group box.
*          This macro also opens the object relations array for chidren obejct definition.If there is missing parameter that is needed by user application
*          used the full macro \ref _D4D_DECLARE_GROUP_BOX instead of this one. The main advantage is less parameters of this macro against the full version.
      The code example:
      @code

      D4D_DECLARE_STD_GROUP_BOX_BEGIN(my_group_box, 10, 10, 120, 120, 6, &my_parent_object, D4D_DEFSTR("My Group Box"), MY_FONT)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object1)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object2)
        D4D_DECLARE_GROUP_BOX_OBJECT(&my_child_object3)
      D4D_DECLARE_GROUP_BOX_OBJECT_END
      @endcode
*******************************************************************************/
#define D4D_DECLARE_STD_GROUP_BOX_BEGIN(name, x, y, cx, cy, radius, pParent, text, fontId) \
  D4D_DECLARE_GROUP_BOX_BEGIN(name, x, y, cx, cy, radius, pParent, &groupBox_marginDefault, text, fontId, (D4D_GROUP_BOX_F_DEFAULT), NULL, NULL, NULL)


//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_GROUP_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_GROUP_BOX_BEGIN_INRAM(name, x, y, cx, cy, radius, pParent, pMargin, text, fontId, flags, pScheme, pUser, pOnUsrMsg)\
    extern const D4D_OBJECT* const name##_pRelations[];\
    _D4D_DECLARE_GROUP_BOX(D4D_NO_CONST, name, x, y, cx, cy, radius, pMargin, name##_pRelations, text, fontId, flags, pScheme, pUser, pOnUsrMsg)\
    D4D_DECLARE_OBJECT_RELATIONS_BEGIN(name##_pRelations, pParent)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_GROUP_BOX_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_GROUP_BOX_BEGIN_INRAM(name, x, y, cx, cy, radius, pParent, text, fontId) \
  D4D_DECLARE_GROUP_BOX_BEGIN_INRAM(name, x, y, cx, cy, radius, pParent, &groupBox_marginDefault, text, fontId, (D4D_GROUP_BOX_F_DEFAULT), NULL,  NULL, NULL)

/*! @} End of doxd4d_group_box_macro                                             */

/******************************************************************************
* Global variables
******************************************************************************/

extern D4D_MARGIN groupBox_marginDefault;

/******************************************************************************
* Global functions
******************************************************************************/



#endif /* __D4D_GROUP_BOX_H */
