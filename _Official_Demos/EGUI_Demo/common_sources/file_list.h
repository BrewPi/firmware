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
* $FileName: file_list.h$
*
* @author    Petr Gargulak
*
* $Version : 3.6.1.0$
*
* $Date    : Aug-14-2012$
*
* Comments:
*
*   This file contains the header information for the eGUI example file browser.
*
*END************************************************************************/


#ifndef __FILE_LIST_H
#define __FILE_LIST_H

/******************************************************************************
* Includes
******************************************************************************/


/******************************************************************************
* Constants
******************************************************************************/

#ifndef PATHNAME_SIZE
  #define PATHNAME_SIZE 256
#endif

#ifndef FILELIST_FILESYSTEM_HANDLE
  #define FILELIST_FILESYSTEM_HANDLE filesystem_handle
#endif

/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

typedef char FILENAME[14];

typedef struct
{
  LWord fileCount;
  LWord dirCount;
  FILENAME * pFileNames;
  FILENAME * pDirNames;
  FILENAME fileMask;
  char pCurrPath[PATHNAME_SIZE];
  
}FILE_LIST;

/******************************************************************************
* Global variables
******************************************************************************/
extern MQX_FILE_PTR FILELIST_FILESYSTEM_HANDLE;

/******************************************************************************
* Global functions
******************************************************************************/
FILE_LIST* FileList_Install(char* pStartPath);
void FileList_UnInstall(FILE_LIST* pFileList);

void FileList_RefreshData(FILE_LIST* pFileList);
void FileList_ChangeDirectory(FILE_LIST* pFileList, LWord ix);
void FileList_SetCurrentPath(FILE_LIST* pFileList, char* pNewPath);

char* FileList_GetCurrentPath(FILE_LIST* pFileList);
LWord FileList_GetDirCnt(FILE_LIST* pFileList);
LWord FileList_GetFilesCnt(FILE_LIST* pFileList);
char* FileList_GetFileName(FILE_LIST* pFileList, LWord ix);
char* FileList_GetDirName(FILE_LIST* pFileList, LWord ix);



#endif    // __FILE_LIST_H

