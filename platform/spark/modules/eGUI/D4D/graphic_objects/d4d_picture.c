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
* @file      d4d_picture.c
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.23.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver picture object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_PictOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_PictGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_pictSysFunc =
{
  D4D_DEFSTR("Picture"),
  D4D_PictOnMessage,
  NULL,
  NULL
};

/*******************************************************
*
* PICTURE Drawing routine
*
*******************************************************/

static void D4D_PictOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_PICTURE* pPict = D4D_GET_PICTURE(pThis);
    D4D_COLOR clrB = D4D_ObjectGetBckgFillColor(pThis);
    D4D_COLOR clrT = D4D_ObjectGetForeColor(pThis, pMsg->prm.draw);

    // Draw the frame if needed
    if(pMsg->prm.draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // draw the bitmap
    if(pPict->pBmp != NULL)
    {
      D4D_GEOMETRY contentGeom;
      D4D_ComputeGeometry(&contentGeom, pThis);

      D4D_DrawRBmpRect(&contentGeom.pnt, &contentGeom.sz, pPict->pBmp, (D4D_PIC_PRTY_DEFAULT), clrB, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
    }
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
* The main PICTURE message handler
*
*******************************************************/

void D4D_PictOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_PictOnDraw(pMsg);
        break;

    default:
        // call the default behavior
        D4D_ObjOnMessage(pMsg);
    }
}



