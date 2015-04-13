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
* @file      d4d.h
*
* @author   Michal hanak, Petr Gargulak
* 
* @version   0.0.91.0
* 
* @date      Oct-2-2013
* 
* @brief     D4D Driver main header file
*
*******************************************************************************/

#ifndef __D4D_H
#define __D4D_H

/******************************************************************************
* Includes
******************************************************************************/

/**************************************************************//*!
*
* Types used by driver (standard HCxx: Byte, Word, LWord, ...)
*
******************************************************************/


/**************************************************************//*!
*
* Implementation header files
*
******************************************************************/
#include "d4d_user_cfg.h"
#include "common_files/d4d_types.h"
#include "common_files/d4d_base.h"
#include "common_files/d4d_string.h"
#include "common_files/d4d_basic_draw.h"
#include "common_files/d4d_object.h"
#include "common_files/d4d_low.h"
#include "common_files/d4d_bmp.h"
#include "common_files/d4d_extsrc.h" 
#include "common_files/d4d_font.h"
#include "common_files/d4d_math.h"
#include "common_files/d4d_scheme.h"
#include "common_files/d4d_screen.h"
#include "common_files/d4d_mouse.h"
#include "graphic_objects/d4d_picture.h"
#include "graphic_objects/d4d_button.h"
#include "graphic_objects/d4d_check_box.h"
#include "graphic_objects/d4d_gauge.h"
#include "graphic_objects/d4d_slider.h"
#include "graphic_objects/d4d_label.h"
#include "graphic_objects/d4d_icon.h"
#include "graphic_objects/d4d_menu.h"
#include "graphic_objects/d4d_graph.h"
#include "graphic_objects/d4d_scroll_bar.h"
#include "graphic_objects/d4d_console.h"
#include "graphic_objects/d4d_text_box.h"
#include "graphic_objects/d4d_progress_bar.h"
#include "graphic_objects/d4d_radio_button.h"
#include "graphic_objects/d4d_group_box.h"
#include "graphic_objects/d4d_list_box.h"
#include "graphic_objects/d4d_edit_box.h"
#include "graphic_objects/d4d_combo_box.h"


/******************************************************************************
* Constants
******************************************************************************/

/**************************************************************//*!
*
* Basic driver configuration
*
******************************************************************/ 


/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/**************************************************************//*!
*
* Main driver API Functions
*
******************************************************************/

/* General driver API */
D4D_BOOL D4D_Init(D4D_SCREEN* pInitScreen);
void D4D_Poll(void);
void D4D_SetOrientation(D4D_ORIENTATION orient);
void D4D_KeysChanged(D4D_KEYS keys);
void D4D_NewKeyEvent(D4D_KEY_SCANCODE scanCode);
void D4D_TimeTickPut(void);
void D4D_FlushOutput(void);

#ifdef D4D_LLD_TCH
  D4D_POINT D4D_GetTouchScreenCoordinates(D4D_OBJECT* pObject);
  void D4D_CheckTouchScreen(void);
  void D4D_PutTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y);
  void D4D_PutRawTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y);
  void D4D_ClearTouchScreenEvents(void);
  void D4D_CalibrateTouchScreen(void);
  D4D_TOUCHSCREEN_CALIB D4D_GetTouchScreenCalibration(void);
  void D4D_SetTouchScreenCalibration(D4D_TOUCHSCREEN_CALIB newCalib);
#else
  #define D4D_GetTouchScreenCoordinates(pObject) ((D4D_POINT)d4d_point_zero)
  #define D4D_CheckTouchScreen()
  #define D4D_PutTouchScreen(touched, x, y)
  #define D4D_PutRawTouchScreen(touched, x, y)
  #define D4D_ClearTouchScreenEvents()
  #define D4D_CalibrateTouchScreen()
  #define D4D_SetTouchScreenCalibration(newCalib)
  #define D4D_GetTouchScreenCalibration() ((D4D_TOUCHSCREEN_CALIB){ 0, 0, 0, 0, 0 })
#endif
/* screen API */


/* object API */
D4D_OBJECT_PTR D4D_GetFocusedObject(D4D_SCREEN* pScreen);
void D4D_FocusSet(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject);
void D4D_FocusNextObject(D4D_SCREEN* pScreen, D4D_BOOL bInitialSearch);
void D4D_FocusPrevObject(D4D_SCREEN* pScreen);

/* Color Schemes */
D4D_CLR_SCHEME* D4D_ObjectGetScheme(D4D_OBJECT * pObj);
D4D_CLR_SCHEME* D4D_ScreenGetScheme(D4D_SCREEN * pScreen);
D4D_CLR_SCHEME* D4D_GetDefaultScheme(void);
void D4D_SetDefaultScheme(D4D_CLR_SCHEME* pScheme);
D4D_COLOR D4D_ObjectGetForeColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw);
D4D_COLOR D4D_ObjectGetBckgColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw);
D4D_COLOR D4D_ObjectGetForeFillColor(D4D_OBJECT * pObj);
D4D_COLOR D4D_ObjectGetBckgFillColor(D4D_OBJECT * pObj);
                             
/* general helpers */
void D4D_ClearKeysBuffer(void);

void D4D_ClearScreen(D4D_COLOR color);

D4D_POINT D4D_SwapCoor(D4D_POINT point);



#define D4D_DrawBmp( ppt, pBmp, greyScale) D4D_DrawRBmpXY((ppt)->x, (ppt)->y, pBmp, greyScale, 0)
#define D4D_DrawBmpXY(x, y, pBmp, greyScale) D4D_DrawRBmpXY(x, y, pBmp, greyScale, 0)

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
  void D4D_DrawRBmpRect(D4D_POINT* ppt, D4D_SIZE* psz, const D4D_BMP* pBmp, D4D_BMP_PROPERTIES bmpProp, D4D_COLOR colorBkgd, D4D_BOOL greyScale, D4D_COOR radius);
  
#else
  #define D4D_DrawRBmpRect(ppt,  psz, pBmp, bmpProp, colorBkgd, greyScale, radius)              D4D_DrawBmpRect(ppt,  psz, pBmp, bmpProp, colorBkgd, greyScale)

#endif

#define D4D_DrawRBmp( ppt, pBmp, greyScale, radius) D4D_DrawRBmpXY((ppt)->x, (ppt)->y, pBmp, greyScale, radius)
void D4D_DrawRBmpXY(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius);

void D4D_DrawBmpRect(D4D_POINT* ppt, D4D_SIZE* psz, const D4D_BMP* pBmp, D4D_BMP_PROPERTIES bmpProp, D4D_COLOR colorBkgd, D4D_BOOL greyScale);


D4D_SIZE D4D_GetBmpSize(const D4D_BMP* pBmp);
D4D_COOR D4D_GetBmpWidth(const D4D_BMP* pBmp);
D4D_COOR D4D_GetBmpHeight(const D4D_BMP* pBmp);
int D4D_GetBmpHeader(D4D_BMP* pBmp, void* pBuff, int max_size);

D4D_BOOL D4D_SetText(D4D_OBJECT_PTR pObject, const D4D_TCHAR* pText);
void D4D_SetTextProperties(D4D_OBJECT_PTR pObject, D4D_TEXT_PROPERTIES property);
void D4D_SetFontProperties(D4D_OBJECT_PTR pObject, D4D_FONT_PROPERTIES property);
D4D_INDEX D4D_GetTextLength(D4D_TCHAR* pText);
D4D_COOR D4D_GetTextWidth(D4D_FONT ix, D4D_TCHAR* pText);

D4D_COOR D4D_GetTextBuffWidthTab(D4D_STRING* text_buffer, D4D_TAB* pTab);
D4D_INDEX D4D_GetTextMaxLength(D4D_STRING* pString, D4D_COOR maxWidth);

Byte D4D_SprintDecU8(Byte val, D4D_TCHAR *pText, D4D_TCHAR fill);
Byte D4D_SprintDecS8(sByte val, D4D_TCHAR *pText, D4D_TCHAR fill);
Byte D4D_SprintDecU16(Word val, D4D_TCHAR *pText, D4D_TCHAR fill);
Byte D4D_SprintDecS16(sWord val, D4D_TCHAR *pText, D4D_TCHAR fill);
Byte D4D_SprintDecU32(LWord val, D4D_TCHAR *pText, D4D_TCHAR fill);
Byte D4D_SprintDecS32(sLWord val, D4D_TCHAR *pText, D4D_TCHAR fill);

D4D_COLOR D4D_GetCrossColor(D4D_COLOR startColor, D4D_COLOR endColor, Byte value);


D4D_COLOR D4D_GetGreyScale(D4D_COLOR color);
D4D_COOR D4D_GetLongerSide(D4D_SIZE* pSz);


#endif /* __D4D_H */









