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
* @file      d4d_scroll_bar.h
*
* @author     Petr Gargulak
*
* @version   0.0.11.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_SCROLL_BAR_H
#define __D4D_SCROLL_BAR_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_scroll_barSysFunc;

/******************************************************************************
* D4D SCROLL BAR setting  constants
*
*//*! @addtogroup doxd4d_scroll_bar_const
* @{
*******************************************************************************/

/*! @brief This is scroll bar init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_MOUSE_NORMAL | \ref D4D_OBJECT_F_FOCUSRECT) as a default.*/
#ifndef D4D_SCRLBR_F_DEFAULT
  #define D4D_SCRLBR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_FOCUSRECT)
#endif

/*! @brief This is minimal width of scroll bar in pixels.
           If not defined, it sets to 4 pixel as a default.*/
#ifndef D4D_SCRLBAR_MIN_TAB_SIZE
  #define D4D_SCRLBAR_MIN_TAB_SIZE  ( 4 )
#endif

/*! @} End of doxd4d_scroll_bar_const                                         */

/******************************************************************************
* D4D SCROLL BAR setting  public types
*
*//*! @addtogroup doxd4d_scroll_bar_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of scroll bar on change callback function
* @param   pThis - pointer to scroll bar instance that call the function
* @param   old_position - the old position of the scroll bar before change
* @param   new_position - the new position of the scroll bar after change
* @return  none
* @note    This is scroll bar call back function used to notify user application about
*          On Change scroll bar event.
*******************************************************************************/
typedef void (*D4D_SCRLBR_ON_CHANGE)(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);


/*! @} End of doxd4d_scroll_bar_type                                        */

/******************************************************************************
* Types
******************************************************************************/



typedef struct D4D_SCRLBAR_DATA_S
{
	D4D_INDEX position;								// index of last line
	D4D_INDEX minimum;
	D4D_INDEX maximum;
	D4D_INDEX step;
	D4D_INDEX page;
}D4D_SCRLBAR_DATA;

// label configuration (goes to ROM by default)

typedef struct
{
    D4D_SCRLBR_ON_CHANGE OnChange;
    D4D_SCRLBAR_DATA* pData;		 // Run time data needed for ScrlBr
} D4D_SCROLL_BAR;


/******************************************************************************
* Macros
******************************************************************************/

// getting the SCROLL_BAR structure from general OBJECT
#define D4D_GET_SCROLL_BAR(pObj) ((D4D_SCROLL_BAR*)((pObj)->pParam))
#define D4D_GET_SCROLL_BAR_DATA(pObj) (D4D_GET_SCROLL_BAR(pObj)->pData)

/******************************************************************************
* D4D SCROLL BAR setting  public macros
*
*//*! @addtogroup doxd4d_scroll_bar_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Scroll bar object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of scroll bar object
* @param   x - coordination of scroll bar in X axis
* @param   y - coordination of scroll bar in Y axis
* @param   cx - size of scroll bar in X axis (width)
* @param   cy - size of scroll bar in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_check_box_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw scroll bar
* @param   pUser - user data of  scroll bar
* @param   pOnChange - Pointer to an on-change user callback function \ref D4D_SCRLBR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SCROLL_BAR structure, including the object data sub structure. Is used to define all properties of scroll bar.
*******************************************************************************/
#define _D4D_DECLARE_SCROLL_BAR(type, name, x, y, cx, cy, radius, pMargin, pRelations, flags, pScheme, pUser, pOnChange, pOnUsrMsg) \
    static D4D_SCRLBAR_DATA name##_data = { 0, 0, 100, 1, 0};\
    static type D4D_SCROLL_BAR name##_params = \
    { \
        pOnChange, /* OnChange*/ \
        &name##_data, /* pData */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_scroll_barSysFunc, &(name##_params), flags, pUser, pScheme)


/**************************************************************************/ /*!
* @brief   Macro that create the Scroll bar object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of scroll bar object
* @param   x - coordination of scroll bar in X axis
* @param   y - coordination of scroll bar in Y axis
* @param   cx - size of scroll bar in X axis (width)
* @param   cy - size of scroll bar in Y axis (height)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_check_box_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw scroll bar
* @param   pUser - user data of  scroll bar
* @param   pOnChange - Pointer to an on-change user callback function \ref D4D_SCRLBR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SCROLL_BAR structure, including the object data sub structure. Is used to define all properties of scroll bar. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SCROLL_BAR instead of this one.
*******************************************************************************/
#define D4D_DECLARE_SCROLL_BAR(name, x, y, cx, cy, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_CONST, name, x, y, cx, cy, 0, NULL, NULL, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Scroll bar object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of scroll bar object
* @param   x - coordination of scroll bar in X axis
* @param   y - coordination of scroll bar in Y axis
* @param   cx - size of scroll bar in X axis (width)
* @param   cy - size of scroll bar in Y axis (height)
* @param   pOnChange - Pointer to an on-change user callback function \ref D4D_SCRLBR_ON_CHANGE

* @note    This macro create complete D4D_SCROLL_BAR structure, including the object data sub structure. Is used to define all properties of scroll bar. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SCROLL_BAR instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_SCROLL_BAR(name, x, y, cx, cy, pOnChange)\
        D4D_DECLARE_SCROLL_BAR(name, x, y, cx, cy, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)

// Rounded scroll bar definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Scroll bar object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of scroll bar object
* @param   x - coordination of scroll bar in X axis
* @param   y - coordination of scroll bar in Y axis
* @param   cx - size of scroll bar in X axis (width)
* @param   cy - size of scroll bar in Y axis (height)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_check_box_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw scroll bar
* @param   pUser - user data of  scroll bar
* @param   pOnChange - Pointer to an on-change user callback function \ref D4D_SCRLBR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SCROLL_BAR structure, including the object data sub structure. Is used to define all properties of scroll bar. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SCROLL_BAR instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RSCROLL_BAR(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_CONST, name, x, y, cx, cy, radius, NULL, NULL, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Scroll bar object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of scroll bar object
* @param   x - coordination of scroll bar in X axis
* @param   y - coordination of scroll bar in Y axis
* @param   cx - size of scroll bar in X axis (width)
* @param   cy - size of scroll bar in Y axis (height)
* @param   radius - radius of corners
* @param   pOnChange - Pointer to an on-change user callback function \ref D4D_SCRLBR_ON_CHANGE

* @note    This macro create complete D4D_SCROLL_BAR structure, including the object data sub structure. Is used to define all properties of scroll bar. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SCROLL_BAR instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RSCROLL_BAR(name, x, y, cx, cy, radius, pOnChange)\
        D4D_DECLARE_RSCROLL_BAR(name, x, y, cx, cy, radius, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_SCROLL_BAR, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_SCROLL_BAR_INRAM(name, x, y, cx, cy, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_NO_CONST, name, x, y, cx, cy, 0, NULL, NULL, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_SCROLL_BAR, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_SCROLL_BAR_INRAM(name, x, y, cx, cy, pOnChange)\
        D4D_DECLARE_SCROLL_BAR_INRAM(name, x, y, cx, cy, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)


// Rounded scroll bar definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RSCROLL_BAR, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RSCROLL_BAR_INRAM(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_NO_CONST, name, x, y, cx, cy, radius, NULL, NULL, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RSCROLL_BAR, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RSCROLL_BAR_INRAM(name, x, y, cx, cy, radius, pOnChange)\
        D4D_DECLARE_RSCROLL_BAR_INRAM(name, x, y, cx, cy, radius, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)

/*! @} End of doxd4d_scroll_bar_macro                                          */

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

void D4D_ScrlBrSetRange(D4D_OBJECT_PTR pObj, D4D_INDEX minimum, D4D_INDEX maximum);
void D4D_ScrlBrSetStep(D4D_OBJECT_PTR pObj, D4D_INDEX page, D4D_INDEX step);
void D4D_ScrlBrSetPosition(D4D_OBJECT_PTR pObj, D4D_INDEX position);
D4D_INDEX D4D_ScrlBrGetPosition(D4D_OBJECT_PTR pObj);
void D4D_ScrlBrChangePosition(D4D_OBJECT_PTR pObj, D4D_INDEX_DELTA change);

#endif /* __D4D_SCROLL_BAR_H */
