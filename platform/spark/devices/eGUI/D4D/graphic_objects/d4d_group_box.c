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
* @file      d4d_group_box.c
*
* @author     Petr Gargulak
*
* @version   0.0.4.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver groupBox object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_GroupBoxOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_GroupBoxGetTextBuffer(D4D_OBJECT* pThis);

D4D_MARGIN groupBox_marginDefault =
{
  D4D_GROUP_BOX_FRAME_MARGIN,
  D4D_GROUP_BOX_HEADER_MARGIN,
  D4D_GROUP_BOX_FRAME_MARGIN,
  D4D_GROUP_BOX_FRAME_MARGIN
};

const D4D_OBJECT_SYS_FUNCTION d4d_groupBoxSysFunc =
{
  D4D_DEFSTR("Group Box"),
  D4D_GroupBoxOnMessage,
  NULL,
  NULL
};

/*******************************************************
*
* GROUP_BOX Drawing routine
*
*******************************************************/

static void D4D_GroupBoxOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_GROUP_BOX* pGroupBox = D4D_GET_GROUP_BOX(pMsg->pObject);

    D4D_POINT pnt = D4D_GetClientToScreenPoint(pThis, &pThis->position);
    D4D_POINT pntLoc = pnt;
    D4D_SIZE size;
    D4D_COLOR clrB = D4D_ObjectGetBckgFillColor(pThis);
    D4D_COLOR clrT = D4D_ObjectGetForeColor(pThis, pMsg->prm.draw);

    // only draw if complete draw is needed
    if(pMsg->prm.draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    {
      D4D_FillRRect(&pntLoc, &pThis->size, clrB, pThis->radius);
    }

    // Draw the frame
    if(pMsg->prm.draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);


    // Draw Focus rect

    if(pThis->initFlags & D4D_GROUP_BOX_F_CONTENT_OUTLINE)
    {
      pntLoc.x += (pThis->pMargin->left / 2);
      pntLoc.y += (pThis->pMargin->top / 2);
      size = pThis->size;
      size.cx -= ((pThis->pMargin->left + pThis->pMargin->right) / 2);
      size.cy -= ((pThis->pMargin->top + pThis->pMargin->bottom) / 2);

  #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
      if(pThis->radius)
      {
        D4D_COOR radius = 0;
        if(pThis->radius - (pThis->pMargin->left / 2) > 0)
          radius = (D4D_COOR)(pThis->radius - (pThis->pMargin->left / 2));

        D4D_RRect(&pntLoc, &size, D4D_LINE_THIN, D4D_ChangeColorIntesity(clrT, D4D_BEVEL_BCKG_INTENSITY), radius);
      }
      else
  #endif
        D4D_Rect(&pntLoc, &size, D4D_LINE_THIN, D4D_ChangeColorIntesity(clrT, D4D_BEVEL_BCKG_INTENSITY));
    }


    if(pGroupBox->textBuff.pText)
    {
      D4D_COOR activeSize = 2 * D4D_GROUP_BOX_TEXT_OFFSET;

      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
        activeSize += (D4D_COOR)(pThis->radius * 2);
      #endif

      if(pThis->initFlags & D4D_OBJECT_F_BEVEL_MASK)
        activeSize += D4D_BEVEL_WIDTH * 2;

      size.cy = pThis->pMargin->top;
      size.cx = (D4D_COOR)(D4D_GetTextWidth(pGroupBox->textBuff.fontId, pGroupBox->textBuff.pText) + (2 * D4D_GROUP_BOX_TEXT_OFFSET));

      if(pThis->initFlags & D4D_OBJECT_F_BEVEL_MASK)
        size.cx += D4D_BEVEL_WIDTH * 2;

      if(size.cx > (pThis->size.cx - activeSize))
        size.cx = (D4D_COOR)(pThis->size.cx - activeSize);

      pnt.x += D4D_GROUP_BOX_TEXT_OFFSET;

      if(pThis->initFlags & D4D_OBJECT_F_BEVEL_MASK)
      {
        pnt.x += D4D_BEVEL_WIDTH;
        pnt.y += D4D_BEVEL_WIDTH;

        if(size.cy - D4D_BEVEL_WIDTH > 0)
          size.cy -= D4D_BEVEL_WIDTH;
        else
          size.cy = 0;
      }

  #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
      if(pThis->radius)
        pnt.x += pThis->radius;
  #endif

      D4D_DrawTextRect(&pnt, &size, &pGroupBox->textBuff, clrT, clrB);
    }

    // draw the bitmap
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
#else
#endif
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
* The main GROUP_BOX message handler
*
*******************************************************/

void D4D_GroupBoxOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_GroupBoxOnDraw(pMsg);
        break;

    default:
        // call the default behavior
        D4D_ObjOnMessage(pMsg);
    }
}
