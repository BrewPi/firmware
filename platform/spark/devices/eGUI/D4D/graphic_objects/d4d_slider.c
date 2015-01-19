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
* @file      d4d_slider.c
*
* @author     Petr Gargulak
*
* @version   0.0.68.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver slider object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_SldrOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_SldrGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_sliderSysFunc =
{
  D4D_DEFSTR("Slider"),
  D4D_SldrOnMessage,
  NULL,
  D4D_SldrGetTextBuffer
};

// temporary structure for bar coordinate calculation
typedef struct
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
    D4D_GEOMETRY bar1Geom;
    D4D_GEOMETRY bar2Geom;
    D4D_GEOMETRY bar3Geom;
    D4D_DIRECTION dir;
    D4D_BOOL isHorizontal;
    D4D_COOR inner_radius;
    D4D_COOR bar_radius;
} D4D_SLIDER_TMP_VAL;



#define _calc (*((D4D_SLIDER_TMP_VAL*)d4d_scratchPad))

static D4D_COLOR D4D_SldrComputeColorBar(D4D_OBJECT* pThis, D4D_SLIDER_VALUE value);

/*******************************************************
*
* calculate pointer endpoint from basepoint, limits and value
*
*******************************************************/

static void D4D_SldrValue2Coor(D4D_OBJECT* pThis)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_COOR barMin;
    D4D_COOR barMax;
    D4D_COOR c1, c2;
    D4D_POINT tmpBarOff = pSldr->barOff;
    sWord tmp_scale = (sWord)(pSldr->pData->limits.valueMax - pSldr->pData->limits.valueMin);

    _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
    D4D_ComputeGeometry(&(_calc.contentGeom), pThis);

    if(tmp_scale < 0)
      tmp_scale *= -1;

    if(pThis->size.cx > pThis->size.cy)
      _calc.isHorizontal = D4D_TRUE;
    else
      _calc.isHorizontal = D4D_FALSE;

    _calc.bar1Geom.sz = pSldr->barSize;

    if(_calc.isHorizontal)
    {
      if(!tmpBarOff.x)
        tmpBarOff.x = D4D_SLDR_BAR_OFF_LENGTH;

      if(!tmpBarOff.y)
        tmpBarOff.y = D4D_SLDR_BAR_OFF_WIDTH;
    }
    else
    {
      if(!tmpBarOff.x)
        tmpBarOff.x = D4D_SLDR_BAR_OFF_WIDTH;

      if(!tmpBarOff.y)
        tmpBarOff.y = D4D_SLDR_BAR_OFF_LENGTH;
    }

    if(!_calc.bar1Geom.sz.cx)
      _calc.bar1Geom.sz.cx = (D4D_COOR)(_calc.contentGeom.sz.cx - (tmpBarOff.x * 2));

    if(!_calc.bar1Geom.sz.cy)
      _calc.bar1Geom.sz.cy = (D4D_COOR)(_calc.contentGeom.sz.cy - (tmpBarOff.y * 2));

    _calc.bar1Geom.pnt = _calc.contentGeom.pnt;
    _calc.bar1Geom.pnt.x += tmpBarOff.x;
    _calc.bar1Geom.pnt.y += tmpBarOff.y;

    // horizontal?
    if(_calc.isHorizontal)
    {
        barMin = (D4D_COOR)(_calc.bar1Geom.pnt.x);
        barMax = (D4D_COOR)(barMin + _calc.bar1Geom.sz.cx - 1);
    }
    // vertical?
    else
    {
        barMin = (D4D_COOR)(_calc.bar1Geom.pnt.y);
        barMax = (D4D_COOR)(barMin + _calc.bar1Geom.sz.cy - 1);
    }


    // value and origin coordinates
    c1 = D4D_MulDivUU((D4D_COOR)(pSldr->pData->limits.valueOrg - pSldr->pData->limits.valueMin), (D4D_COOR)(barMax-barMin),
        (D4D_COOR)tmp_scale);

    c2 = D4D_MulDivUU((D4D_COOR)(pSldr->pData->value - pSldr->pData->limits.valueMin), (D4D_COOR)(barMax-barMin),
        (D4D_COOR)tmp_scale);


    #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
      if(tmpBarOff.x > tmpBarOff.y)
        tmp_scale = (sWord)(tmpBarOff.x - tmpBarOff.y);
      else
        tmp_scale = (sWord)(tmpBarOff.y - tmpBarOff.x);

      tmp_scale /=2;

      tmp_scale = (sWord)(pThis->radius - tmp_scale);

      if(tmp_scale > 0)
        _calc.inner_radius = (D4D_COOR)tmp_scale;
      else
        _calc.inner_radius = 0;

      _calc.bar_radius = _calc.inner_radius;

    #endif


    // bar rectangle range need to be "sorted"
    if(c1 > c2)
    {
      D4D_COOR tmp_coor;
      tmp_coor = c2;
      c2 = c1;
      c1 = tmp_coor;
    }

    _calc.bar3Geom.sz = _calc.bar2Geom.sz = _calc.bar1Geom.sz;

    // X grows to right, Y grows to top
    if(_calc.isHorizontal)
    {

      if(pSldr->pData->limits.valueOrg > pSldr->pData->value)
        _calc.dir = D4D_DIR_LEFT;
      else
        _calc.dir = D4D_DIR_RIGHT;

      _calc.bar3Geom.sz.cx -= c2;
      _calc.bar1Geom.sz.cx =  c1;
      _calc.bar2Geom.sz.cx =  (D4D_COOR)(c2 - c1 + 1);

      _calc.bar2Geom.pnt.x = (D4D_COOR)(_calc.bar1Geom.pnt.x + c1);
      _calc.bar2Geom.pnt.y = _calc.bar1Geom.pnt.y;

      _calc.bar3Geom.pnt.x = (D4D_COOR)(_calc.bar1Geom.pnt.x + c2);
      _calc.bar3Geom.pnt.y = _calc.bar1Geom.pnt.y;

      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

        if(_calc.inner_radius)
        {
          _calc.bar1Geom.sz.cx += _calc.inner_radius * 2;
          _calc.bar3Geom.sz.cx += _calc.inner_radius * 2;

          _calc.bar_radius = D4D_LimitU(_calc.bar_radius, 0, (D4D_COOR)(_calc.bar2Geom.sz.cx / 2));

          if(_calc.inner_radius != _calc.bar_radius)
          {
            D4D_COOR tmp_coor;

            //tmp_coor = D4D_RndCornerGetNonPrintedPxl(_calc.inner_radius, (D4D_COOR)(_calc.pnt2.x + _calc.sz2.cx - (_calc.pnt1.x + _calc.sz1.cx + 0)));
            tmp_coor = (D4D_COOR)(_calc.inner_radius - D4D_RndCornerGetNonPrintedPxl((D4D_COOR)(_calc.inner_radius + 1), (D4D_COOR)(_calc.inner_radius - (_calc.bar2Geom.sz.cx / 2))));

            _calc.bar2Geom.sz.cy -= (D4D_COOR)(tmp_coor*2);
            _calc.bar2Geom.pnt.y += tmp_coor;
          }

          _calc.bar1Geom.sz.cx = D4D_LimitU(_calc.bar1Geom.sz.cx, (D4D_COOR)(_calc.inner_radius * 2), (D4D_COOR)(barMax - barMin + 1));
          _calc.bar3Geom.sz.cx = D4D_LimitU(_calc.bar3Geom.sz.cx, (D4D_COOR)(_calc.inner_radius * 2), (D4D_COOR)(barMax - barMin + 1));

          _calc.bar3Geom.pnt.x -= (D4D_COOR)(2 * _calc.inner_radius);
          _calc.bar3Geom.pnt.x = D4D_LimitU(_calc.bar3Geom.pnt.x, _calc.bar1Geom.pnt.x, (D4D_COOR)(barMax- (_calc.inner_radius * 2)));
        }
      #endif

    }else
    {

      if(pSldr->pData->limits.valueOrg > pSldr->pData->value)
        _calc.dir = D4D_DIR_DOWN;
      else
        _calc.dir = D4D_DIR_UP;

      _calc.bar3Geom.pnt = _calc.bar2Geom.pnt = _calc.bar1Geom.pnt;

      _calc.bar2Geom.pnt.y += (D4D_COOR)(_calc.bar1Geom.sz.cy - (c2 + 1));

      _calc.bar1Geom.pnt.y += (D4D_COOR)(_calc.bar1Geom.sz.cy - (c1 + 1));

      _calc.bar3Geom.sz.cy =  (D4D_COOR)(_calc.bar1Geom.sz.cy - (c2 + 1));
      _calc.bar1Geom.sz.cy =  (D4D_COOR)(c1 + 1);
      _calc.bar2Geom.sz.cy =  (D4D_COOR)(c2 - c1 + 1);

      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

        if(_calc.inner_radius)
        {
          _calc.bar1Geom.sz.cy += (D4D_COOR)(_calc.inner_radius * 2);
          _calc.bar3Geom.sz.cy += (D4D_COOR)(_calc.inner_radius * 2);

          _calc.bar_radius = D4D_LimitU(_calc.bar_radius, 0, (D4D_COOR)(_calc.bar2Geom.sz.cy / 2));

          if(_calc.inner_radius != _calc.bar_radius)
          {
            D4D_COOR tmp_coor;

            tmp_coor = (D4D_COOR)(_calc.inner_radius - D4D_RndCornerGetNonPrintedPxl((D4D_COOR)(_calc.inner_radius + 1), (D4D_COOR)(_calc.inner_radius - (_calc.bar2Geom.sz.cy / 2))));

            _calc.bar2Geom.sz.cx -= (D4D_COOR)(tmp_coor * 2);
            _calc.bar2Geom.pnt.x += tmp_coor;
          }

          _calc.bar1Geom.sz.cy = D4D_LimitU(_calc.bar1Geom.sz.cy, (D4D_COOR)(_calc.inner_radius * 2 + 1), (D4D_COOR)(barMax - barMin + 1));
          _calc.bar3Geom.sz.cy = D4D_LimitU(_calc.bar3Geom.sz.cy, (D4D_COOR)(_calc.inner_radius * 2), (D4D_COOR)(barMax - barMin + 1));

          _calc.bar1Geom.pnt.y -= (D4D_COOR)(2 * _calc.inner_radius);
          _calc.bar1Geom.pnt.y = D4D_LimitU(_calc.bar1Geom.pnt.y, _calc.bar3Geom.pnt.y, (D4D_COOR)(barMax - (_calc.inner_radius * 2)));
        }
      #endif
    }


}


/*******************************************************
*
* SLIDER Drawing routine
*
*******************************************************/

static void D4D_SldrOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_COLOR clrT, clrB;
    D4D_COLOR clrBar = pSldr->pData->colorBar;
    D4D_COLOR clrBarBckg = D4D_ObjectGetScheme(pThis)->objectDepend.slider.barBckg;

    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;

    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif

    // bar coordinate calculation
    D4D_SldrValue2Coor(pThis);

    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgColor(pThis, draw);

    // if slider is disabled draw slider in grey scale
    if(!D4D_IsEnabled(pThis))
    {
      clrBar = D4D_GetGreyScale(clrBar);
      clrBarBckg = D4D_GetGreyScale(clrBarBckg);
    }



    // when background redraw is needed
    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    {
      // draw the bitmap
      if(pSldr->pBmpBkgd != NULL)
          D4D_DrawRBmp(&_calc.position, pSldr->pBmpBkgd, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
      else
          D4D_FillRRect(&_calc.position, &pThis->size, D4D_ObjectGetBckgFillColor(pThis), pThis->radius);
    }

    // draw focus rectangle
    D4D_RRect(&_calc.position, &pThis->size, D4D_LINE_THIN, clrT, pThis->radius);

    // draw first inactive part
    D4D_FillRRect(&_calc.bar1Geom.pnt, &_calc.bar1Geom.sz, clrBarBckg, _calc.inner_radius);

    // draw last inactive part of the bar
    D4D_FillRRect(&_calc.bar3Geom.pnt, &_calc.bar3Geom.sz, clrBarBckg, _calc.inner_radius);

    // draw active part of the bar
    if(pThis->initFlags & D4D_SLDR_F_BAR_SCALECOLOR)
      D4D_FillRRectColorScale(&_calc.bar2Geom.pnt, &_calc.bar2Geom.sz, D4D_COLOR_SLDR_BAR_START, clrBar, _calc.dir, _calc.bar_radius);
    else
      D4D_FillRRect(&_calc.bar2Geom.pnt, &_calc.bar2Geom.sz, clrBar, _calc.inner_radius);

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);


    // draw the text
    if(pSldr->textBuff.pText != NULL)
    {
      D4D_SIZE tmp_size;

      _calc.position.x += pSldr->txtOff.x;
      _calc.position.y += pSldr->txtOff.y;

      if(!pSldr->txtOff.x)
      {
        if(_calc.isHorizontal)
          _calc.position.x += (D4D_COOR) ((_calc.contentGeom.sz.cx - (D4D_COOR)(D4D_GetTextWidth(pSldr->textBuff.fontId, pSldr->textBuff.pText))) / 2);
        else
          _calc.position.x += (D4D_COOR) (_calc.bar1Geom.pnt.x + 1);
      }

      if(!pSldr->txtOff.y)
        _calc.position.y += (D4D_COOR) (((_calc.contentGeom.sz.cy - D4D_GetFontHeight(pSldr->textBuff.fontId)) / 2) + 1);

      if(pThis->initFlags & D4D_SLDR_F_TEXT_AUTOCOLOR)
          clrT = (D4D_COLOR)(~clrBar);

      tmp_size.cx = D4D_GetTextWidth(pSldr->textBuff.fontId, pSldr->textBuff.pText);
      tmp_size.cy = D4D_GetFontHeight(pSldr->textBuff.fontId);


      D4D_DrawTextRect(&_calc.position, &tmp_size, &pSldr->textBuff, clrT, clrB);

    }

}

/*******************************************************
*
* SLIDER key handling routine
*
*******************************************************/

static void D4D_SldrOnKeyUp(D4D_MESSAGE* pMsg)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pMsg->pObject);
    D4D_KEY_SCANCODE tmp_key = pMsg->prm.key;
    // capture the keyboard if enter is pressed
    if(tmp_key == D4D_KEY_SCANCODE_ENTER)
    {
        D4D_CaptureKeys(pMsg->pObject);
        return;
    }

    // exit capture
    if(tmp_key == D4D_KEY_SCANCODE_ESC)
    {
        D4D_CaptureKeys(NULL);
        return;
    }


    if(D4D_GetCapturedObject() == pMsg->pObject)
    {
      // increment value
      if((tmp_key == D4D_KEY_SCANCODE_RIGHT) || (tmp_key == D4D_KEY_SCANCODE_UP))
      {
          D4D_SldrChangeValue(pMsg->pObject, pSldr->pData->limits.step);
      }
      // decrement value
      else if((tmp_key == D4D_KEY_SCANCODE_LEFT) || (tmp_key == D4D_KEY_SCANCODE_DOWN))
      {
          D4D_SldrChangeValue(pMsg->pObject, (D4D_SLIDER_VALUE)(-pSldr->pData->limits.step));
      }
    }
}

/*******************************************************
*
* SLIDER Touched Button handling routine
*
*******************************************************/
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
static void D4D_SldrTouched(D4D_MESSAGE* pMsg, D4D_POINT* pPoint)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);

  if((D4D_GetFocusedObject(pMsg->pScreen) == pThis) || (pThis->pData->flags & D4D_OBJECT_F_FASTTOUCH))
  {
    D4D_FocusSet(pMsg->pScreen, pThis);

    if(D4D_GetCapturedObject() != pThis)
      D4D_CaptureKeys(pThis);

    if(pThis->size.cx > pThis->size.cy)
    {
      if(pPoint->x > ((pThis->size.cx / 2) + pThis->position.x))
        D4D_SldrChangeValue(pThis, pSldr->pData->limits.step);
      else
        D4D_SldrChangeValue(pThis, (D4D_SLIDER_VALUE)(-pSldr->pData->limits.step));
    }else
    {

      if(pPoint->y < ((pThis->size.cy / 2) + pThis->position.y))
        D4D_SldrChangeValue(pThis, pSldr->pData->limits.step);
      else
        D4D_SldrChangeValue(pThis, (D4D_SLIDER_VALUE)(-pSldr->pData->limits.step));
    }
  }else
    D4D_FocusSet(pMsg->pScreen, pThis);

}
#endif

static D4D_COLOR D4D_SldrComputeColorBar(D4D_OBJECT* pThis, D4D_SLIDER_VALUE value)
{
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
  Byte shade;
  D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pThis);

  if(value >= pSldr->pData->limits.valueOrg)
    shade = D4D_MulDivUU8((Byte)(value - pSldr->pData->limits.valueOrg), 255, \
      (Byte)(pSldr->pData->limits.valueMax - pSldr->pData->limits.valueOrg));
  else
    shade = D4D_MulDivUU8((Byte)(pSldr->pData->limits.valueOrg - value), 255, \
      (Byte)(pSldr->pData->limits.valueOrg - pSldr->pData->limits.valueMin));

  return D4D_GetCrossColor(pScheme_tmp->objectDepend.slider.barStart, pScheme_tmp->objectDepend.slider.barEnd, shade);

}

/******************************************************************************
* Begin of D4D_SLIDER public functions
*//*! @addtogroup doxd4d_slider_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function Sets the slider value
* @param   pThis - pointer to the slider object
* @param   value - new value of slider
* @return  none.
* @note    The values must fit in to slider linits, if not is limit to valid range
*******************************************************************************/
void D4D_SldrSetValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE value)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_SLIDER_DATA* pData = pSldr->pData;

    // keep value within limits

    value = D4D_LimitS8(value, pData->limits.valueMin, pData->limits.valueMax);

    if(pData->value == value)
        return ;

    // remember new value
    pData->value = value;

    if(pThis->initFlags & (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR))
      pData->colorBar = D4D_SldrComputeColorBar((D4D_OBJECT*)pThis, value);

    D4D_InvalidateObject(pThis, D4D_FALSE);

    // notify user that the value has changed
    if(pSldr->OnValueChanged)
        pSldr->OnValueChanged((D4D_OBJECT*)pThis);
}

/**************************************************************************/ /*!
* @brief   Function Gets the slider value
* @param   pThis - pointer to the slider object
* @return  value of slider.
* @note    none
*******************************************************************************/
D4D_SLIDER_VALUE D4D_SldrGetValue(D4D_OBJECT_PTR pThis)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    return pSldr->pData->value;
}

/**************************************************************************/ /*!
* @brief   Function Sets the slider bar color
* @param   pThis - pointer to the slider object
* @param   color - new slider bar color
* @return  none
* @note    none
*******************************************************************************/
void D4D_SldrSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_SLIDER_DATA* pData = pSldr->pData;

    if(pThis->initFlags & (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR))
      return;

    if(pData->colorBar == color)
        return ;

    // remember new color
    pData->colorBar = color;

    D4D_InvalidateObject(pThis, D4D_FALSE);
}

D4D_COLOR D4D_SldrGetBarColor(D4D_OBJECT_PTR pThis)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    return pSldr->pData->colorBar;
}


/**************************************************************************/ /*!
* @brief   Function change the slider value by signed increment
* @param   pThis - pointer to the slider object
* @param   incr - signed increment value
* @return  none
* @note    none
*******************************************************************************/
void D4D_SldrChangeValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE incr)
{
    D4D_SLIDER_VALUE value;

    // change the value
    value = D4D_LimitS8((D4D_SLIDER_VALUE)(D4D_GET_SLIDER(pThis)->pData->value + incr), -128, 127);

    D4D_SldrSetValue(pThis, value);
}

/**************************************************************************/ /*!
* @brief   Function set the new limits of slider object
* @param   pThis - pointer to the slider object
* @param   pLimits - pointer to new limits structure
* @return  none
* @note    none
*******************************************************************************/
void D4D_SldrSetLimits(D4D_OBJECT_PTR pThis, const D4D_SLIDER_LIMITS* pLimits)
{
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
  D4D_SLIDER_DATA* pData = pSldr->pData;

  pData->limits = *pLimits;

  if((pData->limits.valueOrg < pData->limits.valueMin) || (pData->limits.valueOrg >= pData->limits.valueMax))
    pData->limits.valueOrg = pData->limits.valueMin;

  D4D_SldrSetValue(pThis, pData->value);
}

/**************************************************************************/ /*!
* @brief   Function get the current limits of slider object
* @param   pThis - pointer to the slider object
* @param   pLimits - pointer to limits structure where will be copied current values what slider is using
* @return  none
* @note    none
*******************************************************************************/
void D4D_SldrGetLimits(D4D_OBJECT_PTR pThis, D4D_SLIDER_LIMITS* pLimits)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    *pLimits = pSldr->pData->limits;
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_slider_func                                              */
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
* The main SLIDER message handler
*
*******************************************************/

void D4D_SldrOnMessage(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pThis);

    #if defined(D4D_LLD_TCH)
    static D4D_INDEX autoTouchTicks;
    #endif

    #if defined(D4D_LLD_MOUSE)
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    #endif

    #if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
    D4D_POINT touchClickPoint;
    #endif

    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_SldrOnDraw(pMsg);
        break;

    case D4D_MSG_KEYUP:
        D4D_SldrOnKeyUp(pMsg);
        break;

    case D4D_MSG_KILLFOCUS:
        D4D_CaptureKeys(NULL);
        break;

    case D4D_MSG_ONINIT:
        pThis->pData->flags &= ~D4D_OBJECT_F_NOTINIT;

        if(!(pThis->initFlags & (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR)))
          D4D_GET_SLIDER(pThis)->pData->colorBar = pScheme_tmp->objectDepend.slider.barFore;
        else
          D4D_GET_SLIDER(pThis)->pData->colorBar = D4D_SldrComputeColorBar(pThis, D4D_GET_SLIDER(pThis)->pData->value);
        break;

#ifdef D4D_LLD_MOUSE
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
        touchClickPoint = D4D_GetMouseCoordinates(pMsg->pObject);
        D4D_SldrTouched(pMsg, &touchClickPoint);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_UP:
      D4D_SldrChangeValue(pThis, pSldr->pData->limits.step);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_DOWN:
      D4D_SldrChangeValue(pThis, -pSldr->pData->limits.step);
      break;
#endif



#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCH_AUTO:
        if(pThis->initFlags & D4D_SLDR_F_AUTOTOUCH_OFF)
          break;

        if(autoTouchTicks--)
          break;

    case D4D_MSG_TOUCHED:
        autoTouchTicks = D4D_SLDR_AUTOTOUCHTICKS;
        touchClickPoint = D4D_GetTouchScreenCoordinates(pMsg->pObject);
        D4D_SldrTouched(pMsg, &touchClickPoint);
        break;
#endif
    default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
    }
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_SldrGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_SLIDER(pThis)->textBuff);
}
