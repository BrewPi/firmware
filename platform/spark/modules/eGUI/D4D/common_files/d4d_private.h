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
* @file      d4d_private.h
*
* @author     Petr Gargulak
*
* @version   0.0.30.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver private header file
*
*******************************************************************************/


#ifndef __D4D_PRIVATE_H
#define __D4D_PRIVATE_H

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

#ifdef __EGUI_DOXYGEN__
  #define D4D_MK_STR(x) d4dlcd_template_ID
#else
  #define D4D_MK_STR1(x) x##_ID
  #define D4D_MK_STR(x) D4D_MK_STR1(x)
#endif

#define D4D_PASTE( a, b)                D4D_PASTE_NOEXPAND( a, b)
#define D4D_PASTE_NOEXPAND( a, b)       a##b

/******************************************************************************
* Global variables
******************************************************************************/

// screen activation hisotry array & index
extern D4D_SCREEN* d4d_screenHistory[D4D_SCREEN_HISTORY];
extern Byte d4d_screenHistoryIndex;

// if not NULL, keys are captured by this object
extern D4D_OBJECT* d4d_pKeysCapturer;

// one and only message object being used to route information to objects
extern D4D_MESSAGE d4d_msg;

// The D4D system flags
extern D4D_SYSTEM_FLAGS d4d_systemFlags;

// zero size structure for automatic function capability
extern const D4D_SIZE d4d_size_zero;

// zero point structure
extern const D4D_POINT d4d_point_zero;

#ifdef D4D_LLD_TCH
  extern D4D_OBJECT* d4d_LastTouchedObj;
#endif

#if (D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE) || (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
  extern Byte d4d_extsrcBuffer[D4D_EXTSRC_BUFF_SIZE];
#endif

/******************************************************************************
* Global functions
******************************************************************************/

// Internal API of base functions

void D4D_HandleKeys(void);


void D4D_SendMessage(D4D_MESSAGE* pMsg);
void D4D_SendMessageMask(D4D_MESSAGE* pMsg, D4D_OBJECT_FLAGS parentFlagsMask, D4D_OBJECT_FLAGS endFlagMask);
void D4D_SendMessageBack(D4D_MESSAGE* pMsg);

// Objects private functions
void D4D_ObjOnMessage(D4D_MESSAGE* pMsg);
void D4D_ChangeText(D4D_STRING* pBuff, D4D_TCHAR* pNewText, D4D_TCHAR fillChar);

D4D_OBJECT* D4D_GetParentObject(D4D_OBJECT* pObject);
D4D_BOOL D4D_HasObjectChildren(D4D_OBJECT* pObject);

D4D_OBJECT* D4D_FindNextObjectOnScreen(D4D_OBJECT* pObject);
D4D_OBJECT* D4D_FindNextParentSiblingObject(D4D_OBJECT* pObject);
D4D_OBJECT* D4D_FindNextSiblingObject(D4D_OBJECT* pObject);
D4D_OBJECT* D4D_FindNextObject(D4D_OBJECT* pObject, D4D_BOOL childrenAlso);
D4D_OBJECT* D4D_FindPreviousObjectOnScreen(D4D_OBJECT* pObject);
D4D_OBJECT* D4D_FindPreviousParentSiblingObject(D4D_OBJECT* pObject);
D4D_OBJECT* D4D_FindPreviousSiblingObject(D4D_OBJECT* pObject);
D4D_OBJECT* D4D_FindPreviousObject(D4D_OBJECT* pObject, D4D_BOOL childrenAlso);


void D4D_RedrawScreen(D4D_SCREEN* pScreen);
D4D_BOOL D4D_ScrCheckExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* point);
D4D_BOOL D4D_ScrCheckCoor(D4D_SCREEN* pScreen, D4D_POINT* point);
D4D_COOR D4D_GetScrHeaderSize(D4D_SCREEN* pScreen);


D4D_OBJECT* D4D_GetLastObject(D4D_SCREEN* pScreen);
void D4D_HandleTimeTicks(D4D_SCREEN* pScreen);


void D4D_SetObjectFlags(D4D_OBJECT* pObject, D4D_OBJECT_FLAGS flagsMask, D4D_BOOL alsoChildren);
void D4D_SetObjectScreenPointer(D4D_OBJECT* pObject, D4D_SCREEN* pScreen);
// Font private function declaration

void D4D_ComputeGeometry(D4D_GEOMETRY * pGeometry, D4D_OBJECT* pObject);
void D4D_DrawFrame(D4D_OBJECT* pObject, D4D_COLOR clrT, D4D_COLOR clrB);

// String & font private functions
D4D_COOR D4D_LCD_PrintChr(D4D_TCHAR ch, D4D_PRINT_DESC* p_CharDes);

#endif /* __D4D_PRIVATE_H */
