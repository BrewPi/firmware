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
* @file      d4d_console.c
*
* @author     Petr Gargulak
*
* @version   0.0.28.0
*
* @date      Oct-15-2013
*
* @brief     D4D driver console object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"


// Internal API
void D4D_ConsoleOnMessage(D4D_MESSAGE* pMsg);



static void D4D_CnslNewLine(D4D_OBJECT_PTR pObj, D4D_BOOL returnCarrier);
static D4D_BOOL D4D_CnslPutCharInt(D4D_OBJECT_PTR pObj, D4D_TCHAR ch);
static void D4D_CnslUpdateScrollbars(D4D_OBJECT* pThis);
static void D4D_CnslOnInit(D4D_MESSAGE* pMsg);
static void D4D_ConsoleTimeTick(D4D_MESSAGE* pMsg);
static D4D_INDEX D4D_CnslFindMaxTextLenght(D4D_TCHAR* pText, D4D_FONT font, D4D_INDEX startOffset, D4D_INDEX maxLenght);

const D4D_OBJECT_SYS_FUNCTION d4d_consoleSysFunc =
{
  D4D_DEFSTR("Console"),
  D4D_ConsoleOnMessage,
  NULL,
  NULL
};

typedef struct
{
  D4D_POINT position;
  D4D_SIZE size;
  D4D_GEOMETRY txtGeom;
  D4D_SIZE lineSize;
  D4D_SIZE charArrSize;
} D4D_CNSL_TMP_VAL;

#define _calc (*((D4D_CNSL_TMP_VAL*)d4d_scratchPad))
//static D4D_CNSL_TMP_VAL _calc;

static void D4D_CnslValue2Coor(D4D_OBJECT* pThis)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pThis);
  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);


  //margin (bevels :-) )
  D4D_ComputeGeometry(&(_calc.txtGeom), pThis);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  _calc.txtGeom.pnt.y += (D4D_COOR)(pThis->radius / 2);
  _calc.txtGeom.pnt.x += (D4D_COOR)(pThis->radius / 2);

  _calc.txtGeom.sz.cx -= (D4D_COOR)(pThis->radius / 2);
  _calc.txtGeom.sz.cy -= (D4D_COOR)(pThis->radius / 2);
#endif

  _calc.charArrSize.cx = (D4D_COOR) (_calc.txtGeom.sz.cx / D4D_GetFontWidth(pCnsl->textFontId));


  // check if the
  if(_calc.charArrSize.cx < pCnsl->txtArrSize.cx)
    _calc.txtGeom.sz.cy -= D4D_CNSL_SCRLBR_WIDTH;
  else
    _calc.charArrSize.cx = pCnsl->txtArrSize.cx;

  _calc.charArrSize.cy = (D4D_COOR) (_calc.txtGeom.sz.cy / D4D_GetFontHeight(pCnsl->textFontId));

  if(_calc.charArrSize.cy < pCnsl->txtArrSize.cy)
  {
    _calc.txtGeom.sz.cx -= D4D_CNSL_SCRLBR_WIDTH;
    _calc.charArrSize.cx = (D4D_COOR) (_calc.txtGeom.sz.cx / D4D_GetFontWidth(pCnsl->textFontId));
  }else
    _calc.charArrSize.cy = pCnsl->txtArrSize.cy;
}

static D4D_INDEX D4D_CnslFindMaxTextLenght(D4D_TCHAR* pText, D4D_FONT font, D4D_INDEX startOffset, D4D_INDEX maxLenght)
{
  D4D_INDEX result;// = _calc.charArrSize.cx;
  D4D_COOR tmp_textWidth;

  result = 0;
  tmp_textWidth = 0;

  while((pText[result] != 0) && ((result + startOffset) < maxLenght))
  {
    if(tmp_textWidth >= (_calc.txtGeom.sz.cx - 4))
      break;

    result++;
    tmp_textWidth += D4D_GetCharWidth(font, pText[result]);
  }

  return result;
}


/*******************************************************
*
* CONSOLE Drawing routine
*
*******************************************************/

static void D4D_ConsoleOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pThis);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_INDEX line_cnt, tmp_realLine;
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size, tmp_winOff;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_strPrties;
  D4D_BOOL tmp_font_proporcional = D4D_FALSE;


  D4D_CnslValue2Coor(pThis);

  clrT = D4D_ObjectGetForeFillColor(pThis);
  clrB = D4D_ObjectGetBckgFillColor(pThis);

  // draw the rectangle around the text
  if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
  {
    D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);

    pData->flags |= (D4D_CNSL_FLAGS_REDRAWALL | D4D_CNSL_FLAGS_REDRAWLINE | D4D_CNSL_FLAGS_REDRAWCURSOR);
  }

  // Draw the frame
  if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
    D4D_DrawFrame(pThis, clrT, clrB);

  tmp_winOff.cx = (D4D_COOR)D4D_ScrlBrGetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis));
  tmp_winOff.cy = (D4D_COOR)D4D_ScrlBrGetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis));
  tmp_strPrties.font_properties = 0;
  tmp_strPrties.text_properties = 0;
  tmp_txtbuff.fontId = pCnsl->textFontId;
  tmp_txtbuff.str_properties = &tmp_strPrties;
  tmp_txtbuff.printOff = 0;


  if(pData->flags & (D4D_CNSL_FLAGS_REDRAWALL | D4D_CNSL_FLAGS_REDRAWLINE))
  {
    // Draw all lines of console
    tmp_point = _calc.txtGeom.pnt;
    tmp_size.cx = _calc.txtGeom.sz.cx;
    tmp_size.cy = D4D_GetFontHeight(pCnsl->textFontId);


    if(D4D_GetFont(pCnsl->textFontId)->pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL)
      tmp_font_proporcional = D4D_TRUE;

    for(line_cnt = 0; line_cnt < _calc.charArrSize.cy; line_cnt++)
    {
      if(!(pData->flags & D4D_CNSL_FLAGS_REDRAWALL))
        if(line_cnt != pData->cursorPos.y)
        {
          tmp_point.y += tmp_size.cy;
          continue;
        }

      // Compute the real line in memory
      tmp_realLine = (D4D_INDEX)((pData->lastLineIx + line_cnt + tmp_winOff.cy) % pCnsl->txtArrSize.cy);

      // found the right pointer of the text to draw in the text array
      tmp_txtbuff.pText = (D4D_TCHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1) + tmp_winOff.cx);

      if((tmp_winOff.cx + _calc.charArrSize.cx) <= pCnsl->txtArrSize.cx)
      {
        if(tmp_font_proporcional)
          tmp_txtbuff.printLen = D4D_CnslFindMaxTextLenght(tmp_txtbuff.pText, pCnsl->textFontId, tmp_winOff.cx, pCnsl->txtArrSize.cx);
        else
          tmp_txtbuff.printLen = _calc.charArrSize.cx;

        tmp_txtbuff.buffSize = 0;

      }
      D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);

      tmp_point.y += tmp_size.cy;
    }
  }

  // redraw cursor
  if(((D4D_INDEX_DELTA)(pData->cursorPos.y - tmp_winOff.cy) >= 0) && ((pData->cursorPos.y - tmp_winOff.cy) < _calc.charArrSize.cy) && (pData->cursorPos.x >= tmp_winOff.cx)) //cursor is on screen - check line and also and check the axis X
  {
    D4D_BOOL cursorVisible = D4D_FALSE;

    // Compute the real line in memory
    tmp_realLine = (D4D_INDEX)((pData->lastLineIx + pData->cursorPos.y) % pCnsl->txtArrSize.cy);

    // found the right pointer of the text to draw in the text array
    tmp_txtbuff.pText = (D4D_TCHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1) + tmp_winOff.cx);

      // Check if the cursor is visible on the line

    tmp_txtbuff.printLen = D4D_CnslFindMaxTextLenght(tmp_txtbuff.pText, pCnsl->textFontId, tmp_winOff.cx, pData->cursorPos.x);

    if(*(tmp_txtbuff.pText + pData->cursorPos.x))
      tmp_size.cx = D4D_GetCharWidth(pCnsl->textFontId, *(tmp_txtbuff.pText + pData->cursorPos.x));
    else
      tmp_size.cx = D4D_GetCharWidth(pCnsl->textFontId, ' ');

    tmp_point.x = D4D_GetTextBuffWidthTab(&tmp_txtbuff, NULL);

    if(tmp_font_proporcional)
    {
      if(tmp_point.x < (D4D_COOR)( _calc.txtGeom.sz.cx - tmp_size.cx))
        cursorVisible = D4D_TRUE;
    }
    else
    {
      if(pData->cursorPos.x < (D4D_INDEX)(_calc.charArrSize.cx + tmp_winOff.cx))
        cursorVisible = D4D_TRUE;
    }

    if(cursorVisible)
    {
      tmp_point.y = (D4D_COOR)((pData->cursorPos.y - tmp_winOff.cy) * D4D_GetFontHeight(pCnsl->textFontId) + _calc.txtGeom.pnt.y);//??+1
      tmp_point.x += _calc.txtGeom.pnt.x;
      tmp_size.cy = (D4D_COOR)((D4D_CNSL_CURSOR_HEIGHT > D4D_GetFontHeight(pCnsl->textFontId))? D4D_CNSL_CURSOR_HEIGHT : D4D_GetFontHeight(pCnsl->textFontId));



      // check the state of cursor and selected right color
      if((!(pData->flags & D4D_CNSL_FLAGS_CURSORSTATE)) || (!tmp_txtbuff.pText[0]))
      {// draw cursor
        D4D_COLOR clr;

        if(pData->flags & D4D_CNSL_FLAGS_CURSORSTATE)
          clr = clrB;
        else
          clr = clrT;

        tmp_point.y += (D4D_GetFontHeight(pCnsl->textFontId) - D4D_CNSL_CURSOR_HEIGHT - 1);
        tmp_size.cy = D4D_CNSL_CURSOR_HEIGHT;

        D4D_FillRect(&tmp_point, &tmp_size, clr);
      }
      else
      {
          // Draw the original char
          tmp_txtbuff.printOff = pData->cursorPos.x;
          tmp_txtbuff.printLen = 1;
          D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);
      }
    }
  }

  pData->flags &= ~(D4D_CNSL_FLAGS_REDRAWALL | D4D_CNSL_FLAGS_REDRAWLINE | D4D_CNSL_FLAGS_REDRAWCURSOR);
}

static void D4D_CnslOnInit(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pMsg->pObject);

  D4D_CnslValue2Coor(pThis);

  if((_calc.charArrSize.cy < pCnsl->txtArrSize.cy)||(_calc.charArrSize.cx < pCnsl->txtArrSize.cx))
  {
    D4D_ScrlBrSetRange(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis), 0, pCnsl->txtArrSize.cx);
    D4D_ScrlBrSetStep(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis), _calc.charArrSize.cx, D4D_CNSL_SCRLBR_STEP_H);
    D4D_ShowObject(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis), D4D_TRUE);

    D4D_ScrlBrSetRange(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), 0, pCnsl->txtArrSize.cy);
    D4D_ScrlBrSetStep(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), _calc.charArrSize.cy, D4D_CNSL_SCRLBR_STEP_V);
    D4D_ShowObject(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), D4D_TRUE);
  }
  else
  {
    D4D_ShowObject(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis), D4D_FALSE);
    D4D_ShowObject(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), D4D_FALSE);
  }

}


static void D4D_ConsoleTimeTick(D4D_MESSAGE* pMsg)
{

  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pMsg->pObject);

  pCnsl->pData->tickCounter++;
  if(pCnsl->pData->tickCounter >= D4D_CNSL_CURSOR_BLINK_TICK_COUNTER)
  {
    pCnsl->pData->tickCounter = 0;
    pCnsl->pData->flags |= D4D_CNSL_FLAGS_REDRAWCURSOR;
    D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);

    if(pCnsl->pData->flags & D4D_CNSL_FLAGS_CURSORSTATE)
      pCnsl->pData->flags &= ~D4D_CNSL_FLAGS_CURSORSTATE;
    else
      pCnsl->pData->flags |= D4D_CNSL_FLAGS_CURSORSTATE;
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

static D4D_BOOL D4D_CnslPutCharInt(D4D_OBJECT_PTR pObj, D4D_TCHAR ch) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  D4D_INDEX tmp_realLine;

  // Compute the real line in memory
  tmp_realLine = (D4D_INDEX)((pData->lastLineIx + pData->cursorPos.y) % pCnsl->txtArrSize.cy);

  if(pData->flags & D4D_CNSL_FLAGS_CHECKLINE) {
    pData->flags &= ~D4D_CNSL_FLAGS_CHECKLINE;
    if(pData->cursorPos.x)
    {
      D4D_INDEX i;
      D4D_TCHAR* pCh;

      pCh = (D4D_TCHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1));

      for(i = 0; i < pData->cursorPos.x; i++) {
        if(*pCh == 0) {
          *pCh = ' ';
        }
        pCh++;
      }
    }
  }

  // found the right pointer of the text to put new char in text array
  *((D4D_TCHAR*)((pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1)) + pData->cursorPos.x)) = ch;

  // move cursor on new char position
  pData->cursorPos.x++;


  // Check if printed char was last on line
  if(pData->cursorPos.x >= pCnsl->txtArrSize.cx) {
    D4D_CnslNewLine(pObj, D4D_TRUE);
    return D4D_TRUE;
  }

  return D4D_FALSE;
}

void D4D_CnslScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_UNUSED(old_position);
  D4D_UNUSED(new_position);

  D4D_InvalidateObject(D4D_GetParentObject(pThis), D4D_FALSE);

  D4D_GET_CONSOLE_DATA(D4D_GetParentObject(pThis))->flags |= (D4D_CNSL_FLAGS_REDRAWALL);
}


static void D4D_CnslUpdateScrollbars(D4D_OBJECT* pThis)
{
  D4D_CNSL_DATA* pData = D4D_GET_CONSOLE_DATA(pThis);


  if(!(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE) && !(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE))
    return;

  D4D_CnslValue2Coor(pThis);

  if(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
  {
    if(pData->cursorPos.x < _calc.charArrSize.cx)
      D4D_ScrlBrSetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis), 0);
    else
      D4D_ScrlBrSetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pThis), (D4D_INDEX)(pData->cursorPos.x - _calc.charArrSize.cx / 2));
  }

  if(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
  {
    if(pData->cursorPos.y < _calc.charArrSize.cy)
      D4D_ScrlBrSetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), 0);
    else
      D4D_ScrlBrSetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), (D4D_INDEX)(pData->cursorPos.y - _calc.charArrSize.cy + 1));
  }

}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslNewLine
// SCOPE:       Console object external API function
// DESCRIPTION: The function move cursor on new line
//
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_BOOL returnCarrier - return  carrier flag
//
// RETURNS:     none
//-----------------------------------------------------------------------------
static void D4D_CnslNewLine(D4D_OBJECT_PTR pObj, D4D_BOOL returnCarrier) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;

  // Check if the new line is last, so whole console will be scrolled by one line
  if((pData->cursorPos.y + 1) >= pCnsl->txtArrSize.cy) {
    // Increment the last line pointer
    pData->lastLineIx++;

    // Check the overroll the buffer runs
    if(pData->lastLineIx >= pCnsl->txtArrSize.cy)
      pData->lastLineIx = 0;

    D4D_CnslClearLine(pObj, (D4D_INDEX)(pCnsl->txtArrSize.cy - 1));

  }else {
    // Increment the cursor position
    pData->cursorPos.y++;
  }


  if(returnCarrier)
  {
    pData->cursorPos.x = 0;
  }else {
    pData->flags |= D4D_CNSL_FLAGS_CHECKLINE;
  }
  pData->flags |= D4D_CNSL_FLAGS_REDRAWALL;
  D4D_CnslUpdateScrollbars((D4D_OBJECT*)pObj);
}

/**************************************************************//*!
*
*
*
*     THE EXTERNAL API FUNCTIONS
*
*
*
******************************************************************/


/******************************************************************************
* Begin of D4D_CONSOLE public functions
*//*! @addtogroup doxd4d_console_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function puts one char on cursor position and updates cursor
* @param   pObj - pointer to the console object
* @param   ch - char that will be puts on console
* @return  D4D_TRUE - cursor was wrapped / D4D_FALSE - cursor was not wrapped
* @note    The result returns if the cursor has been wrapped or not.
*******************************************************************************/
D4D_BOOL D4D_CnslPutChar(D4D_OBJECT_PTR pObj, D4D_TCHAR ch)
{

  D4D_INDEX i, j;
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = D4D_GET_CONSOLE_DATA(pObj);
  D4D_BOOL wrap = D4D_FALSE;

  if(ch >= ' ') {
    wrap = D4D_CnslPutCharInt(pObj, ch);   //Put New char
  }else {
    switch(ch) {
      case '\n': // new line
        D4D_CnslNewLine(pObj, D4D_FALSE);
        wrap = D4D_TRUE;
        break;

      case '\r': // return carrier
        pData->cursorPos.x = 0;
        break;

      case '\t': // Tab
        j = (D4D_INDEX)(pData->cursorPos.x % pCnsl->tabSize);

        if(!j)
          j = pCnsl->tabSize;

        for(i = 0; i < j; j++)
          if(D4D_CnslPutCharInt(pObj, ' '))   // if the tab go to new line end operation
          {
            wrap = D4D_TRUE;
            break;
          }
        break;

      case '\b': // backspace
        // if there is no needed return back to previous line
        if(pData->cursorPos.x)
        {
          pData->cursorPos.x--; //Go back one char
          D4D_CnslPutCharInt(pObj, ' ');   //And put space char
        }else
        {
          // needed return to previous line
          if(pData->cursorPos.y)
          {
            D4D_INDEX i;
            D4D_TCHAR* pCh;

            pData->cursorPos.y--;
            pData->cursorPos.x = pCnsl->txtArrSize.cx;

            pCh = (D4D_TCHAR*)(pCnsl->pTxtArr + ((pData->lastLineIx + pData->cursorPos.y) % pCnsl->txtArrSize.cy) * (pCnsl->txtArrSize.cx + 1));

            // This setup all non space character including the last one
            for(i = 0; i <= pData->cursorPos.x; i++)
            {
              if(*pCh == 0)
                *pCh = ' ';
              pCh++;
            }
          }
        }

        break;
    }
  }

  D4D_CnslUpdateScrollbars((D4D_OBJECT*)pObj);
  D4D_InvalidateObject(pObj, D4D_FALSE);
  pData->flags |= D4D_CNSL_FLAGS_REDRAWLINE;
  return wrap;
}

/**************************************************************************/ /*!
* @brief   The function puts string on cursor position and updates cursor
* @param   pObj - pointer to the console object
* @param   pText - string that will be puts on console
* @return  D4D_TRUE - cursor was wrapped / D4D_FALSE - cursor was not wrapped
* @note    The result returns if the cursor has been wrapped or not.
*******************************************************************************/
D4D_BOOL D4D_CnslPutString(D4D_OBJECT_PTR pObj, D4D_TCHAR* pText) {

D4D_BOOL wrap = D4D_FALSE;
D4D_POINT tmp_scrollbars;

  if(pText == NULL)
    return D4D_FALSE;

  if(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pObj)->pData->flags & D4D_OBJECT_F_VISIBLE)
    tmp_scrollbars.x = (D4D_COOR)D4D_ScrlBrGetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pObj));

  tmp_scrollbars.y = D4D_GET_CONSOLE_DATA(pObj)->cursorPos.y;


  while(*pText) {
    if(D4D_CnslPutChar(pObj, *pText))
      wrap = D4D_TRUE;

    pText++;
  }

  if(tmp_scrollbars.y == D4D_GET_CONSOLE_DATA(pObj)->cursorPos.y)
  {
    if(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pObj)->pData->flags & D4D_OBJECT_F_VISIBLE)
      if(tmp_scrollbars.x != D4D_ScrlBrGetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pObj)))
        return wrap;

    D4D_GET_CONSOLE_DATA(pObj)->flags &= ~D4D_CNSL_FLAGS_REDRAWALL;
  }

  return wrap;
}

/**************************************************************************/ /*!
* @brief   The function clear one line in console
* @param   pObj - pointer to the console object
* @param   line - index of line that should be cleared
* @return  none
* @note    none
*******************************************************************************/
void D4D_CnslClearLine(D4D_OBJECT_PTR pObj, D4D_INDEX line) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  D4D_INDEX tmp_realLine, ix;
  D4D_TCHAR* tmp_charAdrr;

  if(line >= pCnsl->txtArrSize.cy)
    return;

  // Compute the real line in memory
  tmp_realLine = (D4D_INDEX)((pData->lastLineIx + line) % pCnsl->txtArrSize.cy);
  tmp_charAdrr = (D4D_TCHAR*)((tmp_realLine * (pCnsl->txtArrSize.cx + 1)) + pCnsl->pTxtArr);

  for(ix = 0;ix <= pCnsl->txtArrSize.cx; ix++)
    *(tmp_charAdrr++) = 0;
}

/**************************************************************************/ /*!
* @brief   The function move cursor on new position if possible
* @param   pObj - pointer to the console object
* @param   newPosition - new cursor position
* @return  none
* @note    The new position of cursor is set if it's possible.
*******************************************************************************/
void D4D_CnslGoToXY(D4D_OBJECT_PTR pObj, D4D_POINT newPosition) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;

  if(newPosition.x >= pCnsl->txtArrSize.cx)
    return;

  if(newPosition.y >= pCnsl->txtArrSize.cy)
    return;

  pData->cursorPos = newPosition;

  pData->flags |= (D4D_CNSL_FLAGS_REDRAWALL | D4D_CNSL_FLAGS_CHECKLINE);
}

/**************************************************************************/ /*!
* @brief   The function returns the cursor position
* @param   pObj - pointer to the console object
* @return  cursor position
* @note    none
*******************************************************************************/
D4D_POINT D4D_CnslGetCursor(D4D_OBJECT_PTR pObj)
{
  D4D_CNSL_DATA* pData = D4D_GET_CONSOLE_DATA(pObj);

  if(pObj == NULL)
    return d4d_point_zero;

  return pData->cursorPos;
}

/**************************************************************************/ /*!
* @brief   The function update automatically the scroll bars to ensure visible position of cursor
* @param   pObj - pointer to the console object
* @return  none
* @note    none
*******************************************************************************/
void D4D_CnslEnsureVisible(D4D_OBJECT_PTR pObj)
{
  if(pObj == NULL)
    return;

  D4D_CnslUpdateScrollbars((D4D_OBJECT*)pObj);
}

/**************************************************************************/ /*!
* @brief   The function move the scroll bars on to new position if it's applicable
* @param   pObj - pointer to the console object
* @param   hor - new position of horizontal scroll bar
* @param   ver - new position of vertical scroll bar
* @return  none
* @note    none
*******************************************************************************/
void D4D_CnslSetScrollBarPosition(D4D_OBJECT_PTR pObj, D4D_INDEX hor, D4D_INDEX ver)
{
  if(pObj == NULL)
    return;

  if(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pObj)->pData->flags & D4D_OBJECT_F_VISIBLE)
    D4D_ScrlBrSetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pObj), hor);

  if(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pObj)->pData->flags & D4D_OBJECT_F_VISIBLE)
    D4D_ScrlBrSetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pObj), ver);

}

/**************************************************************************/ /*!
* @brief   The function returns the scroll bars position
* @param   pObj - pointer to the console object
* @return  scroll bars position
* @note    none
*******************************************************************************/
D4D_POINT D4D_CnslGetScrollBarPosition(D4D_OBJECT_PTR pObj)
{
  D4D_POINT res;

  if(pObj == NULL)
    return d4d_point_zero;

  res.x = (D4D_COOR)D4D_ScrlBrGetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_HOR(pObj));
  res.y = (D4D_COOR)D4D_ScrlBrGetPosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pObj));

  return res;
}

/**************************************************************************/ /*!
* @brief   The function clear whole console data
* @param   pObj - pointer to the console object
* @return  none
* @note    It's take something like software reset to default state of console.
*******************************************************************************/
void D4D_CnslClearAll(D4D_OBJECT_PTR pObj) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;

  LWord i,i_max;

  // Init the text array
  i_max = (LWord)(pCnsl->txtArrSize.cy * (pCnsl->txtArrSize.cx + 1));

  for(i=0;i<i_max;i++)
    pCnsl->pTxtArr[i] = 0;

  pData->cursorPos.x =0;
  pData->cursorPos.y =0;

  pData->flags &= ~D4D_CNSL_FLAGS_CHECKLINE;
  pData->flags |= D4D_CNSL_FLAGS_REDRAWALL;
  D4D_InvalidateObject(pObj, D4D_FALSE);

}
/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_console_func                                              */
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
* The CONSOLE message handler
*
*******************************************************/

void D4D_ConsoleOnMessage(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;

  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_ConsoleOnDraw(pMsg);
      break;

    case D4D_MSG_ONINIT:
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;
      D4D_CnslOnInit(pMsg);
      break;

#ifdef D4D_LLD_MOUSE
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
      D4D_FocusSet(pMsg->pScreen, pThis);
      D4D_CaptureKeys(pThis);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_UP:
      if(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
        D4D_ScrlBrChangePosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), -1);
      break;
    case D4D_MSG_MOUSE_BTN_WHEEL_DOWN:
      if(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
        D4D_ScrlBrChangePosition(D4D_GET_LIST_BOX_SCROLL_BAR_VER(pThis), 1);
      break;
#endif


#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
      D4D_FocusSet(pMsg->pScreen, pThis);
      D4D_CaptureKeys(pThis);
      break;
#endif
    case D4D_MSG_KEYUP:
     {
      D4D_KEY_SCANCODE tmp_key = pMsg->prm.key;
      // capture the keyboard if enter is pressed
      if(tmp_key == D4D_KEY_SCANCODE_ENTER)
        D4D_CaptureKeys(pThis);

      // exit capture
      if(tmp_key == D4D_KEY_SCANCODE_ESC)
        D4D_CaptureKeys(NULL);
      }
      break;

    case D4D_MSG_TIMETICK:
      #if (D4D_CNSL_CURSOR_BLINK_TICK_COUNTER > 0)
        D4D_ConsoleTimeTick(pMsg);
      #endif
      break;

    case D4D_MSG_KILLFOCUS:
      D4D_CaptureKeys(NULL);
    case D4D_MSG_SETFOCUS:
    case D4D_MSG_SETCAPTURE:
    case D4D_MSG_KILLCAPTURE:
      D4D_InvalidateObject(pThis, D4D_FALSE);
      break;


    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}
