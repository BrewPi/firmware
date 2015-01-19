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
* @file      d4d_screen_openfile.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Aug-14-2012
* 
* @brief     D4D driver - filelist of d4d typical screen source file
*
******************************************************************************/

#ifndef _D4D_SCREEN_FILELIST_H
  #define _D4D_SCREEN_FILELIST_H

/******************************************************************************
* Includes
******************************************************************************/
#include "file_list.h"

/******************************************************************************
* Constants
******************************************************************************/
#define FILE_DIALOG_FILENAME_SIZE 14
/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/
typedef enum
{
  FD_OK,
  FD_CANCEL  
}FILE_DIALOG_RESULT;

typedef struct FILE_DIALOG_S
{
  FILE_LIST* pFileList;
  LWord dirCnt;
  LWord fileCnt;
  D4D_LIST_BOX_ITEM*  pItemsFiles;
  D4D_LIST_BOX_ITEM*  pItemsDirectories; 
  D4D_CHAR pOutputPath[PATHNAME_SIZE];
  void (*FileDialogEnds)(struct FILE_DIALOG_S* pThis, FILE_DIALOG_RESULT result);  
}FILE_DIALOG;
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
FILE_DIALOG* D4D_FileDialogCreate(D4D_CHAR* pStartPath, void (*pFileDialogEndsFunc)(FILE_DIALOG* pThis, FILE_DIALOG_RESULT result));
void D4D_FileDialogShow(FILE_DIALOG* pFileDialog);
void D4D_FileDialogDestroy(FILE_DIALOG* pFileDialog);
D4D_BOOL D4D_FileDialogUpdate(void);
D4D_CHAR* D4D_FileDialogGetDirName(FILE_DIALOG* pFileDialog);
D4D_CHAR* D4D_FileDialogGetFileName(FILE_DIALOG* pFileDialog);
#endif


