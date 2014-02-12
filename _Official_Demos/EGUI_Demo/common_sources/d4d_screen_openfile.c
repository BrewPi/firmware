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
* @file      d4d_screen_openfile.c
*
* @author    Petr Gargulak
* 
* @version   0.0.4.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - filelist of d4d typical screen source file
*
******************************************************************************/

#include "d4d.h"
#include "d4d_screen_openfile.h"
#include "strings.h"
#include "fonts.h"
#include <string.h>


/******************************************************************************
*
*                       locals variables declarations
*
******************************************************************************/

static FILE_DIALOG* pLocFileDialog = NULL;
static D4D_LIST_BOX_ITEM* pDefaultFileList = NULL;
static D4D_LIST_BOX_ITEM* pDefaultDirList = NULL;

/******************************************************************************
*
*                       Graphic objects declarations
*
******************************************************************************/

#define SFL_SCREEN_SIZE_X   440
#define SFL_SCREEN_SIZE_Y   224   

#define RADIUS              6

#define SFL_LABEL_SIZE_Y    18
#define SFL_EDITBOX_SIZE_Y    22
#define SFL_COMBOBOX_SIZE_Y    SFL_EDITBOX_SIZE_Y
#define SFL_BUTTON_SIZE_Y    26
#define SFL_LISTBOX_SIZE_Y  (SFL_SCREEN_SIZE_Y - SFL_OFFSET_3ROW_Y - 3*SFL_OFFSET_Y - SFL_LABEL_SIZE_Y - SFL_BUTTON_SIZE_Y)

#define SFL_DIV_OBJECT_SIZE_X       ((SFL_SCREEN_SIZE_X - (SFL_OFFSET_X*3)) / 2)
#define SFL_OBJECT_SIZE_X           (SFL_SCREEN_SIZE_X - (SFL_OFFSET_X*2))


#define SFL_OFFSET_X        4
#define SFL_OFFSET_Y        4

#define SFL_OFFSET_2COL_X           (SFL_OFFSET_X * 2 + SFL_DIV_OBJECT_SIZE_X)       

#define SFL_OFFSET_2ROW_Y        (SFL_OFFSET_Y + SFL_LABEL_SIZE_Y + SFL_OFFSET_Y)
#define SFL_OFFSET_3ROW_Y        (SFL_OFFSET_2ROW_Y + SFL_COMBOBOX_SIZE_Y + SFL_OFFSET_Y)
#define SFL_OFFSET_4ROW_Y        (SFL_OFFSET_3ROW_Y + SFL_LISTBOX_SIZE_Y + SFL_OFFSET_Y)
#define SFL_OFFSET_5ROW_Y        (SFL_OFFSET_4ROW_Y + SFL_LABEL_SIZE_Y + SFL_OFFSET_Y)

#define SFL_FONT        FONT_SYSTEM       
#define SFL_FONT_SMALL  FONT_SYSTEM_SMALL       

#define SFL_LABEL_FLAGS (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)
#define SFL_EDITBOX_FLAGS (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_BEVEL_DENTED | D4D_OBJECT_F_MOUSE_NORMAL)
#define SFL_LISTBOX_FLAGS (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_BEVEL_DENTED | D4D_OBJECT_F_MOUSE_NORMAL)
#define SFL_BUTTON_FLAGS  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_MOUSE_NORMAL | D4D_OBJECT_F_TOUCHENABLE  | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_BEVEL_RAISED | D4D_BTN_F_3D)


static void SFL_LstBxFilesOnChange(D4D_OBJECT* pThis, D4D_EVENTID eventId);
static void SFL_LstBxDirsOnChange(D4D_OBJECT* pThis, D4D_EVENTID eventId);
static void SFL_BtnOkOnClick(D4D_OBJECT* pThis);
static void SFL_BtnCancelOnClick(D4D_OBJECT* pThis);

// Just Titles 
D4D_DECLARE_RLABEL(sfl_lbl_DirTitle, D4D_STR_DIRECTORIES, SFL_OFFSET_X, SFL_OFFSET_Y, SFL_DIV_OBJECT_SIZE_X, SFL_LABEL_SIZE_Y, RADIUS, SFL_LABEL_FLAGS, NULL, SFL_FONT, NULL, NULL)
D4D_DECLARE_RLABEL(sfl_lbl_FilTitle, D4D_STR_FILES, SFL_OFFSET_2COL_X, SFL_OFFSET_Y, SFL_DIV_OBJECT_SIZE_X, SFL_LABEL_SIZE_Y, RADIUS, SFL_LABEL_FLAGS, NULL, SFL_FONT, NULL, NULL)

// Disk name + file name
D4D_TCHAR sfl_lbl_DiskStr[16] = D4D_DEFSTR("Disk: A:\\");
D4D_DECLARE_RLABEL(sfl_lbl_Disk, sfl_lbl_DiskStr, SFL_OFFSET_X, SFL_OFFSET_2ROW_Y, SFL_DIV_OBJECT_SIZE_X, SFL_COMBOBOX_SIZE_Y, RADIUS, (SFL_LABEL_FLAGS | D4D_OBJECT_F_FOCUSRECT), NULL, SFL_FONT, NULL, NULL)

D4D_DECLARE_REDIT_BOX(sfl_edtBx_File, SFL_OFFSET_2COL_X, SFL_OFFSET_2ROW_Y, SFL_DIV_OBJECT_SIZE_X, SFL_EDITBOX_SIZE_Y, RADIUS, NULL, SFL_EDITBOX_FLAGS, NULL, D4D_DEFSTR("*.*"), SFL_FONT, 14, NULL, NULL, NULL)

// ListBoxes for directories and files
D4D_DECLARE_RLIST_BOX_BEGIN(sfl_lstBx_Dir, SFL_OFFSET_X, SFL_OFFSET_3ROW_Y, SFL_DIV_OBJECT_SIZE_X, SFL_LISTBOX_SIZE_Y, RADIUS, SFL_LISTBOX_FLAGS, NULL, NULL, SFL_FONT, 0, 0, NULL, SFL_LstBxDirsOnChange, NULL)
D4D_DECLARE_LIST_BOX_END() // Nothing it will be added automatically

D4D_DECLARE_RLIST_BOX_BEGIN(sfl_lstBx_Files, SFL_OFFSET_2COL_X, SFL_OFFSET_3ROW_Y, SFL_DIV_OBJECT_SIZE_X, SFL_LISTBOX_SIZE_Y, RADIUS, SFL_LISTBOX_FLAGS, NULL, NULL, SFL_FONT, 0, 0, NULL, SFL_LstBxFilesOnChange, NULL)
D4D_DECLARE_LIST_BOX_END() // Nothing it will be added automatically

// The current directory label
D4D_TCHAR sfl_lbl_CurrDirStr[PATHNAME_SIZE] = D4D_DEFSTR("A:\\");
D4D_DECLARE_RLABEL(sfl_lbl_CurrDir, sfl_lbl_CurrDirStr, SFL_OFFSET_X, SFL_OFFSET_4ROW_Y, SFL_OBJECT_SIZE_X, SFL_LABEL_SIZE_Y, RADIUS, SFL_LABEL_FLAGS, NULL, SFL_FONT_SMALL, NULL, NULL)

// The action buttons
D4D_DECLARE_RBUTTON(sfl_btn_Cancel, D4D_STR_CANCEL, SFL_OFFSET_X, SFL_OFFSET_5ROW_Y, SFL_DIV_OBJECT_SIZE_X, SFL_BUTTON_SIZE_Y, RADIUS, SFL_BUTTON_FLAGS, NULL, NULL, NULL, SFL_FONT, NULL, SFL_BtnCancelOnClick, NULL)
D4D_DECLARE_RBUTTON(sfl_btn_OK, D4D_STR_OK, SFL_OFFSET_2COL_X, SFL_OFFSET_5ROW_Y, SFL_DIV_OBJECT_SIZE_X, SFL_BUTTON_SIZE_Y, RADIUS, SFL_BUTTON_FLAGS, NULL, NULL, NULL, SFL_FONT, NULL, SFL_BtnOkOnClick, NULL)

// Screen declaration
D4D_DECLARE_RSCREEN_BEGIN(screen_filelist, ScreenFileList_, 20 , 20, SFL_SCREEN_SIZE_X, (SFL_SCREEN_SIZE_Y + SFL_LABEL_SIZE_Y), RADIUS, D4D_STR_OPENFILE, SFL_FONT, NULL, ( D4D_SCR_F_OUTLINE | D4D_SCR_F_TITLEBAR | D4D_SCR_F_EXIT | D4D_SCR_FINT_MOUSE_NORMAL | D4D_SCR_F_BCKG), NULL)
  // Titles
  D4D_DECLARE_SCREEN_OBJECT(sfl_lbl_DirTitle)
  D4D_DECLARE_SCREEN_OBJECT(sfl_lbl_FilTitle)
  // Disk name + file name
  D4D_DECLARE_SCREEN_OBJECT(sfl_lbl_Disk)
  D4D_DECLARE_SCREEN_OBJECT(sfl_edtBx_File)
  // List Boxes (Directory + Files)
  D4D_DECLARE_SCREEN_OBJECT(sfl_lstBx_Dir)
  D4D_DECLARE_SCREEN_OBJECT(sfl_lstBx_Files)
    
  //Current Directory Label
  D4D_DECLARE_SCREEN_OBJECT(sfl_lbl_CurrDir)  
  
  //Action Buttons
  D4D_DECLARE_SCREEN_OBJECT(sfl_btn_Cancel)
  D4D_DECLARE_SCREEN_OBJECT(sfl_btn_OK)
    
    
D4D_DECLARE_SCREEN_END()    

/*-----------------------------------------------------------------------------
* FUNCTION:    SFL_LstBxDirsOnChange
* SCOPE:       Open file dialog related function - local
* DESCRIPTION: Event of list box - directories
*
* PARAMETERS:  pThis - pointer of object
*              eventId - type of event 
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void SFL_LstBxDirsOnChange(D4D_OBJECT* pThis, D4D_EVENTID eventId)
{
  // Just on click enets are wanted
  if(eventId != D4D_EVENT_ONCLICK)
    return;
  
  // Change the directory
  FileList_ChangeDirectory(pLocFileDialog->pFileList, D4D_ListBoxGetIndex(pThis));
  
  // Update the objects on screen
  if(!D4D_FileDialogUpdate())
  {    
    // in case that the loaded files are not able to show, load the default 
    D4D_ListBoxChangeItemList(&sfl_lstBx_Dir, pDefaultDirList);
    D4D_ListBoxChangeItemList(&sfl_lstBx_Files, pDefaultFileList);
  }
}

/*-----------------------------------------------------------------------------
* FUNCTION:    SFL_LstBxFilesOnChange
* SCOPE:       Open file dialog related function - local
* DESCRIPTION: Event of list box - files
*
* PARAMETERS:  pThis - pointer of object
*              eventId - type of event 
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void SFL_LstBxFilesOnChange(D4D_OBJECT* pThis, D4D_EVENTID eventId)
{
  if(eventId == D4D_EVENT_ONCLICK)
  {
    // Invoke the OK button action
    SFL_BtnOkOnClick(pThis);
  }else
  {
    // Update edit box
    D4D_EditBoxClearAll(&sfl_edtBx_File);
    D4D_EditBoxPutString(&sfl_edtBx_File, D4D_ListBoxGetItemText(pThis));
  }  
}

/*-----------------------------------------------------------------------------
* FUNCTION:    SFL_BtnOkOnClick
* SCOPE:       Open file dialog related function - local
* DESCRIPTION: Event of button - OK button
*
* PARAMETERS:  pThis - pointer of object
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void SFL_BtnOkOnClick(D4D_OBJECT* pThis)
{
  // Store the current path
  D4D_StrCopy(pLocFileDialog->pOutputPath, pLocFileDialog->pFileList->pCurrPath);
  
  // Call the file dialog ends event
  if(pLocFileDialog->FileDialogEnds)
    pLocFileDialog->FileDialogEnds(pLocFileDialog, FD_OK);
  
  // Escape screen
  D4D_EscapeScreen();
}

/*-----------------------------------------------------------------------------
* FUNCTION:    SFL_BtnCancelOnClick
* SCOPE:       Open file dialog related function - local
* DESCRIPTION: Event of button - Cancel button
*
* PARAMETERS:  pThis - pointer of object
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void SFL_BtnCancelOnClick(D4D_OBJECT* pThis)
{
  // Call the file dialog ends event
  if(pLocFileDialog->FileDialogEnds)
    pLocFileDialog->FileDialogEnds(pLocFileDialog, FD_CANCEL); 
  
  // Escape screen
  D4D_EscapeScreen();
}

/****************************************************************************
*       D4D standard screen functions definitions
****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenFileList_OnInit()
{
  D4D_SetFontProperties(&sfl_lbl_DirTitle, D4D_FNT_PRTY_TRANSPARENT_YES_MASK);
  D4D_SetFontProperties(&sfl_lbl_FilTitle, D4D_FNT_PRTY_TRANSPARENT_YES_MASK);
  D4D_SetTextProperties(&sfl_lbl_Disk, (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK));
  D4D_SetTextProperties(&sfl_lbl_CurrDir, (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK));  
}

// Screen on Activate function called with each screen activation
static void ScreenFileList_OnActivate()
{
  
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenFileList_OnMain()
{
    
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenFileList_OnDeactivate()
{
    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenFileList_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}


/*-----------------------------------------------------------------------------
* FUNCTION:    D4D_FileDialogUpdate
* SCOPE:       Open file dialog related function
* DESCRIPTION: Functions update all information on screen
*
* PARAMETERS:  none
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
D4D_BOOL D4D_FileDialogUpdate(void)
{
    if(pLocFileDialog == NULL)
       return D4D_FALSE;
      
    
    
#ifdef D4D_UNICODE
    {
      D4D_TCHAR tmpBuff[PATHNAME_SIZE];
      D4D_TCHAR ch;
      D4D_TCHAR* pText;
      LWord i;
      // Update the current path
      D4D_StrCopyAscii2Unicode(tmpBuff, pLocFileDialog->pFileList->pCurrPath);      
      D4D_SetText(&sfl_lbl_CurrDir, tmpBuff);
 
      // Update the Current disk name  
      ch = tmpBuff[0];
      D4D_StrCopyAscii2Unicode(tmpBuff, "Disk: A:\\");
      tmpBuff[6] = ch;     
      D4D_SetText(&sfl_lbl_Disk, tmpBuff);
      
      
      // Allocate the new space for file list if needed
      if(pLocFileDialog->fileCnt < (pLocFileDialog->pFileList->fileCount + 1))
      {
        if(pLocFileDialog->pItemsFiles)
          D4D_MemFree((void*)(pLocFileDialog->pItemsFiles));

        pLocFileDialog->pItemsFiles = (D4D_LIST_BOX_ITEM*)D4D_MemAlloc((sizeof(D4D_LIST_BOX_ITEM) + (sizeof(D4D_TCHAR) * FILE_DIALOG_FILENAME_SIZE)) * (pLocFileDialog->pFileList->fileCount + 1));
        
        if(pLocFileDialog->pItemsFiles == NULL)
          return D4D_FALSE;    
      }
      
      // Allocate the new space for file list if needed
      if(pLocFileDialog->dirCnt < (pLocFileDialog->pFileList->dirCount + 1))
      {
        if(pLocFileDialog->pItemsDirectories)
          D4D_MemFree((void*)(pLocFileDialog->pItemsDirectories));

        pLocFileDialog->pItemsDirectories = (D4D_LIST_BOX_ITEM*)D4D_MemAlloc((sizeof(D4D_LIST_BOX_ITEM) + (sizeof(D4D_TCHAR) * FILE_DIALOG_FILENAME_SIZE)) * (pLocFileDialog->pFileList->dirCount + 1));

        if(pLocFileDialog->pItemsDirectories == NULL)
          return D4D_FALSE;
      }
      
      // Update the directory list
      pText = (D4D_TCHAR*)&(pLocFileDialog->pItemsDirectories[(pLocFileDialog->pFileList->dirCount + 1)]);
      for(i=0;i<pLocFileDialog->pFileList->dirCount;i++)
      {               
        pLocFileDialog->pItemsDirectories[i].pUser = NULL;
        pLocFileDialog->pItemsDirectories[i].pText = pText;
        D4D_StrCopyAscii2Unicode(pText, FileList_GetDirName(pLocFileDialog->pFileList, i));
        pText += FILE_DIALOG_FILENAME_SIZE;
      }

      pLocFileDialog->pItemsDirectories[i].pUser = NULL;
      pLocFileDialog->pItemsDirectories[i].pText = NULL;
      pLocFileDialog->dirCnt = pLocFileDialog->pFileList->dirCount;
      
      // Update the file list
      pText = (D4D_TCHAR*)&pLocFileDialog->pItemsFiles[(pLocFileDialog->pFileList->fileCount + 1)];
      for(i=0;i<pLocFileDialog->pFileList->fileCount;i++)
      {               
        pLocFileDialog->pItemsFiles[i].pUser = NULL;
        pLocFileDialog->pItemsFiles[i].pText = pText;
        D4D_StrCopyAscii2Unicode(pText, FileList_GetFileName(pLocFileDialog->pFileList, i));
        pText += FILE_DIALOG_FILENAME_SIZE;
      }

      pLocFileDialog->pItemsFiles[i].pUser = NULL;
      pLocFileDialog->pItemsFiles[i].pText = NULL;
      pLocFileDialog->fileCnt = pLocFileDialog->pFileList->fileCount;          
    }   
#else     
      
#endif 
   
   D4D_ListBoxChangeItemList(&sfl_lstBx_Dir, pLocFileDialog->pItemsDirectories);
   D4D_ListBoxChangeItemList(&sfl_lstBx_Files, pLocFileDialog->pItemsFiles);
   
   return D4D_TRUE; 
}

/*-----------------------------------------------------------------------------
* FUNCTION:    D4D_FileDialogShow
* SCOPE:       Open file dialog related function
* DESCRIPTION: This function is called to show this open dialog
*
* PARAMETERS:  pFileDialog - pointer on file dialog structure
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
void D4D_FileDialogShow(FILE_DIALOG* pFileDialog)
{
  if(pFileDialog == NULL)
    return;
  
  pLocFileDialog = pFileDialog;
  
  if(pDefaultFileList == NULL)
    pDefaultFileList = D4D_ListBoxGetItemList(&sfl_lstBx_Files);
  
  if(pDefaultDirList == NULL)
    pDefaultDirList = D4D_ListBoxGetItemList(&sfl_lstBx_Dir);
  
  if(strcmp(pLocFileDialog->pOutputPath, pLocFileDialog->pFileList->pCurrPath))
    FileList_SetCurrentPath(pLocFileDialog->pFileList, pLocFileDialog->pOutputPath);
  
  if(!D4D_FileDialogUpdate())
  {    
    D4D_ListBoxChangeItemList(&sfl_lstBx_Dir, pDefaultDirList);
    D4D_ListBoxChangeItemList(&sfl_lstBx_Files, pDefaultFileList);
  }
  D4D_ActivateScreen((D4D_SCREEN*)&screen_filelist, D4D_FALSE);    
}

/*-----------------------------------------------------------------------------
* FUNCTION:    D4D_FileDialogCreate
* SCOPE:       Open file dialog related function
* DESCRIPTION: This function creates the open file dialog in memory (not show)
*
* PARAMETERS:  pStartPath - start path of file dialog
*              pFileDialogEndsFunc - pointer on file dialog end event function 
*              
* RETURNS:     pointer on created FILE_DIALOG structure
*-----------------------------------------------------------------------------*/
FILE_DIALOG* D4D_FileDialogCreate(D4D_CHAR* pStartPath, void (*pFileDialogEndsFunc)(FILE_DIALOG* pThis, FILE_DIALOG_RESULT result))
{
  FILE_DIALOG* pFileDialog = NULL;
  LWord i;
  //Create a new instation
  pFileDialog = D4D_MemAlloc(sizeof(FILE_DIALOG));
  if(pFileDialog == NULL)
    return NULL;   

  // Clear up all the structure
  for(i=0;i<sizeof(FILE_DIALOG);i++)
    ((Byte*)pFileDialog)[i] = 0;
  
  pFileDialog->FileDialogEnds = pFileDialogEndsFunc;
  pFileDialog->pFileList = FileList_Install(pStartPath);    
  
  if(pFileDialog->pFileList == NULL)
  {
    D4D_MemFree(pFileDialog);
    return NULL;
  }
  
  D4D_StrCopy(pFileDialog->pOutputPath, pFileDialog->pFileList->pCurrPath);
  
  // Load the  directories and files
  FileList_RefreshData(pFileDialog->pFileList);

  return pFileDialog;
}

/*-----------------------------------------------------------------------------
* FUNCTION:    D4D_FileDialogDestroy
* SCOPE:       Open file dialog related function
* DESCRIPTION: This function destroy the file dialog structures in memory (free mem)
*
* PARAMETERS:  pFileDialog - pointer on file dialog structure
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
void D4D_FileDialogDestroy(FILE_DIALOG* pFileDialog)
{
  if(pFileDialog == NULL)
    return;
  
  if(pFileDialog->pFileList)
    FileList_UnInstall(pFileDialog->pFileList);
  
  if(pFileDialog->pItemsDirectories)
    D4D_MemFree(pFileDialog->pItemsDirectories);
  
  if(pFileDialog->pItemsFiles)
    D4D_MemFree(pFileDialog->pItemsFiles);
  
  D4D_MemFree(pFileDialog);
  
  D4D_ListBoxChangeItemList(&sfl_lstBx_Dir, pDefaultDirList);
  D4D_ListBoxChangeItemList(&sfl_lstBx_Files, pDefaultFileList);
  
  
  pLocFileDialog = NULL;
}

/*-----------------------------------------------------------------------------
* FUNCTION:    D4D_FileDialogGetFileName
* SCOPE:       Open file dialog related function
* DESCRIPTION: This function return the selected file name
*
* PARAMETERS:  pFileDialog - pointer on file dialog structure
*              
* RETURNS:     pointer to file name
*-----------------------------------------------------------------------------*/
D4D_CHAR* D4D_FileDialogGetFileName(FILE_DIALOG* pFileDialog)
{
  LWord ix = D4D_ListBoxGetIndex((D4D_OBJECT*)&sfl_lstBx_Files);
  
  if(!pFileDialog->fileCnt)
    return NULL;
  
  return pFileDialog->pFileList->pFileNames[ix];
}

/*-----------------------------------------------------------------------------
* FUNCTION:    D4D_FileDialogGetDirName
* SCOPE:       Open file dialog related function
* DESCRIPTION: This function return the selected directory name
*
* PARAMETERS:  pFileDialog - pointer on file dialog structure
*              
* RETURNS:     pointer to directory name
*-----------------------------------------------------------------------------*/
D4D_CHAR* D4D_FileDialogGetDirName(FILE_DIALOG* pFileDialog)
{
  LWord ix = D4D_ListBoxGetIndex((D4D_OBJECT*)&sfl_lstBx_Dir);
  
  if(!pFileDialog->fileCnt)
    return NULL;
  
  return pFileDialog->pFileList->pDirNames[ix];
}
