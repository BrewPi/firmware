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
* @file      d4d_basic_draw.c
*
* @author     Petr Gargulak
*
* @version   0.0.3.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver basic drawing functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_lldapi.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/


/**************************************************************//*!
*
* Local variables
*
******************************************************************/
// last point remembered for MoveTo, LineTo etc.
static D4D_POINT d4d_curXY;

/******************************************************************************
* Begin of D4D_BASIC_DRAW public functions
*//*! @addtogroup doxd4d_basic_draw_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function move logic cursor to new position
* @param   ppt - pointer on point structure that defines point on screen to move cursor
* @return  none
* @note    Function just move the last draw cursor to new position.
*******************************************************************************/
void D4D_MoveTo(D4D_POINT* ppt)
{
    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function move logic cursor to new position
* @param   x - coordination in axis X that defines left corner for cursor move
* @param   y - coordination in axis Y that defines top corner for cursor move
* @return  none
* @note    Function just move the last draw cursor to new position.
*******************************************************************************/
void D4D_MoveToXY(D4D_COOR x, D4D_COOR y)
{
  d4d_curXY.x = x;
  d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function puts one pixel on the screen
* @param   ppt - pointer on point structure that defines point on screen to draw pixel
* @param   type - type of the pixel (thin/thick)
* @param   color - color of pixel
* @return  none
* @note    Function just puts pixel on the screen in any color and could be defined type.
          The logical last draw cursor is moved on the position of draw pixel.
*******************************************************************************/
void D4D_PutPixel(D4D_POINT* ppt, D4D_LINETYPE type, D4D_COLOR color)
{
  D4D_LCD_PutPixel(ppt->x, ppt->y, type, color);
  d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function puts one pixel on the screen
* @param   type - type of the pixel (thin/thick)
* @param   color - color of pixel
* @return  none
* @note    Function just puts pixel on the screen in any color and could be defined type.
           The pixel is puts on the draw cursor position
*******************************************************************************/
void D4D_PutPixelTo(D4D_LINETYPE type, D4D_COLOR color)
{
  D4D_LCD_PutPixel(d4d_curXY.x, d4d_curXY.y, type, color);
}

/**************************************************************************/ /*!
* @brief   Function puts one pixel on the screen
* @param   x - coordination in axis X on screen to draw pixel
* @param   y - coordination in axis Y on screen to draw pixel
* @param   type - type of the pixel (thin/thick)
* @param   color - color of pixel
* @return  none
* @note    Function just puts pixel on the screen in any color and could be defined type.
          The logical last draw cursor is moved on the position of draw pixel.
*******************************************************************************/
void D4D_PutPixelXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE type, D4D_COLOR color)
{
  D4D_LCD_PutPixel(x, y, type, color);
  d4d_curXY.x = x;
  d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function readraw complete screen by one color
* @param   color - screen color
* @return  none
* @note    Function redraw complete screen by one color.
           The logic draw cursor is moved to 0,0 coordination.
*******************************************************************************/
void D4D_ClearScreen(D4D_COLOR color)
{
  D4D_LCD_Box(0, 0, D4D_DisplayWidth(), D4D_DisplayHeight(), color);
  d4d_curXY.x = 0;
  d4d_curXY.y = 0;
}

/**************************************************************************/ /*!
* @brief   Function draw line on the screen
* @param   ppt - pointer on point structure of end of line.
* @param   ltype - type of the line (thin/thick)
* @param   color - color of line
* @return  none
* @note    Function draws the line from logical draw cursor to defined point on
          the screen in any color and could be defined type.
          The logical last draw cursor is moved on the position of draw pixel.
*******************************************************************************/
void D4D_LineTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Line(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, ltype, color);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw line on the screen
* @param   x - coordination in axis X on screen to draw line
* @param   y - coordination in axis Y on screen to draw line
* @param   ltype - type of the line (thin/thick)
* @param   color - color of line
* @return  none
* @note    Function draws the line from logical draw cursor to defined point on
          the screen in any color and could be defined type.
          The logical last draw cursor is moved on the position of draw pixel.
*******************************************************************************/
void D4D_LineToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color)
{
   D4D_LCD_Line(d4d_curXY.x, d4d_curXY.y, x, y, ltype, color);

   d4d_curXY.x = x;
   d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color and could be defined type.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_Rect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color)
{
    if(psz->cx && psz->cy)
      D4D_LCD_Rect((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), (D4D_LINETYPE)ltype, (D4D_COLOR)color);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color and could be defined type.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Rect(x1, y1, x2, y2, ltype, color);

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color and could be defined type.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Rect(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, ltype, color);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color and could be defined type.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Rect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, color);

    d4d_curXY.x = x;
    d4d_curXY.y = y;
}


/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color)
{
    if(psz->cx && psz->cy)
      D4D_LCD_Box((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), (D4D_COLOR)color);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color)
{
    D4D_LCD_Box(x1, y1, x2, y2, color);

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectTo(D4D_POINT* ppt, D4D_COLOR color)
{
    D4D_LCD_Box(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, (D4D_COLOR)color);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   color - color of rectangle
* @return  none
* @note    Function draws the rectangle on the screen in any color and could be defined type.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color)
{
    D4D_LCD_Box(d4d_curXY.x, d4d_curXY.y, x, y, color);

    d4d_curXY.x = x;
    d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @return  none
* @note    Function draws the rectangle with outline on the screen in any colors.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_Box(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
  if(psz->cx && psz->cy)
    D4D_BoxXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), ltype, colorLine, colorFill);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @return  none
* @note    Function draws the rectangle with outline on the screen in any colors.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_BoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
    // fill background
    D4D_LCD_Box(x1, y1, x2, y2, colorFill);

    // draw rectangle around
    D4D_LCD_Rect(x1, y1, x2, y2, ltype, colorLine);

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @return  none
* @note    Function draws the rectangle with outline on the screen in any colors.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_BoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
    D4D_BoxToXY(ppt->x, ppt->y, ltype, colorLine, colorFill);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @return  none
* @note    Function draws the rectangle with outline on the screen in any colors.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_BoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
    // fill background
    D4D_LCD_Box(d4d_curXY.x, d4d_curXY.y, x, y, colorFill);

    // draw rectangle around
    D4D_LCD_Rect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, colorLine);

    d4d_curXY.x = x;
    d4d_curXY.y = y;
}


/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with  changing color (cross color from start to end color) on the screen
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @return  none
* @note    Function draws the rectangle on the screen with crossing color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  if(psz->cx && psz->cy)
    D4D_FillRectColorScaleXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), startColor, endColor, direction);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with  changing color (cross color from start to end color) on the screen
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @return  none
* @note    Function draws the rectangle on the screen with crossing color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  D4D_COOR range, i;
  D4D_COLOR color = startColor;

  d4d_curXY.x = x1;
  d4d_curXY.y = y1;

  switch(direction)
  {
    case D4D_DIR_UP:
      // switch X and Y coodinates and change direction of drawing
      startColor = endColor;
      endColor = color;
    case D4D_DIR_DOWN:
      // just switch X and Y coodinates
      range = (D4D_COOR)(y2 - y1);

      i = 0;
      for( ; y1 <= y2; y1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));
        i++;

        D4D_LCD_Box(x1, y1, x2, y1, color);
      }
      break;

    case D4D_DIR_LEFT:
      // switch colors
      startColor = endColor;
      endColor = color;
    case D4D_DIR_RIGHT:
      // just keep all fine
      range = (D4D_COOR)(x2 - x1);
      i = 0;
      for( ; x1 <= x2; x1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));
        i++;

        D4D_LCD_Box(x1, y1, x1, y2, color);
      }

      break;
  }
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with  changing color (cross color from start to end color) on the screen
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @return  none
* @note    Function draws the rectangle on the screen with crossing color.
          The logical last draw cursor is keep on the same position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  D4D_FillRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, startColor, endColor, direction);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with  changing color (cross color from start to end color) on the screen
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @return  none
* @note    Function draws the rectangle on the screen with crossing color.
          The logical last draw cursor is keep on the same position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  D4D_FillRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, x, y, startColor, endColor, direction);
}


/**************************************************************************/ /*!
* @brief   Function draw filled circle on the screen
* @param   pCenter - pointer on point structure of circle center
* @param   r - radius of circle
* @param   color - color of circle
* @return  none
* @note    Function draws the filled circle on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_FillCircle(D4D_POINT* pCenter, D4D_COOR r, D4D_COLOR color)
{
    D4D_LCD_Circle(pCenter->x, pCenter->y, r, D4D_TRUE, D4D_LINE_THIN, color);
}

/**************************************************************************/ /*!
* @brief   Function draw filled circle on the screen
* @param   x - X coordination of circle center
* @param   y - Y coordination of circle center
* @param   r - radius of circle
* @param   color - color of circle
* @return  none
* @note    Function draws the filled circle on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_FillCircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_COLOR color)
{
    D4D_LCD_Circle(x, y, r, D4D_TRUE, D4D_LINE_THIN, color);
}

/**************************************************************************/ /*!
* @brief   Function draw circle on the screen
* @param   pCenter - pointer to point structure that defines circle center
* @param   r - radius of circle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of circle
* @return  none
* @note    Function draws the circle on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_Circle(D4D_POINT* pCenter, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Circle(pCenter->x, pCenter->y, r, D4D_FALSE, ltype, color);
}

/**************************************************************************/ /*!
* @brief   Function draw circle on the screen
* @param   x - coordination in axis X that defines circle center
* @param   y - coordination in axis Y that defines circle center
* @param   ltype - coordination in axis Y that defines circle center
* @param   r - radius of circle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of circle
* @return  none
* @note    Function draws the circle on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_CircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Circle(x, y, r, D4D_FALSE, ltype, color);
}

/**************************************************************************/ /*!
* @brief   Function draw text on XY coordination with TAB function
* @param   x - coordination in axis X that defines left corner of text
* @param   y - coordination in axis Y that defines top corner of text
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextTabXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_PRINT_DESC tmp_strDes;

  tmp_strDes.x = x;
  tmp_strDes.y = y;
  tmp_strDes.pText = buffText->pText;
  tmp_strDes.pFontType = D4D_GetFont(buffText->fontId);
  tmp_strDes.pTab = pTab;
  tmp_strDes.colorText = colorText;
  tmp_strDes.colorBack = colorBkgd;
  tmp_strDes.properties = buffText->str_properties->font_properties;
  tmp_strDes.textLength = buffText->printLen;
  tmp_strDes.textOffset = buffText->printOff;
  tmp_strDes.maxWidth = 0;
  D4D_LCD_PrintStr(&tmp_strDes);
}

/**************************************************************************/ /*!
* @brief   Function draw text with TAB function in rectangle
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRectTab(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_PRINT_DESC tmp_strDes;
  D4D_TEXT_PROPERTIES tmp_txtProp;
  D4D_COOR tmp_txtWidth = D4D_GetTextBuffWidthTab(buffText, pTab);
  D4D_COOR tmp_txtHeight = D4D_GetFontHeight(buffText->fontId);

  tmp_strDes.pText = buffText->pText;
  tmp_strDes.pFontType = D4D_GetFont(buffText->fontId);
  tmp_strDes.pTab = pTab;
  tmp_strDes.colorText = colorText;
  tmp_strDes.colorBack = colorBkgd;
  tmp_strDes.properties = buffText->str_properties->font_properties;
  tmp_strDes.textLength = buffText->printLen;
  tmp_strDes.textOffset = buffText->printOff;

  if(!psz->cx || !psz->cy)
    return;

  if(tmp_txtHeight > psz->cy)
    return;

  if(tmp_txtWidth >= psz->cx)
    tmp_txtWidth = psz->cx;

  tmp_strDes.maxWidth = tmp_txtWidth;

  tmp_txtProp = buffText->str_properties->text_properties;

  tmp_strDes.x = ppt->x;
  tmp_strDes.y = ppt->y;


  if(tmp_txtHeight != psz->cy)
  {
    // Compute the text position in rectangle in Y axis
    switch(tmp_txtProp & D4D_ALIGN_V_MASK)
    {
      case D4D_ALIGN_V_BOTTOM_MASK:
        tmp_strDes.y += (D4D_COOR)(psz->cy - tmp_txtHeight);
        break;

      case D4D_ALIGN_V_CENTER_MASK:
        tmp_strDes.y += (D4D_COOR)((psz->cy - tmp_txtHeight) / 2);
        break;
    }
  }


  if(tmp_txtWidth != psz->cx)
  {
    // Compute the text position in rectangle in X axis
    switch(tmp_txtProp & D4D_ALIGN_H_MASK)
    {
      case D4D_ALIGN_H_RIGHT_MASK:
        tmp_strDes.x += (D4D_COOR)(psz->cx - tmp_txtWidth);
        break;

      case D4D_ALIGN_H_CENTER_MASK:
        tmp_strDes.x += (D4D_COOR)((psz->cx - tmp_txtWidth) / 2);
        break;
    }
  }


  // Redraw the rest of rectangle where isn't text for normal (non trasparent) texts
  if(!(tmp_strDes.properties & D4D_FNT_PRTY_TRANSPARENT_MASK))
  {
    if(tmp_txtWidth != psz->cx)
    {
      D4D_COOR width = tmp_txtWidth;
      if(width)
        width--;

      if((tmp_txtProp & D4D_ALIGN_H_MASK) != D4D_ALIGN_H_LEFT_MASK)
      {
        // Fill the part of rect in front of the text in full height of rect
          D4D_FillRectXY(ppt->x, ppt->y, tmp_strDes.x, (D4D_COOR)(ppt->y + psz->cy - 1), colorBkgd);
      }

      if((tmp_txtProp & D4D_ALIGN_H_MASK) != D4D_ALIGN_H_RIGHT_MASK)
      {
        // Fill the part of rect behind the text in full height of rect
          D4D_FillRectXY((D4D_COOR)(tmp_strDes.x + width), ppt->y, (D4D_COOR)(ppt->x + psz->cx - 1), (D4D_COOR)(ppt->y + psz->cy - 1), colorBkgd);
      }
    }

    if(tmp_txtHeight != psz->cy)
    {
      D4D_COOR width = tmp_txtWidth;
      if(width)
        width--;

      if((tmp_txtProp & D4D_ALIGN_V_MASK) != D4D_ALIGN_V_TOP_MASK)
      {
        // Fill the part of rect above the text in lenght of text
        D4D_FillRectXY(tmp_strDes.x, ppt->y, (D4D_COOR)(tmp_strDes.x + width), tmp_strDes.y, colorBkgd);
      }

      if((tmp_txtProp & D4D_ALIGN_V_MASK) != D4D_ALIGN_V_BOTTOM_MASK)
      {
        // Fill the part of rect under the text in lenght of text
        D4D_FillRectXY(tmp_strDes.x, (D4D_COOR)(tmp_strDes.y + tmp_txtHeight - 1), (D4D_COOR)(tmp_strDes.x + width), (D4D_COOR)(ppt->y + psz->cy - 1), colorBkgd);
      }
    }
  }

  // Print the string itself
  D4D_LCD_PrintStr(&tmp_strDes);
}

/**************************************************************************/ /*!
* @brief   Function draw text on XY coordination with TAB function in rectangle
* @param   ppt - pointer on point structure of bottom right corner on screen
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The rectangle is defined by draw cursor (top, left) and ppt parameter.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRectTabTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_SIZE tmp_size;

  tmp_size.cx = (D4D_COOR)(ppt->x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(ppt->y - d4d_curXY.y);

  D4D_DrawTextRectTab( &d4d_curXY, &tmp_size, buffText, pTab, colorText, colorBkgd);
}

/**************************************************************************/ /*!
* @brief   Function draw text with TAB function in rectangle
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRectTabXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;

  tmp_point.x = x1;
  tmp_point.y = y1;

  tmp_size.cx = (D4D_COOR)(x2 - x1);
  tmp_size.cy = (D4D_COOR)(y2 - y1);


  D4D_DrawTextRectTab( &tmp_point, &tmp_size, buffText, pTab, colorText, colorBkgd);
}

/**************************************************************************/ /*!
* @brief   Function draw text on XY coordination with TAB function in rectangle
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The rectangle is defined by draw cursor (top, left) and x,y parameter.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRectTabToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_SIZE tmp_size;

  tmp_size.cx = (D4D_COOR)(x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(y - d4d_curXY.y);


  D4D_DrawTextRectTab( &d4d_curXY, &tmp_size, buffText, pTab, colorText, colorBkgd);
}


#ifdef __CWCC__
  #pragma mark -
  #pragma mark Drawing Round Corner Functions
  #pragma mark -
#endif

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen with round corners
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color and could be defined type.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
  if(psz->cx && psz->cy)
    D4D_LCD_RRect((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), ltype, color, radius);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen with round corners
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color and could be defined type.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RRect(x1, y1, x2, y2, ltype, color,radius);

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen with round corners
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color and could be defined type.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RRectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RRect(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, ltype, color, radius);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw rectangle on the screen with round corners
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color and could be defined type.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RRectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RRect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, color, radius);

    d4d_curXY.x = x;
    d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen with round corners
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color, D4D_COOR radius)
{
  if(psz->cx && psz->cy)
    D4D_LCD_RBox((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x + psz->cx - 1), (D4D_COOR)(ppt->y + psz->cy - 1), color, radius);
    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen with round corners
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RBox(x1, y1, x2, y2, color, radius);

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen with round corners
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectTo(D4D_POINT* ppt, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RBox(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, (D4D_COLOR)color, radius);

    d4d_curXY = *ppt;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle on the screen with round corners
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   color - color of rectangle
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle on the screen with round corners in any color and could be defined type.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RBox(d4d_curXY.x, d4d_curXY.y, x, y, color, radius);

    d4d_curXY.x = x;
    d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen with round corners
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle with outline on the screen with round corners in any colors.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RBox(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
  if(psz->cx && psz->cy)
    D4D_RBoxXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x + psz->cx - 1), (D4D_COOR)(ppt->y + psz->cy - 1), ltype, colorLine, colorFill, radius);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen with round corners
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle with outline on the screen with round corners in any colors.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RBoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
    // fill background
    D4D_LCD_RBox(x1, y1, x2, y2, colorFill, radius);

    // draw rectangle around
    D4D_LCD_RRect(x1, y1, x2, y2, ltype, colorLine, radius);

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen with round corners
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle with outline on the screen with round corners in any colors.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RBoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
    D4D_RBoxToXY(ppt->x, ppt->y, ltype, colorLine, colorFill, radius);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with outline on the screen with round corners
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   ltype - type of the line (thin/thick)
* @param   colorLine - color of rectangle outline
* @param   colorFill - color of rectangle fill
* @param   radius - radius of rectangle
* @return  none
* @note    Function draws the rectangle with outline on the screen with round corners in any colors.
          As the start point of the drawed rectangle is used logical draw cursor.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_RBoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
    // fill background
    D4D_LCD_RBox(d4d_curXY.x, d4d_curXY.y, x, y, colorFill, radius);

    // draw rectangle around
    D4D_LCD_RRect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, colorLine, radius);

    d4d_curXY.x = x;
    d4d_curXY.y = y;
}

/**************************************************************************/ /*!
* @brief   Function draw fill quadrant of circle on the screen
* @param   pCenter - pointer to coordination that defines quadrant center
* @param   radius - radius of quadrant
* @param   color - color of filling of quadrant
* @param   quadrant - specify which quadrant will be draw
* @return  none
* @note    Function draws the fill quadrant on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_FillQuadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(pCenter->x, pCenter->y, radius, D4D_TRUE, D4D_LINE_THIN, color, quadrant);
}

/**************************************************************************/ /*!
* @brief   Function draw fill quadrant of circle on the screen
* @param   x - coordination in axis X that defines center point of quadrant
* @param   y - coordination in axis Y that defines center point of quadrant
* @param   radius - radius of quadrant
* @param   color - color of filling of quadrant
* @param   quadrant - specify which quadrant will be draw
* @return  none
* @note    Function draws the fill quadrant on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_FillQuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(x, y, radius, D4D_TRUE, D4D_LINE_THIN, color, quadrant);
}

/**************************************************************************/ /*!
* @brief   Function draw quadrant of circle on the screen
* @param   pCenter - pointer to coordination that defines quadrant center
* @param   radius - radius of quadrant
* @param   ltype - type of the line (thin/thick)
* @param   color - color of line of quadrant
* @param   quadrant - specify which quadrant will be draw
* @return  none
* @note    Function draws the quadrant on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_Quadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(pCenter->x, pCenter->y, radius, D4D_FALSE, ltype, color, quadrant);
}

/**************************************************************************/ /*!
* @brief   Function draw quadrant of circle on the screen
* @param   x - coordination in axis X that defines center point of quadrant
* @param   y - coordination in axis Y that defines center point of quadrant
* @param   radius - radius of quadrant
* @param   ltype - type of the line (thin/thick)
* @param   color - color of line of quadrant
* @param   quadrant - specify which quadrant will be draw
* @return  none
* @note    Function draws the quadrant on the screen in any color.
          The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_QuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(x, y, radius, D4D_FALSE, ltype, color, quadrant);
}


/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with round corners with  changing color (cross color from start to end color) on the screen
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @param   radius - radius of round corners
* @return  none
* @note    Function draws the rectangle with round corners on the screen with crossing color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  if(psz->cx && psz->cy)
    D4D_FillRRectColorScaleXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), startColor, endColor, direction, radius);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with round corners with  changing color (cross color from start to end color) on the screen
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @param   radius - radius of round corners
* @return  none
* @note    Function draws the rectangle with round corners on the screen with crossing color.
          The logical last draw cursor is moved on the position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  D4D_COOR range, i, non_print_len;
  D4D_COLOR color = startColor;

  if(radius)
  {

    d4d_curXY.x = x1;
    d4d_curXY.y = y1;

    switch(direction)
    {
    case D4D_DIR_UP:
      // switch X and Y coodinates and change direction of drawing
      startColor = endColor;
      endColor = color;
    case D4D_DIR_DOWN:
      // just switch X and Y coodinates
      range = (D4D_COOR)(y2 - y1);

      i = 0;
      for( ; y1 <= y2; y1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));


        if(i < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, i); // get len of non printed pixels
        else if((range - i) < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(range - i)); // get len of non printed pixels
        else
          non_print_len = 0;

        i++;
        D4D_LCD_Box((D4D_COOR)(x1 + non_print_len), y1, (D4D_COOR)(x2 - non_print_len), y1, color);
      }
      break;

    case D4D_DIR_LEFT:
      // switch colors
      startColor = endColor;
      endColor = color;
    case D4D_DIR_RIGHT:
      // just keep all fine
      range = (D4D_COOR)(x2 - x1);

      i = 0;
      for( ; x1 <= x2; x1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));


        if(i < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, i); // get len of non printed pixels
        else if((range - i) < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(range - i)); // get len of non printed pixels
        else
          non_print_len = 0;

        i++;
        D4D_LCD_Box(x1, (D4D_COOR)(y1 + non_print_len), x1, (D4D_COOR)(y2 - non_print_len), color);
      }

      break;
    }
  }else
  {
    D4D_FillRectColorScaleXY(x1, y1, x2, y2, startColor, endColor, direction);
  }

}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with round corners with  changing color (cross color from start to end color) on the screen
* @param   ppt - pointer on point structure of bottm right corner on screen
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @param   radius - radius of round corners
* @return  none
* @note    Function draws the rectangle with round corners on the screen with crossing color.
          The logical last draw cursor is keep on the same position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  D4D_FillRRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, startColor, endColor, direction, radius);
}

/**************************************************************************/ /*!
* @brief   Function draw filled rectangle with round corners with  changing color (cross color from start to end color) on the screen
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   startColor - start color of rectangle
* @param   endColor - end color of rectangle
* @param   direction - the direction of color crossing
* @param   radius - radius of round corners
* @return  none
* @note    Function draws the rectangle with round corners on the screen with crossing color.
          The logical last draw cursor is keep on the same position of top left corner of rectangle.
*******************************************************************************/
void D4D_FillRRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  D4D_FillRRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, x, y, startColor, endColor, direction, radius);
}


/**************************************************************************/ /*!
* @brief   Function that counts number of printed pixels in specified line of circle quadrant
* @param   radius - radius of quadrant to count the printed pixels
* @param   line - line of quadrant to count the pixels numbers that should be printed
* @return  count of pixels that should be printed on this line of specified quadrant
* @note   Helping function that could be used with printing the circle arcs.
*******************************************************************************/
D4D_COOR D4D_RndCornerGetNonPrintedPxl(D4D_COOR radius, D4D_COOR line)
{
  D4D_INDEX_DELTA error =  (D4D_INDEX_DELTA)(-1 * (D4D_INDEX_DELTA)radius);
  D4D_COOR x = radius;
  D4D_COOR y = 0;
  D4D_COOR x1_bf,y1_bf;
  D4D_BOOL match = D4D_FALSE;

  line = (D4D_COOR)(radius - line);

  if(line == 0) return radius;

  while (x >= y)
  {
    if ((x != 0) && (y != 0)) //Valid point
    {
    	if(y == line) return (D4D_COOR)(radius - x);

    	if(x == line)
    	{
    		match = D4D_TRUE;
    		x1_bf = (Byte)x;
    		y1_bf = (Byte)y;
    	}

      if(match  && (x != x1_bf))
        return (D4D_COOR)(radius - y1_bf);
    }

    error += y;
    ++y;
    error += y;

    if (error >= 0)
    {
      if((radius - x) >= line)
        return (D4D_COOR)(radius - y);

      x--;
      error -= x;
      error -= x;
    }
  }

  return (D4D_COOR)(radius - x);
}

/**************************************************************************/ /*!
* @brief   Function draw text with TAB function in rectangle with round corners
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @param   radius - radius of round corners
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRRectTab(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius)
{
  D4D_PRINT_DESC tmp_strDes;
  //D4D_SIZE tmp_size = *psz;
  D4D_TEXT_PROPERTIES tmp_txtProp;
  D4D_COOR tmp_txtWidth;
  D4D_COOR tmp_txtHeight;
  //D4D_POINT tmp_Pos;

  if(!radius)
  {
    D4D_DrawTextRectTab(ppt, psz, buffText, pTab, colorText, colorBkgd);
    return;
  }

  tmp_txtWidth = D4D_GetTextBuffWidthTab(buffText, pTab);
  tmp_txtHeight = D4D_GetFontHeight(buffText->fontId);

  tmp_strDes.pText = buffText->pText;
  tmp_strDes.pFontType = D4D_GetFont(buffText->fontId);
  tmp_strDes.pTab = pTab;
  tmp_strDes.colorText = colorText;
  tmp_strDes.colorBack = colorBkgd;
  tmp_strDes.properties = buffText->str_properties->font_properties;
  tmp_strDes.textLength = buffText->printLen;
  tmp_strDes.textOffset = buffText->printOff;

  if(tmp_txtWidth + radius >= psz->cx)
  {
    if(psz->cx - radius > 0)
      tmp_txtWidth = (D4D_COOR)(psz->cx - radius);
    else
      tmp_txtWidth = 0;
  }

  tmp_strDes.maxWidth = tmp_txtWidth;

  tmp_txtProp = buffText->str_properties->text_properties;

  tmp_strDes.x = ppt->x;
  tmp_strDes.y = ppt->y;

  // Compute the text position in rectangle in Y axis
  switch(tmp_txtProp & D4D_ALIGN_V_MASK)
  {
    case D4D_ALIGN_V_BOTTOM_MASK:
      tmp_strDes.y += (D4D_COOR)(psz->cy - tmp_txtHeight - radius / 2);
      break;

    case D4D_ALIGN_V_CENTER_MASK:
      tmp_strDes.y += (D4D_COOR)((psz->cy - tmp_txtHeight) / 2);
      break;

    default:
    case D4D_ALIGN_V_TOP_MASK:
      tmp_strDes.y += (D4D_COOR)(radius / 2);
      break;
  }

  // Compute the text position in rectangle in X axis
  switch(tmp_txtProp & D4D_ALIGN_H_MASK)
  {
    case D4D_ALIGN_H_RIGHT_MASK:
      tmp_strDes.x += (D4D_COOR)(psz->cx - tmp_txtWidth - radius / 2);
      break;

    case D4D_ALIGN_H_CENTER_MASK:
      tmp_strDes.x += (D4D_COOR) ((psz->cx - tmp_txtWidth) / 2);
      break;

    default:
    case D4D_ALIGN_H_LEFT_MASK:
      tmp_strDes.x += (D4D_COOR)(radius / 2);
      break;
  }

  // Redraw the rest of rectangle where isn't text for normal (non trasparent) texts
  if(!(tmp_strDes.properties & D4D_FNT_PRTY_TRANSPARENT_MASK))
  {
    D4D_FillRRect(ppt, psz, colorBkgd, radius);
#if 0


    tmp_Pos = *ppt;

    tmp_Pos.x += radius;
    tmp_Pos.y += radius;
    D4D_FillQuadrant(&tmp_Pos, radius, colorBkgd, D4D_QUAD_4);
    tmp_Pos.x += psz->cx - radius * 2 - 1;
    D4D_FillQuadrant(&tmp_Pos, radius, colorBkgd, D4D_QUAD_1);
    tmp_Pos.y += psz->cy - radius * 2 - 1;
    D4D_FillQuadrant(&tmp_Pos, radius, colorBkgd, D4D_QUAD_2);
    tmp_Pos.x -= (psz->cx - radius * 2 - 1);
    D4D_FillQuadrant(&tmp_Pos, radius, colorBkgd, D4D_QUAD_3);


    if((tmp_txtProp & D4D_ALIGN_H_MASK) != D4D_ALIGN_H_LEFT_MASK)
    {
      // Fill the part of rect in front of the text in full height of rect
        D4D_FillRectXY(ppt->x, (D4D_COOR)(ppt->y + radius), tmp_strDes.x, (D4D_COOR)(ppt->y + psz->cy - 1 - radius), colorBkgd);
    }

    if((tmp_txtProp & D4D_ALIGN_H_MASK) != D4D_ALIGN_H_RIGHT_MASK)
    {
      // Fill the part of rect behind the text in full height of rect        if()
        D4D_FillRectXY((D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), (D4D_COOR)(ppt->y), (D4D_COOR)(ppt->x + psz->cx - 1 - radius), (D4D_COOR)(ppt->y + psz->cy - 1), colorBkgd);
    }

    if((tmp_txtProp & D4D_ALIGN_V_MASK) != D4D_ALIGN_V_TOP_MASK)
    {
      // Fill the part of rect above the text in lenght of text
      D4D_FillRectXY((D4D_COOR)(ppt->x + radius), ppt->y, (D4D_COOR)(ppt->x + psz->cx - 1 - radius), (D4D_COOR)(tmp_strDes.y + 1), colorBkgd);
    }

    if((tmp_txtProp & D4D_ALIGN_V_MASK) != D4D_ALIGN_V_BOTTOM_MASK)
    {
      // Fill the part of rect under the text in lenght of text
      D4D_FillRectXY((D4D_COOR)(ppt->x + radius), (D4D_COOR)(tmp_strDes.y + tmp_txtHeight - 1), (D4D_COOR)(ppt->x + psz->cx - 1 - radius), (D4D_COOR)(ppt->y + psz->cy - 1), colorBkgd);
    }
#endif

  }

  // Print the string itself
  D4D_LCD_PrintStr(&tmp_strDes);
}

/**************************************************************************/ /*!
* @brief   Function draw text on XY coordination with TAB function in rectangle with round corners
* @param   ppt - pointer on point structure of bottom right corner on screen
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @param   radius - round corner radius
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The rectangle is defined by draw cursor (top, left) and ppt parameter.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRRectTabTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius)
{
  D4D_SIZE tmp_size;

  tmp_size.cx = (D4D_COOR)(ppt->x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(ppt->y - d4d_curXY.y);

  D4D_DrawTextRRectTab( &d4d_curXY, &tmp_size, buffText, pTab, colorText, colorBkgd, radius);
}

/**************************************************************************/ /*!
* @brief   Function draw text with TAB function in rectangle with round corners
* @param   x1 - coordination in axis X that defines left corner of retangle
* @param   y1 - coordination in axis Y that defines top corner of retangle
* @param   x2 - coordination in axis X that defines right corner of retangle
* @param   y2 - coordination in axis Y that defines bottom corner of retangle
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @param   radius - round corner radius
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRRectTabXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius)
{
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;

  tmp_point.x = x1;
  tmp_point.y = y1;

  tmp_size.cx = (D4D_COOR)(x2 - x1);
  tmp_size.cy = (D4D_COOR)(y2 - y1);


  D4D_DrawTextRRectTab( &tmp_point, &tmp_size, buffText, pTab, colorText, colorBkgd, radius);
}

/**************************************************************************/ /*!
* @brief   Function draw text on XY coordination with TAB function in rectangle with round corners
* @param   x - coordination in axis X that defines right corner of retangle
* @param   y - coordination in axis Y that defines bottom corner of retangle
* @param   buffText - pointer to text buffer
* @param   pTab - pointer to tab table
* @param   colorText - color of text
* @param   colorBkgd - color of background
* @param   radius - round corner radius
* @return  none
* @note    Function draws the text on the screen in any color of text and background.
           The text is drawed in rectangle so it also use the align options in text buffer structure.
           The rectangle is defined by draw cursor (top, left) and x,y parameter.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_DrawTextRRectTabToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius)
{
  D4D_SIZE tmp_size;

  tmp_size.cx = (D4D_COOR)(x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(y - d4d_curXY.y);


  D4D_DrawTextRRectTab( &d4d_curXY, &tmp_size, buffText, pTab, colorText, colorBkgd, radius);
}
#endif

/**************************************************************************/ /*!
* @brief   Function draw standard object bevel specified by parameters
* @param   ppt - pointer on point structure of top left corner on screen
* @param   psz - pointer on the size structure that defines size of the bevel
* @param   color - base color of bevel (the eal used color are computed from this color)
* @param   type - type of bevel
* @param   radius - round corner radius
* @return  none
* @note    Function draws the standard eGUI bevel specified by parameters.
           The used colors are computed from D4D_BEVEL_FORE_INTENSITY &
           D4D_BEVEL_BCKG_INTENSITY constants.
           The logical last draw cursor is not moved.
*******************************************************************************/
void D4D_Bevel(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color, D4D_BEVEL type, D4D_COOR radius)
{
  D4D_COLOR fore, bck;
  D4D_POINT shPos;
  D4D_SIZE shSize;

  #if (D4D_ROUND_CORNER_ENABLE == D4D_FALSE) //if no Round corners
    D4D_UNUSED(radius)
  #endif

  switch(type)
  {
    case D4D_BEVEL_NONE:
      // Draw nothing :-)
      return;
    case D4D_BEVEL_RAISED:
      fore = D4D_ChangeColorIntesity(color, D4D_BEVEL_FORE_INTENSITY);
      bck = D4D_ChangeColorIntesity(color, D4D_BEVEL_BCKG_INTENSITY);
      break;
    case D4D_BEVEL_DENTED:
      fore = D4D_ChangeColorIntesity(color, D4D_BEVEL_BCKG_INTENSITY);
      bck = D4D_ChangeColorIntesity(color, D4D_BEVEL_FORE_INTENSITY);
      break;
    case D4D_BEVEL_RESERVED:
      // Draw nothing :-)
      return;
    default:
      return;
  }

  if(psz->cx < (D4D_BEVEL_WIDTH * 2))
    return;

  if(psz->cy < (D4D_BEVEL_WIDTH * 2))
    return;

  // Draw Bevel
  shPos = *ppt;
  shSize.cx = psz->cx;
  shSize.cy = D4D_BEVEL_WIDTH;

#if (D4D_ROUND_CORNER_ENABLE != D4D_FALSE) //if Round corners
  shPos.x += radius;
  shPos.y += radius;
  D4D_Quadrant(&shPos, radius, D4D_LINE_THICK, fore, D4D_QUAD_4);
  shPos.x += psz->cx - radius * 2 - 2;
  D4D_Quadrant(&shPos, radius, D4D_LINE_THICK, fore, D4D_QUAD_1);
  shPos.y += psz->cy - radius * 2 - 2;
  D4D_Quadrant(&shPos, radius, D4D_LINE_THICK, bck, D4D_QUAD_2);
  shPos.x -= (psz->cx - radius * 2 - 2);
  D4D_Quadrant(&shPos, radius, D4D_LINE_THICK, bck, D4D_QUAD_3);

  shPos = *ppt;
  shPos.x += radius;
  shSize.cx -= radius * 2;
#endif

  D4D_FillRect(&shPos, &shSize, fore);

  shPos.y += (psz->cy - D4D_BEVEL_WIDTH);
  D4D_FillRect(&shPos, &shSize, bck);

  shPos = *ppt;

  // change pos
  shSize.cx = D4D_BEVEL_WIDTH;
  shSize.cy = psz->cy;

#if (D4D_ROUND_CORNER_ENABLE != D4D_FALSE) //if Round corners
  shPos.y += radius;
  shSize.cy -= radius * 2;
#endif
  D4D_FillRect(&shPos, &shSize, fore);

  shPos.x += (psz->cx - D4D_BEVEL_WIDTH);

  D4D_FillRect(&shPos, &shSize, bck);

}


/**************************************************************************/ /*!
* @brief   Function draw standard object frame based on the object settings and current state
* @param   pObject - pointer on object which Frame should be draw
* @param   clrT - fore color of frame
* @param   clrB - background color of frame
* @return  none
* @note    Function draws the Frame of object depending on object settings and current state
*******************************************************************************/
void D4D_DrawFrame(D4D_OBJECT* pObject, D4D_COLOR clrT, D4D_COLOR clrB)
{
  D4D_POINT position = D4D_GetClientToScreenPoint(pObject, &pObject->position);

  // Draw the frame
  if(pObject->pData->flags & D4D_OBJECT_F_BEVEL_MASK)
  {
    D4D_COLOR clrBevel;
    // In case that focus rect is enabled and also BEVEL, just change the color of bevel
    if(pObject->pData->flags & D4D_OBJECT_F_FOCUSRECT)
      clrBevel = clrT;
    else
    {
      if(D4D_GetFocusedObject(pObject->pData->pScreen) == pObject)
        clrBevel = clrT;
      else
        clrBevel = clrB;
    }
    D4D_Bevel(&position, &pObject->size, clrBevel, D4D_OBJECT_FLAGS2BEVEL(pObject), pObject->radius);
  }else
  {
    if(pObject->pData->flags & D4D_OBJECT_F_FOCUSRECT)
      D4D_RRect(&position, &pObject->size, D4D_LINE_THIN, clrT, pObject->radius);
  }
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_basic_draw_func                                               */
/******************************************************************************/

