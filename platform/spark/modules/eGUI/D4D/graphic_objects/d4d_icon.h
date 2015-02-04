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
* @file      d4d_icon.h
*
* @author     Petr Gargulak
*
* @version   0.0.46.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver icon object header file
*
*******************************************************************************/

#ifndef __D4D_ICON_H
#define __D4D_ICON_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_iconSysFunc;

/******************************************************************************
* D4D ICON setting  constants
*
*//*! @addtogroup doxd4d_icon_const
* @{
*******************************************************************************/

/*! @brief This is icon init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED) as a default.*/
#ifndef D4D_ICON_F_DEFAULT
  #define D4D_ICON_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)
#endif

/*! @brief This is icon init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_ICON_TXT_PRTY_DEFAULT
  #define D4D_ICON_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is icon init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_ICON_FNT_PRTY_DEFAULT
  #define D4D_ICON_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is icon enable code for icon animation support
           If not defined, it sets to D4D_TRUE as a default.*/
#ifndef D4D_ICON_ENABLE_ANIMATION
  #define D4D_ICON_ENABLE_ANIMATION D4D_TRUE
#endif

/*! @brief This is icon animation time tick timeout (in ticks of eGUI time ticks - \ref D4D_TimeTickPut), it can be changed in runtime.
           If not defined, it sets to ( 4 ) as a default.*/
#ifndef D4D_ICON_ANIMATION_TICK_COUNTER
  #define D4D_ICON_ANIMATION_TICK_COUNTER  ( 4 )
#endif


/*! @} End of doxd4d_icon_const                                             */

/******************************************************************************
* D4D ICON setting  public types
*
*//*! @addtogroup doxd4d_icon_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Type definition of icon on change callback function
* @param   pThis - pointer to icon instance that call the function
* @return  none
* @note    This is icon call back function used to notify user application about
*          On Change icon event.
*******************************************************************************/
typedef void (*D4D_ICON_ON_CHANGE)(D4D_OBJECT* pThis);

/*! @brief This is icon index variable, it used in API functions to choose the active icon in the list.*/
typedef Byte D4D_ICON_INDEX;

/*! @} End of doxd4d_icon_type                                             */


/******************************************************************************
* Private Types
******************************************************************************/



typedef D4D_BMP D4D_ICON_BMPS;

typedef struct
{
    D4D_ICON_INDEX index;      // current value

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE
    D4D_BOOL animationEnabled;
    Word tickCounter;
    Word tickCounterTrshld;
#endif

} D4D_ICON_DATA;

// ICON configuration (in ROM by default)

typedef struct
{
    D4D_STRING textBuff;       // ICON text
    D4D_POINT txtOff;           // text coordinates as offset from scrPos
    const D4D_ICON_BMPS* const*  pBmpX;           // ICON state X idication bitmaps
    D4D_ICON_ON_CHANGE  OnValueChanged;
    D4D_ICON_DATA* pData;
} D4D_ICON;

/******************************************************************************
* Macros
******************************************************************************/

// getting the ICON structure from general OBJECT
#define D4D_GET_ICON(pObj) ((D4D_ICON*)((pObj)->pParam))

/******************************************************************************
* D4D ICON setting  public macros
*
*//*! @addtogroup doxd4d_icon_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Icon object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of icon object
* @param   text - title text of icon
* @param   x - coordination of icon in X axis
* @param   y - coordination of icon in Y axis
* @param   cx - size of icon in X axis (width)
* @param   cy - size of icon in Y axis (height)
* @param   tx - offset of icon text from top left corner in axis X
* @param   ty - offset of icon text from top left corner in axis Y
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_ICON_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_ICON structure, including the object data sub structure. Is used to define all properties of icon.
The code example:
      @code

      _D4D_DECLARE_ICON_BEGIN(D4D_CONST, my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, 6, NULL, NULL, (D4D_ICON_F_DEFAULT), NULL, MY_FONT, NULL, NULL, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
      @endcode
*******************************************************************************/
#define _D4D_DECLARE_ICON_BEGIN(type, name, text, x, y, cx, cy, tx, ty, radius, pMargin, pRelations, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
    extern const D4D_ICON_BMPS*  const name##_bmps[];\
    static D4D_ICON_DATA name##_data; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_ICON_FNT_PRTY_DEFAULT, D4D_ICON_TXT_PRTY_DEFAULT}; \
    static type D4D_ICON name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0}, /* textBuff */ \
        { tx, ty },   /* txtOff     */ \
        &(name##_bmps[0]),  /* pBmpX     */ \
        pOnValch,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_iconSysFunc, &(name##_params), (flags), pUser, pScheme)\
    \
    const D4D_ICON_BMPS*   const name##_bmps[] = \
    {

/**************************************************************************/ /*!
* @brief   Macro that adds one bitmap to icon object list
* @param   pBmp - pointer to the bitmap
* @note    This macro is used  with \ref D4D_DECLARE_ICON_BEGIN & \ref D4D_DECLARE_ICON_END to create the complete ICON object declaration
* Here is an example demonstrating how to used it.
* @code
      _D4D_DECLARE_ICON_BEGIN(D4D_CONST, my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, 6, NULL, NULL, (D4D_ICON_F_DEFAULT), NULL, MY_FONT, NULL, NULL, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_ICON_BMP(pBmp) (pBmp),

/**************************************************************************/ /*!
* @brief   Macro that ends the icon object declaration
* @note    This macro is used  with \ref D4D_DECLARE_ICON_BEGIN & \ref D4D_DECLARE_ICON_BMP to create the complete ICON object declaration
* Here is an example demonstrating how to used it.
* @code
      _D4D_DECLARE_ICON_BEGIN(D4D_CONST, my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, 6, NULL, NULL, (D4D_ICON_F_DEFAULT), NULL, MY_FONT, NULL, NULL, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_ICON_END() NULL };

/**************************************************************************/ /*!
* @brief   Macro that create the Icon object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of icon object
* @param   text - title text of icon
* @param   x - coordination of icon in X axis
* @param   y - coordination of icon in Y axis
* @param   cx - size of icon in X axis (width)
* @param   cy - size of icon in Y axis (height)
* @param   tx - offset of icon text from top left corner in axis X
* @param   ty - offset of icon text from top left corner in axis Y
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_ICON_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_ICON structure, including the object data sub structure. Is used to define all properties of icon.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_ICON_BEGIN instead of this one.
The code example:
      @code

      D4D_DECLARE_ICON_BEGIN(my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, (D4D_ICON_F_DEFAULT), NULL, MY_FONT, NULL, NULL, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
      @endcode
*******************************************************************************/
#define D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_CONST, name, text, x, y, cx, cy, tx, ty, 0, NULL, NULL, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Icon object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of icon object
* @param   text - title text of icon
* @param   x - coordination of icon in X axis
* @param   y - coordination of icon in Y axis
* @param   cx - size of icon in X axis (width)
* @param   cy - size of icon in Y axis (height)
* @param   tx - offset of icon text from top left corner in axis X
* @param   ty - offset of icon text from top left corner in axis Y
* @param   fontId - Identification number of the used title text font
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_ICON_ON_CHANGE

* @note    This macro create complete D4D_ICON structure, including the object data sub structure. Is used to define all properties of icon.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_ICON_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
The code example:
      @code

      D4D_DECLARE_STD_ICON_BEGIN(my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, MY_FONT, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
      @endcode
*******************************************************************************/
#define D4D_DECLARE_STD_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, fontId, pOnValch) \
  D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, pOnValch, NULL)


// Rounded icon definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Icon object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of icon object
* @param   text - title text of icon
* @param   x - coordination of icon in X axis
* @param   y - coordination of icon in Y axis
* @param   cx - size of icon in X axis (width)
* @param   cy - size of icon in Y axis (height)
* @param   tx - offset of icon text from top left corner in axis X
* @param   ty - offset of icon text from top left corner in axis Y
* @param   radius - radius of corners
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object
* @param   fontId - Identification number of the used title text font
* @param   pUser - user data of  object
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_ICON_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_ICON structure, including the object data sub structure. Is used to define all properties of icon.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_ICON_BEGIN instead of this one.
The code example:
      @code

      D4D_DECLARE_RICON_BEGIN(my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, 6, (D4D_ICON_F_DEFAULT), NULL, MY_FONT, NULL, NULL, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
      @endcode
*******************************************************************************/
#define D4D_DECLARE_RICON_BEGIN(name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_CONST, name, text, x, y, cx, cy, tx, ty, radius, NULL, NULL, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Icon object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of icon object
* @param   text - title text of icon
* @param   x - coordination of icon in X axis
* @param   y - coordination of icon in Y axis
* @param   cx - size of icon in X axis (width)
* @param   cy - size of icon in Y axis (height)
* @param   tx - offset of icon text from top left corner in axis X
* @param   ty - offset of icon text from top left corner in axis Y
* @param   radius - radius of corners
* @param   fontId - Identification number of the used title text font
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_ICON_ON_CHANGE

* @note    This macro create complete D4D_ICON structure, including the object data sub structure. Is used to define all properties of icon.If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_ICON_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
The code example:
      @code

      D4D_DECLARE_STD_RICON_BEGIN(my_icon, D4D_DEFSTR("My Icon"), 10, 10, 120, 120, 100, 100, 6, MY_FONT, NULL)
        D4D_DECLARE_ICON_BMP(&my_IconBmp1)
        D4D_DECLARE_ICON_BMP(&my_IconBmp2)
        D4D_DECLARE_ICON_BMP(&my_IconBmp3)
      D4D_DECLARE_ICON_END()
      @endcode
*******************************************************************************/
#define D4D_DECLARE_STD_RICON_BEGIN(name, text, x, y, cx, cy, tx, ty, radius, fontId, pOnValch) \
  D4D_DECLARE_RICON_BEGIN(name, text, x, y, cx, cy, tx, ty, radius, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, pOnValch, NULL)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_ICON_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, tx, ty, 0, NULL, NULL, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_ICON_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, fontId, onvalch) \
  D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, onvalch, NULL)


// Rounded icon definition
/**************************************************************************/ /*!
* @brief   Same as \ref _D4D_DECLARE_ICON_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, tx, ty, radius, NULL, NULL, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RICON_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, radius, fontId, onvalch) \
  D4D_DECLARE_RICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, radius, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, onvalch, NULL)

/*! @} End of doxd4d_icon_macro                                             */

    // Obsolete macros
#define D4D_DECLARE_STD_ICON1(name, text, x, y, cx, cy, tx, ty, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_END()

#define D4D_DECLARE_STD_ICON2(name, text, x, y, cx, cy, tx, ty, pBmp1, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_BMP((pBmp1))\
    D4D_DECLARE_ICON_END()

#define D4D_DECLARE_STD_ICON1_INRAM(name, text, x, y, cx, cy, tx, ty, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_END()

#define D4D_DECLARE_STD_ICON2_INRAM(name, text, x, y, cx, cy, tx, ty, pBmp1, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_BMP((pBmp1))\
    D4D_DECLARE_ICON_END()

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
void D4D_IconSetIndex(D4D_OBJECT_PTR pThis, D4D_ICON_INDEX index);
void D4D_IconChangeIndex(D4D_OBJECT_PTR pThis, sByte incr);
D4D_ICON_INDEX D4D_IconGetIndex(D4D_OBJECT_PTR pThis);

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE
  void D4D_IconAnimationStart(D4D_OBJECT_PTR pThis);
  void D4D_IconAnimationStop(D4D_OBJECT_PTR pThis);
  void D4D_IconAnimationSetCounts(D4D_OBJECT_PTR pThis, Word cnt);
#endif

D4D_ICON_INDEX D4D_IconGetBmpCount(D4D_OBJECT_PTR pThis);
// Obsolete functions, replaced by any general
#define D4D_IconSetText D4D_SetText




#endif /* __D4D_ICON_H */


