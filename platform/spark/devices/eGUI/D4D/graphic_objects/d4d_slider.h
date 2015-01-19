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
* @file      d4d_slider.h
*
* @author     Petr Gargulak
*
* @version   0.0.48.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver slider object header file
*
*******************************************************************************/

#ifndef __D4D_SLIDER_H
#define __D4D_SLIDER_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_sliderSysFunc;

/******************************************************************************
* D4D SLIDER setting  constants
*
*//*! @addtogroup doxd4d_slider_const
* @{
*******************************************************************************/

//********************* Slider flags for its specific flags *****************
/*!
  @defgroup doxd4d_slider_const_flags D4D SLIDER Defines masks of slider specific behaviour flags
  This section specifies masks of slider behaviour flags.
  @ingroup doxd4d_slider_const
*/

/**
 * @addtogroup doxd4d_slider_const_flags
 * @{
 */
#define D4D_SLDR_F_BAR_AUTOCOLOR  (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< This flag specifies the behavior of the slider. If it is set, the color of a slider bar depends on the current value and it is computed from a color scheme object dependent parameters fore color and barEnd from the slider declaration macro. In another situation, a color specified by the bar parameter from the color scheme object dependent parameters are used.
#define D4D_SLDR_F_TEXT_AUTOCOLOR (0x02 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< Enable text autocolor based on current value (inverted color to bar color)
#define D4D_SLDR_F_BAR_SCALECOLOR (0x04 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< This option enable little bit different type of drawing bar, the color is gradually changed from fore color to end bar color
#define D4D_SLDR_F_AUTOTOUCH_OFF  (0x08 << D4D_OBJECT_F_OBJECT_SHIFT)   ///< This option disables auto touch events to prevent fast changing of slider for longer touches

#define D4D_SLDR_F_AUTOCOLOR      (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_TEXT_AUTOCOLOR)   ///< Help macro to simplify the auto color declaration
/**@}*/

/*! @brief This is slider init flags.
  If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_TABSTOP | \ref D4D_OBJECT_F_TOUCHENABLE | \ref D4D_OBJECT_F_FOCUSRECT | \ref D4D_OBJECT_F_MOUSE_NORMAL) as a default.*/
#ifndef D4D_SLDR_F_DEFAULT
  #define D4D_SLDR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_MOUSE_NORMAL)
#endif

/*! @brief This is slider init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_SLDR_TXT_PRTY_DEFAULT
  #define D4D_SLDR_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is slider init font properties.
           If not defined, it sets to ( \ref D4D_FNT_PRTY_TRANSPARENT_YES_MASK ) as a default.*/
#ifndef D4D_SLDR_FNT_PRTY_DEFAULT
  #define D4D_SLDR_FNT_PRTY_DEFAULT  ( D4D_FNT_PRTY_TRANSPARENT_YES_MASK )
#endif

/*! @brief This is slider working bar offset in axis X to object outline.
           If not defined, it sets to 2 pixels as a default.*/
#ifndef D4D_SLDR_BAR_OFF_LENGTH
  #define D4D_SLDR_BAR_OFF_LENGTH  2
#endif

/*! @brief This is slider working bar offset in axis Y to object outline.
           If not defined, it sets to 2 pixels as a default.*/
#ifndef D4D_SLDR_BAR_OFF_WIDTH
  #define D4D_SLDR_BAR_OFF_WIDTH  4
#endif

/*! @brief This is slider timeout of auto increment for long touch (in ticks of eGUI time ticks - \ref D4D_TimeTickPut), it can be changed in runtime.
           If not defined, it sets to ( 10 ) as a default.*/
#ifndef D4D_SLDR_AUTOTOUCHTICKS
  #define D4D_SLDR_AUTOTOUCHTICKS  10
#endif


/*! @brief This is slider fore color definition.
           If not defined, it sets to \ref D4D_COLOR_DARK_BLUE as a default.*/
#ifndef D4D_COLOR_SLDR_BAR_FORE
  #define D4D_COLOR_SLDR_BAR_FORE        D4D_COLOR_DARK_BLUE
#endif

/*! @brief This is slider background color definition.
           If not defined, it sets to \ref D4D_COLOR_BRIGHT_GREY as a default.*/
#ifndef D4D_COLOR_SLDR_BAR_BCKG
  #define D4D_COLOR_SLDR_BAR_BCKG   D4D_COLOR_BRIGHT_GREY
#endif

/*! @brief This is slider start fore color definition.
           If not defined, it sets to \ref D4D_COLOR_GREEN as a default.*/
#ifndef D4D_COLOR_SLDR_BAR_START
  #define D4D_COLOR_SLDR_BAR_START  D4D_COLOR_GREEN
#endif

/*! @brief This is slider end color definition.
           If not defined, it sets to \ref D4D_COLOR_RED as a default.*/
#ifndef D4D_COLOR_SLDR_BAR_END
  #define D4D_COLOR_SLDR_BAR_END  D4D_COLOR_RED
#endif

/*! @} End of doxd4d_slider_const                                       */

/******************************************************************************
* D4D SLIDER setting  public types
*
*//*! @addtogroup doxd4d_slider_type
* @{
*******************************************************************************/

/*! @brief   Type definition of slider value type - this is standard type used for slider data input.*/
typedef sByte D4D_SLIDER_VALUE;

/*! @brief   Type definition of slider limits structure.*/
typedef struct
{
  D4D_SLIDER_VALUE valueMin;   ///< minimal value - <-128..valueMax>
  D4D_SLIDER_VALUE valueMax;   ///< maximal value - <valueMin..127>
  D4D_SLIDER_VALUE step;       ///< step value of slider change
  D4D_SLIDER_VALUE valueOrg;   ///< bar origin value (should be valueMin <= org <= valueMax)
} D4D_SLIDER_LIMITS;

/**************************************************************************/ /*!
* @brief   Type definition of slider on change callback function
* @param   pThis - pointer to slider instance that call the function
* @return  none
* @note    This is slider call back function used to notify user application about
*          On Change slider event.
*******************************************************************************/
typedef void (*D4D_SLDR_ON_CHANGE)(D4D_OBJECT* pThis);

/*! @} End of doxd4d_slider_type                                             */
/******************************************************************************
* Private Types
******************************************************************************/

// slider runtime data (always in RAM)
typedef struct
{
  D4D_SLIDER_VALUE value;      // current value
  D4D_SLIDER_LIMITS limits;    // slider runtime configuration
  D4D_COLOR colorBar;          // Color v Bar - can be changed
} D4D_SLIDER_DATA;

// slider configuration (in ROM by default)

typedef struct
{
    D4D_STRING textBuff;       // slider text
    D4D_POINT txtOff;           // text coordinates as offset from scrPos
    D4D_POINT barOff;           // hub coordinates as offset from scrPos
    D4D_SIZE  barSize;          // value bar size (if cx>cy, then bar is horizontal, otherwise vertical)
    const D4D_BMP*  pBmpBkgd;         // slider background bitmap
    D4D_SLDR_ON_CHANGE OnValueChanged;  // pointer to on change call back function
    D4D_SLIDER_DATA* pData;
} D4D_SLIDER;


/******************************************************************************
* Macros
******************************************************************************/

// getting the SLIDER structure from general OBJECT
#define D4D_GET_SLIDER(pObj) ((D4D_SLIDER*)((pObj)->pParam))

/******************************************************************************
* D4D SLIDER setting  public macros
*
*//*! @addtogroup doxd4d_slider_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Slider object structure in memory including all substructures
* @param   type - type of object <D4D_CONST; D4D_NO_CONST>
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   tx - coordination of slider text in X axis
* @param   ty - coordination of slider text in Y axis
* @param   bx - coordination of slider working bar in X axis
* @param   by - coordination of slider working bar in Y axis
* @param   bcx - size of slider working bar in X axis
* @param   bcy - size of slider working bar in Y axis
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_slider_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider.
*******************************************************************************/
#define _D4D_DECLARE_SLIDER(type, name, text, x, y, cx, cy, radius, pMargin, pRelations, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
    static D4D_SLIDER_DATA name##_data = { 0, 0, 100, 1, 0, D4D_COLOR_SLDR_BAR_FORE}; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_SLDR_FNT_PRTY_DEFAULT, D4D_SLDR_TXT_PRTY_DEFAULT}; \
    static type D4D_SLIDER name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0}, /* textBuff */ \
        { tx, ty },  /* txtOff     */ \
        { bx, by },  /* barOff     */ \
        { bcx, bcy }, /* barSize    */ \
        pBmp,        /* pBmpBkgd   */ \
        onvalch,     /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_sliderSysFunc, &(name##_params), flags, pUser, pScheme)


/**************************************************************************/ /*!
* @brief   Macro that create the Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   tx - coordination of slider text in X axis
* @param   ty - coordination of slider text in Y axis
* @param   bx - coordination of slider working bar in X axis
* @param   by - coordination of slider working bar in Y axis
* @param   bcx - size of slider working bar in X axis
* @param   bcy - size of slider working bar in Y axis
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_slider_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*******************************************************************************/
#define D4D_DECLARE_SLIDER(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER(D4D_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   tx - coordination of slider text in X axis
* @param   ty - coordination of slider text in Y axis
* @param   bx - coordination of slider working bar in X axis
* @param   by - coordination of slider working bar in Y axis
* @param   bcx - size of slider working bar in X axis
* @param   bcy - size of slider working bar in Y axis
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_SLIDER(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_SLIDER(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The internal coordination values are computed automatically to standard.
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_slider_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*******************************************************************************/
#define D4D_DECLARE_SLIDER_AUTOSIZE(name, text, x, y, cx, cy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_SLIDER(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
*               The internal coordination values are computed automatically to standard.
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_SLIDER_AUTOSIZE(name, text, x, y, cx, cy, pBmp, fontId, onvalch) \
    D4D_DECLARE_STD_SLIDER(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, pBmp, fontId, onvalch)


/**************************************************************************/ /*!
* @brief   Macro that create the rounded Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   radius - radius of corners
* @param   tx - coordination of slider text in X axis
* @param   ty - coordination of slider text in Y axis
* @param   bx - coordination of slider working bar in X axis
* @param   by - coordination of slider working bar in Y axis
* @param   bcx - size of slider working bar in X axis
* @param   bcy - size of slider working bar in Y axis
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_slider_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RSLIDER(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER(D4D_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   radius - radius of corners
* @param   tx - coordination of slider text in X axis
* @param   ty - coordination of slider text in Y axis
* @param   bx - coordination of slider working bar in X axis
* @param   by - coordination of slider working bar in Y axis
* @param   bcx - size of slider working bar in X axis
* @param   bcy - size of slider working bar in Y axis
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RSLIDER(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_RSLIDER(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The internal coordination values are computed automatically to standard.
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_slider_const_flags
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RSLIDER_AUTOSIZE(name, text, x, y, cx, cy, radius, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_RSLIDER(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Slider object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
*               The internal coordination values are computed automatically to standard.
* @param   name - name of slider object
* @param   text - title text of slider
* @param   x - coordination of slider in X axis
* @param   y - coordination of slider in Y axis
* @param   cx - size of slider in X axis (width)
* @param   cy - size of slider in Y axis (height)
* @param   radius - radius of corners
* @param   pBmp - pointer to a bitmap that is shown in background (Could be NULL)
* @param   fontId - Identification number of the used title text font
* @param   onvalch - Pointer to an on-change user callback function \ref D4D_SLDR_ON_CHANGE

* @note    This macro create complete D4D_SLIDER structure, including the object data sub structure. Is used to define all properties of slider. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_SLIDER instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RSLIDER_AUTOSIZE(name, text, x, y, cx, cy, radius, pBmp, fontId, onvalch) \
    D4D_DECLARE_STD_RSLIDER(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, pBmp, fontId, onvalch)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_SLIDER, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_SLIDER_INRAM(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER( D4D_NO_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_SLIDER, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_SLIDER_INRAM(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_SLIDER_INRAM(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_SLIDER_AUTOSIZE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_SLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_SLIDER_INRAM(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_SLIDER, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_SLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, pBmp, font, onvalch) \
    D4D_DECLARE_STD_SLIDER_INRAM(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, pBmp, font, onvalch)

// Rounded slider definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RSLIDER, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER(D4D_NO_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RSLIDER, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RSLIDER_AUTOSIZE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RSLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, radius, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RSLIDER_AUTOSIZE, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RSLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, radius, pBmp, font, onvalch) \
    D4D_DECLARE_STD_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, pBmp, font, onvalch)

/*! @} End of doxd4d_slider_macro                                          */

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/

void D4D_SldrSetValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE value);
D4D_SLIDER_VALUE D4D_SldrGetValue(D4D_OBJECT_PTR pThis);
void D4D_SldrChangeValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE incr);

void D4D_SldrSetLimits(D4D_OBJECT_PTR pThis, const D4D_SLIDER_LIMITS* pLimits);
void D4D_SldrGetLimits(D4D_OBJECT_PTR pThis, D4D_SLIDER_LIMITS* pLimits);

void D4D_SldrSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color);
D4D_COLOR D4D_SldrGetBarColor(D4D_OBJECT_PTR pThis);


// Obsolete functions, replaced by any general
#define D4D_SldrSetText D4D_SetText


#endif /* __D4D_SLIDER_H */











