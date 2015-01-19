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
* @file      d4d_basic_draw.h
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.2.0
*
* @date      Oct-2-2013
*
* @brief     D4D Driver core and base functions header file
*
*******************************************************************************/

#ifndef __D4D_BASIC_DRAW_H
#define __D4D_BASIC_DRAW_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* D4D BASIC DRAW setting  constants
*
*//*! @addtogroup doxd4d_basic_draw_const
* @{
*******************************************************************************/
/*! @brief D4D_BEVEL_WIDTH constant declaration - can't be changed!           */
#define D4D_BEVEL_WIDTH 2

/*! @brief D4D_BEVEL_FORE_INTENSITY constant default declaration. Sets the
percentual change of color for bevel frame for fore color */
#ifndef D4D_BEVEL_FORE_INTENSITY
  #define D4D_BEVEL_FORE_INTENSITY 33
#endif

/*! @brief D4D_BEVEL_BCKG_INTENSITY constant default declaration. Sets the
percentual change of color for bevel frame for background color */
#ifndef D4D_BEVEL_BCKG_INTENSITY
  #define D4D_BEVEL_BCKG_INTENSITY -33
#endif

/*! @} End of doxd4d_basic_draw_const                                         */

/******************************************************************************
* D4D BASIC DRAW types
*
*//*! @addtogroup doxd4d_basic_draw_type
* @{
*******************************************************************************/

/*! @} End of doxd4d_basic_draw_type                                          */

/******************************************************************************
* D4D BASIC DRAW macros
*
*//*! @addtogroup doxd4d_basic_draw_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function that draw text on the screen the coordination is sets directly XY
* @param   x - coordination in axis X that defines left corner of draw text
* @param   y - coordination in axis Y that defines top corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the simple text on the screen with defined XY coordination
*******************************************************************************/
#define D4D_DrawTextXY(x, y, buffText, colorText, colorBkgd) D4D_DrawTextTabXY(x, y, buffText, NULL, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw simple text on the screen
* @param   ppt - pointer on point structure that defines top left corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the simple text on the screen
*******************************************************************************/
#define D4D_DrawText(ppt, buffText, colorText, colorBkgd) D4D_DrawTextTab(ppt, buffText, NULL, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw text on the screen and respects Tabulator functionality
* @param   ppt - pointer on point structure that defines top left corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   pTab - pointer on the tabulator description table
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the text on the screen with tabulator table
*******************************************************************************/
#define D4D_DrawTextTab(ppt, buffText, pTab, colorText, colorBkgd) D4D_DrawTextTabXY((ppt)->x, (ppt)->y, buffText, pTab, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle on the screen
* @param   ppt - pointer on point structure that defines top left corner of draw text
* @param   psz - pointer on the size structure that defines size of the text rectangle
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRect(ppt, psz, buffText, colorText, colorBkgd) D4D_DrawTextRectTab(ppt, psz, buffText, NULL, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle on the screen from previous draw
            point to to point specify by ppt structure
* @param   ppt - pointer on point structure that specify the second point of rectangle.
            The first one is takes last drawing point.
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRectTo(ppt, buffText, colorText, colorBkgd) D4D_DrawTextRectTabTo(ppt, buffText, NULL, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle on the screen
* @param   x1 - coordination in axis X that defines left corner of draw text
* @param   y1 - coordination in axis Y that defines top corner of draw text
* @param   x2 - coordination in axis X that defines right corner of draw text
* @param   y2 - coordination in axis Y that defines bottom corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRectXY(x1, y1, x2, y2, buffText, colorText, colorBkgd) D4D_DrawTextRectTabXY(x1, y1, x2, y2, buffText, NULL, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle on the screen from previous draw
            point to to point specify by ppt structure
* @param   x - coordination in axis X that defines right corner of draw text
* @param   y - coordination in axis Y that defines bottom corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRectToXY(x, y, buffText, colorText, colorBkgd) D4D_DrawTextRectTabToXY(x, y, buffText, NULL, colorText, colorBkgd)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle on the screen with corner radius
* @param   ppt - pointer on point structure that defines top left corner of draw text
* @param   psz - pointer on the size structure that defines size of the text rectangle
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @param   radius - radius or text rectangle
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRRect(ppt, psz, buffText, colorText, colorBkgd, radius) D4D_DrawTextRRectTab(ppt, psz, buffText, NULL, colorText, colorBkgd, radius)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle with corner radius on the screen from previous draw
            point to to point specify by ppt structure
* @param   ppt - pointer on point structure that specify the second point of rectangle.
            The first one is takes last drawing point.
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @param   radius - radius or text rectangle
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRRectTo(ppt, buffText, colorText, colorBkgd, radius) D4D_DrawTextRRectTabTo(ppt, buffText, NULL, colorText, colorBkgd, radius)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle on the screen with corner radius
* @param   x1 - coordination in axis X that defines left corner of draw text
* @param   y1 - coordination in axis Y that defines top corner of draw text
* @param   x2 - coordination in axis X that defines right corner of draw text
* @param   y2 - coordination in axis Y that defines bottom corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @param   radius - radius or text rectangle
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRRectXY(x1, y1, x2, y2, buffText, colorText, colorBkgd, radius) D4D_DrawTextRRectTabXY(x1, y1, x2, y2, buffText, NULL, colorText, colorBkgd, radius)

/**************************************************************************/ /*!
* @brief   Function that draw text into defined rectangle with corner radius on the screen from previous draw
            point to to point specify by ppt structure
* @param   x - coordination in axis X that defines right corner of draw text
* @param   y - coordination in axis Y that defines bottom corner of draw text
* @param   buffText - pointer on the text buffer structure that contains text itself and its parameter
* @param   colorText - color of text
* @param   colorBkgd - color of text background
* @param   radius - radius or text rectangle
* @return  none
* @note    Function that draw the text on the screeninto logic rectangle. In case
            of long texts it make text shorter and add three points "...". It also support
            text align and fill up complete rectangle in case that this is neccessary.
*******************************************************************************/
#define D4D_DrawTextRRectToXY(x, y, buffText, colorText, colorBkgd, radius) D4D_DrawTextRRectTabToXY(x, y, buffText, NULL, colorText, colorBkgd, radius)

/*! @} End of doxd4d_basic_draw_macro                                         */




/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

void D4D_PutPixel(D4D_POINT* ppt, D4D_LINETYPE type, D4D_COLOR color);
void D4D_PutPixelTo(D4D_LINETYPE type, D4D_COLOR color);
void D4D_PutPixelXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE type, D4D_COLOR color);
void D4D_MoveTo(D4D_POINT* ppt);
void D4D_MoveToXY(D4D_COOR x, D4D_COOR y);
void D4D_LineTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_LineToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_FillRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color);
void D4D_FillRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color);
void D4D_FillRectTo(D4D_POINT* ppt, D4D_COLOR color);
void D4D_FillRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color);
void D4D_FillRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction);
void D4D_FillRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction);
void D4D_FillRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction);
void D4D_FillRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction);
void D4D_Rect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_RectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_RectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_RectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_Box(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_BoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_BoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_BoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_FillCircle(D4D_POINT* pCenter, D4D_COOR r, D4D_COLOR color);
void D4D_FillCircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_COLOR color);
void D4D_Circle(D4D_POINT* pCenter, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_CircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_Bevel(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color, D4D_BEVEL type, D4D_COOR radius);

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
  void D4D_RRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_RRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_RRectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_RRectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRectTo(D4D_POINT* ppt, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color, D4D_COOR radius);
  void D4D_RBox(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_RBoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_RBoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_RBoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_FillQuadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_FillQuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_Quadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_QuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_FillRRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // experimental function
  void D4D_FillRRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // same
  void D4D_FillRRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // same
  void D4D_FillRRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // same
  void D4D_DrawTextRRectTab(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius);
  void D4D_DrawTextRRectTabTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius);
  void D4D_DrawTextRRectTabXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius);
  void D4D_DrawTextRRectTabToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd, D4D_COOR radius);
#else
  #define D4D_RRect(ppt, psz, ltype, color, radius)                                             D4D_Rect(ppt, psz, ltype, color)
  #define D4D_RRectXY(x1, y1, x2, y2, ltype, color, radius)                                     D4D_RectXY(x1, y1, x2, y2, ltype, color)
  #define D4D_RRectTo(ppt, ltype, color, radius)                                                D4D_RectTo(ppt, ltype, color)
  #define D4D_RRectToXY(x, y, ltype, color, radius)                                             D4D_RectToXY(x, y, ltype, color)
  #define D4D_FillRRect(ppt, psz, color, radius)                                                D4D_FillRect(ppt, psz, color)
  #define D4D_FillRRectXY(x1, y1, x2, y2, color, radius)                                        D4D_FillRectXY(x1, y1, x2, y2, color)
  #define D4D_FillRRectTo(ppt, color, radius)                                                   D4D_FillRectTo(ppt, color)
  #define D4D_FillRRectToXY(x, y, color, radius)                                                D4D_FillRectToXY(x, y, color)
  #define D4D_RBox(ppt, psz, ltype, colorLine, colorFill, radius)                               D4D_Box(ppt, psz, ltype, colorLine, colorFill)
  #define D4D_RBoxXY(x1, y1, x2, y2, ltype, colorLine, colorFill, radius)                       D4D_BoxXY(x1, y1, x2, y2, ltype, colorLine, colorFill)
  #define D4D_RBoxTo(ppt, ltype, colorLine, colorFill, radius)                                  D4D_BoxTo(ppt, ltype, colorLine, colorFill)
  #define D4D_RBoxToXY(x, y, ltype, colorLine, colorFill, radius)                               D4D_BoxToXY(x, y, ltype, colorLine, colorFill)
  #define D4D_FillQuadrant(pCenter, radius, color, quadrant)                                    ;
  #define D4D_FillQuadrantXY(x, y, radius, color, quadrant)                                     ;
  #define D4D_Quadrant(pCenter, radius, ltype, color, quadrant)                                 ;
  #define D4D_QuadrantXY(x, y, radius, ltype, color, quadrant)                                  ;
  #define D4D_FillRRectColorScale(ppt, psz, startColor, endColor, direction, radius)            D4D_FillRectColorScale(ppt, psz, startColor, endColor, direction)
  #define D4D_FillRRectColorScaleXY(x1, y1, x2, y2, startColor, endColor, direction, radius)    D4D_FillRectColorScaleXY(x1, y1, x2, y2, startColor, endColor, direction)
  #define D4D_FillRRectColorScaleTo(ppt, startColor, endColor, direction, radius)               D4D_FillRectColorScaleTo(ppt, startColor, endColor, direction)
  #define D4D_FillRRectColorScaleToXY(x, y, startColor, endColor, direction, radius)            D4D_FillRectColorScaleToXY(x, y, startColor, endColor, direction)
  #define D4D_DrawTextRRectTab(ppt, psz, buffText, pTab, colorText, colorBkgd, radius)          D4D_DrawTextRectTab(ppt, psz, buffText, pTab, colorText, colorBkgd)
  #define D4D_DrawTextRRectTabTo(ppt, buffText, pTab, colorText, colorBkgd, radius)             D4D_DrawTextRectTabTo(ppt, buffText, pTab, colorText, colorBkgd)
  #define D4D_DrawTextRRectTabXY(x1, y1, x2, y2, buffText, pTab, colorText, colorBkgd, radius)  D4D_DrawTextRectTabXY(x1, y1, x2, y2, buffText, pTab, colorText, colorBkgd)
  #define D4D_DrawTextRRectTabToXY(x, y, buffText, pTab, colorText, colorBkgd, radius)          D4D_DrawTextRectTabToXY(x, y, buffText, pTab, colorText, colorBkgd)

#endif

void D4D_DrawTextTabXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);

void D4D_DrawTextRectTab(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextRectTabTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextRectTabXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextRectTabToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);

D4D_COOR D4D_RndCornerGetNonPrintedPxl(D4D_COOR radius, D4D_COOR line);
D4D_COLOR D4D_ChangeColorIntesity(D4D_COLOR color, sByte intensity);
#endif /* __D4D_BASIC_DRAW_H */










