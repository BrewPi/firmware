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
* @file      d4d_low.h
*
* @author     Petr Gargulak
*
* @version   0.0.19.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver low level graphic function header file
*
*******************************************************************************/

#ifndef __D4D_LOW_H
#define __D4D_LOW_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global D4Drv low level functions
******************************************************************************/

D4D_BOOL D4D_LCD_Init(void); // Hardware init of LCD
D4D_COOR D4D_DisplayHeight(void);
D4D_COOR D4D_DisplayWidth(void);
D4D_ORIENTATION D4D_LCD_GetOrientation(void);
void D4D_LCD_SetOrientation(D4D_ORIENTATION new_orientation);
void D4D_LCD_SwapCoor(D4D_COOR* c1, D4D_COOR* c2);

void D4D_LCD_PutPixel(D4D_COOR x1, D4D_COOR y1, D4D_LINETYPE line_type, D4D_COLOR color);
void D4D_LCD_Line(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,D4D_LINETYPE line_type ,D4D_COLOR color);
void D4D_LCD_Box(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color);
void D4D_LCD_Rect(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color);
void D4D_LCD_Circle(D4D_COOR x1, D4D_COOR y1, D4D_COOR r, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  void D4D_LCD_RBox(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius);
  void D4D_LCD_RRect(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color, D4D_COOR radius);
  void D4D_LCD_QCircle (D4D_COOR cx, D4D_COOR cy, D4D_COOR radius, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant);
#endif



/******************************************************************************
* Global D4Drv Touch Screen low level functions
******************************************************************************/


Byte D4D_TCH_GetCalibrationStatus(void);
D4D_TOUCHSCREEN_CALIB D4D_TCH_GetCalibration(void);
void D4D_TCH_SetCalibration(D4D_TOUCHSCREEN_CALIB newCalib);
void D4D_TCH_GetCalibratedPosition(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);
Byte D4D_TCH_GetRawPosition(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);
void D4D_TCH_Calibrate(D4D_COLOR fore, D4D_COLOR bckg);
void D4D_TCH_DrawCalibrationPoint (D4D_COOR X, D4D_COOR Y, D4D_COLOR Colour);

#endif /* __D4D_LOW_H */










