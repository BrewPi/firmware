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
* @file      d4d_screen.h
*
* @author     Petr Gargulak
*
* @version   0.0.38.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver screen header file
*
******************************************************************************/

#ifndef __D4D_SCREEN_H
#define __D4D_SCREEN_H

/******************************************************************************
* D4D SCREEN setting  constants
*
*//*! @addtogroup doxd4d_screen_const
* @{
*******************************************************************************/

/*! @brief This is screen title offset definition in axis X.
           If not defined, it sets to 3 pixels as a default.*/
#ifndef D4D_SCR_TITLE_OFF_X
  #define D4D_SCR_TITLE_OFF_X  3
#endif

/*! @brief This is screen title offset definition in axis Y.
           If not defined, it sets to 1 pixels as a default.*/
#ifndef D4D_SCR_TITLE_OFF_Y
  #define D4D_SCR_TITLE_OFF_Y  1
#endif

/*! @brief This is screen header minimal size definition in axis Y.
           If not defined, it sets to 13 pixels as a default.*/
#ifndef D4D_SCR_HEADER_SIZE_MIN_SIZE
  #define D4D_SCR_HEADER_SIZE_MIN_SIZE  13
#endif

/*! @brief This is screen exit button minimal size definition.
           If not defined, it sets to 6 pixels as a default.*/
#ifndef D4D_SCR_TITLE_EXITBTN_MIN_SIZE
  #define D4D_SCR_TITLE_EXITBTN_MIN_SIZE  6
#endif

/*! @brief This is screen exit button cross minimal size definition.
           If not defined, it sets to 2 pixels as a default.*/
#ifndef D4D_SCR_EXITBTN_CROSS_SIZE
  #define D4D_SCR_EXITBTN_CROSS_SIZE 2
#endif

/*! @brief This is screen exit button offset definition.
           If not defined, it sets to 3 pixels as a default.*/
#ifndef D4D_SCR_TITLE_EXITBTN_OFFSET
  #define D4D_SCR_TITLE_EXITBTN_OFFSET  3
#endif

// Common flags as for initial as for runtime
/*!
  @defgroup doxd4d_screen_const_flags D4D SCREEN Defines masks of screen behaviour flags
  This section specifies masks of screen behaviour flags.
  @ingroup doxd4d_screen_const
*/

/**
 * @addtogroup doxd4d_screen_const_flags
 * @{
 */
#define D4D_SCR_F_OUTLINE       (0x01)  ///< The screen has out line
#define D4D_SCR_F_BEVEL         (0x02)  ///< The screen has bevel
#define D4D_SCR_F_TITLEBAR      (0x04)  ///< The screen has title bar
#define D4D_SCR_F_EXIT          (0x08)  ///< The screen has exit button in title bar
#define D4D_SCR_F_BCKG          (0x10)  ///< The screen has filled background (desktop)

#define D4D_SCR_F_TOUCHENABLE           (D4D_SCR_FINT_TOUCHENABLE << 8)         ///< The screen is touchable by touch screen
#define D4D_SCR_F_MOUSE_NORMAL          (D4D_SCR_FINT_MOUSE_NORMAL << 8)        ///< The screen has normal mouse cursor
#define D4D_SCR_F_MOUSE_BUSY            (D4D_SCR_FINT_MOUSE_BUSY << 8)          ///< The screen has busy mouse cursor
#define D4D_SCR_F_MOUSE_UNAVAILABLE     (D4D_SCR_FINT_MOUSE_UNAVAILABLE << 8)   ///< The screen has unavailable mouse cursor

/**@}*/

/*! @brief This is screen default behaviour definition.
           If not defined, it sets to (D4D_SCR_FINT_MOUSE_NORMAL) as a default.*/
#ifndef D4D_SCR_F_DEFAULT
  #define D4D_SCR_F_DEFAULT        (D4D_SCR_FINT_MOUSE_NORMAL)
#endif

/*! @} End of doxd4d_screen_const                                             */

/******************************************************************************
* Internal constants
******************************************************************************/
#define D4D_SCR_FINT_INITDONE       (0x01)
#define D4D_SCR_FINT_TOUCHENABLE    (0x02)
#define D4D_SCR_FINT_REDRAWC        (0x04)
#define D4D_SCR_FINT_CHECKOBJECTS   (0x08)

#define D4D_SCR_FINT_MOUSE_MASK     (0x30)
#define D4D_SCR_FINT_MOUSE_SHIFT    4

#define D4D_SCR_FINT_MOUSE_NORMAL       0x10
#define D4D_SCR_FINT_MOUSE_BUSY         0x20
#define D4D_SCR_FINT_MOUSE_UNAVAILABLE  0x30

struct D4D_STRING_S;
/******************************************************************************
* D4D Screen types
*
*//*! @addtogroup doxd4d_screen_type
* @{
*******************************************************************************/


/*! @brief The screen flags type. The masks are described here \ref doxd4d_screen_const_flags.*/
typedef Byte D4D_SCREEN_FLAGS;

/*! @brief The screen run time data type. It used to internal store the runtime data by eGUI. */
typedef struct
{
  // other runtime data
  D4D_OBJECT* focusedObject;            ///< Pointer on currently focused object
  Byte flags;                           ///< Internal screen flags
} D4D_SCREEN_DATA;

// screen configuration (goes to ROM by default)

/*! @brief The screen structure type. The main screen structure that contains all needed data to run the eGUI screen.*/
typedef struct D4D_SCREEN_S
{
    // object list
    const D4D_OBJECT* const * pObjects;                 ///< NULL-terminated array of objects (may lay in ROM)

    // event handlers
    void (*OnInit)(void);                               ///< Screen event handler of one-time initialization
    void (*OnMain)(void);                               ///< Screen event handler of main screen handler function
    void (*OnActivate)(void);                           ///< Screen event handler of called before screen activation
    void (*OnDeactivate)(void);                         ///< Screen event handler of called before deactivating
    Byte (*OnObjectMsg)(struct D4D_MESSAGE_S* pMsg);    ///< Screen event handler of called before object receives the message

    // Screen advanced properties
    D4D_POINT position;                 ///< Screen coordination on physical screen (standard screen has {0,0})
    D4D_SIZE size;                      ///< Screen size (standard screen has full size of physical screen)
    D4D_COOR radius;                    ///< Screen corner radius
    D4D_MARGIN* pMargin;                ///< Screen margin of user content
    struct D4D_STRING_S textBuff;       ///< Screen title bar text buffer
    const D4D_BMP* pIcon;               ///< Screen title icon

    D4D_SCREEN_FLAGS flags;             ///< Screen behaviour flags
    struct D4D_CLR_SCHEME_S* clrScheme; ///< Screen color scheme

    // pointer to runtime data
    D4D_SCREEN_DATA* pData;             ///< pointer to screen private run time data
} D4D_SCREEN;

/*! @} End of doxd4d_screen_type                                              */
/*********************************************************
*
* static declaration macros
*
*********************************************************/
/******************************************************************************
* D4D Screen macros
*
*//*! @addtogroup doxd4d_screen_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the screen structure in memory including all substructures
* @param   type - type of screen <D4D_CONST; D4D_NO_CONST>
* @param   name - name of screen
* @param   funcPrefix - Prefix Name of screen event handlers
* @param   x - coordination of screen in X axis
* @param   y - coordination of screen in Y axis
* @param   cx - size of screen in X axis
* @param   cy - size of screen in Y axis
* @param   radius - radius of corners
* @param   text - title bar text (if used)
* @param   fontId - title bar text font
* @param   icon - title bar icon picture
* @param   initFlags - init flags of screen corresponding to \ref doxd4d_screen_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw screen and objects

* @note    This macro create complete D4D_SCREEN structure, including the screen data sub structure. Is used to declare the eGUI screen.
*          This is complete screen declaration macro.
*******************************************************************************/
#define _D4D_DECLARE_SCREEN_BEGIN(type, name, funcPrefix, x ,y, cx, cy, radius, text, fontId, icon, initFlags, pScheme) \
    extern const D4D_OBJECT * const name##_objects[]; \
    static void funcPrefix##OnInit(void); \
    static void funcPrefix##OnMain(void); \
    static void funcPrefix##OnActivate(void); \
    static void funcPrefix##OnDeactivate(void); \
    static Byte funcPrefix##OnObjectMsg(D4D_MESSAGE* pMsg); \
    static D4D_SCREEN_DATA name##_data = { NULL,  (Byte)((D4D_SCREEN_FLAGS)(initFlags >> 8) & 0x00FF)}; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_FNT_PRTY_TRANSPARENT_YES_MASK, (D4D_ALIGN_H_LEFT_MASK | D4D_ALIGN_V_CENTER_MASK)}; \
    type D4D_SCREEN name = \
    { \
        name##_objects, \
        funcPrefix##OnInit, \
        funcPrefix##OnMain, \
        funcPrefix##OnActivate, \
        funcPrefix##OnDeactivate, \
        funcPrefix##OnObjectMsg, \
        {x, y}, \
        {cx, cy}, \
        radius, \
        NULL, \
        { text, sizeof(text), fontId,  &name##_strPrties, 0, 0}, /* textBuff */ \
        icon, \
        (D4D_SCREEN_FLAGS)(initFlags & 0x00FF), \
        pScheme, \
        &(name##_data) \
    }; \
    const D4D_OBJECT * const name##_objects[] = \
    {

/**************************************************************************/ /*!
* @brief   Macro that create the screen structure in ROM memory including all substructures
* @param   name - name of screen
* @param   funcPrefix - Prefix Name of screen event handlers
* @param   x - coordination of screen in X axis
* @param   y - coordination of screen in Y axis
* @param   cx - size of screen in X axis
* @param   cy - size of screen in Y axis
* @param   text - title bar text (if used)
* @param   fontId - title bar text font
* @param   icon - title bar icon picture
* @param   initFlags - init flags of screen corresponding to \ref doxd4d_screen_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw screen and objects

* @note    This macro create complete D4D_SCREEN structure in ROM, including the screen data sub structure. Is used to declare the eGUI screen.
*          This is screen declaration macro to create screen in ROM without round corners.
*******************************************************************************/
#define D4D_DECLARE_SCREEN_BEGIN(name, funcPrefix, x ,y, cx, cy, text, fontId, icon, initFlags, pScheme)\
  _D4D_DECLARE_SCREEN_BEGIN(D4D_CONST, name, funcPrefix, x ,y, cx, cy, 0, text, fontId, icon, initFlags, pScheme)

/**************************************************************************/ /*!
* @brief   Macro that create the screen structure in ROM memory with standard settings
* @param   name - name of screen
* @param   funcPrefix - Prefix Name of screen event handlers
* @note    This macro create complete \ref D4D_SCREEN structure in ROM, including the screen data sub structure. Is used to declare the eGUI screen.
*          This is simplified screen declaration macro to create screen in ROM without round corners. Instead of some missing parameters are used default one.
*******************************************************************************/
#define D4D_DECLARE_STD_SCREEN_BEGIN(name, funcPrefix) D4D_DECLARE_SCREEN_BEGIN(name, funcPrefix, 0 ,0, \
          (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), NULL, 0, NULL, D4D_SCR_F_DEFAULT, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the screen structure in ROM memory with standard settings in portrait orientation
* @param   name - name of screen
* @param   funcPrefix - Prefix Name of screen event handlers
* @note    This macro create complete \ref D4D_SCREEN structure in ROM, including the screen data sub structure. Is used to declare the eGUI screen.
*          This is simplified screen declaration macro to create screen in ROM without round corners.
*          Instead of some missing parameters are used default one.
*******************************************************************************/
#define D4D_DECLARE_STD_PORTRAIT_SCREEN_BEGIN(name, funcPrefix) D4D_DECLARE_SCREEN_BEGIN(name, funcPrefix, 0 ,0, \
          (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), NULL, 0, NULL, D4D_SCR_F_DEFAULT, NULL)

/**************************************************************************/ /*!
* @brief   Macro that create the screen structure in ROM memory including all substructures with round corners
* @param   name - name of screen
* @param   funcPrefix - Prefix Name of screen event handlers
* @param   x - coordination of screen in X axis
* @param   y - coordination of screen in Y axis
* @param   cx - size of screen in X axis
* @param   cy - size of screen in Y axis
* @param   radius - radius of corners
* @param   text - title bar text (if used)
* @param   fontId - title bar text font
* @param   icon - title bar icon picture
* @param   initFlags - init flags of screen corresponding to \ref doxd4d_screen_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw screen and objects

* @note    This macro create complete \ref D4D_SCREEN structure in ROM, including the screen data sub structure. Is used to declare the eGUI screen.
*          This is screen declaration macro to create screen in ROM with round corners.
*******************************************************************************/
#define D4D_DECLARE_RSCREEN_BEGIN(name, funcPrefix, x ,y, cx, cy, radius, text, fontId, icon, initFlags, pScheme)\
  _D4D_DECLARE_SCREEN_BEGIN(D4D_CONST, name, funcPrefix, x ,y, cx, cy, radius, text, fontId, icon, initFlags, pScheme)

/**************************************************************************/ /*!
* @brief   Macro that is used to add object to screen definition
* @param   name - name of object
* @note    This macro is used after the screen declaration to add to screen object array new object.
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_SCREEN_BEGIN(my_screen, MyScreen_)
*       D4D_DECLARE_SCREEN_OBJECT(my_object1)
*       D4D_DECLARE_SCREEN_OBJECT(my_object2)
*       D4D_DECLARE_SCREEN_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_SCREEN_OBJECT(name) &(name),

/**************************************************************************/ /*!
* @brief   Macro that is used to make end of object array in screen definition
* @note    This macro is used after the last D4D_DECLARE_SCREEN_OBJECT declaration to finish the screen object array.
* Here is an example demonstrating how to used it.
* @code
*       D4D_DECLARE_STD_SCREEN_BEGIN(my_screen, MyScreen_)
*       D4D_DECLARE_SCREEN_OBJECT(my_object1)
*       D4D_DECLARE_SCREEN_OBJECT(my_object2)
*       D4D_DECLARE_SCREEN_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_SCREEN_END() NULL };

/**************************************************************************/ /*!
* @brief   Macro that externs the D4D_SCREEN struxture
* @param   name of screen structure
* @note    This is help macro to write clear D4D code to extern D4D_SCREEN to other C file
*******************************************************************************/
#define D4D_EXTERN_SCREEN(name) \
    extern D4D_SCREEN name;

/*! @} End of doxd4d_screen_macro                                             */

/**************************************************************//*!
*
* Main driver API Functions
*
******************************************************************/
#ifdef D4D_LLD_TCH
  void D4D_EnableScrTouchScreen(const D4D_SCREEN* pScr, D4D_BOOL bEnable);
#else
  #define  D4D_EnableScrTouchScreen(pScr, bEnable)
#endif

D4D_SCREEN* D4D_GetActiveScreen(void);
void D4D_ActivateScreen(D4D_SCREEN* pNewScreen, D4D_BOOL bReplaceCurrent);
void D4D_EscapeScreen(void);
void D4D_EscapeToBaseScreen(void);
void D4D_InitScreen(D4D_SCREEN* pScreen);
void D4D_InvalidateScreen(D4D_SCREEN* pScreen, D4D_BOOL bComplete);
void D4D_SetScreenTextProperties(D4D_SCREEN* pScreen, D4D_TEXT_PROPERTIES property);
void D4D_SetScreenFontProperties(D4D_SCREEN* pScreen, D4D_FONT_PROPERTIES property);


D4D_POINT D4D_GetClientToScreenPoint(D4D_OBJECT* pObject, D4D_POINT* nClientPoint);
D4D_POINT D4D_GetScreenToClientPoint(D4D_OBJECT* pObject, D4D_POINT* nScreenPoint);



#endif /* __D4D_SCREEN_H */
