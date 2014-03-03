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
* @file      d4d_button.c
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.59.0
*
* @date      Feb-19-2013
*
* @brief     D4D driver button object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_BtnOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_BtnGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_btnSysFunc =
{
  D4D_DEFSTR("Button"),
  D4D_BtnOnMessage,
  NULL,
  D4D_BtnGetTextBuffer
};

typedef struct
{
    D4D_POINT Position;
    D4D_GEOMETRY contentGeom;
    D4D_POINT ButPos;
    D4D_SIZE ButSize;
    D4D_COOR ButRadius;
    D4D_GEOMETRY corGeom1;
    D4D_GEOMETRY corGeom2;
} D4D_BTN_TMP_VAL;

#define _calc (*((D4D_BTN_TMP_VAL*)d4d_scratchPad))

static void D4D_BtnValue2Coor(D4D_OBJECT* pThis)
{
  D4D_BUTTON* pBtn = D4D_GET_BUTTON(pThis);

  _calc.Position = D4D_GetClientToScreenPoint(pThis, &pThis->position);

  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);

  //---------------- Get Max size of content  -----------------
  // 1. compare bitmap sizes
  // Get normal bitmap size if exists

  _calc.ButSize = _calc.contentGeom.sz;

  if(pThis->initFlags & D4D_BTN_F_3D)
  {
    _calc.ButSize.cx -= D4D_BTN_3D_SHIFT;
    _calc.ButSize.cy -= D4D_BTN_3D_SHIFT;
  }

// 3D Buttons Enabled
  if(pThis->initFlags & D4D_BTN_F_3D)
  {
    _calc.ButPos = _calc.contentGeom.pnt;

    _calc.corGeom1.sz.cy = (D4D_COOR) D4D_BTN_3D_SHIFT;
    _calc.corGeom2.sz.cx = (D4D_COOR) D4D_BTN_3D_SHIFT;

    _calc.corGeom1.pnt = _calc.corGeom2.pnt = _calc.ButPos;


  #if (D4D_ROUND_CORNER_ENABLE != D4D_FALSE) //if Round corners

    _calc.ButRadius = 0;

    if(pThis->radius)
    {
      if(pThis->radius > D4D_BEVEL_WIDTH - 1)
        _calc.ButRadius = (D4D_COOR)(pThis->radius - D4D_BEVEL_WIDTH);

      _calc.corGeom1.sz.cx = (D4D_COOR)(_calc.ButSize.cx - _calc.ButRadius * 2);
      _calc.corGeom2.sz.cy = (D4D_COOR)(_calc.ButSize.cy - _calc.ButRadius * 2);

      _calc.corGeom1.pnt.x += _calc.ButRadius;
      _calc.corGeom2.pnt.y += _calc.ButRadius;


    }else
  #endif
    {
      _calc.corGeom1.sz.cx = (D4D_COOR)(_calc.ButSize.cx + D4D_BTN_3D_SHIFT);
      _calc.corGeom2.sz.cy = (D4D_COOR)(_calc.ButSize.cy + D4D_BTN_3D_SHIFT);
    }


    if(*(pBtn->pStatus) & D4D_BUTTON_STATUS_PRESSED_MASK)
    {
      _calc.ButPos.x +=  D4D_BTN_3D_SHIFT;
      _calc.ButPos.y +=  D4D_BTN_3D_SHIFT;
    }else
    {
      _calc.corGeom1.pnt.y += _calc.ButSize.cy;
      _calc.corGeom2.pnt.x += _calc.ButSize.cx;

    }
  }else
  {
    #if (D4D_ROUND_CORNER_ENABLE != D4D_FALSE) //if Round corners
      _calc.ButRadius = pThis->radius;
      if(_calc.ButRadius)
        if(pThis->initFlags & D4D_OBJECT_F_FOCUSRECT)
          _calc.ButRadius -= pThis->size.cx - _calc.contentGeom.sz.cx ;
    #endif

    _calc.ButPos = _calc.contentGeom.pnt;
  }
  //D4D_BTN_3D_ENABLE

}


/*******************************************************
*
* BUTTON Drawing routine
*
*******************************************************/

static void D4D_BtnOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_BUTTON* pBtn = D4D_GET_BUTTON(pThis);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    const D4D_BMP* pBmp = pBtn->pBmpNormal;
    D4D_BOOL pressed = *(pBtn->pStatus) & D4D_BUTTON_STATUS_PRESSED_MASK;

//-------- 3D Buttons Enabled --------
    D4D_BMP_PROPERTIES bmpProp;

    bmpProp = (D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_CENTER_MASK);

    D4D_BtnValue2Coor(pThis);

    // focused bitmap (may be also NULL)

    if (pressed && (pThis->initFlags & D4D_BTN_F_INVERT)) {
      clrB = D4D_ObjectGetForeColor(pThis, draw);
      clrT = D4D_ObjectGetBckgColor(pThis, draw);
    } else {
      clrT = D4D_ObjectGetForeColor(pThis, draw);
      clrB = D4D_ObjectGetBckgColor(pThis, draw);
    }

    if((draw & D4D_OBJECT_DRAWFLAGS_FOCUSED) && (pBtn->pBmpFocus))
      pBmp = pBtn->pBmpFocus;

    if(pThis->initFlags & D4D_BTN_F_3D)
    {
      // Draw Button Corners
      if(pThis->radius)
      {
        D4D_POINT shPos = _calc.Position;

        shPos.x += pThis->radius;
        shPos.y += pThis->radius;
        D4D_FillQuadrant(&shPos, pThis->radius, clrB, D4D_QUAD_4);
        shPos.x += pThis->size.cx - pThis->radius * 2 - 1;
        D4D_FillQuadrant(&shPos, pThis->radius, clrB, D4D_QUAD_1);
        shPos.y += pThis->size.cy - pThis->radius * 2 - 1;
        D4D_FillQuadrant(&shPos, pThis->radius, clrB, D4D_QUAD_2);
        shPos.x -= (pThis->size.cx - pThis->radius * 2 - 1);
        D4D_FillQuadrant(&shPos, pThis->radius, clrB, D4D_QUAD_3);
      }

      // draw the shading
      if(pressed)
      {
        if(pThis->pData->flags & D4D_OBJECT_F_BEVEL_MASK)
          if((pThis->pData->flags & D4D_OBJECT_F_BEVEL_MASK) == D4D_OBJECT_F_BEVEL_RAISED)
          {
            pThis->pData->flags &= ~D4D_OBJECT_F_BEVEL_MASK;
            pThis->pData->flags |= D4D_OBJECT_F_BEVEL_DENTED;
          }else
          {
            pThis->pData->flags &= ~D4D_OBJECT_F_BEVEL_MASK;
            pThis->pData->flags |= D4D_OBJECT_F_BEVEL_RAISED;
          }

        D4D_DrawFrame(pThis, clrT, clrB);

        if(pThis->pData->flags & D4D_OBJECT_F_BEVEL_MASK) {
          if((pThis->pData->flags & D4D_OBJECT_F_BEVEL_MASK) == D4D_OBJECT_F_BEVEL_RAISED)
          {
            pThis->pData->flags &= ~D4D_OBJECT_F_BEVEL_MASK;
            pThis->pData->flags |= D4D_OBJECT_F_BEVEL_DENTED;
          }else
          {
            pThis->pData->flags &= ~D4D_OBJECT_F_BEVEL_MASK;
            pThis->pData->flags |= D4D_OBJECT_F_BEVEL_RAISED;
          }
        }
      }else
      {
        D4D_DrawFrame(pThis, clrT, clrB);
      }

      D4D_FillRect(&_calc.corGeom1.pnt, &_calc.corGeom1.sz,clrB);
      D4D_FillRect(&_calc.corGeom2.pnt, &_calc.corGeom2.sz,clrB);

    }else if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

  // Draw the content of button

  pBtn->textBuff.str_properties->font_properties &= ~D4D_FNT_PRTY_TRANSPARENT_MASK;

  // Draw the bitmap
  if(pBmp != NULL)
  {
    D4D_DrawRBmpRect(&_calc.ButPos,&_calc.ButSize, pBmp, bmpProp, clrB, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
    pBtn->textBuff.str_properties->font_properties |= D4D_FNT_PRTY_TRANSPARENT_MASK;
  }

  // draw the text
  if(pBtn->textBuff.pText)
    D4D_DrawTextRRectTab(&_calc.ButPos, &_calc.ButSize,&pBtn->textBuff, NULL, clrT, clrB, _calc.ButRadius);

  if(!pBtn->textBuff.pText && !pBmp)
    D4D_FillRRect(&_calc.ButPos, &_calc.ButSize, clrB, pThis->radius);

   if(pThis->initFlags & D4D_BTN_F_CONT_RECT)
    D4D_RRect(&_calc.ButPos, &_calc.ButSize, D4D_LINE_THIN, clrT, _calc.ButRadius);

}

/*******************************************************
*
* BUTTON key handling routine
*
*******************************************************/

static void D4D_BtnOnKeyDown(D4D_MESSAGE* pMsg)
{
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
  {
    D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);
    *pStatus |= D4D_BUTTON_STATUS_PRESSED_MASK;
    D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);

  }
}

/*******************************************************
*
* BUTTON key handling routine
*
*******************************************************/

static void D4D_BtnOnKeyUp(D4D_MESSAGE* pMsg)
{

  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
  {
    D4D_BUTTON* pBtn = D4D_GET_BUTTON(pMsg->pObject);
    D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);

    if(*pStatus & D4D_BUTTON_STATUS_PRESSED_MASK)
    {
      *pStatus &= ~D4D_BUTTON_STATUS_PRESSED_MASK;
      if(pBtn->OnClicked)
          pBtn->OnClicked(pMsg->pObject);
    }

    D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
  }
}

/*******************************************************
*
* BUTTON focus handling routine
*
*******************************************************/

static void D4D_BtnKillFocus(D4D_MESSAGE* pMsg)
{
  D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);

  *pStatus &= ~D4D_BUTTON_STATUS_MOUSE_CLICK_MASK;

  if(*pStatus & D4D_BUTTON_STATUS_PRESSED_MASK)
  {
    *pStatus &= ~D4D_BUTTON_STATUS_PRESSED_MASK;
    D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
  }

}

/*******************************************************
*
* BUTTON Touched Button handling routine
*
*******************************************************/
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
static void D4D_BtnTouched(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;

  if(pThis->pData->flags & D4D_OBJECT_F_TABSTOP)
  {
    D4D_BUTTON* pBtn = D4D_GET_BUTTON(pThis);

    if((D4D_GetFocusedObject(pMsg->pScreen) == pThis) || (pThis->pData->flags & D4D_OBJECT_F_FASTTOUCH))
    {
       *(pBtn->pStatus) |= D4D_BUTTON_STATUS_PRESSED_MASK;
       *(pBtn->pStatus) &= ~D4D_BUTTON_STATUS_1ST_TOUCH_MASK;
    }else
       *(pBtn->pStatus) |= D4D_BUTTON_STATUS_1ST_TOUCH_MASK;

    D4D_FocusSet(pMsg->pScreen, pThis);

    D4D_InvalidateObject(pThis, D4D_FALSE);
  }
}

/*******************************************************
*
* BUTTON UnTouched Button handling routine
*
*******************************************************/
static void D4D_BtnUnTouched(D4D_MESSAGE* pMsg)
{
    D4D_BUTTON* pBtn = D4D_GET_BUTTON(pMsg->pObject);
    D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);

    if(*pStatus & D4D_BUTTON_STATUS_PRESSED_MASK)
    {
      *pStatus &= ~D4D_BUTTON_STATUS_PRESSED_MASK;
      if(pBtn->OnClicked)
        pBtn->OnClicked(pMsg->pObject);

      D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
    }

}


#endif

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
* The main BUTTON message handler
*
*******************************************************/

void D4D_BtnOnMessage(D4D_MESSAGE* pMsg)
{
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
        D4D_OBJECT* pThis = pMsg->pObject;
        D4D_BUTTON* pBtn = D4D_GET_BUTTON(pThis);
#endif

	switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_BtnOnDraw(pMsg);
        break;

    case D4D_MSG_KEYDOWN:
        D4D_BtnOnKeyDown(pMsg);
        break;

    case D4D_MSG_KEYUP:
        D4D_BtnOnKeyUp(pMsg);
        break;

    case D4D_MSG_KILLFOCUS:
        D4D_BtnKillFocus(pMsg);
        break;

   #if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
    case D4D_MSG_MOUSE_BTN_LEFT_DOWN:
      *(pBtn->pStatus) |= D4D_BUTTON_STATUS_MOUSE_CLICK_MASK;

    case D4D_MSG_TOUCHED:
        D4D_BtnTouched(pMsg);
      break;

    case D4D_MSG_MOUSE_ENTER:
      if((*(pBtn->pStatus) & D4D_BUTTON_STATUS_MOUSE_CLICK_MASK) == 0)
        break;

    case D4D_MSG_TOUCH_AUTO:
        if((*(pBtn->pStatus) & (D4D_BUTTON_STATUS_PRESSED_MASK | D4D_BUTTON_STATUS_1ST_TOUCH_MASK)) == 0)
        {
          *(pBtn->pStatus) |= D4D_BUTTON_STATUS_PRESSED_MASK;
          D4D_InvalidateObject(pThis, D4D_FALSE);
        }
    	break;

    case D4D_MSG_MOUSE_LEAVE:
      if((*(pBtn->pStatus) & D4D_BUTTON_STATUS_MOUSE_CLICK_MASK) == 0)
        break;

    case D4D_MSG_TOUCH_LOST:
        if((*(pBtn->pStatus) & (D4D_BUTTON_STATUS_PRESSED_MASK | D4D_BUTTON_STATUS_1ST_TOUCH_MASK)) == D4D_BUTTON_STATUS_PRESSED_MASK)
        {
          *(pBtn->pStatus) &= ~D4D_BUTTON_STATUS_PRESSED_MASK;
          D4D_InvalidateObject(pThis, D4D_FALSE);
        }
      	break;

    case D4D_MSG_MOUSE_BTN_LEFT_UP:
        *(pBtn->pStatus) &= ~D4D_BUTTON_STATUS_MOUSE_CLICK_MASK;
    case D4D_MSG_UNTOUCHED:
        D4D_BtnUnTouched(pMsg);
      break;
   #endif
    default:
        // call the default behavior
        D4D_ObjOnMessage(pMsg);
    }
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_BtnGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_BUTTON(pThis)->textBuff);
}
