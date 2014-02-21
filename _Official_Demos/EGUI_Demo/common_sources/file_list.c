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
**************************************************************************
*
* $FileName: file_list.c$
* $Version : 3.6.2.0$
* $Date    : Jan-14-2014$
*
* Comments:
*
*   This file contains the source for the eGUI example file browser.
*
*END************************************************************************/

/****************************************************************************
*       Includes header files
****************************************************************************/
#include "d4d.h"

#include <mqx.h>
#include <bsp.h>
#include <mfs.h>
#include <string.h>
#include "file_list.h"

/****************************************************************************
*       Local functions declarations
****************************************************************************/
static LWord FileList_Explore(FILE_LIST* pFileList, uint32_t directory, uint32_t justCount);
static void FileList_FreeMem(FILE_LIST* pFileList);



/****************************************************************************
*       Public API functions
****************************************************************************/

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_Install
* SCOPE:       File list related function
* DESCRIPTION: Function install file list to memory
*
* PARAMETERS:  pStartPath - pointer of start path  
*              
* RETURNS:     pointer to created file list structure
*-----------------------------------------------------------------------------*/
FILE_LIST* FileList_Install(char* pStartPath)
{
  FILE_LIST* pFileList;
  LWord i;
  
  if(strlen(pStartPath) > PATHNAME_SIZE)
    return NULL;
  
  pFileList = (FILE_LIST*)_mem_alloc(sizeof(FILE_LIST));
  
  if(pFileList == NULL)
    return NULL;
  else
  {
    // Clear up all the structure
    for(i=0;i<sizeof(FILE_LIST);i++)
      ((Byte*)pFileList)[i] = 0;
  }
  
  strcpy(pFileList->pCurrPath, pStartPath);
  
  pFileList->fileCount = 0;
  pFileList->dirCount = 0;
  
  strcpy((char*)pFileList->fileMask, "*.*");
  
  
  return pFileList;
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_UnInstall
* SCOPE:       File list related function
* DESCRIPTION: Function un-install file list from memory
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
void FileList_UnInstall(FILE_LIST* pFileList)
{
  FileList_FreeMem(pFileList);
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_RefreshData
* SCOPE:       File list related function
* DESCRIPTION: Function refresh the lists of files/directories
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
void FileList_RefreshData(FILE_LIST* pFileList)
{
  LWord count;
  
  if(pFileList == NULL)
    return;
  
  // Load the directories
  
  // Just count all the directories
  count = FileList_Explore(pFileList, 1, 1);
  
  if(count > pFileList->dirCount)
  {
    if(pFileList->pDirNames)
      _mem_free(pFileList->pDirNames);
    pFileList->pDirNames = (FILENAME*)_mem_alloc(sizeof(FILENAME) * count);
     
    if(pFileList->pDirNames == NULL)
    {
      pFileList->dirCount = 0;
      return;
    }        
  }
  
  pFileList->dirCount = count;
  
  // Fill up all the directories
  count = FileList_Explore(pFileList, 1, 0);
  
  // Load the files
  
  // Just count all the files
  count = FileList_Explore(pFileList, 0, 1);
  
  if(count > pFileList->fileCount)
  {
    if(pFileList->pFileNames)
      _mem_free(pFileList->pFileNames);
    pFileList->pFileNames = (FILENAME*)_mem_alloc(sizeof(FILENAME) * count);
     
    if(pFileList->pFileNames == NULL)
    {
      pFileList->fileCount = 0;
      return;
    }        
  }
  
  pFileList->fileCount = count;
  
  // Fill up all the directories
  count = FileList_Explore(pFileList, 0, 0);
}
 
/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_GetFilesCnt
* SCOPE:       File list related function
* DESCRIPTION: Function returns the file count
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              
* RETURNS:     file count
*-----------------------------------------------------------------------------*/
LWord FileList_GetFilesCnt(FILE_LIST* pFileList)
{
  return pFileList->fileCount;  
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_GetDirCnt
* SCOPE:       File list related function
* DESCRIPTION: Function returns the directories count
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              
* RETURNS:     directories count
*-----------------------------------------------------------------------------*/
LWord FileList_GetDirCnt(FILE_LIST* pFileList)
{
  return pFileList->dirCount;  
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_GetCurrentPath
* SCOPE:       File list related function
* DESCRIPTION: Function returns the pointer on stored current working path
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              
* RETURNS:     pointer to current working path
*-----------------------------------------------------------------------------*/
char* FileList_GetCurrentPath(FILE_LIST* pFileList)
{
  return pFileList->pCurrPath;  
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_GetFileName
* SCOPE:       File list related function
* DESCRIPTION: Function returns the pointer on file name
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              ix        - index of file to get name
*              
* RETURNS:     pointer to file name
*-----------------------------------------------------------------------------*/
char* FileList_GetFileName(FILE_LIST* pFileList, LWord ix)
{
  if(pFileList == NULL)
    return NULL;
  
  if(pFileList->fileCount <= ix)
    return NULL;
  
  return (char*)(pFileList->pFileNames[ix]);
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_GetDirName
* SCOPE:       File list related function
* DESCRIPTION: Function returns the pointer on directory name
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              ix        - index of directory to get name
*              
* RETURNS:     pointer to directory name
*-----------------------------------------------------------------------------*/
char* FileList_GetDirName(FILE_LIST* pFileList, LWord ix)
{
  if(pFileList == NULL)
    return NULL;
  
  if(pFileList->dirCount <= ix)
    return NULL;
  
  return (char*)(pFileList->pDirNames[ix]);
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_ChangeDirectory
* SCOPE:       File list related function
* DESCRIPTION: Function changes the working directory
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              ix        - index of directory to go in
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
void FileList_ChangeDirectory(FILE_LIST* pFileList, LWord ix)
{
  char* pDirname = FileList_GetDirName(pFileList, ix);
  
  if(pDirname == NULL)
    return;
    
  if(strcmp((const char*)pDirname,".."))
  {
    if(strlen(pFileList->pCurrPath) + strlen(pDirname) > PATHNAME_SIZE)
      return;
    
    strcat(pFileList->pCurrPath, (const char*)pDirname);
    strcat(pFileList->pCurrPath, "\\");
  }else
  {
    LWord len;
    
    len = strlen(pFileList->pCurrPath);
    len--;
    do
    {
      pFileList->pCurrPath[len] = '\0';
      len--;   
    }while(pFileList->pCurrPath[len] != '\\');    
  }
  
  FileList_RefreshData(pFileList);  
  
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_SetCurrentPath
* SCOPE:       File list related function
* DESCRIPTION: Function sets the working directory 
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              pNewPath  - new working path
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
void FileList_SetCurrentPath(FILE_LIST* pFileList, char* pNewPath)
{
  LWord len;
  
  if(pFileList == NULL)
    return;
  
  len = strlen(pNewPath);
  
  if(len > PATHNAME_SIZE)
    return;
  
  // check if the last record in path is really folder  
  len--;
  
  while(len && pNewPath[len])
  {
    if(pNewPath[len] == '\\')
    {
      // last record is folder
      strcpy(pFileList->pCurrPath, pNewPath);
      FileList_RefreshData(pFileList);
      return;
    }
    
    if(pNewPath[len] == '.')
    {
      // it looks that last record is file
      while(len && pNewPath[len])
      {
        // look for last folder
        if(pNewPath[len] == '/')
        {
          // last record is folder
          strcpy(pFileList->pCurrPath, pNewPath);
          pFileList->pCurrPath[len] = '\0';
          FileList_RefreshData(pFileList);
          return;
        }
        len--;
      }  
    }
    len--;
  }
  
}

/****************************************************************************
*       Local functions
****************************************************************************/

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_Explore
* SCOPE:       File list related function - local
* DESCRIPTION: Function fill ups the list of directories or files
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              directory  - direcotries or files
*              justCount - only count files/directories ?     
*              
* RETURNS:     count of files/directories
*-----------------------------------------------------------------------------*/
static LWord FileList_Explore(FILE_LIST* pFileList, uint32_t directory, uint32_t justCount)
{
  MFS_SEARCH_DATA search_data;
  MFS_SEARCH_PARAM search;
  _mqx_uint error_code;
  LWord count = 0;
  
  char tmp_Dirname[PATHNAME_SIZE];
  
  if(strlen(pFileList->pCurrPath) + strlen(pFileList->fileMask) > PATHNAME_SIZE)
    return 0;
  
  strcpy(tmp_Dirname, pFileList->pCurrPath);
  // find and count all directories  
  if(directory)
  {
    search.ATTRIBUTE = MFS_SEARCH_SUBDIR;
    strcat(tmp_Dirname, "*.*");
  
  }
  else
  {
    search.ATTRIBUTE = MFS_SEARCH_NORMAL;
    strcat(tmp_Dirname, pFileList->fileMask);
  }
    
  search.WILDCARD = tmp_Dirname;
  search.SEARCH_DATA_PTR = &search_data;
   
  error_code = ioctl(FILELIST_FILESYSTEM_HANDLE, IO_IOCTL_FIND_FIRST_FILE, (uint32_t *) &search);

  while(error_code == MFS_NO_ERROR)
  {
    if(directory)
    { 
      if(strcmp(search_data.NAME, "."))
      {
        if(!justCount && pFileList->pDirNames)
          strcpy((char*)&(pFileList->pDirNames[count]), search_data.NAME);
        
        count++;
      }
    }          
    else
    {
      if(!(search_data.ATTRIBUTE & MFS_ATTR_DIR_NAME))
      {        
        if(!justCount && pFileList->pFileNames)
          strcpy((char*)&(pFileList->pFileNames[count]), search_data.NAME);
        
        count++;
      }
    }
      
      
      
    error_code = ioctl(FILELIST_FILESYSTEM_HANDLE, IO_IOCTL_FIND_NEXT_FILE, (uint32_t *) &search_data);
  }
  
  return count;
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileList_FreeMem
* SCOPE:       File list related function - local
* DESCRIPTION: Function free mem of allocated arrays
*
* PARAMETERS:  pFileList - pointer of file list structure  
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void FileList_FreeMem(FILE_LIST* pFileList)
{
  if(pFileList == NULL)
    return;
  
  if(pFileList->pFileNames)
    _mem_free(pFileList->pFileNames);
  
  if(pFileList->pDirNames)
    _mem_free(pFileList->pDirNames);
  
  _mem_free(pFileList);
}

