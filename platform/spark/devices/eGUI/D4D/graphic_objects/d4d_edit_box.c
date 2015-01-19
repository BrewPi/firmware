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
* @file      d4d_edit_box.c
*
* @author     Petr Gargulak
*
* @version   0.0.5.0
*
* @date      Oct-15-2013
*
* @brief     D4D driver editBox object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_EditBoxOnMessage(D4D_MESSAGE* pMsg);
static D4D_BOOL D4D_EditBoxPutCharInt(D4D_OBJECT_PTR pObj, D4D_TCHAR ch);
static void D4D_EditBoxTimeTick(D4D_MESSAGE* pMsg);

static void D4D_EditBoxOnInit(D4D_MESSAGE* pMsg);

const D4D_OBJECT_SYS_FUNCTION d4d_editBoxSysFunc =
{
  D4D_DEFSTR("Edit Box"),
  D4D_EditBoxOnMessage,
  NULL,
  NULL
};

// temporary structure for editBox temporary value calculation
typedef struct
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
} D4D_EDIT_BOX_TMP_VAL;

#define _calc (*((D4D_EDIT_BOX_TMP_VAL*)d4d_scratchPad))




/*******************************************************
*
* EDIT_BOX Helper computing routines
*
*******************************************************/

static void D4D_EditBoxValue2Coor(D4D_OBJECT* pThis)
{
  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);
}

/*******************************************************
*
* EDIT_BOX Drawing routine
*
*******************************************************/

static void D4D_EditBoxOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pThis);
    D4D_EDIT_BOX_DATA* pEditBoxData = D4D_GET_EDIT_BOX_DATA(pEditBox);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;

    // Get background simple color
    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetScheme(pThis)->bckgFocus;

    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif

    // Compute all tempoarary values
    D4D_EditBoxValue2Coor(pThis);

    // draw just a color rectangle instead of bitmap
    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    {
      // Draw the background
      #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      if(pThis->radius)
        D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);
      else
      #endif
        D4D_FillRect(&_calc.position, &pThis->size, clrB);

      pEditBoxData->flags |= (D4D_EDIT_BOX_FLAGS_REDRAWTEXT | D4D_EDIT_BOX_FLAGS_REDRAWCURSOR);
    }

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // Draw text
    if(pEditBoxData->flags & D4D_EDIT_BOX_FLAGS_REDRAWTEXT)
    {
      pEditBoxData->flags &= ~D4D_EDIT_BOX_FLAGS_REDRAWTEXT;
      // draw the text
      #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      if(pThis->radius)
        D4D_DrawTextRRect(&_calc.contentGeom.pnt, &_calc.contentGeom.sz, &pEditBoxData->string, clrT, clrB, pThis->radius);
      else
      #endif
        D4D_DrawTextRect(&_calc.contentGeom.pnt, &_calc.contentGeom.sz, &pEditBoxData->string, clrT, clrB);
    }

    // Draw cursor
    if(pEditBoxData->flags & D4D_EDIT_BOX_FLAGS_REDRAWCURSOR)
    {
      D4D_STRING tmp_textbuff = pEditBoxData->string;
      D4D_POINT tmp_point = _calc.contentGeom.pnt;
      D4D_SIZE tmp_size = _calc.contentGeom.sz;
      D4D_COOR widthChar, widthText;
      pEditBoxData->flags &= ~D4D_EDIT_BOX_FLAGS_REDRAWCURSOR;

      tmp_textbuff.printLen = (D4D_INDEX)(pEditBoxData->curPos - tmp_textbuff.printOff);
      widthText = D4D_GetTextBuffWidthTab(&tmp_textbuff, NULL);
      tmp_point.x += widthText;

      widthChar = D4D_GetCharWidth(tmp_textbuff.fontId, *(tmp_textbuff.pText + pEditBoxData->curPos));

      if((widthText + widthChar) > _calc.contentGeom.sz.cx)
        widthChar = 0;

      if(((pEditBoxData->flags & D4D_EDIT_BOX_FLAGS_CURSORSTATE) && !(pEditBoxData->flags & D4D_EDIT_BOX_FLAGS_MOVECURSOR)) || (widthChar == 0))
      {

        //Draw the cursor
        tmp_size.cy = D4D_GetFontHeight(tmp_textbuff.fontId);
        tmp_size.cx = 2;

        // moed to cursor to right position in Y
        if (_calc.contentGeom.sz.cy > tmp_size.cy)
          tmp_point.y += ( _calc.contentGeom.sz.cy - tmp_size.cy) / 2;

        if (pThis->radius > 0)
        {
          // if rounded box we need to move the cursor to the right slightly if it will fit
          if ((tmp_point.x + pThis->radius/2) < (_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cx - 1))
            tmp_point.x += pThis->radius/2;
        }


        if(pEditBoxData->flags & D4D_EDIT_BOX_FLAGS_CURSORSTATE)
          D4D_FillRect(&tmp_point, &tmp_size, clrT);
        else
          D4D_FillRect(&tmp_point, &tmp_size, clrB);
      }else
      {
        // Hide the Cursor
        pEditBoxData->flags &= ~D4D_EDIT_BOX_FLAGS_MOVECURSOR;

        tmp_size.cx = widthChar;
        tmp_textbuff.printLen = 1;
        tmp_textbuff.printOff = pEditBoxData->curPos;
        D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_textbuff, clrT, clrB);
      }
    }


}


/*******************************************************
*
* EDIT_BOX key handling routine
*
*******************************************************/

static void D4D_EditBoxOnKeyUp(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pThis);
  D4D_EDIT_BOX_DATA* pEditBoxData = D4D_GET_EDIT_BOX_DATA(pEditBox);

  if(D4D_GetCapturedObject() == pMsg->pObject)
  {
    switch(pMsg->prm.key)
    {
      case D4D_KEY_SCANCODE_LEFT:
        // move cursor on new char position
        if(pEditBoxData->curPos)
          D4D_EditBoxSetCursor(pThis, (D4D_INDEX)(pEditBoxData->curPos - 1));
      break;

      case D4D_KEY_SCANCODE_RIGHT:
        D4D_EditBoxSetCursor(pThis, (D4D_INDEX)(pEditBoxData->curPos + 1));
      break;

      case D4D_KEY_SCANCODE_ESC:
        D4D_CaptureKeys(NULL);
      break;

      case D4D_KEY_SCANCODE_ENTER:
      case D4D_KEY_SCANCODE_UP:
      case D4D_KEY_SCANCODE_DOWN:
      break;

      default:
        // Standard keys input
        D4D_EditBoxPutChar(pThis, pMsg->prm.key);
        break;
    }
  }else
    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    D4D_CaptureKeys(pMsg->pObject);


}

/*******************************************************
*
* EDIT_BOX touch screen handling routine
*
*******************************************************/
#ifdef D4D_LLD_TCH
static void D4D_EditBoxOnTouch(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;

  if(D4D_GetFocusedObject(pMsg->pScreen) != pThis)
  {
    if(pMsg->nMsgId == D4D_MSG_TOUCHED)
      D4D_FocusSet(pMsg->pScreen, pThis);
  }
}
#endif

static void D4D_EditBoxOnInit(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pThis);
  D4D_EDIT_BOX_DATA* pEditBoxData = D4D_GET_EDIT_BOX_DATA(pEditBox);


  // If there is any init text just copy it into work buffer
  if(pEditBox->pInitText)
  {
    D4D_INDEX i = (D4D_INDEX)(pEditBoxData->string.buffSize - 1);
    D4D_TCHAR* s = pEditBox->pInitText;
    D4D_TCHAR* d = pEditBoxData->string.pText;
    D4D_INDEX cnt = 0;
    D4D_COOR maxWidth = (D4D_COOR)(pThis->size.cx - 1);

    if(pThis->pMargin)
    {
      maxWidth -= pThis->pMargin->left;
      maxWidth -= pThis->pMargin->right;
    }

    while((i--) && *s)
    {
      *d++ = *s++;
      cnt++;
    }

    *d = '\0'; // And terminate string
    pEditBoxData->string.printLen = cnt;
    pEditBoxData->string.printLen = D4D_GetTextMaxLength(&(pEditBoxData->string), maxWidth);

    if(pEditBoxData->string.printLen != cnt)
      pEditBoxData->flags |= D4D_EDIT_BOX_FLAGS_FULL;

    D4D_EditBoxSetCursor(pThis, cnt);

    pEditBoxData->flags &= ~D4D_EDIT_BOX_FLAGS_CHANGETEXT;

    if(pEditBox->pOnEvent)
     pEditBox->pOnEvent(pThis, D4D_EVENT_ONCHANGEDONE);

  }

  // set flag to redraw screen
  D4D_InvalidateObject(pMsg->pObject, D4D_TRUE);
}


static void D4D_EditBoxTimeTick(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pThis);
  D4D_EDIT_BOX_DATA* pEditBoxData = D4D_GET_EDIT_BOX_DATA(pEditBox);

  pEditBoxData->tickCounter++;
  if(pEditBoxData->tickCounter >= D4D_EDIT_BOX_CURSOR_BLINK_TICK_COUNTER)
  {
    pEditBoxData->tickCounter = 0;
    pEditBoxData->flags |= D4D_EDIT_BOX_FLAGS_REDRAWCURSOR;
    D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);

    if(pEditBoxData->flags & D4D_EDIT_BOX_FLAGS_CURSORSTATE)
      pEditBoxData->flags &= ~D4D_EDIT_BOX_FLAGS_CURSORSTATE;
    else
      pEditBoxData->flags |= D4D_EDIT_BOX_FLAGS_CURSORSTATE;
  }
}

/**************************************************************//*!
*
*
*
*     THE INTERNAL HELP FUNCTIONS
*
*
*
******************************************************************/

static D4D_BOOL D4D_EditBoxPutCharInt(D4D_OBJECT_PTR pObj, D4D_TCHAR ch)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;

  D4D_INDEX max_size = (D4D_INDEX)(pData->string.buffSize - 1);
  D4D_COOR maxWidth = (D4D_COOR)(pObj->size.cx - 1);

  if(pData->curPos >= max_size)
    return D4D_FALSE;

  // Place new character
  pData->string.pText[pData->curPos] = ch;

  if(pObj->pMargin)
  {
    maxWidth -= pObj->pMargin->left;
    maxWidth -= pObj->pMargin->right;
  }
  // just for case
  pData->string.printLen++;
  pData->string.printLen = D4D_GetTextMaxLength(&(pData->string), maxWidth);

  // move cursor on new char position
  D4D_EditBoxSetCursor(pObj, (D4D_INDEX)(pData->curPos + 1));

  pData->flags |= D4D_EDIT_BOX_FLAGS_CHANGETEXT;

  if(pEditBox->pOnEvent)
     pEditBox->pOnEvent((D4D_OBJECT*)pObj, D4D_EVENT_ONCHANGE);

  return D4D_TRUE;
}

/******************************************************************************
* Begin of D4D_EDIT_BOX public functions
*//*! @addtogroup doxd4d_edit_box_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function puts one char on cursor position and updates cursor
* @param   pObj - pointer to the edit box object
* @param   ch - char that will be puts on edit box
* @return  D4D_TRUE - character has been add / D4D_FALSE - character hasn't been add
* @note    The result returns if the character was add or not (non printed characters, max count of characters achived).
*******************************************************************************/
D4D_BOOL D4D_EditBoxPutChar(D4D_OBJECT_PTR pObj, D4D_TCHAR ch)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;
  D4D_BOOL res = D4D_TRUE;

  if(ch >= ' ') {
    res = D4D_EditBoxPutCharInt(pObj, ch);   //Put New char
  }else {
    switch(ch) {
      case '\r': // return carrier
        D4D_EditBoxSetCursor(pObj, 0);
        break;

      case '\b': // backspace
        if(pData->curPos)
        {
          D4D_TCHAR* pText = pData->string.pText;
          D4D_COOR maxWidth = (D4D_COOR)(pObj->size.cx - 1);
          D4D_INDEX i = pData->curPos; //Go back one char

          i--;

          while(pText[i])
          {
            pText[i] = pText[i+1];
            i++;
          }

          if(pObj->pMargin)
          {
            maxWidth -= pObj->pMargin->left;
            maxWidth -= pObj->pMargin->right;
          }

          pData->string.printLen = D4D_GetTextMaxLength(&(pData->string), maxWidth);

          // move cursor on new char position
          D4D_EditBoxSetCursor(pObj, (D4D_INDEX)(pData->curPos - 1));

          pData->flags |= D4D_EDIT_BOX_FLAGS_CHANGETEXT;

          if(pEditBox->pOnEvent)
            pEditBox->pOnEvent((D4D_OBJECT*)pObj, D4D_EVENT_ONCHANGE);

        }else
          res = D4D_FALSE;
        break;
    }
  }
  pData->flags |= (D4D_EDIT_BOX_FLAGS_REDRAWTEXT | D4D_EDIT_BOX_FLAGS_REDRAWCURSOR);
  D4D_InvalidateObject(pObj, D4D_FALSE);
  return res;
}

/**************************************************************************/ /*!
* @brief   The function puts string on cursor position and updates cursor
* @param   pObj - pointer to the edit box object
* @param   pText - pointer to string that should be added
* @return  D4D_TRUE - string has been completely add / D4D_FALSE - string hasn't ben complete add
* @note    The result returns if the string was add or not (non printed characters, max count of characters achived).
*******************************************************************************/
D4D_BOOL D4D_EditBoxPutString(D4D_OBJECT_PTR pObj, D4D_TCHAR* pText)
{
  D4D_TCHAR* pIntText = D4D_GetInternalStringPointer(pText);

  if(pIntText == NULL)
    return D4D_FALSE;

  while(*pIntText) {
    if(!D4D_EditBoxPutChar(pObj, *pIntText))
      return D4D_FALSE;
    pIntText++;
  }

  if(pObj != D4D_GetFocusedObject(D4D_GetActiveScreen()))
  {
    D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
    D4D_EDIT_BOX_DATA* pData = pEditBox->pData;

    pData->flags &= ~D4D_EDIT_BOX_FLAGS_CHANGETEXT;

    if(pEditBox->pOnEvent)
      pEditBox->pOnEvent((D4D_OBJECT*)pObj, D4D_EVENT_ONCHANGEDONE);
  }


  return D4D_TRUE;
}

/**************************************************************************/ /*!
* @brief   The function chenge the cursor position to new one
* @param   pObj - pointer to the edit box object
* @param   newPos - new position index
* @return  the real position of cursor after this operation
* @note    The real position could be different to request one in case the the limits are exceed.
*******************************************************************************/
D4D_INDEX D4D_EditBoxSetCursor(D4D_OBJECT_PTR pObj, D4D_INDEX newPos)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;
  D4D_COOR maxWidth = (D4D_COOR)(pObj->size.cx - 1);


  // Object exists?
  if(pObj == NULL)
    return 0;

  // Check if the cursor is not out of range
  if(newPos >= pData->string.buffSize)
    return pData->curPos;

  // Check if the cursor in on requested position
  if(pData->curPos == newPos)
    return newPos;

  if(pObj->pMargin)
  {
    maxWidth -= pObj->pMargin->left;
    maxWidth -= pObj->pMargin->right;
  }

  // In case that the cursor overjump non text , fill up overjumped palce by spaces
  if(pData->curPos < newPos)
  {
    D4D_INDEX i = (D4D_INDEX)(newPos - 1);
    D4D_TCHAR* pText = pData->string.pText;
   // D4D_INDEX cnt = pData->string.printLen;
    while(i && (!pText[i]))
    {
      pText[i--] = ' ';
   //   cnt++;
    }

    // the visible area is not full
    if(!(pData->flags & D4D_EDIT_BOX_FLAGS_FULL))
    {
    //  D4D_INDEX len = pData->string.printLen;
      if(pData->string.printLen < newPos)
      {
        pData->string.printLen = newPos;
        pData->string.printLen = D4D_GetTextMaxLength(&(pData->string), maxWidth);

        if(pData->string.printLen != newPos)
          pData->flags |= D4D_EDIT_BOX_FLAGS_FULL;
      }
    }

  }

  // Check if the cursor is in visible range
  if((newPos >= pData->string.printOff) && (newPos <= (pData->string.printLen + pData->string.printOff - 0)))
  {
    pData->curPos = newPos;
  }else
  {
    // requested position is not in visible area
    if(pData->curPos > newPos)
    {
      // Go back with visible area
      pData->string.printOff = newPos;
      // Temporary fill up printLen with maximal possible value to be used by function D4D_GetTextMaxLength(...)
      pData->string.printLen = (D4D_INDEX)(pData->string.buffSize - newPos - 1);
      // Compute real printLen
      pData->string.printLen = D4D_GetTextMaxLength(&(pData->string),  maxWidth);
    }else
    {
      // Go forward with visible area
      D4D_COOR currentWidth = D4D_GetTextBuffWidthTab(&(pData->string), NULL);
      D4D_INDEX currentLast =  (D4D_INDEX)(pData->string.printOff + pData->string.printLen);
      D4D_COOR width;
      D4D_FONT fontIx = pData->string.fontId;

      while(currentLast < newPos)
      {
        currentWidth -= D4D_GetCharWidth(fontIx, pData->string.pText[pData->string.printOff]);
        pData->string.printOff++;
        pData->string.printLen--;
        while(1)
        {
          if(!pData->string.pText[currentLast])
            break;

          width = D4D_GetCharWidth(fontIx, pData->string.pText[currentLast]);

          if((width + currentWidth) > maxWidth)
            break;

          currentWidth += width;
          currentLast++;
          pData->string.printLen++;
        }
      }
    }
    pData->curPos = newPos;
  }

  pData->flags |= (D4D_EDIT_BOX_FLAGS_REDRAWTEXT | D4D_EDIT_BOX_FLAGS_REDRAWCURSOR | D4D_EDIT_BOX_FLAGS_MOVECURSOR);
  D4D_InvalidateObject(pObj, D4D_FALSE);
  return pData->curPos;
}




/**************************************************************************/ /*!
* @brief   The function returns the cursor position
* @param   pObj - pointer to the edit box object
* @return  the position of cursor
* @note    none
*******************************************************************************/
D4D_INDEX D4D_EditBoxGetCursor(D4D_OBJECT_PTR pObj)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;

  if(pObj == NULL)
    return 0;

  return pData->curPos;
}

/**************************************************************************/ /*!
* @brief   The function clear whole edit box data
* @param   pObj - pointer to the edit box object
* @return  none
* @note    none
*******************************************************************************/
void D4D_EditBoxClearAll(D4D_OBJECT_PTR pObj)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;

  LWord i,i_max;

  // Init the text array
  i_max = pData->string.buffSize;

  for(i=0;i<i_max;i++)
    pData->string.pText[i] = '\0';

  pData->curPos = 0;
  pData->string.printOff = 0;
  pData->string.printLen = 0;
  pData->flags &= ~(D4D_EDIT_BOX_FLAGS_FULL | D4D_EDIT_BOX_FLAGS_CHANGETEXT);

  if(pEditBox->pOnEvent)
     pEditBox->pOnEvent((D4D_OBJECT*)pObj, D4D_EVENT_ONCLEAR);

  pData->flags |= (D4D_EDIT_BOX_FLAGS_REDRAWTEXT | D4D_EDIT_BOX_FLAGS_REDRAWCURSOR);
  D4D_InvalidateObject(pObj, D4D_FALSE);
}

/**************************************************************************/ /*!
* @brief   The function returns the pointer to the edit box text buffer
* @param   pObj - pointer to the edit box object
* @return  pointer to edit box text
* @note    none
*******************************************************************************/
D4D_TCHAR* D4D_EditBoxGetText(D4D_OBJECT_PTR pObj)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pObj);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;

  return pData->string.pText;
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_edit_box_func                                            */
/******************************************************************************/

/**************************************************************//*!
*
*
*
*     THE INTERNAL API FUNCTIONS
*
*
*
******************************************************************/

/*******************************************************
*
* The EDIT_BOX message handler
*
*******************************************************/

void D4D_EditBoxOnMessage(D4D_MESSAGE* pMsg)
{
  D4D_EDIT_BOX* pEditBox = D4D_GET_EDIT_BOX(pMsg->pObject);
  D4D_EDIT_BOX_DATA* pData = pEditBox->pData;

  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_EditBoxOnDraw(pMsg);
      break;

    case D4D_MSG_KEYDOWN:
      D4D_EditBoxOnKeyUp(pMsg);
      break;

#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
    case D4D_MSG_TOUCH_AUTO:
        D4D_EditBoxOnTouch(pMsg);
      break;
#endif
    case D4D_MSG_ONINIT:
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;
      D4D_EditBoxOnInit(pMsg);
      break;

    case D4D_MSG_TIMETICK:
      #if (D4D_EDIT_BOX_CURSOR_BLINK_TICK_COUNTER > 0)
        D4D_EditBoxTimeTick(pMsg);
      #endif
      break;

    case D4D_MSG_KILLFOCUS:

      D4D_CaptureKeys(NULL);

      if(pData->flags & D4D_EDIT_BOX_FLAGS_CHANGETEXT)
      {
        pData->flags &= ~D4D_EDIT_BOX_FLAGS_CHANGETEXT;

        if(pEditBox->pOnEvent)
          pEditBox->pOnEvent(pMsg->pObject, D4D_EVENT_ONCHANGEDONE);
      }

    case D4D_MSG_KILLCAPTURE:
    case D4D_MSG_SETFOCUS:
    case D4D_MSG_SETCAPTURE:
        D4D_EnableTimeTicks(pMsg->pObject, pMsg->nMsgId == D4D_MSG_SETCAPTURE);

        pData->flags |= (D4D_EDIT_BOX_FLAGS_REDRAWTEXT | D4D_EDIT_BOX_FLAGS_REDRAWCURSOR);
        pData->flags &= ~D4D_EDIT_BOX_FLAGS_CURSORSTATE;
        D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
      break;

    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}
