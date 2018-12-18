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
* @file      d4d_gauge.h
*
* @author     Petr Gargulak
*
* @version   0.0.48.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver gauge object header file
*
*******************************************************************************/

#ifndef __D4D_GAUGE_H
#define __D4D_GAUGE_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_gaugeSysFunc;

/******************************************************************************
* D4D GAUGE setting  constants
*
*//*! @addtogroup doxd4d_gauge_const
* @{
*******************************************************************************/

/*! @brief This is size of gauge hub in pixels.
           If not defined, it sets to 3 pixel as a default.*/
#ifndef D4D_GAUGE_HUB_RADIUS
  #define D4D_GAUGE_HUB_RADIUS 3
#endif

/*! @brief This is default gauge hub color definition.
           If not defined, it sets to \ref D4D_COLOR_DARK_RED as a default.*/
#ifndef D4D_COLOR_GAUG_HUB
  #define D4D_COLOR_GAUG_HUB        D4D_COLOR_DARK_RED
#endif

/*! @brief This is default gauge pointer color definition.
           If not defined, it sets to \ref D4D_COLOR_DARK_BLUE as a default.*/
#ifndef D4D_COLOR_GAUG_POINTER
  #define D4D_COLOR_GAUG_POINTER    D4D_COLOR_DARK_BLUE
#endif

//********************* Gauge flags for its specific flags *****************
/*!
  @defgroup doxd4d_gauge_const_flags D4D GAUGE Defines masks of its specific behaviour flags
  This section specifies masks of gauge behaviour flags.
  @ingroup doxd4d_gauge_const
*/

/**
 * @addtogroup doxd4d_gauge_const_flags
 * @{
 */
#define D4D_GAUGE_F_REDRAW_TEXT     (0x01 << D4D_OBJECT_F_OBJECT_SHIFT) ///< This flags force the redrawing gauge text after each redraw of new value. This is usable in case that the text is placed in pointer draeing area.
#define D4D_GAUGE_F_HUB             (0x02 << D4D_OBJECT_F_OBJECT_SHIFT) ///< This flag enables draw of the HUB.
#define D4D_GAUGE_F_THICK_POINTER   (0x04 << D4D_OBJECT_F_OBJECT_SHIFT) ///< This flag switch the pointer to thick line.

/**@}*/

/*! @brief This is gauge init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_GAUGE_F_REDRAW_TEXT | \ref D4D_GAUGE_F_HUB) as a default.*/
#ifndef D4D_GAUGE_F_DEFAULT
  #define D4D_GAUGE_F_DEFAULT     (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_GAUGE_F_REDRAW_TEXT | D4D_GAUGE_F_HUB)
#endif

/*! @brief This is gauge init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_GAUGE_TXT_PRTY_DEFAULT
  #define D4D_GAUGE_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is gauge init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_GAUGE_FNT_PRTY_DEFAULT
  #define D4D_GAUGE_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @} End of doxd4d_gauge_const                                             */
/******************************************************************************
* D4D GAUGE setting  public types
*
*//*! @addtogroup doxd4d_gauge_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of gauge on change callback function
* @param   pThis - pointer to gauge instance that call the function
* @return  none
* @note    This is gauge call back function used to notify user application about
*          On Change gauge event.
*******************************************************************************/
typedef void (*D4D_GAUGE_ON_CHANGE)(D4D_OBJECT* pThis);


/*! @brief The gauge value variable, is used to hadle inputs of gauga functions API.*/
typedef sByte D4D_GAUGE_VALUE;
/*! @brief The gauge angle variable, is used to set up limits of gauge angle <0-FF == 0° - 359°>.*/
typedef Byte D4D_GAUGE_ANGLE;


/*! @brief The gauge limits structure that defines input value range and for this range specify the final angles on LCD.*/
typedef struct
{
  D4D_GAUGE_VALUE valueMin;   ///< minimal value (corresponds to angleMin)
  D4D_GAUGE_VALUE valueMax;   ///< maximal value (corresponds to angleMax)
  D4D_GAUGE_ANGLE angleMin;   ///< line angle from 0x00 to 0xFF
  D4D_GAUGE_ANGLE angleMax;   ///< line angle from 0x00 to 0xFF
} D4D_GAUGE_LIMITS;

/*! @} End of doxd4d_gauge_type                                             */

/******************************************************************************
* Private Types
******************************************************************************/

// gauge runtime data (always in RAM)

typedef struct
{
    D4D_GAUGE_VALUE value;      // current value
    D4D_GAUGE_VALUE valueLast;  // last pointer value drawn
    D4D_TREND trend;            // trend of pointer move (D4D_CLOCK_WISE or D4D_ANTI_CLOCK_WISE)
    D4D_GAUGE_LIMITS limits;    // gauge runtime configuration
} D4D_GAUGE_DATA;

// gauge configuration (in ROM by default)

typedef struct
{
    D4D_STRING textBuff;       // gauge text
    D4D_POINT txtOff;           // text coordinates as offset from scrPos
    D4D_POINT hubOff;           // hub coordinates as offset from scrPos
    D4D_COOR  pointerLen;       // lentgth of the pointer line
    const D4D_BMP*  pBmpBkgd;   // gauge background bitmap

    D4D_GAUGE_ON_CHANGE OnValueChanged;

    D4D_GAUGE_DATA* pData;

} D4D_GAUGE;

/******************************************************************************
* Macros
******************************************************************************/

// getting the GAUGE structure from general OBJECT
#define D4D_GET_GAUGE(pObj) ((D4D_GAUGE*)((pObj)->pParam))

/******************************************************************************
* D4D GAUGE setting  public macros
*
*//*! @addtogroup doxd4d_gauge_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Gauge object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of gauge object
* @param   text - title text of gauge
* @param   x - coordination of gauge in X axis
* @param   y - coordination of gauge in Y axis
* @param   cx - size of gauge in X axis (width)
* @param   cy - size of gauge in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   tx - offset of gauge text from top left corner in axis X
* @param   ty - offset of gauge text from top left corner in axis Y
* @param   kx - offset of gauge hub from top left corner in axis X
* @param   ky - offset of gauge hub from top left corner in axis Y
* @param   plen - lenght of pointer
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background of gauge (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  button
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_GAUGE_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GAUGE structure, including the object data sub structure. Is used to define all properties of gauge.
*******************************************************************************/
#define _D4D_DECLARE_GAUGE(type, name, text, x, y, cx, cy, radius, pMargin, pRelations, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
    static D4D_GAUGE_DATA name##_data = {0, 1, D4D_CLOCK_WISE, {0, 100, 0, 0x7F } }; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_GAUGE_FNT_PRTY_DEFAULT, D4D_GAUGE_TXT_PRTY_DEFAULT}; \
    static type D4D_GAUGE name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0},      /* pText */ \
        { tx, ty },     /* txtOff */ \
        { kx, ky },     /* hubOff */ \
        plen,           /* pointerLen */ \
        pBmp,           /* pBmpBkgd */ \
        onvalch,        /* OnValueChanged */ \
        &(name##_data)  /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_gaugeSysFunc, &(name##_params), flags, pUser, pScheme)


/**************************************************************************/ /*!
* @brief   Macro that create the Gauge object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of gauge object
* @param   text - title text of gauge
* @param   x - coordination of gauge in X axis
* @param   y - coordination of gauge in Y axis
* @param   cx - size of gauge in X axis (width)
* @param   cy - size of gauge in Y axis (height)
* @param   tx - offset of gauge text from top left corner in axis X
* @param   ty - offset of gauge text from top left corner in axis Y
* @param   kx - offset of gauge hub from top left corner in axis X
* @param   ky - offset of gauge hub from top left corner in axis Y
* @param   plen - lenght of pointer
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_button_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background of gauge (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  button
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_GAUGE_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GAUGE structure, including the object data sub structure. Is used to define all properties of gauge. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*******************************************************************************/
#define D4D_DECLARE_GAUGE(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Gauge object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of gauge object
* @param   text - title text of gauge
* @param   x - coordination of gauge in X axis
* @param   y - coordination of gauge in Y axis
* @param   cx - size of gauge in X axis (width)
* @param   cy - size of gauge in Y axis (height)
* @param   tx - offset of gauge text from top left corner in axis X
* @param   ty - offset of gauge text from top left corner in axis Y
* @param   kx - offset of gauge hub from top left corner in axis X
* @param   ky - offset of gauge hub from top left corner in axis Y
* @param   plen - lenght of pointer
* @param   pBmp - pointer to a bitmap that is shown in background of gauge (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_GAUGE_ON_CHANGE

* @note    This macro create complete D4D_GAUGE structure, including the object data sub structure. Is used to define all properties of gauge. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_GAUGE(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, pBmp, fontId, onvalch) \
    D4D_DECLARE_GAUGE(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

// Rounded gauge definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Gauge object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of gauge object
* @param   text - title text of gauge
* @param   x - coordination of gauge in X axis
* @param   y - coordination of gauge in Y axis
* @param   cx - size of gauge in X axis (width)
* @param   cy - size of gauge in Y axis (height)
* @param   radius - radius of corners
* @param   tx - offset of gauge text from top left corner in axis X
* @param   ty - offset of gauge text from top left corner in axis Y
* @param   kx - offset of gauge hub from top left corner in axis X
* @param   ky - offset of gauge hub from top left corner in axis Y
* @param   plen - lenght of pointer
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_button_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background of gauge (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  button
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_GAUGE_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GAUGE structure, including the object data sub structure. Is used to define all properties of gauge. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RGAUGE(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Gauge object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of gauge object
* @param   text - title text of gauge
* @param   x - coordination of gauge in X axis
* @param   y - coordination of gauge in Y axis
* @param   cx - size of gauge in X axis (width)
* @param   cy - size of gauge in Y axis (height)
* @param   radius - radius of corners
* @param   tx - offset of gauge text from top left corner in axis X
* @param   ty - offset of gauge text from top left corner in axis Y
* @param   kx - offset of gauge hub from top left corner in axis X
* @param   ky - offset of gauge hub from top left corner in axis Y
* @param   plen - lenght of pointer
* @param   pBmp - pointer to a bitmap that is shown in background of gauge (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_GAUGE_ON_CHANGE

* @note    This macro create complete D4D_GAUGE structure, including the object data sub structure. Is used to define all properties of gauge. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_BUTTON instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RGAUGE(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, pBmp, fontId, onvalch) \
    D4D_DECLARE_RGAUGE(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_GAUGE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_GAUGE_INRAM(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_NO_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_GAUGE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_GAUGE_INRAM(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, pBmp, fontId, onvalch) \
    D4D_DECLARE_GAUGE_INRAM(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

// Rounded gauge definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RGAUGE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RGAUGE_INRAM(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_NO_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RGAUGE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RGAUGE_INRAM(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, pBmp, fontId, onvalch) \
    D4D_DECLARE_RGAUGE_INRAM(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

/*! @} End of doxd4d_gauge_macro                                             */
/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/

void D4D_GaugSetValue(D4D_OBJECT_PTR pThis, D4D_GAUGE_VALUE value);
D4D_GAUGE_VALUE D4D_GaugGetValue(D4D_OBJECT_PTR pThis);

void D4D_GaugSetLimits(D4D_OBJECT_PTR pThis, const D4D_GAUGE_LIMITS* pLimits);
void D4D_GaugGetLimits(D4D_OBJECT_PTR pThis, D4D_GAUGE_LIMITS* pLimits);

void D4D_GaugSetDir(D4D_OBJECT_PTR pThis, D4D_TREND direction);
D4D_TREND D4D_GaugGetDir(D4D_OBJECT_PTR pThis);


// Obsolete functions, replaced by any general
#define D4D_GaugeSetText D4D_SetText




#endif /* __D4D_GAUGE_H */




