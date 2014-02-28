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
* @file      d4d_radio_button.c
*
* @author     Petr Gargulak
*
* @version   0.0.9.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver radio button object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_RadioButtonOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_RadioButtonGetTextBuffer(D4D_OBJECT* pThis);

static void D4D_RadioButtonChangeValue(D4D_OBJECT_PTR pThis, D4D_BOOL value);

const D4D_OBJECT_SYS_FUNCTION d4d_radioButtonSysFunc =
{
  D4D_DEFSTR("Radio Button"),
  D4D_RadioButtonOnMessage,
  NULL,
  D4D_RadioButtonGetTextBuffer
};

typedef struct
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
    D4D_GEOMETRY BmpGeom;
    D4D_GEOMETRY TxtGeom;
    D4D_POINT iconCentre;
    D4D_COOR iconRadius;
} D4D_RADIOBUTTON_TMP_VAL;

#define _calc (*((D4D_RADIOBUTTON_TMP_VAL*)d4d_scratchPad))


static void D4D_RadioButtonValue2Coor(D4D_OBJECT* pThis)
{
  D4D_RADIOBUTTON* pRdBtn = D4D_GET_RADIOBUTTON(pThis);
  D4D_SIZE sizeBmpEnabled;
  D4D_COOR tmp_BorderOffset;

  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);

  sizeBmpEnabled = D4D_GetBmpSize(pRdBtn->pBmpChecked);

  _calc.BmpGeom = _calc.contentGeom;

  if(!sizeBmpEnabled.cx || !sizeBmpEnabled.cy)
  {
    _calc.BmpGeom.sz.cy = (D4D_COOR)(_calc.BmpGeom.sz.cy / 2);
    _calc.BmpGeom.sz.cx = _calc.BmpGeom.sz.cy;
  }else
    _calc.BmpGeom.sz = sizeBmpEnabled;

  tmp_BorderOffset = (D4D_COOR)((_calc.contentGeom.sz.cy - _calc.BmpGeom.sz.cy) / 2);
  _calc.BmpGeom.pnt.x += tmp_BorderOffset;
  _calc.BmpGeom.pnt.y += tmp_BorderOffset;

  if(pRdBtn->textBuff.pText)
  {
    _calc.TxtGeom = _calc.contentGeom;

    _calc.TxtGeom.pnt.x = (D4D_COOR) (_calc.BmpGeom.pnt.x + _calc.BmpGeom.sz.cx + D4D_CHECKBOX_TEXT_OFFSET);
    _calc.TxtGeom.sz.cx -= (D4D_COOR) (_calc.TxtGeom.pnt.x - _calc.contentGeom.pnt.x);
    #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      _calc.TxtGeom.sz.cx -= (D4D_COOR) (pThis->radius / 2);
    #endif
  }

  if(D4D_RADIOBUTTON_MIN_SIZE > (_calc.BmpGeom.sz.cx / 2))
    _calc.iconRadius = (D4D_COOR)(_calc.BmpGeom.sz.cx / 2);
  else
    _calc.iconRadius = D4D_RADIOBUTTON_MIN_SIZE;

  _calc.iconCentre.x = (D4D_COOR)(_calc.BmpGeom.pnt.x + _calc.BmpGeom.sz.cx / 2);
  _calc.iconCentre.y = (D4D_COOR)(_calc.BmpGeom.pnt.y + _calc.BmpGeom.sz.cy / 2);



}


/*******************************************************
*
* RADIO BUTTON Drawing routine
*
*******************************************************/

static void D4D_RadioButtonOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_RADIOBUTTON* pRadioBtn = D4D_GET_RADIOBUTTON(pThis);
    D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pThis);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_CLR_SCHEME* pScheme = D4D_ObjectGetScheme(pThis);
    D4D_BMP_PROPERTIES bmpProp;

    bmpProp = (D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_CENTER_MASK);

    D4D_RadioButtonValue2Coor(pThis);

    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgFillColor(pThis);

    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
      D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // normal bitmap (also may be NULL if simple rect button is needed
    if(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK)
    {
      if(pRadioBtn->pBmpChecked != NULL)
        D4D_DrawRBmpRect(&_calc.BmpGeom.pnt, &_calc.BmpGeom.sz, pRadioBtn->pBmpChecked, bmpProp, pScheme->objectDepend.checkBox.iconBckg, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);  // draw the bitmap
      else if(_calc.iconRadius > D4D_RADIOBUTTON_ICON_RING)
        D4D_FillCircle(&_calc.iconCentre, (D4D_COOR)(_calc.iconRadius - D4D_RADIOBUTTON_ICON_RING), clrT);

    }
    // no bitmap
    else
    {
      if(pRadioBtn->pBmpUnChecked)
      {
        // draw the bitmap
        D4D_DrawRBmpRect(&_calc.BmpGeom.pnt, &_calc.BmpGeom.sz, pRadioBtn->pBmpUnChecked, bmpProp, clrB, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
      }else
      {
        // draw just a color rectangle instead of bitmap
        D4D_FillCircle(&_calc.iconCentre, _calc.iconRadius, clrB);
      }
    }

    if(pThis->initFlags & D4D_RADIOBUTTON_F_ICON_RING)
      D4D_Circle(&_calc.iconCentre, _calc.iconRadius, D4D_LINE_THIN, clrT);



    // draw the text
    if(pRadioBtn->textBuff.pText)
      D4D_DrawTextRect(&_calc.TxtGeom.pnt, &_calc.TxtGeom.sz, &pRadioBtn->textBuff, clrT, D4D_ObjectGetBckgColor(pThis, draw));

}

/*******************************************************
*
* RADIO BUTTON key handling routine
*
*******************************************************/

static void D4D_RadioButtonOnKeyDown(D4D_MESSAGE* pMsg)
{
    D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pMsg->pObject);

    // invoke "Click" event when enter key is pressed
    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    {
      *pStatus |= D4D_RADIOBUTTON_STATUS_PRESSED_MASK;
      D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
    }
}

/*******************************************************
*
* RADIO BUTTON key handling routine
*
*******************************************************/

static void D4D_RadioButtonOnKeyUp(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pThis);

  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
  {
    if(*pStatus & D4D_RADIOBUTTON_STATUS_PRESSED_MASK)
    {
      *pStatus &= ~D4D_RADIOBUTTON_STATUS_PRESSED_MASK;

      if(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK)
        return;

      if(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK)
        *pStatus &= ~D4D_RADIOBUTTON_STATUS_CHECKED_MASK;
      else
        *pStatus |= D4D_RADIOBUTTON_STATUS_CHECKED_MASK;

      D4D_RadioButtonChangeValue(pThis,(D4D_BOOL)(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK));
    }
    D4D_InvalidateObject(pThis, D4D_FALSE);
  }
}

/*******************************************************
*
* RADIO BUTTON focus handling routine
*
*******************************************************/

static void D4D_RadioButtonKillFocus(D4D_MESSAGE* pMsg)
{
  D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pMsg->pObject);

  *pStatus &= ~D4D_RADIOBUTTON_STATUS_PRESSED_MASK;
}

/*******************************************************
*
* RADIO BUTTON Touched Button handling routine
*
*******************************************************/
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  static void D4D_RadioButtonTouched(D4D_MESSAGE* pMsg)
  {
    if(pMsg->pObject->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_FASTTOUCH))
    {
      if((D4D_GetFocusedObject(pMsg->pScreen) == pMsg->pObject) || (pMsg->pObject->pData->flags & D4D_OBJECT_F_FASTTOUCH))
      {
        D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pMsg->pObject);

        if(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK)
          return;

        if(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK)
          *pStatus &= ~D4D_RADIOBUTTON_STATUS_CHECKED_MASK;
        else
          *pStatus |= D4D_RADIOBUTTON_STATUS_CHECKED_MASK;

        D4D_RadioButtonChangeValue(pMsg->pObject, (D4D_BOOL)(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK));

        D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
      }
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
    }
  }
#endif

/******************************************************************************
* Begin of D4D_RADIO_BUTTON public functions
*//*! @addtogroup doxd4d_radio_button_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function Sets the the radio button value
* @param   pThis - pointer to the radio_button object that should be sets.
* @param   value - new value of radio_button
* @return  none.
* @note    This sets the value of radio button.
*******************************************************************************/
void D4D_RadioButtonSetValue(D4D_OBJECT_PTR pThis, D4D_BOOL value)
{
  D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pThis);

  if(((*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK) && value) || (!(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK) && !value))
    return ;


  // remember new value
  if(value)
    *pStatus |= D4D_RADIOBUTTON_STATUS_CHECKED_MASK;
  else
    *pStatus &= ~D4D_RADIOBUTTON_STATUS_CHECKED_MASK;

  D4D_InvalidateObject(pThis, D4D_TRUE);

  // notify user that the value has changed
  D4D_RadioButtonChangeValue(pThis, (D4D_BOOL)(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK));
}


/**************************************************************************/ /*!
* @brief   Function Gets the the radio button value
* @param   pThis - pointer to the radio button object
* @return  Current value of radio button object
* @note    This gets the value of radio button.
*******************************************************************************/
D4D_BOOL D4D_RadioButtonGetValue(D4D_OBJECT_PTR pThis)
{
  D4D_RADIOBUTTON_STATUS* pStatus = D4D_GET_RADIOBUTTON_STATUS(pThis);

  return (D4D_BOOL)(*pStatus & D4D_RADIOBUTTON_STATUS_CHECKED_MASK);
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_radio_button_func                                        */
/******************************************************************************/

static void D4D_RadioButtonChangeValue(D4D_OBJECT_PTR pThis, D4D_BOOL value)
{
  D4D_RADIOBUTTON* pRadioBtn = D4D_GET_RADIOBUTTON(pThis);

  if(value)
  {
    D4D_OBJECT** pObjects = (D4D_OBJECT**)(pThis->pData->pScreen->pObjects);
    D4D_INDEX ix = 0;

    // uncheck rest radio buttons on screen!
    if(pThis->pRelations)
    {
      D4D_OBJECT* pParent = D4D_GetParentObject((D4D_OBJECT*)pThis);
      if(pParent)
        pObjects = (D4D_OBJECT**)&pParent->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];
    }

    while(pObjects[ix])
    {
      if(pObjects[ix]->pObjFunc->strName == d4d_radioButtonSysFunc.strName)
        if(pObjects[ix] != pThis)
          if(D4D_RadioButtonGetValue(pObjects[ix]))
            D4D_RadioButtonSetValue(pObjects[ix], D4D_FALSE);

      ix++;
    }
  }


  // notify user that the value has changed
  if(pRadioBtn->OnChange)
    pRadioBtn->OnChange((D4D_OBJECT*)pThis);
}


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
* The main RADIO BUTTON message handler
*
*******************************************************/

void D4D_RadioButtonOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_RadioButtonOnDraw(pMsg);
        break;

    case D4D_MSG_KEYDOWN:
          D4D_RadioButtonOnKeyDown(pMsg);
        break;

    case D4D_MSG_KEYUP:
          D4D_RadioButtonOnKeyUp(pMsg);
        break;

    case D4D_MSG_KILLFOCUS:
        D4D_RadioButtonKillFocus(pMsg);
        break;

#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
      if(D4D_GetMouseHoverObject() != pMsg->pObject)
        break;
    case D4D_MSG_TOUCHED:
      D4D_RadioButtonTouched(pMsg);
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
D4D_STRING* D4D_RadioButtonGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_RADIOBUTTON(pThis)->textBuff);
}
