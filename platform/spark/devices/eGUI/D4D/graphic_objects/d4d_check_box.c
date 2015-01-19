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
* @file      d4d_check_box.c
*
* @author     Petr Gargulak
*
* @version   0.0.43.0
*
* @date      Oct-14-2013
*
* @brief     D4D driver check box object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_CheckBoxOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_CheckBoxGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_checkBoxSysFunc =
{
  D4D_DEFSTR("Check Box"),
  D4D_CheckBoxOnMessage,
  NULL,
  D4D_CheckBoxGetTextBuffer
};

typedef struct
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
    D4D_GEOMETRY BmpGeom;
    D4D_GEOMETRY TxtGeom;
    D4D_COOR iconRadius;
} D4D_CHECKBOX_TMP_VAL;

#define _calc (*((D4D_CHECKBOX_TMP_VAL*)d4d_scratchPad))

//static D4D_CHECKBOX_TMP_VAL  _calc;

static void D4D_CheckBoxValue2Coor(D4D_OBJECT* pThis)
{
  D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pThis);
  D4D_SIZE sizeBmpEnabled;
  D4D_COOR tmp_BorderOffset;

  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);

  sizeBmpEnabled = D4D_GetBmpSize(pCheckB->pBmpChecked);

  _calc.BmpGeom = _calc.contentGeom;

  if(!sizeBmpEnabled.cx || !sizeBmpEnabled.cy)
  {
    _calc.BmpGeom.sz.cy = (D4D_COOR)((_calc.BmpGeom.sz.cy * 2) / 3);
    _calc.BmpGeom.sz.cx = _calc.BmpGeom.sz.cy;
  }else
    _calc.BmpGeom.sz = sizeBmpEnabled;

  tmp_BorderOffset = (D4D_COOR)((_calc.contentGeom.sz.cy - _calc.BmpGeom.sz.cy) / 2);
  _calc.BmpGeom.pnt.x += tmp_BorderOffset;
  _calc.BmpGeom.pnt.y += tmp_BorderOffset;

  if(pCheckB->textBuff.pText)
  {
    _calc.TxtGeom = _calc.contentGeom;

    _calc.TxtGeom.pnt.x = (D4D_COOR) (_calc.BmpGeom.pnt.x + _calc.BmpGeom.sz.cx + D4D_CHECKBOX_TEXT_OFFSET);
    _calc.TxtGeom.sz.cx -= (D4D_COOR) (_calc.TxtGeom.pnt.x - _calc.contentGeom.pnt.x);
    #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      _calc.TxtGeom.sz.cx -= (D4D_COOR) (pThis->radius / 2);
    #endif
  }

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
    _calc.iconRadius = pThis->radius;
#endif

}


/*******************************************************
*
* CHECK BOX Drawing routine
*
*******************************************************/

static void D4D_CheckBoxOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pThis);
    D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pThis);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_CLR_SCHEME* pScheme = D4D_ObjectGetScheme(pThis);
    D4D_BMP_PROPERTIES bmpProp;

    bmpProp = (D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_CENTER_MASK);

    D4D_CheckBoxValue2Coor(pThis);

    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgFillColor(pThis);

    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
      D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // normal bitmap (also may be NULL if simple rect button is needed
    if(*pStatus & D4D_CHECKBOX_STATUS_CHECKED_MASK)
    {
      if(pCheckB->pBmpChecked != NULL)
        D4D_DrawRBmpRect(&_calc.BmpGeom.pnt, &_calc.BmpGeom.sz, pCheckB->pBmpChecked, bmpProp, pScheme->objectDepend.checkBox.iconBckg, (D4D_BOOL)!D4D_IsEnabled(pThis), _calc.iconRadius);  // draw the bitmap
      else
      {
        D4D_POINT tmp_point = _calc.BmpGeom.pnt;
        D4D_SIZE tmp_size = _calc.BmpGeom.sz;
        if(tmp_size.cy - 2 > 0)
        {
          tmp_point.x++;
          tmp_point.y++;

          tmp_size.cx -= 2;
          tmp_size.cy -= 2;

          D4D_RBox(&tmp_point, &tmp_size, D4D_LINE_THICK, clrB, clrT, _calc.iconRadius);
        }
      }
    }
    // no bitmap
    else
    {
      if(pCheckB->pBmpUnChecked)
      {
        // draw the bitmap
        D4D_DrawRBmpRect(&_calc.BmpGeom.pnt, &_calc.BmpGeom.sz, pCheckB->pBmpUnChecked, bmpProp, pScheme->objectDepend.checkBox.iconBckg, (D4D_BOOL)!D4D_IsEnabled(pThis), _calc.iconRadius);
      }else
      {
        // draw just a color rectangle instead of bitmap

        D4D_FillRRect(&_calc.BmpGeom.pnt, &_calc.BmpGeom.sz, pScheme->objectDepend.checkBox.iconBckg, _calc.iconRadius);
      }
    }

    if(pThis->initFlags & D4D_CHECKBOX_F_ICON_RECTANGLE)
      D4D_RRect(&_calc.BmpGeom.pnt, &_calc.BmpGeom.sz, D4D_LINE_THIN, clrT, _calc.iconRadius);

    // draw the text
    if(pCheckB->textBuff.pText)
      D4D_DrawTextRect(&_calc.TxtGeom.pnt, &_calc.TxtGeom.sz, &pCheckB->textBuff, clrT, D4D_ObjectGetBckgColor(pThis, draw));

}

/*******************************************************
*
* CHECK BOX key handling routine
*
*******************************************************/

static void D4D_CheckBoxOnKeyDown(D4D_MESSAGE* pMsg)
{
    D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);

    // invoke "Click" event when enter key is pressed
    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    {
      *pStatus |= D4D_CHECKBOX_STATUS_PRESSED_MASK;
      D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
    }
}

/*******************************************************
*
* CHECK BOX key handling routine
*
*******************************************************/

static void D4D_CheckBoxOnKeyUp(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pThis);

  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
  {
    if(*pStatus & D4D_CHECKBOX_STATUS_PRESSED_MASK)
    {
      D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pMsg->pObject);
      *pStatus &= ~D4D_CHECKBOX_STATUS_PRESSED_MASK;

      if(*pStatus & D4D_CHECKBOX_STATUS_CHECKED_MASK)
        *pStatus &= ~D4D_CHECKBOX_STATUS_CHECKED_MASK;
      else
        *pStatus |= D4D_CHECKBOX_STATUS_CHECKED_MASK;

      if(pCheckB->OnChange)
        pCheckB->OnChange(pThis);
    }
    D4D_InvalidateObject(pThis, D4D_FALSE);
  }
}

/*******************************************************
*
* CHECK BOX focus handling routine
*
*******************************************************/

static void D4D_CheckBoxKillFocus(D4D_MESSAGE* pMsg)
{
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);

  *pStatus &= ~D4D_CHECKBOX_STATUS_PRESSED_MASK;
}

/*******************************************************
*
* CHECK BOX Touched Button handling routine
*
*******************************************************/
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  static void D4D_CheckBoxTouched(D4D_MESSAGE* pMsg)
  {
    if(pMsg->pObject->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_FASTTOUCH))
    {
      if((D4D_GetFocusedObject(pMsg->pScreen) == pMsg->pObject) || (pMsg->pObject->pData->flags & D4D_OBJECT_F_FASTTOUCH))
      {
        D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);
        D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pMsg->pObject);

        if(*pStatus & D4D_CHECKBOX_STATUS_CHECKED_MASK)
          *pStatus &= ~D4D_CHECKBOX_STATUS_CHECKED_MASK;
        else
          *pStatus |= D4D_CHECKBOX_STATUS_CHECKED_MASK;

        if(pCheckB->OnChange)
          pCheckB->OnChange(pMsg->pObject);

        D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
      }
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
    }
  }
#endif

/******************************************************************************
* Begin of D4D_CHECK_BOX public functions
*//*! @addtogroup doxd4d_check_box_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function Sets the the check box value
* @param   pThis - pointer to the check box object that should be sets.
* @param   value - new value of check box
* @return  none.
* @note    This sets the value of check box.
*******************************************************************************/
void D4D_CheckBoxSetValue(D4D_OBJECT_PTR pThis, D4D_BOOL value)
{
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pThis);
  D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pThis);

  if(((*pStatus & D4D_CHECKBOX_STATUS_CHECKED_MASK) && value) || (!(*pStatus & D4D_CHECKBOX_STATUS_CHECKED_MASK) && !value))
    return ;

  // remember new value
  if(value)
    *pStatus |= D4D_CHECKBOX_STATUS_CHECKED_MASK;
  else
    *pStatus &= ~D4D_CHECKBOX_STATUS_CHECKED_MASK;

  D4D_InvalidateObject(pThis, D4D_TRUE);

  // notify user that the value has changed
  if(pCheckB->OnChange)
    pCheckB->OnChange((D4D_OBJECT*)pThis);
}

/**************************************************************************/ /*!
* @brief   Function Gets the the check box value
* @param   pThis - pointer to the check box object
* @return  Current value of check box object
* @note    This gets the value of check box.
*******************************************************************************/
D4D_BOOL D4D_CheckBoxGetValue(D4D_OBJECT_PTR pThis)
{
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pThis);

  return (D4D_BOOL)(*pStatus & D4D_CHECKBOX_STATUS_CHECKED_MASK);
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_check_box_func                                              */
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
* The main CHECK BOX message handler
*
*******************************************************/

void D4D_CheckBoxOnMessage(D4D_MESSAGE* pMsg)
{
  switch(pMsg->nMsgId)
  {
  case D4D_MSG_DRAW:
      D4D_CheckBoxOnDraw(pMsg);
      break;

  case D4D_MSG_KEYDOWN:
      D4D_CheckBoxOnKeyDown(pMsg);
      break;

  case D4D_MSG_KEYUP:
      D4D_CheckBoxOnKeyUp(pMsg);
      break;

  case D4D_MSG_KILLFOCUS:
      D4D_CheckBoxKillFocus(pMsg);
      break;

#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
      if(D4D_GetMouseHoverObject() != pMsg->pObject)
        break;
    case D4D_MSG_TOUCHED:
      D4D_CheckBoxTouched(pMsg);
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
D4D_STRING* D4D_CheckBoxGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_CHECKBOX(pThis)->textBuff);
}
