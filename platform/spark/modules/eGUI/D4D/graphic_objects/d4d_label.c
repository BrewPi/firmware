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
* @file      d4d_label.c
*
* @author     Petr Gargulak
*
* @version   0.0.44.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver label object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"


// Internal API
void D4D_LabelOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_LabelGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_labelSysFunc =
{
  D4D_DEFSTR("Label"),
  D4D_LabelOnMessage,
  NULL,
  D4D_LabelGetTextBuffer
};

typedef struct
{
  D4D_POINT position;
  D4D_GEOMETRY contentGeom;
} D4D_LBL_TMP_VAL;

#define _calc (*((D4D_LBL_TMP_VAL*)d4d_scratchPad))


static void D4D_LblValue2Coor(D4D_OBJECT* pThis)
{
  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&_calc.contentGeom, pThis);
}



/*******************************************************
*
* LABEL Drawing routine
*
*******************************************************/

static void D4D_LabelOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_LABEL* pLbl = D4D_GET_LABEL(pThis);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;

    D4D_LblValue2Coor(pThis);

    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgColor(pThis, draw);

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // draw the text
    if(pLbl->textBuff.pText)
      D4D_DrawTextRRect(&_calc.contentGeom.pnt, &_calc.contentGeom.sz, &pLbl->textBuff, clrT, clrB, pThis->radius);
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
* The LABEL message handler
*
*******************************************************/

void D4D_LabelOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_LabelOnDraw(pMsg);
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

/*******************************************************
*
* The LABEL check coordinators  handler
*
*******************************************************/

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_LabelGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_LABEL(pThis)->textBuff);
}
