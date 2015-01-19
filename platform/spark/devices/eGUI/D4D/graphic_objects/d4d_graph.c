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
* @file      d4d_graph.c
*
* @author     Petr Gargulak
*
* @version   0.0.36.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver graph object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_GraphOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_GraphGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_graphSysFunc =
{
  D4D_DEFSTR("Graph"),
  D4D_GraphOnMessage,
  NULL,
  D4D_GraphGetTextBuffer
};

// temporary structure for icon coordinate calculation
typedef struct
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
    D4D_POINT graphPos;
    D4D_SIZE  graphSize;
    D4D_POINT valLblOff;
    Byte line_cnt;
    D4D_COOR last_Xpos;
    D4D_COOR curr_Xpos;
} D4D_GRAPH_TMP_VAL;

#define _calc (*((D4D_GRAPH_TMP_VAL*)d4d_scratchPad))

static void D4D_GraphUpdateTempPosX(D4D_OBJECT* pThis) {
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  D4D_GRAPH_DATA* pData = pGraph->pData;

  _calc.last_Xpos = (D4D_COOR)(_calc.graphPos.x + D4D_MulDivUU16((Word)((pData->xPosCnt - 1) % pData->dataShowX), _calc.graphSize.cx, pData->dataShowX));
  _calc.curr_Xpos = (D4D_COOR)(_calc.graphPos.x + D4D_MulDivUU16((Word)(pData->xPosCnt % pData->dataShowX), _calc.graphSize.cx, pData->dataShowX));

  if(_calc.curr_Xpos < _calc.last_Xpos)
    _calc.curr_Xpos = (D4D_COOR)(_calc.last_Xpos + D4D_MulDivUU16(1, _calc.graphSize.cx, pData->dataShowX));
}



static void D4D_GraphValue2Coor(D4D_OBJECT* pThis)
{
  Byte i;
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);

  _calc.valLblOff.y = 0;
  _calc.valLblOff.x = 0;

  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);

  D4D_ComputeGeometry(&_calc.contentGeom, pThis);

  _calc.graphPos = _calc.contentGeom.pnt;
  _calc.graphPos.x += D4D_GRAPH_BORDER_OFF;

  if((pGraph->textBuff.pText == NULL) || (D4D_GetFont(pGraph->textBuff.fontId) == NULL))
  {
    _calc.graphPos.y += D4D_GRAPH_BORDER_OFF;
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    if(pThis->radius > D4D_GRAPH_BORDER_OFF)
      _calc.graphPos.y += (D4D_COOR)(pThis->radius - D4D_GRAPH_BORDER_OFF);
#endif
  }
  else
    _calc.graphPos.y += (D4D_COOR)(D4D_GetFontHeight(pGraph->textBuff.fontId) * 3 / 2);

  if(pThis->initFlags & D4D_GRAPH_F_VALUE_X_TOP)
  {
    _calc.valLblOff.y = (D4D_COOR)(_calc.graphPos.y - _calc.contentGeom.pnt.y);
    _calc.graphPos.y += (D4D_COOR)(D4D_GRAPH_VALUE_OFF + D4D_GetFontHeight(pGraph->labelFont));
  }

  if(pThis->initFlags & D4D_GRAPH_F_VALUE_Y_LEFT)
  {
    _calc.valLblOff.x = (D4D_COOR)(D4D_GRAPH_VALUE_OFF);
    _calc.graphPos.x += (D4D_COOR)(D4D_GRAPH_VALUE_OFF + pGraph->pData->lblSizeY);
  }

  _calc.graphSize.cx = (D4D_COOR)(_calc.contentGeom.sz.cx - (_calc.graphPos.x -_calc.contentGeom.pnt.x) - D4D_GRAPH_BORDER_OFF);

  _calc.graphSize.cy = (D4D_COOR)(_calc.contentGeom.sz.cy  - (_calc.graphPos.y -_calc.contentGeom.pnt.y) - D4D_GRAPH_BORDER_OFF);


  if(pThis->initFlags & D4D_GRAPH_F_VALUE_X_BOTT)
  {
    _calc.graphSize.cy -= (D4D_COOR)(D4D_GRAPH_VALUE_OFF + D4D_GetFontHeight(pGraph->labelFont));
    _calc.valLblOff.y = (D4D_COOR)(_calc.graphPos.y - _calc.contentGeom.pnt.y + _calc.graphSize.cy + D4D_GRAPH_VALUE_OFF + 2);
  }
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  else
    if(pThis->radius > D4D_GRAPH_BORDER_OFF)
      _calc.graphSize.cy -= (D4D_COOR)(pThis->radius - D4D_GRAPH_BORDER_OFF);
#endif


  if(pThis->initFlags & D4D_GRAPH_F_VALUE_Y_RIGHT)
  {
    _calc.graphSize.cx -= (D4D_COOR)(D4D_GRAPH_VALUE_OFF +  pGraph->pData->lblSizeY);
    _calc.valLblOff.x = (D4D_COOR)(_calc.graphPos.x - _calc.contentGeom.pnt.x + _calc.graphSize.cx + D4D_GRAPH_VALUE_OFF + 2);
  }
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  else
    if(pThis->radius > D4D_GRAPH_BORDER_OFF)
      _calc.graphSize.cx -= (D4D_COOR)(pThis->radius - D4D_GRAPH_BORDER_OFF);
#endif

  i=0;
  while(pGraph->traces[i].pData != NULL)
    i++;
  _calc.line_cnt = i;
}

static D4D_COOR D4D_GraphGetSizeLblY(D4D_OBJECT* pThis) {
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);

  Byte i, j, k;

  if(pGraph->OnNeedLabelText == NULL)
    return (D4D_COOR)(3 * D4D_GetFontWidth(pGraph->labelFont)); // for label will be used numbers from 0 to 255

  j = 0;

  for(i=0; i < pGraph->grid.y_cnt; i++)
  {
    k = (Byte)D4D_GetTextLength(pGraph->OnNeedLabelText(pThis,D4D_FALSE, i, 1));
    if(j < k)
    {
      j = k;
    }
  }

  return (D4D_COOR)(j * D4D_GetFontWidth(pGraph->labelFont));
}

static D4D_COOR D4D_GraphGetGridCoorX(D4D_OBJECT* pThis, Byte gridIx)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  return(D4D_COOR)(_calc.graphPos.x + D4D_MulDivUU16(gridIx, _calc.graphSize.cx, (Word)(pGraph->grid.x_cnt + 1)));
}

static D4D_COOR D4D_GraphGetGridCoorY(D4D_OBJECT* pThis, Byte gridIx)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  return(D4D_COOR)(_calc.graphPos.y + D4D_MulDivUU16(gridIx, _calc.graphSize.cy, (Word)(pGraph->grid.y_cnt + 1)));
}


static void D4D_GraphPrintLabel(D4D_OBJECT* pThis, D4D_BOOL axisX, Byte gridIx, D4D_GRAPH_SAMPLE_IX sampleIx)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  D4D_COOR GridOffX = (D4D_COOR)(_calc.graphSize.cx / (pGraph->grid.x_cnt + 1));
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_str_prty;
  D4D_TCHAR tmp_str[8];
  D4D_TCHAR* pStr;

  tmp_str_prty.font_properties = D4D_GRAPH_LBL_FNT_PRTY_DEFAULT;
  tmp_str_prty.text_properties = 0;

  tmp_txtbuff.str_properties = &tmp_str_prty;


  if(axisX) {
    if(gridIx > pGraph->grid.x_cnt)
      return;
  }else {
    if(gridIx > pGraph->grid.y_cnt)
      return;
  }

  if(pGraph->OnNeedLabelText == NULL) {
    if(axisX) {
      tmp_str[D4D_SprintDecU16((Word)(sampleIx), &tmp_str[0], 0)] = 0;
    }else{
      tmp_str[D4D_SprintDecU8((Byte)((256 / (pGraph->grid.y_cnt + 1)) * (pGraph->grid.y_cnt - gridIx + 1)), &tmp_str[0], 0)] = 0;
    }

    pStr = tmp_str;
  }else {
    pStr = pGraph->OnNeedLabelText(pThis, axisX, (D4D_INDEX)((axisX)? gridIx : (pGraph->grid.y_cnt - gridIx + 1)), sampleIx);
  }

  tmp_size.cy = D4D_GetFontHeight(pGraph->labelFont); // invoke autosize capability in draw function for y axis

  if(axisX) {
    tmp_point.x = D4D_GraphGetGridCoorX(pThis, gridIx);
    tmp_point.x -= (D4D_COOR)(GridOffX / 2);
    tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.valLblOff.y);
    tmp_size.cx = (D4D_COOR)(GridOffX - 1);
  }else {
    // axis Y
    tmp_point.y = D4D_GraphGetGridCoorY(pThis, gridIx);
    tmp_point.y -= (D4D_COOR)(1 + (tmp_size.cy / 2));
    tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.valLblOff.x);
    tmp_size.cx = pData->lblSizeY;
  }



  tmp_txtbuff.pText = pStr;
  tmp_txtbuff.fontId = pGraph->labelFont;
  tmp_txtbuff.str_properties->text_properties = D4D_ALIGN_H_CENTER_MASK; // center aligment
  tmp_txtbuff.buffSize = 0;
  tmp_txtbuff.printLen = D4D_GetTextLength(tmp_txtbuff.pText);
  tmp_txtbuff.printOff = 0;
  D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, D4D_ObjectGetForeFillColor(pThis), D4D_ObjectGetBckgFillColor(pThis));

}


static void D4D_GraphDrawEmptyGraph(D4D_OBJECT* pThis) {
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  D4D_COLOR clrT;
  D4D_COOR tmp_coor;
  Byte i;

  D4D_GRAPH_SAMPLE_IX tmp_sampleIx = pGraph->pData->xPosCnt;

  tmp_sampleIx -= tmp_sampleIx % pGraph->pData->dataShowX;

  // Draw graph background box
  D4D_BoxXY((D4D_COOR)(_calc.graphPos.x - 1), (D4D_COOR)(_calc.graphPos.y - 1), (D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 2), (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy + 2), D4D_LINE_THIN, D4D_ObjectGetForeFillColor(pThis), D4D_ObjectGetBckgFillColor(pThis));

  clrT = D4D_ObjectGetScheme(pThis)->objectDepend.graph.grid;

  if(!D4D_IsEnabled(pThis))
    clrT = D4D_GetGreyScale(clrT);

  // Draw drig lines in X direction
  for(i = 1; i < (pGraph->grid.x_cnt + 1);i++ )
  {
    tmp_coor = D4D_GraphGetGridCoorX(pThis, i);
    D4D_MoveToXY(tmp_coor, _calc.graphPos.y);
    D4D_LineToXY(tmp_coor, (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy + 1), D4D_LINE_THIN, clrT);
    // Draw values on axis X if are enabled

    if(_calc.valLblOff.y)
    {
      D4D_GraphPrintLabel(pThis, D4D_TRUE, (Byte)i, tmp_sampleIx + (i * (pGraph->pData->dataShowX / (pGraph->grid.x_cnt + 1))));
    }
  }

  // Draw drig lines in Y direction
  for(i=1; i < (pGraph->grid.y_cnt + 1);i++ )
  {
    D4D_MoveToXY(_calc.graphPos.x, D4D_GraphGetGridCoorY(pThis, i));
    D4D_LineToXY((D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 1), D4D_GraphGetGridCoorY(pThis, i), D4D_LINE_THIN, clrT);
    if(_calc.valLblOff.x)
    {
      D4D_GraphPrintLabel(pThis, D4D_FALSE, (Byte)i, 1);
    }
  }
}

/*******************************************************
*
* GRAPH Drawing routine
*
*******************************************************/

static void D4D_GraphOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
    D4D_GRAPH_DATA* pData = pGraph->pData;
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_COLOR clrGrid = D4D_ObjectGetScheme(pThis)->objectDepend.graph.grid;
    D4D_INDEX i;
    D4D_POINT tmp_linePoint;

    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgFillColor(pThis);

    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif

    D4D_GraphValue2Coor(pThis);

    /*****************************************
     * Draw complete object
     ****************************************/
    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    {
      if(pThis->initFlags & (D4D_GRAPH_F_VALUE_Y_LEFT | D4D_GRAPH_F_VALUE_Y_RIGHT))
        pData->lblSizeY = D4D_GraphGetSizeLblY(pThis);

      D4D_GraphValue2Coor(pThis);

      // Draw main background box
      D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);

      // draw the text
      if(pGraph->textBuff.pText != NULL)
      {
        D4D_SIZE tmp_size;
        tmp_size.cy = D4D_GetFontHeight(pGraph->textBuff.fontId); // invoke automat
        tmp_linePoint.x = (D4D_COOR)(_calc.contentGeom.pnt.x);
        tmp_linePoint.y = (D4D_COOR)(_calc.contentGeom.pnt.y + tmp_size.cy / 4);
        tmp_size.cx = (D4D_COOR)(_calc.contentGeom.sz.cx - 2);
        #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
          tmp_linePoint.x += pThis->radius;
          tmp_size.cx -= (D4D_COOR)(2 * pThis->radius);
        #endif
        D4D_DrawTextRect(&tmp_linePoint, &tmp_size, &pGraph->textBuff, D4D_ObjectGetForeFillColor(pThis), D4D_ObjectGetBckgFillColor(pThis));
      }

      D4D_GraphDrawEmptyGraph(pThis);



      // Sets the last data index to redraw all bytes in Memory on screen in complete redraw action
      if(pData->initData < (pGraph->dataLenght - 1)) {
        pData->xPosCnt = 1;
        pData->lastShowPos = 0;
      }
      else {
        pData->lastShowPos = (D4D_GRAPH_DATA_LEN)(pData->lastDataPos + 1);
        i = (D4D_COOR)(pData->xPosCnt % pData->dataShowX);
        if(i > pGraph->dataLenght)
          i = pGraph->dataLenght;
        pData->xPosCnt -= (D4D_COOR)i;//(pData->xPosCnt % _calc.graphSize.cx);
      }

      D4D_GraphUpdateTempPosX(pThis);
      pData->columnDrawed = (D4D_COOR)(_calc.last_Xpos - 1);

      i = 1;
      while((D4D_GraphGetGridCoorX(pThis,(Byte) i) < _calc.curr_Xpos) && (i < pGraph->grid.x_cnt)) {
        i++;
      };
      pData->drigDrawedX = i;
    }

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

   /*********************************************
   * Draw graph all lines from all pending input record
   ********************************************/
    while(pData->lastShowPos != pData->lastDataPos)
    {

      D4D_GraphUpdateTempPosX(pThis);

      // if normla mode of graph is selected then in each lap of run the graph has to be redrawed
      if((pThis->initFlags & D4D_GRAPH_F_MODE_MASK) == D4D_GRAPH_F_MODE_NORMAL)
      {
        /*********************************************
         * Draw graph in normal mode when it over run
         ********************************************/
        if(pData->columnDrawed > _calc.curr_Xpos)
        {
          D4D_GraphDrawEmptyGraph(pThis);
          pData->drigDrawedX = 1;
        }else
        {
          // Draw values on axis X if are enabled
          if(_calc.valLblOff.y)
          {
            if(_calc.curr_Xpos != pData->columnDrawed) // check if this label was drawed yet on
            {
              if(D4D_GraphGetGridCoorX(pThis, (Byte)pData->drigDrawedX) < _calc.curr_Xpos)
              {
                D4D_GraphPrintLabel(pThis, D4D_TRUE, (Byte)pData->drigDrawedX, pData->xPosCnt);
                pData->drigDrawedX++;
              }
            }
          }
        }
      }

      if(pData->columnDrawed > _calc.curr_Xpos) {
        pData->drigDrawedX = 1;
        pData->columnDrawed = (D4D_COOR)(_calc.graphPos.x - 1);
      }

      if((pThis->initFlags & D4D_GRAPH_F_MODE_MASK) == D4D_GRAPH_F_MODE_ROLLOVER)
      {
        if(pData->columnDrawed < _calc.curr_Xpos) {

         D4D_POINT tmp_Point;

         if(_calc.last_Xpos == _calc.graphPos.x) {
           D4D_MoveToXY((D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 1), _calc.graphPos.y);
           D4D_FillRectToXY((D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 1), (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy), clrB);
         }

          // *****  Clear the old drawed traces *****

          tmp_Point.y = _calc.graphPos.y;
          tmp_Point.x = (D4D_COOR)(pData->columnDrawed + 1);//(D4D_COOR)(_calc.last_Xpos + 1);
          D4D_MoveTo(&tmp_Point);

          tmp_Point.y += (D4D_COOR)(_calc.graphSize.cy + 1);
          tmp_Point.x = _calc.curr_Xpos;
          D4D_FillRectTo(&tmp_Point, clrB);

          // *****  End of: Clear the old drawed traces *****

          if(D4D_IsEnabled(pThis))
            clrT = clrGrid;
          else
            clrT = D4D_GetGreyScale(clrGrid);


          //Draw vertical grid lines if needed
          for(i = _calc.last_Xpos; i < _calc.curr_Xpos;i++)
          {
            if(D4D_GraphGetGridCoorX(pThis, (Byte)pData->drigDrawedX) <= i)
            {
              if(pData->drigDrawedX <= pGraph->grid.x_cnt) {

                tmp_Point.x = (D4D_COOR)(i);
                tmp_Point.y = _calc.graphPos.y;
                D4D_MoveTo(&tmp_Point);
                tmp_Point.y += (D4D_COOR)(_calc.graphSize.cy + 1) ;
                D4D_LineTo(&tmp_Point, D4D_LINE_THIN, clrT);

                if(_calc.valLblOff.y)
                  D4D_GraphPrintLabel(pThis, D4D_TRUE, (Byte)pData->drigDrawedX,  pData->xPosCnt);

                pData->drigDrawedX++;
              }
            }
          }

          //Draw horizontal grid lines if needed
          for(i=1; i < (pGraph->grid.y_cnt + 1);i++ )
          {
            tmp_Point.y = D4D_GraphGetGridCoorY(pThis,(Byte) i);
            tmp_Point.x = _calc.last_Xpos;
            D4D_MoveTo(&tmp_Point);
            tmp_Point.x = _calc.curr_Xpos;
            D4D_LineTo(&tmp_Point, D4D_LINE_THIN, clrT);
          }

          // Draw disjuctive line

          tmp_Point.x = (D4D_COOR)(_calc.curr_Xpos + 1);
          if(tmp_Point.x <= (_calc.graphPos.x + _calc.graphSize.cx)) {

            tmp_Point.y = _calc.graphPos.y;
            D4D_MoveTo(&tmp_Point);
            tmp_Point.y += (D4D_COOR)(_calc.graphSize.cy + 1);
            D4D_LineTo(&tmp_Point, D4D_LINE_THIN, D4D_ObjectGetForeColor(pThis, draw));
          }
        }
      }

      // add only a new data
      for(i=0; i < _calc.line_cnt;i++) // for all lines
      {
        tmp_linePoint.x = _calc.last_Xpos;

        switch(pGraph->traces[i].type & D4D_GRAPH_TRACE_TYPE_MASK)
        {
          case D4D_GRAPH_TRACE_TYPE_LINE:
            // Compute the last position of line

            if(pData->xPosCnt == 0)
            {
              tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy);
              D4D_MoveTo(&tmp_linePoint);
            }else
            {
              D4D_INDEX lastData;

              if(pGraph->pData->lastShowPos == 0)
                lastData = (D4D_INDEX)(pGraph->dataLenght - 1);
              else
                lastData = (D4D_INDEX)(pGraph->pData->lastShowPos - 1);

              tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy - D4D_MulDivUU(pGraph->traces[i].pData[lastData], _calc.graphSize.cy, 255));
              D4D_MoveTo(&tmp_linePoint);
            }
            break;

          case D4D_GRAPH_TRACE_TYPE_DOT:
            tmp_linePoint.x = _calc.curr_Xpos;
            tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy - D4D_MulDivUU(pGraph->traces[i].pData[pData->lastShowPos], _calc.graphSize.cy, 255));
            D4D_MoveTo(&tmp_linePoint);
            break;

          case D4D_GRAPH_TRACE_TYPE_AREA:
            if(_calc.curr_Xpos <= (D4D_COOR)(_calc.last_Xpos + 1))
              tmp_linePoint.x = _calc.curr_Xpos;
            tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy);
            D4D_MoveTo(&tmp_linePoint);
            break;

          case D4D_GRAPH_TRACE_TYPE_AREA_INV:
            if(_calc.curr_Xpos <= (D4D_COOR)(_calc.last_Xpos + 1))
              tmp_linePoint.x = _calc.curr_Xpos;
            tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y);
            D4D_MoveTo(&tmp_linePoint);
            break;

          default:
            break;
        }

        // Draw line
        tmp_linePoint.x = _calc.curr_Xpos;
        tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy - D4D_MulDivUU(pGraph->traces[i].pData[pData->lastShowPos], _calc.graphSize.cy, 255));

        if((((pGraph->traces[i].type & D4D_GRAPH_TRACE_TYPE_MASK) == D4D_GRAPH_TRACE_TYPE_AREA) || ((pGraph->traces[i].type & D4D_GRAPH_TRACE_TYPE_MASK) == D4D_GRAPH_TRACE_TYPE_AREA_INV)) && (_calc.curr_Xpos > (D4D_COOR)(_calc.last_Xpos + 1)))
          D4D_FillRectTo(&tmp_linePoint, pGraph->traces[i].color);
        else
          D4D_LineTo(&tmp_linePoint, pGraph->traces[i].line, pGraph->traces[i].color);
      }

      if(++pData->lastShowPos >= pGraph->dataLenght)
      {
        pData->lastShowPos = 0;
      }

      pData->columnDrawed = _calc.curr_Xpos;
      pData->xPosCnt++;
    }

}

/******************************************************************************
* Begin of D4D_GRAPH public functions
*//*! @addtogroup doxd4d_graph_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function add new data to one trace
* @param   pObj - pointer to graph object
* @param   trace_ix - index of trace to add data
* @param   value - new data to trace
* @return  none
* @note    In case of more traces is used in graph, all other traces are updated with their last value.
*******************************************************************************/
D4D_BOOL D4D_GraphAddTraceData(D4D_OBJECT_PTR pObj, D4D_INDEX trace_ix,  D4D_GRAPH_VALUE value)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  Byte traces_cnt=0;
  Byte i;
  D4D_INDEX lastData;

  value = (D4D_GRAPH_VALUE)(((1<<(sizeof(D4D_GRAPH_VALUE)*8))- 1) - value);

  if(pGraph->pData->lastDataPos == 0)
    lastData = (D4D_INDEX)(pGraph->dataLenght - 1);
  else
    lastData = (D4D_INDEX)(pGraph->pData->lastDataPos - 1);


  while(pGraph->traces[traces_cnt].pData != NULL)
    traces_cnt++;

  if(trace_ix > traces_cnt)
    return D4D_FALSE;

  for(i=0;i< traces_cnt;i++)
  {
    if(i == trace_ix)
    {
      pGraph->traces[i].pData[pGraph->pData->lastDataPos] = value;
    }else
    {
      if(pGraph->pData->xPosCnt)
        pGraph->traces[i].pData[pGraph->pData->lastDataPos] = 0;
      else
        pGraph->traces[i].pData[pGraph->pData->lastDataPos] = pGraph->traces[i].pData[lastData];
    }
  }

  if(pGraph->pData->initData < (pGraph->dataLenght - 1)) {
    pGraph->pData->initData = (Word)(pGraph->pData->lastDataPos + 1);
  }

  if(++pGraph->pData->lastDataPos >= pGraph->dataLenght)
    pGraph->pData->lastDataPos = 0;


  if(pGraph->pData->lastDataPos == pGraph->pData->lastShowPos)
  {
    if(++pGraph->pData->lastShowPos >= pGraph->dataLenght)
      pGraph->pData->lastShowPos = 0;

    pGraph->pData->xPosCnt++;
  }

  D4D_InvalidateObject(pObj, D4D_FALSE);

  return D4D_TRUE;
}

/**************************************************************************/ /*!
* @brief   The function add new data to all traces
* @param   pObj - pointer to graph object
* @param   pValues - array of new data for all traces (it should has same lenght as is traces count)
* @return  none
* @note    This is preffered way how to add data to all traces at ones
*******************************************************************************/
void D4D_GraphAddTracesData(D4D_OBJECT_PTR pObj, D4D_GRAPH_VALUE* pValues)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  Byte line_cnt=0;
  Byte i;

  while(pGraph->traces[line_cnt].pData != NULL)
    line_cnt++;

  for(i=0;i< line_cnt;i++)
  {
      pGraph->traces[i].pData[pGraph->pData->lastDataPos] = (D4D_GRAPH_VALUE)(((1<<((sizeof(D4D_GRAPH_VALUE)) * 8))- 1) - pValues[i]);
  }

  if(pGraph->pData->initData < (pGraph->dataLenght - 1)) {
    pGraph->pData->initData = (Word)(pGraph->pData->lastDataPos + 1);
  }


  if(++pGraph->pData->lastDataPos >= pGraph->dataLenght)
    pGraph->pData->lastDataPos = 0;

  if(pGraph->pData->lastDataPos == pGraph->pData->lastShowPos)
  {
    if(++pGraph->pData->lastShowPos >= pGraph->dataLenght)
      pGraph->pData->lastShowPos = 0;
    pGraph->pData->xPosCnt++;
  }


  D4D_InvalidateObject(pObj, D4D_FALSE);
}

/**************************************************************************/ /*!
* @brief   The function clears all internal data of graph object
* @param   pObj - pointer to graph object
* @return  none
* @note    This function is used to restart graph object
*******************************************************************************/
void D4D_GraphClearAll(D4D_OBJECT_PTR pObj)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  pData->xPosCnt = 0;
  pData->lastShowPos = 0;
  pData->lastDataPos = 0;
  pData->initData = 0;
  D4D_InvalidateObject(pObj, D4D_TRUE);
}

/**************************************************************************/ /*!
* @brief   The function returns the current data sample index
* @param   pObj - pointer to graph object
* @return  data sample counter value
* @note    Each add data function incremeant the 32-bit variable of sample count and this function returns the current value
*******************************************************************************/
LWord D4D_GraphGetSampleIndex(D4D_OBJECT_PTR pObj)
{
  return D4D_GET_GRAPH(pObj)->pData->xPosCnt;
}

/**************************************************************************/ /*!
* @brief   The function returns the size of graph traces in axis X
* @param   pObj - pointer to graph object
* @return  width of active part of graph
* @note    none
*******************************************************************************/
D4D_COOR D4D_GraphGetSizeX(D4D_OBJECT_PTR pObj)
{
  D4D_GraphValue2Coor((D4D_OBJECT*)pObj);
  return _calc.graphSize.cx;
}

/**************************************************************************/ /*!
* @brief   Function sets the new scale of axis X of the graph
* @param   pObj - pointer to graph object
* @param   mul - multiplier of the scale of axis X
* @param   div - divider of the scale of axis X
* @return  result of operation, true if it was executed correctly, false if not
* @note    It allows to set up a graph object to show
*               more or less samples of the graph active area, than is native to axis X. The scale of axis X is set-up by the
*               ratio of input parameters. To set-up a new scale another function \ref D4D_GraphSetDataWidth can be used.
*******************************************************************************/
D4D_BOOL D4D_GraphSetScaleX(D4D_OBJECT_PTR pObj, Byte mul, Byte div)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  D4D_GRAPH_DATA* pData = pGraph->pData;

  if((mul == 0) || (div == 0))
    return D4D_FALSE;

  D4D_GraphValue2Coor((D4D_OBJECT*)pObj);

  pData->dataShowX = (D4D_GRAPH_DATA_LEN)D4D_MulDivUU16(mul, _calc.graphSize.cx, div);

  D4D_InvalidateObject(pObj, D4D_TRUE);

  return D4D_TRUE;
}

/**************************************************************************/ /*!
* @brief   Function sets the new scale of the axis X of the graph
* @param   pObj - pointer to graph object
* @param   samples - a count of samples shown on the graph
* @return  result of operation, true if it was executed correctly, false if not
* @note    Function sets the new scale of the axis X of the graph. It allows setting up a graph object to
*               show more or less samples of a graph active area, than is native to axis X. The scale of axis X sets-up by
*               the count of the shown samples. To set up a new scale, \ref D4D_GraphSetScaleX can be used too.
*******************************************************************************/
D4D_BOOL D4D_GraphSetDataWidth(D4D_OBJECT_PTR pObj, D4D_INDEX samples)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  D4D_GRAPH_DATA* pData = pGraph->pData;

  if(samples == 0)
    return D4D_FALSE;

  pData->dataShowX = (D4D_GRAPH_DATA_LEN)samples;

  D4D_InvalidateObject(pObj, D4D_TRUE);

  return D4D_TRUE;
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_graph_func                                               */
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
* The main GRAPH message handler
*
*******************************************************/

void D4D_GraphOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_GraphOnDraw(pMsg);
        break;
#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
      break;
#endif

    case D4D_MSG_ONINIT:
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;

      if(pMsg->pObject->initFlags & (D4D_GRAPH_F_VALUE_Y_LEFT | D4D_GRAPH_F_VALUE_Y_RIGHT))
        D4D_GET_GRAPH(pMsg->pObject)->pData->lblSizeY = D4D_GraphGetSizeLblY(pMsg->pObject);

      D4D_GraphValue2Coor(pMsg->pObject);

      D4D_GET_GRAPH(pMsg->pObject)->pData->initData = 0;
      D4D_GET_GRAPH(pMsg->pObject)->pData->columnDrawed = (D4D_COOR)(_calc.graphPos.x - 1);

      if(!D4D_GET_GRAPH(pMsg->pObject)->pData->dataShowX) {
        D4D_GET_GRAPH(pMsg->pObject)->pData->dataShowX = _calc.graphSize.cx; // initialization of scale for axis X to scale 1:1
      }
      break;

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
D4D_STRING* D4D_GraphGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_GRAPH(pThis)->textBuff);
}

