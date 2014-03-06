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
* @file      d4d_icon.c
*
* @author     Petr Gargulak
*
* @version   0.0.48.0
*
* @date      Oct-16-2013
*
* @brief     D4D driver icon object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_IconOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_IconGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_iconSysFunc =
{
  D4D_DEFSTR("Icon"),
  D4D_IconOnMessage,
  NULL,
  D4D_IconGetTextBuffer
};

// temporary structure for icon coordinate calculation
typedef struct
{
    D4D_POINT position;
    D4D_POINT txtPos;
    D4D_GEOMETRY contentGeom;
} D4D_ICON_TMP_VAL;

#define _calc (*((D4D_ICON_TMP_VAL*)d4d_scratchPad))


static void D4D_IconValue2Coor(D4D_OBJECT* pThis)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);

  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&_calc.contentGeom, pThis);
  _calc.txtPos = _calc.contentGeom.pnt;
  _calc.txtPos.x += pIcon->txtOff.x;
  _calc.txtPos.y += pIcon->txtOff.y;

}

/*******************************************************
*
* ICON Drawing routine
*
*******************************************************/

static void D4D_IconOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis  =  pMsg->pObject;
    D4D_ICON* pIcon = D4D_GET_ICON(pThis);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_BMP_PROPERTIES bmpProp;

    D4D_IconValue2Coor(pThis);

    bmpProp = (D4D_ALIGN_V_TOP_MASK | D4D_ALIGN_H_CENTER_MASK);

    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgColor(pThis, draw);

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // draw the bitmap
    if(pIcon->pBmpX[pIcon->pData->index] != NULL)
      D4D_DrawRBmpRect(&_calc.contentGeom.pnt,&_calc.contentGeom.sz, pIcon->pBmpX[pIcon->pData->index], bmpProp, clrB, (D4D_BOOL)(!D4D_IsEnabled(pThis)), pThis->radius);

    // draw the text
    if(pIcon->textBuff.pText != NULL)
    {
      D4D_SIZE tmp_size;
      tmp_size.cx = D4D_GetTextWidth(pIcon->textBuff.fontId, pIcon->textBuff.pText);
      tmp_size.cy = D4D_GetFontHeight(pIcon->textBuff.fontId);

      D4D_DrawTextRect(&_calc.txtPos, &tmp_size, &pIcon->textBuff, clrT, clrB);
    }
}

/******************************************************************************
* Begin of D4D_ICON public functions
*//*! @addtogroup doxd4d_icon_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function returns the icon bitmap count
* @param   pThis - pointer to the icon object that should be sets.
* @return  count of icon bitmaps
* @note    none
*******************************************************************************/
D4D_ICON_INDEX D4D_IconGetBmpCount(D4D_OBJECT_PTR pThis)
{
   D4D_ICON* pIcon = D4D_GET_ICON(pThis);
   D4D_ICON_INDEX  ix = 0;


   while(pIcon->pBmpX[ix] != NULL)
   {
     ix++;
   }

   return ix;
}

/**************************************************************************/ /*!
* @brief   Function sets the new bitmap of icon by icon index
* @param   pThis - pointer to the icon object that should be sets.
* @param   index - index of new bitmpa that should be shown
* @return  none
* @note    none
*******************************************************************************/
void D4D_IconSetIndex(D4D_OBJECT_PTR pThis, D4D_ICON_INDEX index)
{
    D4D_ICON* pIcon = D4D_GET_ICON(pThis);
    D4D_ICON_DATA* pData = pIcon->pData;

    if(pData->index == index)
        return ;

    if(D4D_IconGetBmpCount(pThis) == 0)  // There is no loaded bitmaps - go out
        return;

    if(index >= D4D_IconGetBmpCount(pThis))  // If value is bigger then count of BMPs
        return;

    // remember new value
    pData->index = index;

    D4D_InvalidateObject(pThis, D4D_TRUE);

    // notify user that the value has changed
    if(pIcon->OnValueChanged)
        pIcon->OnValueChanged((D4D_OBJECT*)pThis);
}


/**************************************************************************/ /*!
* @brief   Function sets the new bitmap of icon by icon index difference
* @param   pThis - pointer to the icon object that should be sets.
* @param   incr - sign value for compute of new bitmap index relative to current one
* @return  none
* @note    none
*******************************************************************************/
void D4D_IconChangeIndex(D4D_OBJECT_PTR pThis, sByte incr)
{
  D4D_ICON_INDEX tmp_ix = (D4D_ICON_INDEX)(D4D_GET_ICON(pThis)->pData->index + incr);

  if(tmp_ix >= D4D_IconGetBmpCount(pThis))
    tmp_ix = 0;

  D4D_IconSetIndex(pThis, tmp_ix);
}


/**************************************************************************/ /*!
* @brief   Function returns current visible bitmap index
* @param   pThis - pointer to the icon object that should be sets.
* @return  index of current visible index
* @note    none
*******************************************************************************/
D4D_ICON_INDEX D4D_IconGetIndex(D4D_OBJECT_PTR pThis)
{
    D4D_ICON* pIcon = D4D_GET_ICON(pThis);
    return pIcon->pData->index;
}

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE

/**************************************************************************/ /*!
* @brief   Function starts the animation of icon
* @param   pThis - pointer to the icon object that should be sets.
* @return  none
* @note    In fact starts periodic changes of ICON bitmap, time is specified by
*          D4D Time Ticks and defined ticks treshold.
*******************************************************************************/
void D4D_IconAnimationStart(D4D_OBJECT_PTR pThis)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  D4D_ICON_DATA* pData = pIcon->pData;

  if(D4D_IconGetBmpCount(pThis))
  {
    pData->animationEnabled = D4D_TRUE;
    pData->tickCounter = 0;
    D4D_EnableTimeTicks(pThis, D4D_TRUE);
  }
}

/**************************************************************************/ /*!
* @brief   Function stops the animation of icon
* @param   pThis - pointer to the icon object that should be sets.
* @return  none
* @note    In fact stops periodic changes of ICON bitmap, time is specified by
*          D4D Time Ticks and defined ticks treshold.
*******************************************************************************/
void D4D_IconAnimationStop(D4D_OBJECT_PTR pThis)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  D4D_ICON_DATA* pData = pIcon->pData;

  pData->animationEnabled = D4D_FALSE;
  D4D_EnableTimeTicks(pThis, D4D_FALSE);
}


/**************************************************************************/ /*!
* @brief   Function sets the time tick tresholds for animation speed
* @param   pThis - pointer to the icon object that should be sets.
* @param   cnt - count of time ticks to change the bitmap
* @return  none
* @note    The speed of animation is done by period of D4D 1 / (time ticks * count of treshold)
*******************************************************************************/
void D4D_IconAnimationSetCounts(D4D_OBJECT_PTR pThis, Word cnt)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  D4D_ICON_DATA* pData = pIcon->pData;

  pData->tickCounterTrshld = cnt;
}

#endif

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_icon_func                                                */
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
* The main ICON message handler
*
*******************************************************/

void D4D_IconOnMessage(D4D_MESSAGE* pMsg)
{

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE
  D4D_ICON* pIcon = D4D_GET_ICON(pMsg->pObject);
  D4D_ICON_DATA* pData = pIcon->pData;
#endif

  switch(pMsg->nMsgId)
  {



    case D4D_MSG_DRAW:
      D4D_IconOnDraw(pMsg);
      break;

#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
      break;
#endif

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE

    case D4D_MSG_ONINIT:
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;

      pData->tickCounterTrshld = D4D_ICON_ANIMATION_TICK_COUNTER;
      break;

    case D4D_MSG_TIMETICK:
      if(pData->animationEnabled)
      {
        if(++(pData->tickCounter) > pData->tickCounterTrshld)
        {
          pData->tickCounter = 0;
          // update animation (change icon)
          D4D_IconChangeIndex(pMsg->pObject, 1);
        }
      }
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
D4D_STRING* D4D_IconGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_ICON(pThis)->textBuff);
}

