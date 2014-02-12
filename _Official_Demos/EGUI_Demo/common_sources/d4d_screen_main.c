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
* @file      d4d_screen_main.c
*
* @author    Petr Gargulak
* 
* @version   0.0.4.0
* 
* @date      Jan-15-2014
* 
* @brief     D4D driver - main screen of application
*
******************************************************************************/


/****************************************************************************
*       Includes header files
****************************************************************************/
#include "d4d.h"
#include "d4d_screen_openfile.h"
#include "fonts.h"
#include "strings.h"
#include "pictures.h"

/****************************************************************************
*       Local type definitions
***************************************************************************/
typedef struct
{
  D4D_CHAR* strFileExt;
  void (*pFileHandler)(D4D_CHAR *pFileName);
}FILE_ASSOCIATION;

/****************************************************************************
*       Local functions declarations
***************************************************************************/

static void SFL_BtnOpenOnClick(D4D_OBJECT* pThis);
static void SFL_BtnSettingsOnClick(D4D_OBJECT* pThis);
static void RadioButnTxtType_OnChange(D4D_OBJECT* pThis);
static void ScrMain_FileHandleUnsupported(D4D_CHAR* pFileName);
static void ScrMain_FileHandlePicture(D4D_CHAR* pFileName);
static void ScrMain_FileHandleText(D4D_CHAR* pFileName);


/****************************************************************************
*       Local variables declarations
***************************************************************************/

static FILE_DIALOG* pFileDialog = NULL;

const static FILE_ASSOCIATION fileAssociationTable[] = 
{
  {"BMP",       ScrMain_FileHandlePicture},
  {"D4L",       ScrMain_FileHandlePicture},
  {"TXT",       ScrMain_FileHandleText},
  {"C",         ScrMain_FileHandleText},
  {"CPP",       ScrMain_FileHandleText},
  {"H",         ScrMain_FileHandleText},
  {"HPP",       ScrMain_FileHandleText},
  {"ICF",       ScrMain_FileHandleText},
  {"LCF",       ScrMain_FileHandleText},
  {NULL,        NULL}
};

/****************************************************************************
*       D4D widgets (graphic objects) declarations
****************************************************************************/

#define CONTENT_POS_X           D4D_BEVEL_WIDTH
#define CONTENT_POS_Y           D4D_BEVEL_WIDTH

#define CONTENT_SIZE_X          320
#define CONTENT_SIZE_Y          240

#define PANEL_SIZE_X            (D4D_SCREEN_SIZE_LONGER_SIDE - CONTENT_SIZE_X - 2*D4D_BEVEL_WIDTH)
#define PANEL_SIZE_Y            (CONTENT_SIZE_Y + 2*D4D_BEVEL_WIDTH)

#define PANEL_POS_X             (CONTENT_SIZE_X + 2*D4D_BEVEL_WIDTH)
#define PANEL_POS_Y             0

#define BTN_SIZE_X              100
#define BTN_SIZE_Y              24

#define BTN_POS_X(x)            (10 + (x) * (BTN_SIZE_X + 10))
#define BTN_POS_Y               (CONTENT_SIZE_Y + 2 + 2*D4D_BEVEL_WIDTH)

#define PNL_OBJ_SIZE_X          (PANEL_SIZE_X - 8)
#define PNL_OBJ_SIZE_Y          22

#define PNL_OBJ_POS_X           0
#define PNL_OBJ_POS_Y(y)        ((y) * (PNL_OBJ_SIZE_Y + 4))

#define MAIN_RADIUS             6

// My help macro to simplify label declaration for my application purposes
#define D4D_DECLARE_MY_LABEL(name, text, x, y, cx, cy, font, pRelations) \
        _D4D_DECLARE_LABEL(D4D_CONST, name, text, x, y, cx, cy, MAIN_RADIUS, NULL, pRelations, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED), NULL, font, NULL, NULL)

/*******************************************
* Main content objects - not supported file
*******************************************/          
D4D_TCHAR sMain_CntntMsgStr[5] = D4D_STR_DUMMY_LABEL;
D4D_DECLARE_STD_RLABEL(sMain_CntntMsg, sMain_CntntMsgStr, CONTENT_POS_X, CONTENT_POS_Y, CONTENT_SIZE_X, CONTENT_SIZE_Y, MAIN_RADIUS, FONT_SYSTEM)

/*******************************************
* Main content objects - picture file
*******************************************/
// Declare the array of file path in RAM
D4D_CHAR pictureFileName[PATHNAME_SIZE] = "a:\\";
// Declare the the bitmap structure
D4D_DECLARE_BMPFILE(sMain_CntntPict_bmp, pictureFileName)
// Declare the picture object
_D4D_DECLARE_PICTURE(D4D_CONST, sMain_CntntPict, CONTENT_POS_X, CONTENT_POS_Y, CONTENT_SIZE_X, CONTENT_SIZE_Y, MAIN_RADIUS, NULL, NULL, &sMain_CntntPict_bmp, D4D_OBJECT_F_ENABLED, NULL, NULL)

/*******************************************
* Main content objects - text file
*******************************************/
// Declare the tab table
D4D_DECLARE_TAB_TABLE_BEGIN(sMain_CntntTextTabs)
  D4D_DECLARE_TAB(10) // tab space in pixels
  D4D_DECLARE_TAB(20) // tab space in pixels
  D4D_DECLARE_TAB(30) // tab space in pixels
  D4D_DECLARE_TAB(40) // tab space in pixels
  D4D_DECLARE_TAB(50) // tab space in pixels
  D4D_DECLARE_TAB(60) // tab space in pixels
  D4D_DECLARE_TAB(70) // tab space in pixels
  D4D_DECLARE_TAB(80) // tab space in pixels
  D4D_DECLARE_TAB(90) // tab space in pixels
  D4D_DECLARE_TAB(100) // tab space in pixels
D4D_DECLARE_TAB_TABLE_END

//Declare text box object to showing text files
D4D_DECLARE_RTEXTBOX(sMain_CntntText, CONTENT_POS_X, CONTENT_POS_Y, CONTENT_SIZE_X, CONTENT_SIZE_Y, MAIN_RADIUS, NULL, &sMain_CntntTextTabs, (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE), NULL, FONT_SYSTEM_SMALL, NULL, NULL)

/*******************************************
* Information panel objects - not supported file
*******************************************/

// Dummy panel information group box
D4D_DECLARE_STD_GROUP_BOX_BEGIN(sMain_GrpBxMsg, PANEL_POS_X, PANEL_POS_Y, PANEL_SIZE_X, PANEL_SIZE_Y, MAIN_RADIUS, NULL, D4D_STR_INFO, FONT_SYSTEM_SMALL)

D4D_DECLARE_GROUP_BOX_OBJECT_END


/*******************************************
* Information panel objects - picture file
*******************************************/
// Objects relations structure declaration for picture files information panel
D4D_EXTERN_OBJECT(sMain_GrpBxPict)
D4D_DECLARE_OBJECT_RELATIONS_BEGIN(grpBxPic_Rel, &sMain_GrpBxPict)
D4D_DECLARE_OBJECT_RELATIONS_END

// Declaration of label object of string "Type"
D4D_DECLARE_MY_LABEL(grpBx_LblTypeName, D4D_STR_TYPE, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(0), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Type text Name

// Declaration of label object of string type value
D4D_TCHAR grpBx_LblTypeStr[8] = D4D_DEFSTR("None");
D4D_DECLARE_MY_LABEL(grpBx_LblType, grpBx_LblTypeStr, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(1), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Type text

// Declaration of label object of string "Width"
D4D_DECLARE_MY_LABEL(grpBx_LblWidthName, D4D_STR_WIDTH, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(2), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Width text Name

// Declaration of label object of string width value
D4D_TCHAR grpBx_LblWidthStr[10] = D4D_DEFSTR("0px");
D4D_DECLARE_MY_LABEL(grpBx_LblWidth, grpBx_LblWidthStr, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(3), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Width text

// Declaration of label object of string "Height"
D4D_DECLARE_MY_LABEL(grpBx_LblHeightName, D4D_STR_HEIGHT, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(4), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Height text Name

// Declaration of label object of string height value
D4D_TCHAR grpBx_LblHeightStr[10] = D4D_DEFSTR("0px");
D4D_DECLARE_MY_LABEL(grpBx_LblHeight, grpBx_LblHeightStr, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(5), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Height text

// Declaration of label object of string "Size"
D4D_DECLARE_MY_LABEL(grpBx_LblSizeName, D4D_STR_SIZE, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(6), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Size text Name

// Declaration of label object of string size value
D4D_TCHAR grpBx_LblSizeStr[10] = D4D_DEFSTR("0B");
D4D_DECLARE_MY_LABEL(grpBx_LblSize, grpBx_LblSizeStr, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(7), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxPic_Rel) // Size text

// Group Box Bitmap panel information
D4D_DECLARE_GROUP_BOX_BEGIN(sMain_GrpBxPict, PANEL_POS_X, PANEL_POS_Y, PANEL_SIZE_X, PANEL_SIZE_Y, MAIN_RADIUS, NULL, &groupBox_marginDefault, D4D_STR_IMAGE_INFO, FONT_SYSTEM_SMALL, (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_BEVEL_RAISED | D4D_GROUP_BOX_F_CONTENT_OUTLINE), NULL, NULL, NULL)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblTypeName)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblType)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblWidthName)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblWidth)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblHeightName)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblHeight)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblSizeName)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBx_LblSize)    
D4D_DECLARE_GROUP_BOX_OBJECT_END

/*******************************************
* Information panel objects - text file
*******************************************/
// Objects relations structure declaration for text files information panel
D4D_EXTERN_OBJECT(sMain_GrpBxText)
D4D_DECLARE_OBJECT_RELATIONS_BEGIN(grpBxTxtBx_Rel, &sMain_GrpBxText)
D4D_DECLARE_OBJECT_RELATIONS_END

// Declaration of label object of string "Type"
D4D_DECLARE_MY_LABEL(grpBxT_LblTypeName, D4D_STR_TYPE, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(0), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxTxtBx_Rel) // Type text Name

// Declaration of label object of string type value
D4D_TCHAR grpBxT_LblTypeStr[8] = D4D_DEFSTR("None");
D4D_DECLARE_MY_LABEL(grpBxT_LblType, grpBxT_LblTypeStr, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(1), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxTxtBx_Rel) // Type text

// Declaration of label object of string "Size"
D4D_DECLARE_MY_LABEL(grpBxT_LblSizeName, D4D_STR_SIZE, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(2), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxTxtBx_Rel) // Size text Name

// Declaration of label object of string size value
D4D_TCHAR grpBxT_LblSizeStr[10] = D4D_DEFSTR("0B");
D4D_DECLARE_MY_LABEL(grpBxT_LblSize, grpBxT_LblSizeStr, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(3), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, FONT_SYSTEM, grpBxTxtBx_Rel) // Size text

// Declaration of radio buttons ASCII/UNICODE
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxT_RbAscii, D4D_STR_ASCII, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(4), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, MAIN_RADIUS, NULL, grpBxTxtBx_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, NULL, RadioButnTxtType_OnChange, NULL)
_D4D_DECLARE_RADIOBUTTON(D4D_CONST, grpBxT_RbUnicode, D4D_STR_UNICODE, PNL_OBJ_POS_X, PNL_OBJ_POS_Y(5), PNL_OBJ_SIZE_X, PNL_OBJ_SIZE_Y, MAIN_RADIUS, NULL, grpBxTxtBx_Rel, D4D_RADIOBUTTON_F_DEFAULT, NULL, NULL, NULL, FONT_SYSTEM, NULL, NULL, NULL)

// Declaration of Text panel information group box
D4D_DECLARE_GROUP_BOX_BEGIN(sMain_GrpBxText, PANEL_POS_X, PANEL_POS_Y, PANEL_SIZE_X, PANEL_SIZE_Y, MAIN_RADIUS, NULL, &groupBox_marginDefault, D4D_STR_TEXT_INFO, FONT_SYSTEM_SMALL, (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_BEVEL_RAISED | D4D_OBJECT_F_MOUSE_NORMAL | D4D_GROUP_BOX_F_CONTENT_OUTLINE), NULL, NULL, NULL)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxT_LblTypeName)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxT_LblType)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxT_LblSizeName)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxT_LblSize)  
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxT_RbAscii)
  D4D_DECLARE_GROUP_BOX_OBJECT(grpBxT_RbUnicode)
D4D_DECLARE_GROUP_BOX_OBJECT_END

/*******************************************
* Control buttons objects
*******************************************/
D4D_DECLARE_TXT_RBUTTON(sMain_btnOpenFie, D4D_STR_OPEN, BTN_POS_X(0), BTN_POS_Y, BTN_SIZE_X, BTN_SIZE_Y, MAIN_RADIUS, FONT_SYSTEM, SFL_BtnOpenOnClick)


D4D_DECLARE_TXT_RBUTTON(sMain_btnSettings, D4D_STR_SYSTEM, BTN_POS_X(1), BTN_POS_Y, BTN_SIZE_X, BTN_SIZE_Y, MAIN_RADIUS, FONT_SYSTEM, SFL_BtnSettingsOnClick)

D4D_DECLARE_STD_PICTURE(pict_logo, BTN_POS_X(2), BTN_POS_Y, 240, 24, &bmp_logo)

D4D_EXTERN_SCREEN(screen_settings);

/****************************************************************************
*       D4D screen declarations
****************************************************************************/
D4D_DECLARE_STD_SCREEN_BEGIN(screen_main, ScreenMain_)  
  D4D_DECLARE_SCREEN_OBJECT(sMain_CntntMsg)
  D4D_DECLARE_SCREEN_OBJECT(sMain_GrpBxMsg)
  D4D_DECLARE_SCREEN_OBJECT(sMain_CntntPict)
  D4D_DECLARE_SCREEN_OBJECT(sMain_GrpBxPict)
  D4D_DECLARE_SCREEN_OBJECT(sMain_CntntText)
  D4D_DECLARE_SCREEN_OBJECT(sMain_GrpBxText)
  D4D_DECLARE_SCREEN_OBJECT(sMain_btnOpenFie)    
  D4D_DECLARE_SCREEN_OBJECT(sMain_btnSettings)
  D4D_DECLARE_SCREEN_OBJECT(pict_logo)
D4D_DECLARE_SCREEN_END()    

/****************************************************************************
*       D4D widgets static events functions definitions
****************************************************************************/
// On Click Event of "OPEN" button
static void SFL_BtnOpenOnClick(D4D_OBJECT* pThis)
{
  D4D_FileDialogShow(pFileDialog);
}

// On Click Event of "SETTINGS" button
static void SFL_BtnSettingsOnClick(D4D_OBJECT* pThis)
{
  D4D_ActivateScreen(&screen_settings, D4D_FALSE);
}

// On Change Event of "ASCII / UNICODE"  radio buttons
static void RadioButnTxtType_OnChange(D4D_OBJECT* pThis)
{
  D4D_CHAR fileName[PATHNAME_SIZE];
  
  D4D_StrCopy(fileName, FileList_GetCurrentPath(pFileDialog->pFileList));
  D4D_StrCat(fileName, D4D_FileDialogGetFileName(pFileDialog));
  
  // It's called when the ASCII radio button is changed
  if(D4D_IsVisible((D4D_OBJECT*)&sMain_CntntText))
    ScrMain_FileHandleText(fileName);
}

/*-----------------------------------------------------------------------------
* FUNCTION:    FileDialogEnds
* SCOPE:       Main screen related function
* DESCRIPTION: Call Back Function called by open file dialog
*
* PARAMETERS:  pThis - pointer of current open file dialog
*              result - result of open file dialog  
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void FileDialogEnds(FILE_DIALOG* pThis, FILE_DIALOG_RESULT result)
{
  static D4D_CHAR fileName[PATHNAME_SIZE];
  D4D_CHAR* pExt;
  LWord i; 
  
  // Only for success continue
  if(result != FD_OK)
    return;
  
  // Create the full path name
  D4D_StrCopy(fileName, FileList_GetCurrentPath(pThis->pFileList));
  D4D_StrCat(fileName, D4D_FileDialogGetFileName(pThis));
  
  // Get the file extension
  pExt = D4D_GetFileExtension(fileName);
  
  // Just switch off redraw of last component
  sMain_CntntMsg.pData->flags &= ~D4D_OBJECT_F_VISIBLE;
  sMain_CntntPict.pData->flags &= ~D4D_OBJECT_F_VISIBLE;
  sMain_CntntText.pData->flags &= ~D4D_OBJECT_F_VISIBLE;
  
  sMain_GrpBxMsg.pData->flags &= ~D4D_OBJECT_F_VISIBLE;
  sMain_GrpBxPict.pData->flags &= ~D4D_OBJECT_F_VISIBLE;
  sMain_GrpBxText.pData->flags &= ~D4D_OBJECT_F_VISIBLE;
    
  i = 0;
  
  // Look to association table for right file handler
  while(fileAssociationTable[i].strFileExt && fileAssociationTable[i].pFileHandler)
  {
    if(!D4D_CompareStrings(pExt, fileAssociationTable[i].strFileExt))
    {
      // Run the file content open Handler
      fileAssociationTable[i].pFileHandler(fileName);
      break;
    }
    i++;
  }
  
  // if there is no any usable file handler just show unsupported type
  if(fileAssociationTable[i].strFileExt == NULL)
  {
    // Not Supported File type
    // Show the screen that this file is not supported.
      ScrMain_FileHandleUnsupported(fileName);
  } 
}


/****************************************************************************
*       D4D standard screen functions definitions
****************************************************************************/

// One time called screen function in screen initialization proces
static void ScreenMain_OnInit()
{
  // Create the File dialog
  pFileDialog = D4D_FileDialogCreate("A:\\", FileDialogEnds);
  
  // Initialized some one time properties of some objects
  D4D_SetFontProperties(&grpBx_LblTypeName, D4D_FNT_PRTY_UNDERLINE_LINE_MASK);
  D4D_SetFontProperties(&grpBx_LblWidthName, D4D_FNT_PRTY_UNDERLINE_LINE_MASK);
  D4D_SetFontProperties(&grpBx_LblHeightName, D4D_FNT_PRTY_UNDERLINE_LINE_MASK);
  D4D_SetFontProperties(&grpBx_LblSizeName, D4D_FNT_PRTY_UNDERLINE_LINE_MASK);
  D4D_SetFontProperties(&grpBxT_LblTypeName, D4D_FNT_PRTY_UNDERLINE_LINE_MASK);
  D4D_SetFontProperties(&grpBxT_LblSizeName, D4D_FNT_PRTY_UNDERLINE_LINE_MASK);
  D4D_RadioButtonSetValue(&grpBxT_RbAscii, D4D_TRUE);
}

// Screen on Activate function called with each screen activation
static void ScreenMain_OnActivate()
{

}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenMain_OnMain()
{

}


// Screen on DeActivate function called with each screen deactivation
static void ScreenMain_OnDeactivate()
{
    
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenMain_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  if((pMsg->pObject == NULL) && (pMsg->nMsgId == D4D_MSG_DRAWDONE))
  {
    // When the screen content is drawed - just add one extra bevel around the content area
  
    D4D_POINT pnt = {0,0};
    D4D_SIZE sz = {CONTENT_SIZE_X + 2*D4D_BEVEL_WIDTH ,CONTENT_SIZE_Y + 2*D4D_BEVEL_WIDTH};
    D4D_Bevel(&pnt, &sz, D4D_ScreenGetScheme((D4D_SCREEN*)&screen_main)->bckg, D4D_BEVEL_RAISED, MAIN_RADIUS);
  }
  return 0;
}


/*-----------------------------------------------------------------------------
* FUNCTION:    ScrMain_FileHandleUnsupported
* SCOPE:       Main screen related function
* DESCRIPTION: Function shows that the selected file is unssuported 
*
* PARAMETERS:  pFileName - pointer to file name
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void ScrMain_FileHandleUnsupported(D4D_CHAR* pFileName)
{
  // Show dummy panels
  D4D_SetText(&sMain_CntntMsg, D4D_STR_COULD_NOT_SHOW);
  D4D_ShowObject(&sMain_CntntMsg, D4D_TRUE);
  D4D_ShowObject((D4D_OBJECT*)&sMain_GrpBxMsg, D4D_TRUE);
  D4D_InvalidateObject((D4D_OBJECT*)&sMain_GrpBxMsg, D4D_TRUE);
}

/*-----------------------------------------------------------------------------
* FUNCTION:    ScrMain_FileHandlePicture
* SCOPE:       Main screen related function
* DESCRIPTION: Function load the picture file to D4D_PICTURE and set up the side information panel 
*
* PARAMETERS:  pFileName - pointer to file name
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void ScrMain_FileHandlePicture(D4D_CHAR* pFileName)
{
  D4D_BMP tmp_bmp;
  D4D_SIZE tmp_size;
  D4D_CHAR tmp_str[20];  
  D4D_WCHAR tmp_wstr[20];
  D4D_FILEPTR filePtr;
  
  // Update the d4d bitmap structure
  tmp_bmp.pData = NULL;
  tmp_bmp.pParam = pFileName;
  
  // Fill up the information panel
  tmp_size = D4D_GetBmpSize(&tmp_bmp);
  
  D4D_ShowObject(&sMain_GrpBxPict, D4D_TRUE);
  
  // Update file type label
  D4D_StrCopyAscii2Unicode(tmp_wstr, D4D_GetFileExtension(pFileName));
  D4D_SetText(&grpBx_LblType, tmp_wstr);
  
  // Update file width label
  snprintf(tmp_str, 10, "%dpx", tmp_size.cx);
  D4D_StrCopyAscii2Unicode(tmp_wstr, tmp_str);
  D4D_SetText(&grpBx_LblWidth, tmp_wstr);
  
  // Update file height label
  snprintf(tmp_str, 10, "%dpx", tmp_size.cy);
  D4D_StrCopyAscii2Unicode(tmp_wstr, tmp_str);
  D4D_SetText(&grpBx_LblHeight, tmp_wstr);
  
  // Get the size of file
  filePtr = D4D_FileOpen(pFileName, "r");
  
  if(filePtr)
  {
    LWord size;
    
    D4D_FileSeek(filePtr, 0, D4D_SEEK_END);    
    size = D4D_FileTell(filePtr);
    D4D_FileClose(filePtr);
    
    if(size < 1024)
      snprintf(tmp_str, 20, "%dB", size);
    else if(size < 1024*1024)
      snprintf(tmp_str, 20, "%dKB", size / 1024);
    else
      snprintf(tmp_str, 20, "%dMB", size / (1024 * 1024));
    // Update file size label  
    D4D_StrCopyAscii2Unicode(tmp_wstr, tmp_str);
    D4D_SetText(&grpBx_LblSize, tmp_wstr);
  }else
    D4D_SetText(&grpBx_LblSize, D4D_DEFSTR("0B"));
  
  // Show the picture
  if(tmp_size.cx > CONTENT_SIZE_X || tmp_size.cy > CONTENT_SIZE_Y)
  {
    // So big picture - we not able to show it in our content area
    D4D_SetText(&sMain_CntntMsg, D4D_STR_COULD_NOT_SHOW);
    D4D_ShowObject(&sMain_CntntMsg, D4D_TRUE);
  }else
  {
    D4D_StrCopy(pictureFileName, pFileName);
    D4D_ShowObject(&sMain_CntntPict, D4D_TRUE);
    D4D_InvalidateObject(&sMain_CntntPict, D4D_TRUE);    
  }
}

/*-----------------------------------------------------------------------------
* FUNCTION:    ScrMain_FileHandleText
* SCOPE:       Main screen related function
* DESCRIPTION: Function load the text file to D4D_TEXT_BOX and set up the side information panel 
*
* PARAMETERS:  pFileName - pointer to file name
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void ScrMain_FileHandleText(D4D_CHAR* pFileName)
{
  static D4D_TCHAR* pText = NULL;
  D4D_FILEPTR filePtr;
  D4D_CHAR tmp_str[20];  
  D4D_WCHAR tmp_wstr[20];
  LWord size = 0;
  // Get the size of file
  filePtr = D4D_FileOpen(pFileName, "r");
  
  // Get the file size
  if(filePtr)
  {
    D4D_FileSeek(filePtr, 0, D4D_SEEK_END);    
    size = D4D_FileTell(filePtr);
    D4D_FileClose(filePtr);  
  }
  
  // Change the file type label
  D4D_StrCopyAscii2Unicode(tmp_wstr, D4D_GetFileExtension(pFileName));
  D4D_SetText(&grpBxT_LblType, tmp_wstr);
  
  
  if(size)
  {
    // Change the text
    if(pText)
      D4D_MemFree(pText);
    
    // Show the size of file
    
    if(size < 1024)
      snprintf(tmp_str, 20, "%dB", size);
    else if(size < 1024*1024)
      snprintf(tmp_str, 20, "%dKB", size / 1024);
    else
      snprintf(tmp_str, 20, "%dMB", size / (1024 * 1024));
      
    D4D_StrCopyAscii2Unicode(tmp_wstr, tmp_str);
    D4D_SetText(&grpBxT_LblSize, tmp_wstr);
    
    
    if(size < 100000)
    {    
      Byte array[512];
      LWord i = 0;
      D4D_FILEPTR pFile;
      D4D_TCHAR* pTextTmp;
      
      // Encode the text file to RAM
      if(D4D_RadioButtonGetValue(&grpBxT_RbUnicode))
      {
        pText = D4D_MemAlloc(size + 2); // + termination char
        if(pText == NULL)
          return;
        
        pFile = D4D_FileOpen(pFileName, "r");
        if(pFile == NULL)
          return;
        
        i = D4D_FileRead(pFile, pText, size);
        
        pText[i/2] = 0;

      }else
      {
        pText = D4D_MemAlloc((size * 2) + 2); // + termination char
        if(pText == NULL)
          return;
        
        pFile = D4D_FileOpen(pFileName, "r");
        if(pFile == NULL)
          return;
        
        pTextTmp = pText;
        do
        {
          LWord j;
          i = D4D_FileRead(pFile, array, 512);
          for(j = 0; j<i; j++)
            *(pTextTmp++) = array[j]; 
           
        }while(512 == i);
      }
      *((D4D_TCHAR*)pTextTmp) = 0; // End char
 
      // Show the text box + text information panel
      D4D_TextBoxChangeText((D4D_OBJECT*)&sMain_CntntText, pText);
      D4D_ShowObject((D4D_OBJECT*)&sMain_CntntText, D4D_TRUE);
      D4D_InvalidateObject((D4D_OBJECT*)&sMain_CntntText, D4D_TRUE);
      
      D4D_ShowObject((D4D_OBJECT*)&sMain_GrpBxText, D4D_TRUE);
      D4D_InvalidateObject((D4D_OBJECT*)&sMain_GrpBxText, D4D_TRUE);
      
    }
  }
  
  
}


