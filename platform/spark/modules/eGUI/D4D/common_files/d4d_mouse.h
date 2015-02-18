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
* @file      d4d_mouse.h
*
* @author     Petr Gargulak
*
* @version   0.0.3.0
*
* @date      Oct-2-2013
*
* @brief     D4D Driver mouse functions header file
*
*******************************************************************************/

#ifndef __D4D_MOUSE_H
#define __D4D_MOUSE_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* D4D Mouse setting  constants
*
*//*! @addtogroup doxd4d_mouse_const
* @{
*******************************************************************************/
/*! @brief D4D_MOUSE_BUFF_LENGTH constant default declaration                 */
#ifndef D4D_MOUSE_BUFF_LENGTH
  #define D4D_MOUSE_BUFF_LENGTH 4       ///< 4
#endif

/*! @brief D4D_MOUSE_NORMAL_BMP constant default declaration                 */
#ifndef D4D_MOUSE_NORMAL_BMP
  #define D4D_MOUSE_NORMAL_BMP            NULL  ///< NULL - no bitmap for normal cursor
#endif

/*! @brief D4D_MOUSE_BUSY_BMP constant default declaration                 */
#ifndef D4D_MOUSE_BUSY_BMP
  #define D4D_MOUSE_BUSY_BMP              NULL  ///< NULL - no bitmap for busy cursor
#endif

/*! @brief D4D_MOUSE_UNAVAILABLE_BMP constant default declaration                 */
#ifndef D4D_MOUSE_UNAVAILABLE_BMP
  #define D4D_MOUSE_UNAVAILABLE_BMP       NULL  ///< NULL - no bitmap for unavailable cursor
#endif

/*! @brief D4D_MOUSE_BMP_KEY_COLOR constant default declaration                 */
#ifndef D4D_MOUSE_BMP_KEY_COLOR
  #define D4D_MOUSE_BMP_KEY_COLOR       0x003F3F3F  ///< 0x003F3F3F - the most white key color that support LCDC peripheral
#endif

/*! @} End of doxd4d_mouse_const                                              */

/******************************************************************************
* Internal constants
******************************************************************************/
#define D4D_MOUSE_CURSOR_TYPES  3


/******************************************************************************
* D4D Mouse types
*
*//*! @addtogroup doxd4d_mouse_type
* @{
*******************************************************************************/
/*! @brief D4D_MOUSE_EVENT Mouse input events enumeration                     */
typedef enum
{
  D4D_MOUSE_EVENT_MOVE = 0,     ///< Move event
  D4D_MOUSE_EVENT_SETPOS,       ///< Set abosolute position event
  D4D_MOUSE_EVENT_DOWN_L,       ///< Left button down event
  D4D_MOUSE_EVENT_UP_L,         ///< Left button up event
  D4D_MOUSE_EVENT_DOWN_R,       ///< Right button down event
  D4D_MOUSE_EVENT_UP_R,         ///< Right button up event
  D4D_MOUSE_EVENT_DOWN_M,       ///< Middle button down event
  D4D_MOUSE_EVENT_UP_M,         ///< Middle button up event
  D4D_MOUSE_EVENT_WHEEL_UP,     ///< Wheel up event
  D4D_MOUSE_EVENT_WHEEL_DOWN    ///< Wheel down event
}D4D_MOUSE_EVENT;


/*! @brief D4D_MOUSECURSOR_BMP structure contains the pointer to cursor bitmaps and key color */
typedef struct
{
  D4D_BMP* cursorArray[D4D_MOUSE_CURSOR_TYPES];  ///< cursor bitmap pointers array
  D4D_COLOR keyColor;                           ///< key Color for transparent color
}D4D_MOUSECURSOR_BMP;

/*! @brief D4D_MOUSE_CURSOR_TYPE enumeration type contains all types of MOUSE cursors */
typedef enum
{
  D4D_MOUSE_CURSOR_TYPE_INACTIVE = 0,   ///< Drawed normal cursor but the object/screen doesn't get any click/wheel events
  D4D_MOUSE_CURSOR_TYPE_NORMAL = 1,     ///< Drawed normal cursor and all events are sending to objects/screens
  D4D_MOUSE_CURSOR_TYPE_BUSY = 2,       ///< Drawed busy cursor but the object/screen doesn't get any click/wheel events
  D4D_MOUSE_CURSOR_TYPE_UNAVAILABLE = 3, ///< Drawed unavailable cursor but the object/screen doesn't get any click/wheel events
  D4D_MOUSE_CURSOR_TYPE_CNT             ///< Just system define that specified the count of cursors, must be kept on end of the enumeration field
}D4D_MOUSE_CURSOR_TYPE;

/*! @} End of doxd4d_mouse_type                                               */
/******************************************************************************
* Internal types
******************************************************************************/
typedef struct
{
  sWord x;
  sWord y;
  D4D_MOUSE_EVENT event;
}D4D_MOUSE;

typedef struct
{
  D4D_MOUSE buff[D4D_MOUSE_BUFF_LENGTH];
  Byte readPos;
  Byte writePos;
} D4D_MOUSE_BUFFER;

typedef Byte D4D_MOUSE_STATUS;

#define D4D_MOUSE_BTN_LEFT_MASK         (0x01)
#define D4D_MOUSE_BTN_RIGHT_MASK        (0x02)
#define D4D_MOUSE_BTN_MIDDLE_MASK       (0x04)

#define D4D_MOUSE_CURSOR_MASK           (0x70)
#define D4D_MOUSE_VISIBLE_MASK          (0x80)

#define D4D_MOUSE_CURSOR_SHIFT          (4)

/******************************************************************************
* D4D Mouse macros
*
*//*! @addtogroup doxd4d_mouse_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that define the cursor bitmaps array
* @param   name - name of the cursor bitmap array
* @param   pNormal - pointer on D4D_BMP object of bitmap that represents normal cursor
* @param   pBusy - pointer on D4D_BMP object of bitmap that represents busy cursor
* @param   pUnavailable - pointer on D4D_BMP object of bitmap that represents unavailable cursor
* @param   keyColor - key color for current set of cursor bitmaps
* @note    This define array of mouse cursor bitmaps. If busy or unavailable cursors
*          aren't used in project just used instead of cursor to D4D_BMP NULL.
*******************************************************************************/
#define D4D_DECLARE_MOUSE_CURSOR_BMP(name, pNormal, pBusy, pUnavailable, keyColor) \
  extern D4D_BMP  pNormal;      \
  extern D4D_BMP  pBusy;        \
  extern D4D_BMP  pUnavailable; \
  const D4D_MOUSECURSOR_BMP name = \
    { { (D4D_BMP*)&pNormal, (D4D_BMP*)&pBusy, (D4D_BMP*)&pUnavailable }, keyColor};


/*! @} End of doxd4d_mouse_macro                                              */

/******************************************************************************
* Global variables
******************************************************************************/


#ifdef D4D_LLD_MOUSE
/******************************************************************************
* Global functions
******************************************************************************/
void D4D_ClearMouseInputBuffer(void);
void D4D_NewMouseEvent(sWord x, sWord y, D4D_MOUSE_EVENT event);
D4D_OBJECT* D4D_GetMouseHoverObject(void);
D4D_POINT D4D_GetMouseCoordinates(D4D_OBJECT* pObject);
D4D_POINT D4D_GetMouseRawCoordinates(void);
void D4D_MouseShow(D4D_BOOL show);
D4D_BOOL D4D_MouseVisible(void);
void D4D_SetMouseCursorType(D4D_MOUSE_CURSOR_TYPE cursorIx, D4D_BOOL force);
D4D_MOUSE_CURSOR_TYPE D4D_GetMouseCursorType(void);
void D4D_ObjectSetMouseCursor(D4D_OBJECT_PTR pObj, D4D_MOUSE_CURSOR_TYPE cursorType);
D4D_MOUSE_CURSOR_TYPE D4D_ObjectGetMouseCursor(D4D_OBJECT_PTR pObj);
void D4D_ScreenSetMouseCursor(D4D_SCREEN* pScr, D4D_MOUSE_CURSOR_TYPE cursorType);
D4D_MOUSE_CURSOR_TYPE D4D_ScreenGetMouseCursor(D4D_SCREEN* pScr);
/*********************************************************
*
* internal API
*
*********************************************************/
void D4D_MouseInit(void);
void D4D_HandleMouse(void);
void D4D_MouseCenterCursor(void);
void D4D_MouseChangedScreen(void);
#else

/******************************************************************************
* Global functions
******************************************************************************/
#define D4D_ClearMouseInputBuffer()
#define D4D_NewMouseEvent(x, y, event)
#define D4D_GetMouseHoverObject() NULL
#define D4D_GetMouseCoordinates(pObject) d4d_point_zero
#define D4D_GetMouseRawCoordinates() d4d_point_zero
#define D4D_MouseShow(show)
#define D4D_MouseVisible() D4D_FALSE
#define D4D_SetMouseCursorType(cursorIx, force)
#define D4D_GetMouseCursorType() 0
#define D4D_ObjectSetMouseCursor(pObj, cursorType)
#define D4D_ObjectGetMouseCursor(pObj) 0
#define D4D_ScreenSetMouseCursor(pScr, cursorType)
#define D4D_ScreenGetMouseCursor(pScr) 0

/*********************************************************
*
* internal API
*
*********************************************************/
#define D4D_MouseInit()
#define D4D_HandleMouse()
#define D4D_MouseCenterCursor()
#define D4D_MouseChangedScreen()
#endif

#endif /* __D4D_MOUSE_H */










