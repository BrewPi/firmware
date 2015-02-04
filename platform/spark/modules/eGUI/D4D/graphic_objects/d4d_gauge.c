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
* @file      d4d_gauge.c
*
* @author     Petr Gargulak
*
* @version   0.0.57.0
*
* @date      Oct-15-2013
*
* @brief     D4D driver gauge object c file
*
******************************************************************************/


#include "d4d.h"
#include "common_files/d4d_private.h"


// Internal API
void D4D_GaugOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_GaugeGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_gaugeSysFunc =
{
  D4D_DEFSTR("Gauge"),
  D4D_GaugOnMessage,
  NULL,
  D4D_GaugeGetTextBuffer
};

// temporary structure for pointer coordinates calculations
typedef struct D4D_GAUGE_VAL2PT_S
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
    D4D_POINT hubXY;
    D4D_POINT currPointXY;
    D4D_POINT lastPointXY;
} D4D_GAUGE_VAL2PT;


#define _calc (*((D4D_GAUGE_VAL2PT*)d4d_scratchPad))

/*******************************************************
*
* calculate pointer endpoint from basepoint, limits and value
*
* global structure _calc is used to exchange in/out data
*
*******************************************************/

static D4D_POINT D4D_GaugeGetAnglePoint(D4D_GAUGE* pGaug, D4D_GAUGE_VALUE value) {
  D4D_GAUGE_DATA* pData = pGaug->pData;
  D4D_POINT tmp_res;

  Byte angle;
  sByte sin;
  sByte cos;

  // Compute angle in allowed angle range from allowed value range
  angle = (Byte)D4D_MulDivUU8((Byte)(value - pData->limits.valueMin),
                            (Byte)(pData->limits.angleMax - pData->limits.angleMin),
                            (Byte)(pData->limits.valueMax - pData->limits.valueMin));


  // Take care direction
  if(pData->trend == D4D_CLOCK_WISE)
    angle += pData->limits.angleMin;
  else
    angle = (Byte)(pData->limits.angleMax - angle);

  // Compute sin value and cos value of computed pointer angle
  sin = D4D_Sin(angle);
  cos = D4D_Cos(angle);

 // Compute and fill up result point
  tmp_res = _calc.hubXY;
  tmp_res.x -= D4D_MulDivSU(cos, pGaug->pointerLen, 128);
  tmp_res.y -= D4D_MulDivSU(sin, pGaug->pointerLen, 128);

  return tmp_res;
}

static void D4D_GaugValue2Point(D4D_OBJECT* pThis)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;

    D4D_ComputeGeometry(&_calc.contentGeom, pThis);

    _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);

     if(pGaug->hubOff.x)
      _calc.hubXY.x = (D4D_COOR)(_calc.contentGeom.pnt.x + pGaug->hubOff.x);
    else
      _calc.hubXY.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cx / 2);

    if(pGaug->hubOff.y)
      _calc.hubXY.y = (D4D_COOR)(_calc.contentGeom.pnt.y + pGaug->hubOff.y);
    else
      _calc.hubXY.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.contentGeom.sz.cy / 2);

    _calc.currPointXY = D4D_GaugeGetAnglePoint(pGaug, pData->value);
    _calc.lastPointXY = D4D_GaugeGetAnglePoint(pGaug, pData->valueLast);
}

/*******************************************************
*
* GAUGE Drawing routine
*
*******************************************************/

static void D4D_GaugOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;

    D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pThis);
    D4D_BMP_PROPERTIES bmpProp;
    D4D_COLOR clrT = D4D_ObjectGetForeColor(pThis, draw);
    D4D_COLOR clrB = D4D_ObjectGetBckgFillColor(pThis);

    bmpProp = (D4D_ALIGN_V_CENTER_MASK | D4D_ALIGN_H_CENTER_MASK);

    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif

    D4D_GaugValue2Point(pThis);

    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    {
      // draw the bitmap
      if(pGaug->pBmpBkgd != NULL)
        D4D_DrawRBmpRect(&_calc.contentGeom.pnt, &_calc.contentGeom.sz, pGaug->pBmpBkgd, bmpProp, clrB, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
      else
        D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);
    }

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    if(!(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE))
    {
      D4D_MoveTo(&_calc.hubXY);
      D4D_LineTo(&_calc.lastPointXY, (D4D_LINETYPE)((pThis->initFlags & D4D_GAUGE_F_THICK_POINTER)? D4D_LINE_THICK:D4D_LINE_THIN), pScheme_tmp->bckg);
    }
    // draw the text
    if(((pThis->initFlags & D4D_GAUGE_F_REDRAW_TEXT) || (draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)) && pGaug->textBuff.pText != NULL)
    {
        D4D_POINT tmp_point;
        D4D_SIZE tmp_size;
        tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + pGaug->txtOff.x);
        tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + pGaug->txtOff.y);

        tmp_size.cx = D4D_GetTextWidth(pGaug->textBuff.fontId, pGaug->textBuff.pText);
        tmp_size.cy = D4D_GetFontHeight(pGaug->textBuff.fontId);


        D4D_DrawTextRect(&tmp_point, &tmp_size, &pGaug->textBuff,
            D4D_ObjectGetForeColor(pThis, draw), D4D_ObjectGetBckgFillColor(pThis));
    }

    // draw the hub circle
    if(pThis->initFlags & D4D_GAUGE_F_HUB)
    {
      if(D4D_IsEnabled(pThis))
        D4D_FillCircle(&_calc.hubXY, D4D_GAUGE_HUB_RADIUS, pScheme_tmp->objectDepend.gauge.hub);
      else
        D4D_FillCircle(&_calc.hubXY, D4D_GAUGE_HUB_RADIUS, D4D_GetGreyScale(pScheme_tmp->objectDepend.gauge.hub));
    }
    // draw the pointer on value
    D4D_MoveTo(&_calc.hubXY);

    if(D4D_IsEnabled(pThis))
        D4D_LineTo(&_calc.currPointXY, (D4D_LINETYPE)((pThis->initFlags & D4D_GAUGE_F_THICK_POINTER)? D4D_LINE_THICK:D4D_LINE_THIN), pScheme_tmp->objectDepend.gauge.pointer);
      else
        D4D_LineTo(&_calc.currPointXY, (D4D_LINETYPE)((pThis->initFlags & D4D_GAUGE_F_THICK_POINTER)? D4D_LINE_THICK:D4D_LINE_THIN), D4D_GetGreyScale(pScheme_tmp->objectDepend.gauge.pointer));

    // remember the last value drawn
    pData->valueLast = pData->value;
}


/******************************************************************************
* Begin of D4D_GAUGE public functions
*//*! @addtogroup doxd4d_gauge_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function sets the new value of gauge
* @param   pThis - pointer to gauge object
* @param   value - input value, MUST fit into the range sets by \ref D4D_GaugSetLimits
* @return  none
* @note    none
*******************************************************************************/
void D4D_GaugSetValue(D4D_OBJECT_PTR pThis, D4D_GAUGE_VALUE value)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;

    if(pGaug->pData->value == value)
        return ;

    // keep within limits
     value = D4D_LimitS8(value, pData->limits.valueMin, pData->limits.valueMax);

    // remember new value
    pGaug->pData->value = value;
    D4D_InvalidateObject(pThis, D4D_FALSE);

    // notify user that the value has changed
    if(pGaug->OnValueChanged)
        pGaug->OnValueChanged((D4D_OBJECT*)pThis);
}

/**************************************************************************/ /*!
* @brief   The function gets the current value of gauge
* @param   pThis - pointer to gauge object
* @return  currnet value of gauge object
* @note    none
*******************************************************************************/
D4D_GAUGE_VALUE D4D_GaugGetValue(D4D_OBJECT_PTR pThis)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    return pGaug->pData->value;
}


/**************************************************************************/ /*!
* @brief   The function sets the new limits values of gauge
* @param   pThis - pointer to gauge object
* @param   pLimits - pointer to defined gauge limits structure
* @return  none
* @note    In limit structure is defined as range of input values as visible range of pointer angle
*******************************************************************************/
void D4D_GaugSetLimits(D4D_OBJECT_PTR pThis, const D4D_GAUGE_LIMITS* pLimits)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;

    pGaug->pData->limits = *pLimits;

    if(pData->limits.valueMin > pData->limits.valueMax)
    {
      D4D_GAUGE_VALUE value_tmp = pData->limits.valueMin;

      pData->limits.valueMin = pData->limits.valueMax;

      pData->limits.valueMax = value_tmp;

      if(pData->trend != D4D_ANTI_CLOCK_WISE)
        pData->trend = D4D_ANTI_CLOCK_WISE;

    }

    // keep value within limits
    pData->value = D4D_LimitS8(pData->value, pData->limits.valueMin, pData->limits.valueMax);

    D4D_InvalidateObject(pThis, D4D_TRUE);

    // notify user that the value has changed
    if(pGaug->OnValueChanged)
        pGaug->OnValueChanged((D4D_OBJECT*)pThis);
}


/**************************************************************************/ /*!
* @brief   The function gets the values of gauge limit structure
* @param   pThis - pointer to gauge object
* @param   pLimits - the pointer to limit structure where will be stored the data of current using limit structure
* @return  none
* @note    none
*******************************************************************************/
void D4D_GaugGetLimits(D4D_OBJECT_PTR pThis, D4D_GAUGE_LIMITS* pLimits)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    *pLimits = pGaug->pData->limits;
}

/**************************************************************************/ /*!
* @brief   The function set the trend/direction of gauge
* @param   pThis - pointer to gauge object
* @param   trend - the new trend of gauge
* @return  none
* @note    none
*******************************************************************************/
void D4D_GaugSetDir(D4D_OBJECT_PTR pThis, D4D_TREND trend)
{
  D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
  D4D_GAUGE_DATA* pData = pGaug->pData;

  if(trend != pData->trend)
  {
    pData->valueLast = pData->value;
    pData->trend = trend;

    D4D_InvalidateObject(pThis, D4D_TRUE);

    if(pGaug->OnValueChanged)
        pGaug->OnValueChanged((D4D_OBJECT*)pThis);
  }
}

/**************************************************************************/ /*!
* @brief   The function gets the trend/direction of gauge object
* @param   pThis - pointer to gauge object
* @return  current trend/direction of gauge object
* @note    none
*******************************************************************************/
D4D_TREND D4D_GaugGetDir(D4D_OBJECT_PTR pThis)
{
  D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
  return pGaug->pData->trend;
}
/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_gauge_func                                               */
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
* The main GAUGE message handler
*
*******************************************************/

void D4D_GaugOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_GaugOnDraw(pMsg);
        break;
#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
        D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
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
D4D_STRING* D4D_GaugeGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_GAUGE(pThis)->textBuff);
}
