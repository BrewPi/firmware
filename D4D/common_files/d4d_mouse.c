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
* @file      d4d_mouse.c
*
* @author     Petr Gargulak
*
* @version   0.0.4.0
*
* @date      Jan-15-2014
*
* @brief     D4D driver core and base function c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_lldapi.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/**************************************************************//*!
*
* Local variables
*
******************************************************************/

#ifdef D4D_LLD_MOUSE
  D4D_DECLARE_MOUSE_CURSOR_BMP(d4d_mouseCursorBmp_default, \
        D4D_MOUSE_NORMAL_BMP, D4D_MOUSE_BUSY_BMP, D4D_MOUSE_UNAVAILABLE_BMP, D4D_MOUSE_BMP_KEY_COLOR)

  static D4D_MOUSECURSOR_BMP * pMouseCursors = NULL;

  // The Buffer of mouse input events
  static D4D_MOUSE_BUFFER d4d_MouseBuff;
  static D4D_POINT d4d_mousePos;
  static D4D_OBJECT * d4d_LastClickedObj;
  static D4D_OBJECT * d4d_MouseHoverObj;
  static D4D_MOUSE_STATUS d4d_MouseStatus;



  static D4D_OBJECT* D4D_FindMouseHoverObject(D4D_OBJECT* pObject, D4D_POINT point);




/******************************************************************************
* Begin of D4D_MOUSE public functions
*//*! @addtogroup doxd4d_mouse_func
* @{
*******************************************************************************/


/**************************************************************************/ /*!
* @brief   Function to control visibility of mouse cursor.
* @param   show         - D4D_TRUE, D4D_FALSE
* @return  none
* @note    If there is no change the function leave without calling low level
*               functions.
*******************************************************************************/
void D4D_MouseShow(D4D_BOOL show)
{
  // Check if there is any change
  if(((d4d_MouseStatus & D4D_MOUSE_VISIBLE_MASK) && !show) || (!(d4d_MouseStatus & D4D_MOUSE_VISIBLE_MASK) && show))
  {
    if(show)
    {
      D4D_ClearMouseInputBuffer();
      d4d_MouseStatus |= D4D_MOUSE_VISIBLE_MASK;
      D4D_SetMouseCursorType(D4D_GetMouseCursorType(), D4D_TRUE);
    }else
    {
      d4d_MouseStatus &= ~D4D_MOUSE_VISIBLE_MASK;
      D4D_LLD_MOUSE.D4DMOUSE_SetPointer(0, D4D_FALSE);
    }
  }
}

/**************************************************************************/ /*!
* @brief   Function to get visibility of mouse cursor.
* @param   none
* @return  show         - true, false
* @note    Returns the visibility of cursor
*******************************************************************************/
D4D_BOOL D4D_MouseVisible(void)
{
  return (d4d_MouseStatus & D4D_MOUSE_VISIBLE_MASK);
}

/**************************************************************************/ /*!
* @brief   Function return current using cursors bitmap array.
* @param   none
* @return  pointer to current using mouse cursor support
* @note    none
*******************************************************************************/
D4D_MOUSECURSOR_BMP* D4D_GetMouseCursors(void)
{
  return pMouseCursors;
}

/**************************************************************************/ /*!
* @brief   Function return current using cursors bitmap array.
* @param   none
* @return  pointer to current using mouse cursor support
* @note    none
*******************************************************************************/
void D4D_SetMouseCursors(D4D_MOUSECURSOR_BMP* pMouseCrs)
{
  LWord i;
  void* d4d_MouseBmps[D4D_MOUSE_CURSOR_TYPES];

  if(pMouseCrs == NULL)
    pMouseCursors = (D4D_MOUSECURSOR_BMP*)&d4d_mouseCursorBmp_default;
  else
    pMouseCursors = pMouseCrs;


  for(i=0;i<D4D_MOUSE_CURSOR_TYPES;i++)
  {
    void* pCursorData = NULL;

    // check if the bitmap is OK
    if(pMouseCursors->cursorArray[i])
    {
      // Check the right format
      if(D4D_GetImgSrcType(pMouseCursors->cursorArray[i]) == D4D_EXTIMG_INTD4D)
      {
        D4D_SIZE size = D4D_GetBmpSize(pMouseCursors->cursorArray[i]);
        D4DMOUSE_DESC* pCurDesc = D4D_LLD_MOUSE.D4DMOUSE_GetDescriptor();
        LWord c_mode;

        if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pMouseCursors->cursorArray[i])->key == D4D_IMG_HEADER_2_KEY_D4DINT)
          c_mode = D4D_GET_IMG_HEADER_2_FROM_FILENAME(pMouseCursors->cursorArray[i])->colorMode;
        else
          c_mode = D4D_GET_IMG_HEADER_FROM_FILENAME(pMouseCursors->cursorArray[i])->colorMode;

        //check the format of bitmap
        if((size.cx == pCurDesc->size_x) && (size.cy == pCurDesc->size_y) && (c_mode == pCurDesc->bmpFormat)) // this is just only one format must be updated
        {
          if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pMouseCursors->cursorArray[i])->key == D4D_IMG_HEADER_2_KEY_D4DINT)
            pCursorData = (void*)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pMouseCursors->cursorArray[i])->pData;
          else
            pCursorData = (void*) &((Word*)(pMouseCursors->cursorArray[i]->pData))[3];
        }
      }
    }

//    if(i && (pCursorData == NULL))
//      d4d_MouseBmps[i] = d4d_MouseBmps[0];
//    else
      d4d_MouseBmps[i] = pCursorData;
  }

  D4D_LLD_MOUSE.D4DMOUSE_SetPointerBmp(d4d_MouseBmps, D4D_MOUSE_CURSOR_TYPES, pMouseCursors->keyColor);

}

/**************************************************************************/ /*!
* @brief   Clear input buffer of mouse evnts function.
* @param   none
* @return  none
* @note    The function clears the input buffer of mouse events
*******************************************************************************/
void D4D_ClearMouseInputBuffer(void)
{
  d4d_MouseBuff.readPos = 0;
  d4d_MouseBuff.writePos = 0;
}

/**************************************************************************/ /*!
* @brief   Puts the new event into mouse event buffer
* @param   x - for move event, the change in axis X
* @param   y - for move event, the change in axis Y
* @param   event - input event type
* @return  None
* @note    The only one way how to tell eGUI the mouse did any event
*******************************************************************************/
void D4D_NewMouseEvent(sWord x, sWord y, D4D_MOUSE_EVENT event)
{
  // Is there place for new record?
  if((d4d_MouseBuff.writePos + 1 == d4d_MouseBuff.readPos) || ((d4d_MouseBuff.readPos == 0) && (d4d_MouseBuff.writePos + 1 == D4D_MOUSE_BUFF_LENGTH)))
    return;

  // place new record to buffer
  d4d_MouseBuff.buff[d4d_MouseBuff.writePos].x = x;
  d4d_MouseBuff.buff[d4d_MouseBuff.writePos].y = y;
  d4d_MouseBuff.buff[d4d_MouseBuff.writePos].event = event;

  d4d_MouseBuff.writePos++;
  if(d4d_MouseBuff.writePos >= D4D_MOUSE_BUFF_LENGTH)
    d4d_MouseBuff.writePos = 0;

  #ifdef D4D_INPUT_EVENT_CALLBACK
    D4D_INPUT_EVENT_CALLBACK();
  #endif
}

/**************************************************************************/ /*!
* @brief   Gets the pointer of object where is mouse hover
* @param   None
* @return  Pointer on object where is mouse hover, NULL if there is no object
*               under mouse cursor.
* @note    none
*******************************************************************************/
D4D_OBJECT* D4D_GetMouseHoverObject(void)
{
  return d4d_MouseHoverObj;
}

/**************************************************************************/ /*!
* @brief   Gets the mouse raw cursor client coordination
* @param   none
* @return  Point structure of current raw coordination
* @note    None
*******************************************************************************/
D4D_POINT D4D_GetMouseRawCoordinates(void)
{
  return d4d_mousePos;
}

/**************************************************************************/ /*!
* @brief   Gets the mouse cursor client coordination
* @param   pObject - pointer of object what wants to know the client coordinations
* @return  Point structure of current coordination
* @note    None
*******************************************************************************/
D4D_POINT D4D_GetMouseCoordinates(D4D_OBJECT* pObject)
{
  return D4D_GetScreenToClientPoint(pObject, &d4d_mousePos);
}

/**************************************************************************/ /*!
* @brief   Function try to change the graphic cursor from current used cursor sets
* @param   cursorIx - index of cursor
* @param   force - force the change already if the index of bitmap is same
* @return  none
* @note    If its possible the function change the bitmap used for cursor.\n
*          The conditions are: \n
*                              input parametr is in range of valid indexes of bitmaps \n
*                              the index must be different then current one\n
*                              the bitmap must exist\n
*                              the bitmap must be in right format\n
*                              the bitmap must have right size
*******************************************************************************/
void D4D_SetMouseCursorType(D4D_MOUSE_CURSOR_TYPE cursorIx, D4D_BOOL force)
{
  if(cursorIx >= D4D_MOUSE_CURSOR_TYPE_CNT)
    return;

  if(force || (((d4d_MouseStatus & D4D_MOUSE_CURSOR_MASK) >> D4D_MOUSE_CURSOR_SHIFT) != cursorIx))
  {

    // store the new index of cursor
    d4d_MouseStatus &= ~D4D_MOUSE_CURSOR_MASK;
    d4d_MouseStatus |= ((cursorIx << D4D_MOUSE_CURSOR_SHIFT) & D4D_MOUSE_CURSOR_MASK);

    // The inactive and the normal type hase some picture (normal)
    if(cursorIx)
      cursorIx--;

    //Change Bitmap
    D4D_LLD_MOUSE.D4DMOUSE_SetPointer(cursorIx, (d4d_MouseStatus & D4D_MOUSE_VISIBLE_MASK));
  }
}

/**************************************************************************/ /*!
* @brief   Function return current used cursor type index
* @param   none
* @return  index of current used cursor type
* @note    none
*******************************************************************************/
D4D_MOUSE_CURSOR_TYPE D4D_GetMouseCursorType(void)
{
  return (D4D_MOUSE_CURSOR_TYPE)((d4d_MouseStatus & D4D_MOUSE_CURSOR_MASK) >> D4D_MOUSE_CURSOR_SHIFT);
}

/**************************************************************************/ /*!
* @brief   Function sets the object mouse cursor behaviour
* @param   pObj - pointer on object that should be set
* @param   cursorType - new cursor type that should be used
* @return  none
* @note    none
*******************************************************************************/
void D4D_ObjectSetMouseCursor(D4D_OBJECT_PTR pObj, D4D_MOUSE_CURSOR_TYPE cursorType)
{
  // check the input parameter
  if(pObj == NULL)
    return;

  // prevent actions if there is no change
  if(((pObj->pData->flags & D4D_OBJECT_F_MOUSE_MASK) >> D4D_OBJECT_F_MOUSE_SHIFT) == cursorType)
    return;

  // Change the obect settings
  pObj->pData->flags &= ~D4D_OBJECT_F_MOUSE_MASK;
  pObj->pData->flags |= ((cursorType << D4D_OBJECT_F_MOUSE_SHIFT) & D4D_OBJECT_F_MOUSE_MASK);

  // Check if the mouse hover above this object  and those the cursor must be updated immediatly
  if(pObj == D4D_GetMouseHoverObject())
    D4D_SetMouseCursorType(cursorType, D4D_TRUE);
}

/**************************************************************************/ /*!
* @brief   Function gets the object mouse cursor behaviour
* @param   pObj - pointer on object
* @return  cursorType - new cursor type that should be used
* @note    none
*******************************************************************************/
D4D_MOUSE_CURSOR_TYPE D4D_ObjectGetMouseCursor(D4D_OBJECT_PTR pObj)
{
  D4D_MOUSE_CURSOR_TYPE cursorType;

  // check the input parameter
  if(pObj == NULL)
    return D4D_MOUSE_CURSOR_TYPE_INACTIVE;

  cursorType = (D4D_MOUSE_CURSOR_TYPE)((pObj->pData->flags & D4D_OBJECT_F_MOUSE_MASK) >> D4D_OBJECT_F_MOUSE_SHIFT);

  if(cursorType != D4D_MOUSE_CURSOR_TYPE_NORMAL)
    return cursorType;

  if(pObj->pRelations)
  {
    D4D_OBJECT* pParent = D4D_GetParentObject((D4D_OBJECT*)pObj);
    if(pParent)
      return D4D_ObjectGetMouseCursor(pParent);
    else
      D4D_ScreenGetMouseCursor(pObj->pData->pScreen);
  }

  // return the type of mouse cursor
  return cursorType;
}

/**************************************************************************/ /*!
* @brief   Function sets the screen mouse cursor behaviour
* @param   pScr - pointer on screen that should be set
* @param   cursorType - new cursor type that should be used
* @return  none
* @note    none
*******************************************************************************/
void D4D_ScreenSetMouseCursor(D4D_SCREEN* pScr, D4D_MOUSE_CURSOR_TYPE cursorType)
{
  // check the input parameter
  if(pScr == NULL)
    return;

  // prevent actions if there is no change
  if(((pScr->pData->flags & D4D_SCR_FINT_MOUSE_MASK) >> D4D_SCR_FINT_MOUSE_SHIFT) == cursorType)
    return;

  // Change the obect settings
  pScr->pData->flags &= ~D4D_SCR_FINT_MOUSE_MASK;
  pScr->pData->flags |= ((cursorType << D4D_SCR_FINT_MOUSE_SHIFT) & D4D_SCR_FINT_MOUSE_MASK);

  // Check if the mouse hover above this object  and those the cursor must be updated immediatly
  if((D4D_GetActiveScreen() == pScr) && (NULL == D4D_GetMouseHoverObject()))
    D4D_SetMouseCursorType(cursorType, D4D_TRUE);
}

/**************************************************************************/ /*!
* @brief   Function gets the screen mouse cursor behaviour
* @param   pScr - pointer on screen
* @return  cursorType - new cursor type that should be used
* @note    none
*******************************************************************************/
D4D_MOUSE_CURSOR_TYPE D4D_ScreenGetMouseCursor(D4D_SCREEN* pScr)
{
  // check the input parameter
  if(pScr == NULL)
    return D4D_MOUSE_CURSOR_TYPE_INACTIVE;

  return (D4D_MOUSE_CURSOR_TYPE)((pScr->pData->flags & D4D_SCR_FINT_MOUSE_MASK) >> D4D_SCR_FINT_MOUSE_SHIFT);
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_mouse_func                                               */
/******************************************************************************/

/******************************************************************************
*       Private functions                                                     *
******************************************************************************/

/**************************************************************//*!
*
* Initialization of Mouse support in eGUI
*
******************************************************************/
void D4D_MouseInit(void)
{
  D4D_ClearMouseInputBuffer();
  d4d_MouseStatus |= D4D_MOUSE_VISIBLE_MASK;
  d4d_MouseHoverObj = NULL;
  d4d_LastClickedObj = NULL;

  D4D_MouseCenterCursor();

  D4D_SetMouseCursors(( D4D_MOUSECURSOR_BMP *)&d4d_mouseCursorBmp_default);

  D4D_SetMouseCursorType(D4D_MOUSE_CURSOR_TYPE_NORMAL, D4D_TRUE);
}

/**************************************************************//*!
*
* Centralize the mouse cursor to center of screen
*
******************************************************************/
void D4D_MouseCenterCursor(void)
{
  D4D_NewMouseEvent(D4D_DisplayWidth() / 2, D4D_DisplayHeight() / 2, D4D_MOUSE_EVENT_SETPOS);
}


/**************************************************************//*!
*
* Ends the focus on object from previous screen and set the current cursor type
*
******************************************************************/
void D4D_MouseChangedScreen(void)
{
  D4D_MESSAGE my_msg;
  D4D_SCREEN* pScreen = D4D_GetActiveScreen();

  if(d4d_MouseHoverObj)
  {
    my_msg.pScreen = pScreen;
    my_msg.pObject = d4d_MouseHoverObj;
    d4d_MouseHoverObj = NULL;
    my_msg.nMsgId = D4D_MSG_MOUSE_LEAVE;
    D4D_SendMessage(&my_msg);
  }
  // Change the cursor bitmap when the cursor leaves the object
  D4D_SetMouseCursorType((D4D_MOUSE_CURSOR_TYPE)((pScreen->pData->flags & D4D_SCR_FINT_MOUSE_MASK) >> D4D_SCR_FINT_MOUSE_SHIFT), d4d_MouseStatus & D4D_MOUSE_VISIBLE_MASK);

  D4D_ClearMouseInputBuffer();

  // invoke looking for the object where is mouse hover
  // place new record to buffer
  d4d_MouseBuff.buff[d4d_MouseBuff.writePos].x = 0;
  d4d_MouseBuff.buff[d4d_MouseBuff.writePos].y = 0;
  d4d_MouseBuff.buff[d4d_MouseBuff.writePos].event = D4D_MOUSE_EVENT_MOVE;

  d4d_MouseBuff.writePos++;
}

/**************************************************************//*!
*
* Handle Mouse events handler
*
******************************************************************/
void D4D_HandleMouse(void)
{
  // get active screen
  D4D_SCREEN* pScreen = D4D_GetActiveScreen();
  D4D_OBJECT** pObjects;
  D4D_MOUSE mouseEvent;

  // Check the buffer of input keys changes
  if(d4d_MouseBuff.writePos == d4d_MouseBuff.readPos)
    return;

  // Read the latest record in the buffer of input key event
  mouseEvent = d4d_MouseBuff.buff[d4d_MouseBuff.readPos++];

  // Check the overflow of the read pointer of readed input keys
  if(d4d_MouseBuff.readPos >= D4D_MOUSE_BUFF_LENGTH)
      d4d_MouseBuff.readPos = 0;


  // prepare message
  d4d_msg.pScreen = pScreen;
  d4d_msg.prm.draw = 0;
  d4d_msg.nMsgId = D4D_MSG_NONE;
  switch(mouseEvent.event)
  {
  case D4D_MOUSE_EVENT_MOVE:
    mouseEvent.x += d4d_mousePos.x;
    mouseEvent.y += d4d_mousePos.y;


  case D4D_MOUSE_EVENT_SETPOS:
    // Correct the input coordination to physical screen
    if(mouseEvent.x < 0)
      mouseEvent.x = 0;
    else if(mouseEvent.x >= (D4D_DisplayWidth() - 16))
      mouseEvent.x = D4D_DisplayWidth() - 1 - 16;

    if(mouseEvent.y < 0)
      mouseEvent.y = 0;
    else if(mouseEvent.y >= (D4D_DisplayHeight() - 16))
      mouseEvent.y = D4D_DisplayHeight() - 1 - 16;

    d4d_mousePos.x = mouseEvent.x;
    d4d_mousePos.y = mouseEvent.y;

    // Manage the hover mouse above objects
    if(d4d_MouseHoverObj)
    {
      D4D_BOOL validCoor;
      // Check the coordination if its valid
      if(d4d_MouseHoverObj->pObjFunc->CheckCoordinates)
        validCoor = d4d_MouseHoverObj->pObjFunc->CheckCoordinates(d4d_MouseHoverObj, D4D_GetScreenToClientPoint(d4d_MouseHoverObj, &d4d_mousePos));
      else
        validCoor = D4D_ObjectCheckCoordinates(d4d_MouseHoverObj, D4D_GetScreenToClientPoint(d4d_MouseHoverObj, &d4d_mousePos));

      if(validCoor && D4D_HasObjectChildren(d4d_MouseHoverObj))
      {
        pObjects = (D4D_OBJECT**)&d4d_MouseHoverObj->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];

        while(*pObjects)
        {
          if(D4D_FindMouseHoverObject(*pObjects, d4d_mousePos))
          {
            validCoor = D4D_FALSE;
            break;
          }
          pObjects++;
        }
      }

      if(!validCoor)
      {
        d4d_msg.pObject = d4d_MouseHoverObj;
        d4d_MouseHoverObj = NULL;
        d4d_msg.nMsgId = D4D_MSG_MOUSE_LEAVE;
        D4D_SendMessage(&d4d_msg);
        d4d_msg.nMsgId = D4D_MSG_NONE;
        // Change the cursor bitmap when the cursor leaves the object
        D4D_SetMouseCursorType((D4D_MOUSE_CURSOR_TYPE)((pScreen->pData->flags & D4D_SCR_FINT_MOUSE_MASK) >> D4D_SCR_FINT_MOUSE_SHIFT), D4D_FALSE);
      }
    }

    if(!d4d_MouseHoverObj)
    {
      pObjects = (D4D_OBJECT**)pScreen->pObjects;
      D4D_OBJECT* pHoverObj;
      // check all objects
      while(*pObjects)
      {
        pHoverObj = D4D_FindMouseHoverObject(*pObjects, d4d_mousePos);
        if(pHoverObj)
        {
          d4d_MouseHoverObj = pHoverObj;
          break;
        }
        pObjects++;
      }

      if(d4d_MouseHoverObj)
      {
        d4d_msg.nMsgId = D4D_MSG_MOUSE_ENTER;

        // change cursor D4D_OBJECT_F_MOUSE_MASK
        D4D_SetMouseCursorType(D4D_ObjectGetMouseCursor(d4d_MouseHoverObj), D4D_FALSE);
      }
    }

    D4D_LLD_MOUSE.D4DMOUSE_SetCoor(d4d_mousePos.x, d4d_mousePos.y);
    break;

  case D4D_MOUSE_EVENT_DOWN_L:
    d4d_MouseStatus |= D4D_MOUSE_BTN_LEFT_MASK;

    if(d4d_MouseHoverObj)
    {
      if(d4d_LastClickedObj)
      {
        d4d_msg.pObject = d4d_LastClickedObj;
        d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_LEFT_UP;
        D4D_SendMessage(&d4d_msg);
      }
    }

    d4d_LastClickedObj = d4d_MouseHoverObj;

    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_LEFT_DOWN;
    break;

  case D4D_MOUSE_EVENT_UP_L:
    d4d_MouseStatus &= ~D4D_MOUSE_BTN_LEFT_MASK;

    if(d4d_LastClickedObj)
    {
      if(d4d_LastClickedObj != d4d_MouseHoverObj)
      {
        d4d_msg.pObject = d4d_LastClickedObj;
        d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_LEFT_UP;
        D4D_SendMessage(&d4d_msg);
      }
      d4d_LastClickedObj = NULL;
    }

    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_LEFT_UP;
    break;

  case D4D_MOUSE_EVENT_DOWN_R:
    d4d_MouseStatus |= D4D_MOUSE_BTN_RIGHT_MASK;
    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_RIGHT_DOWN;
    break;

  case D4D_MOUSE_EVENT_UP_R:
    d4d_MouseStatus &= ~D4D_MOUSE_BTN_RIGHT_MASK;
    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_RIGHT_UP;
    break;

  case D4D_MOUSE_EVENT_DOWN_M:
    d4d_MouseStatus |= D4D_MOUSE_BTN_MIDDLE_MASK;
    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_MIDDLE_DOWN;
    break;

  case D4D_MOUSE_EVENT_UP_M:
    d4d_MouseStatus &= ~D4D_MOUSE_BTN_MIDDLE_MASK;
    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_MIDDLE_UP;
    break;

  case D4D_MOUSE_EVENT_WHEEL_UP:
    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_WHEEL_UP;
    break;

  case D4D_MOUSE_EVENT_WHEEL_DOWN:
    d4d_msg.nMsgId = D4D_MSG_MOUSE_BTN_WHEEL_DOWN;
     break;
  }

  if((d4d_msg.nMsgId == D4D_MSG_MOUSE_ENTER) || (d4d_msg.nMsgId == D4D_MSG_MOUSE_LEAVE) || ((d4d_msg.nMsgId != D4D_MSG_NONE) && (((d4d_MouseStatus & D4D_MOUSE_CURSOR_MASK) >> D4D_MOUSE_CURSOR_SHIFT) == D4D_MOUSE_CURSOR_TYPE_NORMAL))) // any valid message to send
  {

    // Handle Screen events
    if(pScreen->pData->flags & D4D_SCR_FINT_MOUSE_MASK)
    {
      if(pScreen->flags & D4D_SCR_F_EXIT)
      {
        if(D4D_ScrCheckExitBtnCoor(pScreen, &d4d_mousePos))
        {
          D4D_EscapeScreen();
          d4d_MouseHoverObj = NULL;
          d4d_LastClickedObj = NULL;
          return;
        }
      }

      if(D4D_ScrCheckCoor(pScreen, &d4d_mousePos))
      {
        d4d_msg.pObject = NULL;
        // send the MOUSE EVENT message to screen
        D4D_SendMessage(&d4d_msg);
      }
    }

    if(d4d_MouseHoverObj)
    {
      d4d_msg.pObject = d4d_MouseHoverObj;
      D4D_SendMessage(&d4d_msg);
    }
  }


}

/**************************************************************//*!
*
* Handle the Mouse looking for the goal object (part of main poll call)
*
******************************************************************/

static D4D_OBJECT* D4D_FindMouseHoverObject(D4D_OBJECT* pObject, D4D_POINT point)
{
  D4D_BOOL validCoor;
  D4D_OBJECT* lastHoverObject = NULL;

  if(pObject == NULL)
    return lastHoverObject;

  if(!D4D_IsVisible(pObject))
    return lastHoverObject;

  if(!D4D_IsEnabled(pObject))
    return lastHoverObject;

  if(!(pObject->pData->flags & D4D_OBJECT_F_MOUSE_MASK))
    return lastHoverObject;

  if(pObject->pObjFunc->CheckCoordinates == NULL)
    validCoor = D4D_ObjectCheckCoordinates(pObject, D4D_GetScreenToClientPoint(pObject, &point));
  else
    // Check the coordination if its valid
    validCoor = pObject->pObjFunc->CheckCoordinates(pObject, D4D_GetScreenToClientPoint(pObject, &point));

  // if the coordinations are valid or autorepeat is enabled
  if(validCoor)
  {
    D4D_OBJECT* pFoundObject;
    // This is latest object what I'm able to find
    lastHoverObject = pObject;

    if(D4D_HasObjectChildren(pObject))
    {
      D4D_OBJECT** pChildren = (D4D_OBJECT**)&pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];

      while(*pChildren && !(pFoundObject = D4D_FindMouseHoverObject(*pChildren, point)))
      {
        pChildren++;
      }
      if(pFoundObject)
        lastHoverObject = pFoundObject;
    }
  }
  return lastHoverObject;
}
#endif
