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
* @file      d4d_picture.h
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.30.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver picture object header file
*
*******************************************************************************/

#ifndef __D4D_PICTURE_H
#define __D4D_PICTURE_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_pictSysFunc;

/******************************************************************************
* D4D PICTURE setting  constants
*
*//*! @addtogroup doxd4d_picture_const
* @{
*******************************************************************************/

/*! @brief This is picture init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED) as a default.*/
#ifndef D4D_PIC_F_DEFAULT
  #define D4D_PIC_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)
#endif

/*! @brief This is picture init properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_PIC_PRTY_DEFAULT
  #define D4D_PIC_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @} End of doxd4d_picture_const                                            */

/******************************************************************************
* Types
******************************************************************************/

// picture configuration (goes to ROM by default)

typedef struct
{
    const D4D_BMP*  pBmp;
} D4D_PICTURE;


/******************************************************************************
* Macros
******************************************************************************/

// getting the PICTURE structure from general OBJECT
#define D4D_GET_PICTURE(pObj) ((D4D_PICTURE*)((pObj)->pParam))

/******************************************************************************
* D4D PICTURE setting  public macros
*
*//*! @addtogroup doxd4d_picture_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Picture object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of picture object
* @param   x - coordination of picture in X axis
* @param   y - coordination of picture in Y axis
* @param   cx - size of picture in X axis (width)
* @param   cy - size of picture in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   pBmp - pointer to bitmap structure \ref D4D_BMP
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pUser - user data of  menu
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_PICTURE structure, including the object data sub structure. Is used to define all properties of picture.
*******************************************************************************/
#define _D4D_DECLARE_PICTURE(type, name, x, y, cx, cy, radius, pMargin, pRelations, pBmp, flags, pUser, pOnUsrMsg) \
    static type D4D_PICTURE name##_params = \
    { \
        pBmp        /* pBmp */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_pictSysFunc, &(name##_params), flags, pUser, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the Picture object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of picture object
* @param   x - coordination of picture in X axis
* @param   y - coordination of picture in Y axis
* @param   cx - size of picture in X axis (width)
* @param   cy - size of picture in Y axis (height) (with hidden list box)
* @param   pBmp - pointer to bitmap structure \ref D4D_BMP
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pUser - user data of  menu
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_PICTURE structure, including the object data sub structure. Is used to define all properties of picture. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_PICTURE instead of this one.
*******************************************************************************/
#define D4D_DECLARE_PICTURE(name, x, y, cx, cy, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_CONST, name, x, y, cx, cy, 0, NULL, NULL, pBmp, flags, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Picture object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of picture object
* @param   x - coordination of picture in X axis
* @param   y - coordination of picture in Y axis
* @param   cx - size of picture in X axis (width)
* @param   cy - size of picture in Y axis (height) (with hidden list box)
* @param   pBmp - pointer to bitmap structure \ref D4D_BMP

* @note    This macro create complete D4D_PICTURE structure, including the object data sub structure. Is used to define all properties of picture. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_PICTURE instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_PICTURE(name, x, y, cx, cy, pBmp) \
  D4D_DECLARE_PICTURE(name, x, y, cx, cy, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)

// Rounded button definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Picture object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of picture object
* @param   x - coordination of picture in X axis
* @param   y - coordination of picture in Y axis
* @param   cx - size of picture in X axis (width)
* @param   cy - size of picture in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   pBmp - pointer to bitmap structure \ref D4D_BMP
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pUser - user data of  menu
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_PICTURE structure, including the object data sub structure. Is used to define all properties of picture. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_PICTURE instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RPICTURE(name, x, y, cx, cy, radius, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_CONST, name, x, y, cx, cy, radius, NULL, NULL, pBmp, flags, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Picture object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of picture object
* @param   x - coordination of picture in X axis
* @param   y - coordination of picture in Y axis
* @param   cx - size of picture in X axis (width)
* @param   cy - size of picture in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   pBmp - pointer to bitmap structure \ref D4D_BMP

* @note    This macro create complete D4D_PICTURE structure, including the object data sub structure. Is used to define all properties of picture. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_PICTURE instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RPICTURE(name, x, y, cx, cy, radius, pBmp) \
  D4D_DECLARE_RPICTURE(name, x, y, cx, cy, radius, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)


//IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_PICTURE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_PICTURE_INRAM(name, x, y, cx, cy, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_NO_CONST, name, x, y, cx, cy, 0, NULL, NULL, pBmp, flags, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_PICTURE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_PICTURE_INRAM(name, x, y, cx, cy, pBmp) \
  D4D_DECLARE_PICTURE_INRAM(name, x, y, cx, cy, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)

// Rounded button definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RPICTURE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RPICTURE_INRAM(name, x, y, cx, cy, radius, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_NO_CONST, name, x, y, cx, cy, radius, NULL, NULL, pBmp, flags, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RPICTURE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RPICTURE_INRAM(name, x, y, cx, cy, radius, pBmp) \
  D4D_DECLARE_RPICTURE_INRAM(name, x, y, cx, cy, radius, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)

/*! @} End of doxd4d_picture_macro                                             */
/******************************************************************************
* Global variables
******************************************************************************/



/******************************************************************************
* Global functions
******************************************************************************/



#endif /* __D4D_PICTURE_H */
