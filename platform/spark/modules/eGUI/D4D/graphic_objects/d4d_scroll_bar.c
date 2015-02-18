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
* @file      d4d_scroll_bar.c
*
* @author     Petr Gargulak
*
* @version   0.0.21.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver scroll_bar object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"


// Internal API
static void D4D_ScrlBrOnMessage(D4D_MESSAGE* pMsg);
static void D4D_ScrlBrOnKeyDown(D4D_MESSAGE* pMsg);

#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  static void D4D_ScrlBrOnTouch(D4D_MESSAGE* pMsg, D4D_POINT* point);
#endif
static void D4D_ScrlBrDrawArrow(D4D_POINT* ppt, D4D_COOR size, D4D_DIRECTION dir, D4D_COLOR color);



const D4D_OBJECT_SYS_FUNCTION d4d_scroll_barSysFunc =
{
  D4D_DEFSTR("Scroll Bar"),
  D4D_ScrlBrOnMessage,
  NULL,
  NULL
};

typedef struct
{
  D4D_POINT position;
  D4D_GEOMETRY contentGeom;
  D4D_POINT scrollPos;
  D4D_SIZE scrollSize;
  D4D_POINT refreshPos;
  D4D_SIZE refreshSize;
  D4D_COOR scrollRadius;
} D4D_SCRLBAR_TMP_VAL;

#define _calc (*((D4D_SCRLBAR_TMP_VAL*)d4d_scratchPad))
//static D4D_SCRLBAR_TMP_VAL _calc;

static void D4D_ScrlBrValue2Coor(D4D_OBJECT* pThis)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pThis);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;

  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);


  _calc.scrollRadius = pThis->radius;


 if(pThis->size.cx > pThis->size.cy)
  {
    // horizontal scroll bar
    _calc.refreshPos = _calc.contentGeom.pnt;
    _calc.refreshPos.x += (D4D_COOR)(_calc.contentGeom.sz.cy + 1);
    //_calc.refreshPos.y++;

    _calc.refreshSize = _calc.contentGeom.sz;
    _calc.refreshSize.cx -= (D4D_COOR)((_calc.contentGeom.sz.cy * 2) + 2);
    //_calc.refreshSize.cy -= 2;

    _calc.scrollSize = _calc.refreshSize;


    _calc.scrollSize.cy -= 2;

    _calc.scrollSize.cx = (D4D_COOR)D4D_MulDivUU16(_calc.refreshSize.cx, (Word)pData->page, (Word)(pData->maximum - pData->minimum));

    if(_calc.scrollSize.cx < D4D_SCRLBAR_MIN_TAB_SIZE)
      _calc.scrollSize.cx = D4D_SCRLBAR_MIN_TAB_SIZE;

    _calc.scrollPos = _calc.refreshPos;
    if(pData->maximum - pData->page - pData->minimum)
      _calc.scrollPos.x += (D4D_COOR)(D4D_MulDivUU16((Word)(_calc.refreshSize.cx - _calc.scrollSize.cx - 1), (Word)pData->position, (Word)(pData->maximum - pData->page - pData->minimum)) + 1);
    _calc.scrollPos.y++;


  }else
  {
    // vertical scroll bar
    _calc.refreshPos = _calc.contentGeom.pnt;
    _calc.refreshPos.y += (D4D_COOR)(_calc.contentGeom.sz.cx + 1);
   // _calc.refreshPos.x++;

    _calc.refreshSize = _calc.contentGeom.sz;
    //_calc.refreshSize.cx -= 2;
    _calc.refreshSize.cy -= (D4D_COOR)((_calc.contentGeom.sz.cx * 2) + 2);

    _calc.scrollSize = _calc.refreshSize;

    _calc.scrollSize.cx -= 2;

    _calc.scrollSize.cy = (D4D_COOR)D4D_MulDivUU16((Word)_calc.refreshSize.cy, (Word)pData->page, (Word)(pData->maximum - pData->minimum));

    if(_calc.scrollSize.cy < D4D_SCRLBAR_MIN_TAB_SIZE)
      _calc.scrollSize.cy = D4D_SCRLBAR_MIN_TAB_SIZE;

    _calc.scrollPos = _calc.refreshPos;
    if(pData->maximum - pData->page - pData->minimum)
      _calc.scrollPos.y += (D4D_COOR)(D4D_MulDivUU16((Word)(_calc.refreshSize.cy - _calc.scrollSize.cy - 1), (Word)pData->position, (Word)(pData->maximum - pData->page - pData->minimum)) + 1);
    _calc.scrollPos.x++;
  }


  if(_calc.scrollRadius)
    _calc.scrollRadius = (D4D_COOR)(pThis->radius - ((pThis->size.cx - _calc.contentGeom.sz.cx) / 2));

}



/*******************************************************
*
* SCROLL_BAR Drawing routine
*
*******************************************************/

static void D4D_ScrlBrOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;

  D4D_ScrlBrValue2Coor(pThis);

  clrT = D4D_ObjectGetForeColor(pThis, draw);
  clrB = D4D_ObjectGetBckgColor(pThis, draw);

  // draw the rectangle around the text
  if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);

  // Draw the frame & arrow buttons
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
    {
      D4D_DrawFrame(pThis, clrT, clrB);

      tmp_point = _calc.contentGeom.pnt;

      if(pThis->size.cx > pThis->size.cy)
      {
        // horizontal scroll bar
        // draw first arrow button left

        tmp_size.cx = tmp_size.cy = _calc.contentGeom.sz.cy;

        D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, _calc.scrollRadius);

        tmp_point.x += (D4D_COOR)(_calc.contentGeom.sz.cx - _calc.contentGeom.sz.cy);
        D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, _calc.scrollRadius);

        tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cy / 8);
        tmp_point.y += (D4D_COOR)(_calc.contentGeom.sz.cy / 2);
        D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((_calc.contentGeom.sz.cy / 2) - 2),D4D_DIR_LEFT,clrT);

        // draw second arrow button right
        tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cx - _calc.contentGeom.sz.cy / 8);
        D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((_calc.contentGeom.sz.cy / 2) - 2),D4D_DIR_RIGHT,clrT);

      }else
      {
        // vertical scroll bar
        tmp_size.cx = tmp_size.cy = _calc.contentGeom.sz.cx;

        D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, _calc.scrollRadius);

        tmp_point.y += (D4D_COOR)(_calc.contentGeom.sz.cy - _calc.contentGeom.sz.cx);
        D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, _calc.scrollRadius);

        tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.contentGeom.sz.cx / 8);
        tmp_point.x += (D4D_COOR)(_calc.contentGeom.sz.cx / 2);
        D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((_calc.contentGeom.sz.cx / 2) - 2),D4D_DIR_UP,clrT);

        // draw second arrow button right
        tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.contentGeom.sz.cy - _calc.contentGeom.sz.cx / 8);
        D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((_calc.contentGeom.sz.cx / 2) - 2),D4D_DIR_DOWN,clrT);
      }
    }

  D4D_FillRect(&_calc.refreshPos, &_calc.refreshSize, clrB);
  D4D_FillRRect(&_calc.scrollPos, &_calc.scrollSize, clrT, _calc.scrollRadius);

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

void D4D_ScrlBrDrawArrow(D4D_POINT* ppt, D4D_COOR size, D4D_DIRECTION dir, D4D_COLOR color)
{

  D4D_COOR i,j;
  D4D_POINT tmp_point = {0, 0};
  D4D_POINT tmp_pointReal;
  D4D_BOOL swap = D4D_FALSE;

  if((dir == D4D_DIR_LEFT) || (dir == D4D_DIR_RIGHT))
    swap = D4D_TRUE;

  D4D_PutPixel(ppt, D4D_LINE_THIN, color);

  for(i = 0; i < size; i++)
  {
    if((dir == D4D_DIR_DOWN) || (dir == D4D_DIR_RIGHT))
      tmp_point.y--;
    else
      tmp_point.y++;

    tmp_point.x--;

    if(swap)
      tmp_pointReal = D4D_SwapCoor(tmp_point);
    else
      tmp_pointReal = tmp_point;

    tmp_pointReal.x += ppt->x;
    tmp_pointReal.y += ppt->y;


    D4D_MoveTo(&tmp_pointReal);

    j = tmp_point.x;
    tmp_point.x += (i+1) * 2;

    if(swap)
      tmp_pointReal = D4D_SwapCoor(tmp_point);
    else
      tmp_pointReal = tmp_point;

    tmp_pointReal.x += ppt->x;
    tmp_pointReal.y += ppt->y;

      D4D_LineTo(&tmp_pointReal, D4D_LINE_THIN, color);

    tmp_point.x = j;
  }
}

#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  static void D4D_ScrlBrOnTouch(D4D_MESSAGE* pMsg, D4D_POINT* point)
  {
    D4D_OBJECT* pThis = pMsg->pObject;

    if(pThis->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_FASTTOUCH))
    {
      if((D4D_GetFocusedObject(pMsg->pScreen) == pThis) || (pThis->pData->flags & D4D_OBJECT_F_FASTTOUCH))
      {
        D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pThis);
        D4D_SCRLBAR_DATA* pData = pScrlBr->pData;

        if(pThis->pData->flags & D4D_OBJECT_F_TABSTOP)
          D4D_CaptureKeys(pThis);

        if(pThis->size.cx > pThis->size.cy)
        {
          if(point->x > ((pThis->size.cx / 2) + pThis->position.x))
            D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(pData->step));
          else
            D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(-1 * pData->step));
        }else
        {

          if(point->y > ((pThis->size.cy / 2) + pThis->position.y))
            D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(pData->step));
          else
            D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(-1 * pData->step));
        }


      }
      if(pThis->pData->flags & D4D_OBJECT_F_TABSTOP)
        D4D_FocusSet(pMsg->pScreen, pThis);
    }
  }
#endif


static void D4D_ScrlBrOnKeyDown(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pThis);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;

  if(pThis->size.cx > pThis->size.cy)
  {
    if(pMsg->prm.key == D4D_KEY_SCANCODE_LEFT)
      D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(-1 * pData->step));
    else if(pMsg->prm.key == D4D_KEY_SCANCODE_RIGHT)
      D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(pData->step));
  }else
  {
    if(pMsg->prm.key == D4D_KEY_SCANCODE_UP)
      D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(-1 * pData->step));
    else if(pMsg->prm.key == D4D_KEY_SCANCODE_DOWN)
      D4D_ScrlBrChangePosition(pThis, (D4D_INDEX_DELTA)(pData->step));
  }

}



/******************************************************************************
* Begin of D4D_SCROLL_BAR public functions
*//*! @addtogroup doxd4d_scroll_bar_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function sets the range of scroll bar scale
* @param   pObj - pointer to the scroll_bar object.
* @param   minimum - minimum value of scroll bar
* @param   maximum - maximum value of scroll bar
* @return  none
* @note    none
*******************************************************************************/
void D4D_ScrlBrSetRange(D4D_OBJECT_PTR pObj, D4D_INDEX minimum, D4D_INDEX maximum)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;


  if(maximum < minimum)
    return;

  pData->minimum = minimum;
  pData->maximum = maximum;

  if(pData->step > pData->maximum)
    pData->step = 1;

  if(pData->page > pData->maximum)
    pData->page = 0;

  if(pData->position > pData->maximum)
    pData->position = pData->maximum;

  if(pData->position < pData->minimum)
    pData->position = pData->minimum;

  D4D_InvalidateObject(pObj, D4D_FALSE);
}

/**************************************************************************/ /*!
* @brief   The function sets the step and page of scroll bar scale
* @param   pObj - pointer to the scroll_bar object.
* @param   page - page value of scroll bar
* @param   step - step value of scroll bar
* @return  none
* @note    none
*******************************************************************************/
void D4D_ScrlBrSetStep(D4D_OBJECT_PTR pObj, D4D_INDEX page, D4D_INDEX step)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;


  if(step > (pData->maximum - pData->minimum))
    return;

  if(page > (pData->maximum - pData->minimum))
    return;

  pData->step = step;
  pData->page = page;

  D4D_InvalidateObject(pObj, D4D_FALSE);
}

/**************************************************************************/ /*!
* @brief   The function sets the scroll bar position
* @param   pObj - pointer to the scroll_bar object.
* @param   position -  value of scroll bar position
* @return  none
* @note    none
*******************************************************************************/
void D4D_ScrlBrSetPosition(D4D_OBJECT_PTR pObj, D4D_INDEX position)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  D4D_INDEX old_pos = pData->position;

  if(position > (D4D_INDEX)(pData->maximum - pData->page))
    pData->position = (D4D_INDEX)(pData->maximum - pData->page);
  else if(position < pData->minimum)
    pData->position = pData->minimum;
  else
    pData->position = position;

  if(pData->position == old_pos)
    return;

  if(pScrlBr->OnChange != NULL)
    pScrlBr->OnChange((D4D_OBJECT*)pObj, old_pos, pData->position);

  D4D_InvalidateObject(pObj, D4D_FALSE);

}

/**************************************************************************/ /*!
* @brief   The function gets the scroll bar position
* @param   pObj - pointer to the scroll_bar object.
* @return  value of scroll bar position
* @note    none
*******************************************************************************/
D4D_INDEX D4D_ScrlBrGetPosition(D4D_OBJECT_PTR pObj)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;

  return pData->position;
}

/**************************************************************************/ /*!
* @brief   The function change the scroll bar position
* @param   pObj - pointer to the scroll_bar object
* @param   change - change of scroll bar position
* @return  none
* @note    none
*******************************************************************************/
void D4D_ScrlBrChangePosition(D4D_OBJECT_PTR pObj, D4D_INDEX_DELTA change)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;

  D4D_INDEX_DELTA tmp_pos = (D4D_INDEX_DELTA)(change + pData->position);

  if(tmp_pos < 0)
    tmp_pos = 0;

  D4D_ScrlBrSetPosition(pObj, (D4D_INDEX)tmp_pos);

}
/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_scroll_bar_func                                          */
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
* The SCROLL_BAR message handler
*
*******************************************************/

void D4D_ScrlBrOnMessage(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  D4D_POINT touchClickPoint;
#endif
  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_ScrlBrOnDraw(pMsg);
      break;

    case D4D_MSG_KEYDOWN:
      D4D_ScrlBrOnKeyDown(pMsg);
      break;

#ifdef D4D_LLD_MOUSE
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
        touchClickPoint = D4D_GetMouseCoordinates(pMsg->pObject);
        D4D_ScrlBrOnTouch(pMsg, &touchClickPoint);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_UP:
      D4D_ScrlBrChangePosition(pThis, -1);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_DOWN:
      D4D_ScrlBrChangePosition(pThis, 1);
      break;
#endif

#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
    case D4D_MSG_TOUCH_AUTO:
      touchClickPoint = D4D_GetTouchScreenCoordinates(pMsg->pObject);
      D4D_ScrlBrOnTouch(pMsg, &touchClickPoint);
      break;
#endif

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

