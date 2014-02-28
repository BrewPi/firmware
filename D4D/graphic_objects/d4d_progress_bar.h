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
* @file      d4d_progress_bar.h
*
* @author     Petr Gargulak
*
* @version   0.0.6.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver progress_bar object header file
*
*******************************************************************************/

#ifndef __D4D_PROGRESS_BAR_H
#define __D4D_PROGRESS_BAR_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_progress_barSysFunc;
/******************************************************************************
* D4D PROGRESS BAR setting  constants
*
*//*! @addtogroup doxd4d_progress_bar_const
* @{
*******************************************************************************/

//********************* Progress Bar flags for button specific flags *****************
/*!
  @defgroup doxd4d_progress_bar_const_flags D4D PROGRESS BAR Defines masks of progress bar specific behaviour flags
  This section specifies masks of progress bar behaviour flags.
  @ingroup doxd4d_progress_bar_const
*/

/**
 * @addtogroup doxd4d_progress_bar_const_flags
 * @{
 */
  #define D4D_PRGRS_BAR_F_BAR_AUTOCOLOR 0x0100          ///< This flag specifies the behavior of the progress bar. If it is set, the color of a progress bar depends on the current value and it is computed from a color scheme object dependent parameters fore color and barEnd from the progress bar declaration macro. In another situation, a color specified by the bar parameter from the color scheme object dependent parameters are used.
  #define D4D_PRGRS_BAR_F_TEXT_AUTOCOLOR 0x0200         ///< Enable text autocolor based on current value (inverted color to bar color)
  #define D4D_PRGRS_BAR_F_BAR_SCALECOLOR 0x0400         ///< This option enable little bit different type of drawing bar, the color is gradually changed from fore color to end bar color

  #define D4D_PRGRS_BAR_F_AUTOCOLOR                     (D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_TEXT_AUTOCOLOR) ///< Help macro to simplify the auto color declaration
/**@}*/

/*! @brief This is progress bar init flags.
  If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | \ref D4D_PRGRS_BAR_F_BAR_SCALECOLOR) as a default.*/
#ifndef D4D_PRGRS_BAR_F_DEFAULT
  #define D4D_PRGRS_BAR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_BAR_SCALECOLOR)
#endif

/*! @brief This is offset value of active work area of progress bar.
  If not defined, it sets to 5 pixels as a default.*/
#ifndef D4D_PRGRS_BAR_BAR_OFF
  #define D4D_PRGRS_BAR_BAR_OFF  3
#endif

/*! @brief This is progress bar fore color definition.
           If not defined, it sets to \ref D4D_COLOR_DARK_GREEN as a default.*/
#ifndef D4D_COLOR_PRGRS_BAR_BAR_FORE
  #define D4D_COLOR_PRGRS_BAR_BAR_FORE    D4D_COLOR_DARK_GREEN
#endif

/*! @brief This is progress bar end color definition.
           If not defined, it sets to \ref D4D_COLOR_YELLOW as a default.*/
#ifndef D4D_COLOR_PRGRS_BAR_BAR_END
  #define D4D_COLOR_PRGRS_BAR_BAR_END  D4D_COLOR_YELLOW
#endif

/*! @brief This is progress bar background color definition.
           If not defined, it sets to \ref D4D_COLOR_BRIGHT_GREY as a default.*/
#ifndef D4D_COLOR_PRGRS_BAR_BAR_BCKG
  #define D4D_COLOR_PRGRS_BAR_BAR_BCKG   D4D_COLOR_BRIGHT_GREY
#endif

/*! @brief This is progress bar maximal value definition.
           If not defined, it sets to 100 (to reflect standard percentual view) as a default.*/
#ifndef D4D_COLOR_PRGRS_BAR_MAX_VAL
  #define D4D_COLOR_PRGRS_BAR_MAX_VAL  100
#endif

/*! @} End of doxd4d_progress_bar_const                                       */


/******************************************************************************
* D4D PROGRESS_BAR setting  public types
*
*//*! @addtogroup doxd4d_progress_bar_type
* @{
*******************************************************************************/

/*! @brief   Type definition of progress bar value type - this is standard type used for progress bar data input.*/
typedef Byte D4D_PROGRESS_BAR_VALUE;

/*! @} End of doxd4d_progress_bar_type                                             */
/******************************************************************************
* Private Types
******************************************************************************/


// progress_bar runtime data (always in RAM)

typedef struct
{
    D4D_PROGRESS_BAR_VALUE value;       // current value
    D4D_BOOL drawActiveOnly;    // old Value
    D4D_COLOR colorBar;                 // Color v Bar - can be changed
} D4D_PROGRESS_BAR_DATA;

// progress_bar configuration (in ROM by default)

typedef struct
{
    D4D_PROGRESS_BAR_DATA* pData;
} D4D_PROGRESS_BAR;


/******************************************************************************
* Macros
******************************************************************************/

// getting the PROGRESS_BAR structure from general OBJECT
#define D4D_GET_PROGRESS_BAR(pObj) ((D4D_PROGRESS_BAR*)((pObj)->pParam))

/******************************************************************************
* D4D PROGRESS BAR setting  public macros
*
*//*! @addtogroup doxd4d_progress_bar_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Progress bar object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of progress bar object
* @param   x - coordination of progress bar in X axis
* @param   y - coordination of progress bar in Y axis
* @param   cx - size of progress bar in X axis (width)
* @param   cy - size of progress bar in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_PROGRESS_BAR structure, including the object data sub structure. Is used to define all properties of progress bar.
*******************************************************************************/
#define _D4D_DECLARE_PROGRESS_BAR(type, name, x, y, cx, cy, radius, pMargin, pRelations, flags, pScheme, pUser, pOnUsrMsg) \
    static D4D_PROGRESS_BAR_DATA name##_data = { 0, 0, D4D_COLOR_PRGRS_BAR_BAR_FORE}; \
    static type D4D_PROGRESS_BAR name##_params = \
    { \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_progress_barSysFunc, &(name##_params), flags, pUser, pScheme)


/**************************************************************************/ /*!
* @brief   Macro that create the Progress bar object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of progress bar object
* @param   x - coordination of progress bar in X axis
* @param   y - coordination of progress bar in Y axis
* @param   cx - size of progress bar in X axis (width)
* @param   cy - size of progress bar in Y axis (height) (with hidden list box)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   pUser - user data of  object
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_PROGRESS_BAR structure, including the object data sub structure. Is used to define all properties of progress bar. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_PROGRESS_BAR instead of this one.
*******************************************************************************/
#define D4D_DECLARE_PROGRESS_BAR(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg) \
  _D4D_DECLARE_PROGRESS_BAR(D4D_CONST, name, x, y, cx, cy, radius, NULL, NULL, flags, pScheme, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Progress bar object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of progress bar object
* @param   x - coordination of progress bar in X axis
* @param   y - coordination of progress bar in Y axis
* @param   cx - size of progress bar in X axis (width)
* @param   cy - size of progress bar in Y axis (height) (with hidden list box)
* @param   radius - radius of corners

* @note    This macro create complete D4D_PROGRESS_BAR structure, including the object data sub structure. Is used to define all properties of progress bar. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_PROGRESS_BAR instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_PROGRESS_BAR(name, x, y, cx, cy, radius) \
    D4D_DECLARE_PROGRESS_BAR(name, x, y, cx, cy, radius, (D4D_PRGRS_BAR_F_DEFAULT), NULL, NULL, NULL)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_PROGRESS_BAR, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_PROGRESS_BAR_INRAM(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg) \
  _D4D_DECLARE_PROGRESS_BAR( D4D_NO_CONST, name, x, y, cx, cy, radius, NULL, NULL, flags, pScheme, pUser, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_PROGRESS_BAR, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_PROGRESS_BAR_INRAM(name, x, y, cx, cy, radius) \
    D4D_DECLARE_PROGRESS_BAR_INRAM(name, x, y, cx, cy, radius, (D4D_PRGRS_BAR_F_DEFAULT), NULL, NULL, NULL)

/*! @} End of doxd4d_progress_bar_macro                                             */

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/

void D4D_PrgrsBarSetValue(D4D_OBJECT_PTR pThis, D4D_PROGRESS_BAR_VALUE value);
D4D_PROGRESS_BAR_VALUE D4D_PrgrsBarGetValue(D4D_OBJECT_PTR pThis);
void D4D_PrgrsBarChangeValue(D4D_OBJECT_PTR pThis, D4D_PROGRESS_BAR_VALUE incr);

void D4D_PrgrsBarSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color);
D4D_COLOR D4D_PrgrsBarGetBarColor(D4D_OBJECT_PTR pThis);



#endif /* __D4D_PROGRESS_BAR_H */











