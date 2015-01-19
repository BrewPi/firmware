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
* @file      d4d_text_box.c
*
* @author     Petr Gargulak
*
* @version   0.0.29.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver textbox object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"


typedef struct
{
  D4D_TCHAR* pWord;
  D4D_FONT fnt;
  D4D_COOR cur_pos;
  D4D_COOR pixLen;
  D4D_INDEX charCnt;
  D4D_BOOL newLine;
  D4D_TAB* pTab;
  D4D_INDEX offset;
}D4D_TXTBX_WORD_DESC;

// Internal API
void D4D_TextBoxOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_TextBoxGetTextBuffer(D4D_OBJECT* pThis);



static D4D_BOOL D4D_TextBoxScrollBarSetup(D4D_OBJECT* pObject);
static D4D_INDEX D4D_TextBoxGetMaxLineChars(D4D_TCHAR* pText, D4D_INDEX offset, D4D_TAB* pTab, D4D_COOR len, D4D_FONT fnt);
static D4D_BOOL D4D_TextBoxGetWordSpec(D4D_TXTBX_WORD_DESC* wordDesc);
static D4D_BOOL D4D_TextBoxGetCharsToLine(D4D_TXTBX_WORD_DESC* wordDesc);
static D4D_INDEX D4D_TextBoxGetLineCount(D4D_OBJECT* pThis);
static D4D_INDEX D4D_TextBoxFindLineStartChar(D4D_OBJECT* pThis, D4D_INDEX line);
static void D4D_TextBoxOnInit(D4D_OBJECT* pObject);


const D4D_OBJECT_SYS_FUNCTION d4d_textboxSysFunc =
{
  D4D_DEFSTR("Text Box"),
  D4D_TextBoxOnMessage,
  NULL,
  NULL
};

typedef struct
{
  D4D_POINT position;
  D4D_GEOMETRY contentGeom;
  D4D_GEOMETRY txtGeom;
  D4D_INDEX lineCount;
} D4D_TXTBX_TMP_VAL;

#define _calc (*((D4D_TXTBX_TMP_VAL*)d4d_scratchPad))

static void D4D_TextBoxValue2Coor(D4D_OBJECT* pThis)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  D4D_INDEX tmpIx;

  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);

  _calc.txtGeom = _calc.contentGeom;

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  _calc.txtGeom.pnt.y += (D4D_COOR)(pThis->radius / 2);
  _calc.txtGeom.pnt.x += (D4D_COOR)(pThis->radius / 2);
#endif

  if(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
    _calc.txtGeom.sz.cx -= D4D_TXTBX_SCRLBR_WIDTH;

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  _calc.txtGeom.sz.cx -= pThis->radius;
  _calc.txtGeom.sz.cy -= pThis->radius;
#endif

  // Compute the lines count in object
  _calc.lineCount = (D4D_COOR) (_calc.txtGeom.sz.cy / D4D_GetFontHeight(pTextBox->textFontId));

  tmpIx = D4D_TextBoxGetLineCount(pThis);

  if(_calc.lineCount > tmpIx)
    _calc.lineCount = tmpIx;

}

static D4D_BOOL D4D_TextBoxGetWordSpecInt(D4D_TCHAR ch, D4D_TXTBX_WORD_DESC* wordDesc)
{
  D4D_COOR tab;

  // normal char or any special char
  if(ch <= ' ')
  {
    // special char

    if(wordDesc->charCnt) // if any chars are decoded (Word loaded) finish the function
      return D4D_FALSE;

    switch(ch)
    {
      case ' ': // space
        wordDesc->pixLen = D4D_GetCharWidth(wordDesc->fnt, ' ');
        break;

      case '\n': // new line
        wordDesc->newLine = D4D_TRUE;
        break;

      case '\t': // Tab
        tab = D4D_GetNextTab(wordDesc->pTab, wordDesc->cur_pos);

        if(tab)
          wordDesc->pixLen = (D4D_COOR)(tab - wordDesc->cur_pos);
        else
          wordDesc->pixLen = D4D_GetCharWidth(wordDesc->fnt, '\t');
        break;

      default:
        // unsupported char
        break;
    }

    wordDesc->charCnt++; // increament the one processed char
    return D4D_FALSE;

  }else
  {
    // normal char
    wordDesc->pixLen += D4D_GetCharWidth(wordDesc->fnt, ch);
  }
  return D4D_TRUE;

}



static D4D_BOOL D4D_TextBoxGetWordSpec(D4D_TXTBX_WORD_DESC* wordDesc)
{
  D4D_TCHAR* pTmpWord = wordDesc->pWord;
  D4D_TCHAR* pTextInt;
  wordDesc->newLine = D4D_FALSE;
  wordDesc->pixLen = 0;
  wordDesc->charCnt = 0;

  if(pTmpWord == NULL)
    return D4D_TRUE;

  pTextInt = D4D_GetInternalStringPointer(pTmpWord);

#if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE

  if(pTextInt == NULL)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = wordDesc->offset;
    D4D_INDEX charCntTmp;

    (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pTmpWord, (D4D_TCHAR*)d4d_extsrcBuffer, wordDesc->offset, 1);

    if(!((D4D_TCHAR*)d4d_extsrcBuffer)[0])
      return D4D_TRUE;

    do
    {
      charOff += charCnt;

      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pTmpWord, (D4D_TCHAR*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);


      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if(D4D_TextBoxGetWordSpecInt(((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp], wordDesc))
        {

          wordDesc->charCnt++; // increament the one processed char
        }else
          return D4D_FALSE;
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
  }
  else
  #endif

  {
    pTextInt += wordDesc->offset;

    if(! *pTextInt)
      return D4D_TRUE;

    while(*pTextInt) // check if the tested strings isn't on end
    {
      if(D4D_TextBoxGetWordSpecInt(*pTextInt, wordDesc))
      {

        wordDesc->charCnt++; // increament the one processed char
        pTextInt++; // increment WORD pointer to next char
      }else
        return D4D_FALSE;
    }
  }

  return D4D_FALSE;

}


static D4D_BOOL D4D_TextBoxGetCharsToLine(D4D_TXTBX_WORD_DESC* wordDesc)
{
  D4D_TCHAR* pTmpWord = wordDesc->pWord;
  D4D_COOR len, tmp_len;

  len = wordDesc->pixLen;
  wordDesc->pixLen = 0;
  wordDesc->newLine = D4D_FALSE;
  wordDesc->charCnt = 0;

  if(pTmpWord == NULL)
    return D4D_FALSE;

  pTmpWord = D4D_GetInternalStringPointer(pTmpWord);

  #if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE

  if(pTmpWord == NULL)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = wordDesc->offset;
    D4D_INDEX charCntTmp;


    do
    {
      charOff += charCnt;

      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pTmpWord, (D4D_TCHAR*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);


      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        tmp_len = D4D_GetCharWidth(wordDesc->fnt, ((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp]);

          // normal char
        if((wordDesc->pixLen + tmp_len) > len)
          return D4D_TRUE;

        wordDesc->pixLen += tmp_len;
        wordDesc->charCnt++; // increament the one processed char
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
  }
  else
  #endif

  {
    pTmpWord += wordDesc->offset;

    while(*pTmpWord) // check if the tested strings isn't on end
    {
      tmp_len = D4D_GetCharWidth(wordDesc->fnt, *pTmpWord);

        // normal char
      if((wordDesc->pixLen + tmp_len) > len)
        return D4D_TRUE;

      wordDesc->pixLen += tmp_len;
      wordDesc->charCnt++; // increament the one processed char
      pTmpWord++; // increment WORD pointer to next char
    }
  }

  return D4D_TRUE;
}




static D4D_INDEX D4D_TextBoxGetMaxLineChars(D4D_TCHAR* pText, D4D_INDEX offset, D4D_TAB* pTab, D4D_COOR len, D4D_FONT fnt)
{
  D4D_TXTBX_WORD_DESC tmp_wordDesc;
  D4D_COOR tmp_len = 0;
  D4D_INDEX res_charCnt = 0;

  tmp_wordDesc.fnt = fnt;
  tmp_wordDesc.pWord = pText;
  tmp_wordDesc.pTab = pTab;
  tmp_wordDesc.cur_pos = 0;
  tmp_wordDesc.offset = offset;

  while(!D4D_TextBoxGetWordSpec(&tmp_wordDesc))
  {
    if((tmp_len + tmp_wordDesc.pixLen) > len)
    {
      if(tmp_len == 0)
      {
        // very long word - longer than one line
        tmp_wordDesc.pixLen = len;
        (void)D4D_TextBoxGetCharsToLine(&tmp_wordDesc);
      }else
        return res_charCnt;
    }


    tmp_len += tmp_wordDesc.pixLen;
    res_charCnt += tmp_wordDesc.charCnt;

    if(tmp_wordDesc.newLine)
      return res_charCnt;

    //tmp_wordDesc.pWord = &pText[res_charCnt];
    tmp_wordDesc.offset += tmp_wordDesc.charCnt;
    tmp_wordDesc.cur_pos = tmp_len;
  }

  return (D4D_INDEX)(tmp_wordDesc.charCnt + res_charCnt);
}

static D4D_INDEX D4D_TextBoxGetLineCount(D4D_OBJECT* pThis)
{
  // The function counts with filled _calc variable
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  D4D_TCHAR* pText = pTextBox->pData->pTxtArr;
  D4D_INDEX lineCharCnt;
  D4D_INDEX textIx = 0;
  D4D_INDEX lineCnt = 0;
  D4D_TAB tmp_strTab;
  D4D_TAB* pTab = NULL;
  // Try to check if the Text Box needs scroll Bar

  if(pTextBox->pTabTable)
  {
    tmp_strTab.pTabTable = pTextBox->pTabTable;
    tmp_strTab.tabOffset = (D4D_COOR)(_calc.contentGeom.pnt.x - _calc.position.x);
    pTab = &tmp_strTab;
  }

  do
  {
    lineCharCnt = D4D_TextBoxGetMaxLineChars(pText, textIx, pTab, _calc.txtGeom.sz.cx, pTextBox->textFontId);
    textIx += lineCharCnt;

    if(lineCharCnt)
      lineCnt++;

  }while(lineCharCnt);

  return lineCnt;
}


static D4D_INDEX D4D_TextBoxFindLineStartChar(D4D_OBJECT* pThis, D4D_INDEX line)
{
  // The function counts with filled _calc variablez;
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  D4D_TCHAR* pText = pTextBox->pData->pTxtArr;
  D4D_INDEX lineCharCnt;
  D4D_INDEX textIx = 0;
  D4D_INDEX lineCnt = 0;
  D4D_TAB tmp_strTab;
  D4D_TAB* pTab = NULL;

  D4D_TextBoxValue2Coor(pThis);

  if(pTextBox->pTabTable)
  {
    tmp_strTab.pTabTable = pTextBox->pTabTable;
    tmp_strTab.tabOffset = (D4D_COOR)(_calc.txtGeom.pnt.x - _calc.position.x);
    pTab = &tmp_strTab;
  }

  do
  {
    if(line == lineCnt)
      return textIx;

    lineCharCnt = D4D_TextBoxGetMaxLineChars(pText, textIx, pTab, _calc.txtGeom.sz.cx, pTextBox->textFontId);

    textIx += lineCharCnt;

    if(lineCharCnt)
      lineCnt++;

  }while(lineCharCnt);

  return 0;
}



static D4D_BOOL D4D_TextBoxScrollBarSetup(D4D_OBJECT* pObject)
{

  // Try to check if the Text Box needs scroll Bar

  // Switch off possible scroll bar in refresh case
  D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pObject)->pData->flags  &= ~D4D_OBJECT_F_VISIBLE;

  D4D_TextBoxValue2Coor(pObject);

  if(D4D_TextBoxGetLineCount(pObject) > _calc.lineCount)
  {
    D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pObject)->pData->flags |= D4D_OBJECT_F_VISIBLE;

    D4D_TextBoxValue2Coor(pObject);

    D4D_ScrlBrSetRange(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pObject), 0, D4D_TextBoxGetLineCount(pObject));
    D4D_ScrlBrSetStep(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pObject), _calc.lineCount, D4D_TXTBX_SCRLBR_STEP);
    D4D_ScrlBrSetPosition(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pObject), 0);
    return D4D_TRUE;
  }
  return D4D_FALSE;
}


/*******************************************************
*
* TEXTBOX Drawing routine
*
*******************************************************/

static void D4D_TextBoxOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  D4D_TXTBX_DATA* pData = pTextBox->pData;
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_INDEX line_cnt;
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_strPrties;
  D4D_TAB tmp_strTab;
  D4D_TAB* pTab = NULL;

  D4D_TextBoxValue2Coor(pThis);

  clrT = D4D_ObjectGetForeFillColor(pThis);
  clrB = D4D_ObjectGetBckgFillColor(pThis);

  // draw the rectangle around the text
  if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
  {

    D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);
    pData->redrawText = D4D_TRUE;

  }

  // Draw the frame
  if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
    D4D_DrawFrame(pThis, clrT, clrB);


  if(pData->redrawText)
  {
    D4D_INDEX tmp_index = pData->firstShowedCharIx;

    pData->redrawText = D4D_FALSE;

    tmp_txtbuff.fontId = pTextBox->textFontId;
    tmp_txtbuff.str_properties = &tmp_strPrties;

    tmp_strPrties.font_properties = 0;
    tmp_strPrties.text_properties = 0;

    // Draw all lines of textbox
    tmp_point = _calc.txtGeom.pnt;
    tmp_size.cx = _calc.txtGeom.sz.cx;
    tmp_size.cy = D4D_GetFontHeight(pTextBox->textFontId);

    tmp_txtbuff.pText = pData->pTxtArr;
    tmp_txtbuff.buffSize = 0;

    if(pTextBox->pTabTable)
    {
      tmp_strTab.pTabTable = pTextBox->pTabTable;
      tmp_strTab.tabOffset = (D4D_COOR)(_calc.txtGeom.pnt.x - _calc.position.x);
      pTab = &tmp_strTab;
    }

    for(line_cnt = 0; line_cnt < _calc.lineCount; line_cnt++)
    {
      tmp_txtbuff.printLen = D4D_TextBoxGetMaxLineChars(tmp_txtbuff.pText, tmp_index, pTab, tmp_size.cx, tmp_txtbuff.fontId);
      tmp_txtbuff.printOff = tmp_index;
      tmp_index +=  tmp_txtbuff.printLen;

      D4D_DrawTextRectTab(&tmp_point, &tmp_size, &tmp_txtbuff, pTab, clrT, clrB);

      tmp_point.y += tmp_size.cy;
    }
  }
}

static void D4D_TextBoxOnInit(D4D_OBJECT* pObject)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pObject);

  pTextBox->pData->firstShowedCharIx = 0;

  (void)D4D_TextBoxScrollBarSetup(pObject);

  // set flag to redraw screen
  D4D_InvalidateObject(pObject, D4D_TRUE);
}

/**************************************************************//*!
*
*
*
*     THE INTERNAL HELP FUNCTIONS
*
*
*
******************************************************************/

void D4D_TextBoxScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_TXTBX_DATA* pData = D4D_GET_TEXTBOX_DATA(D4D_GetParentObject(pThis));

  D4D_UNUSED(old_position);

  // compute the start char of current screen
  pData->firstShowedCharIx = D4D_TextBoxFindLineStartChar(D4D_GetParentObject(pThis), new_position);

  // set flag to redraw screen
  D4D_InvalidateObject(D4D_GetParentObject(pThis), D4D_FALSE);
  pData->redrawText = D4D_TRUE;
}

/**************************************************************//*!
*
*
*
*     THE EXTERNAL API FUNCTIONS
*
*
*
******************************************************************/

/******************************************************************************
* Begin of D4D_TEXT_BOX public functions
*//*! @addtogroup doxd4d_text_box_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function reinitialize whole object
* @param   pObject - pointer to the text box object
* @return  none.
* @note    The function returns back the object to default state
*******************************************************************************/
void D4D_TextBoxRefreshAll(D4D_OBJECT* pObject)
{
  if(pObject == NULL)
    return;

  D4D_TextBoxOnInit(pObject);
}

/**************************************************************************/ /*!
* @brief   The function change the text of the text box
* @param   pObject - pointer to the text box object
* @param   pText - pointer to the text array that should be newly showed
* @return  none
* @note    none
*******************************************************************************/
void D4D_TextBoxChangeText(D4D_OBJECT* pObject, D4D_TCHAR* pText)
{
  D4D_TXTBX_DATA* pData = D4D_GET_TEXTBOX_DATA(pObject);

  // check the intput pointers
  if(pObject == NULL)
    return;

  if(pText == NULL)
    return;

  // change the text pointer
  pData->pTxtArr = pText;

  // refresh whole object
  D4D_TextBoxOnInit(pObject);

}
/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_text_box_func                                              */
/******************************************************************************/

/**************************************************************//*!
*
*
*
*     THE INTERNAL API FUNCTIONS
*
*
*
******************************************************************/

/*******************************************************
*
* The TEXTBOX message handler
*
*******************************************************/

void D4D_TextBoxOnMessage(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;

  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_TextBoxOnDraw(pMsg);
      break;

    case D4D_MSG_ONINIT:
      pThis->pData->flags &= ~D4D_OBJECT_F_NOTINIT;

      D4D_TextBoxOnInit(pThis);
      break;

#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
      D4D_FocusSet(pMsg->pScreen, pThis);
      D4D_CaptureKeys(pThis);
      break;
#endif

#ifdef D4D_LLD_MOUSE
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
      D4D_FocusSet(pMsg->pScreen, pThis);
      D4D_CaptureKeys(pThis);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_UP:
      if(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
        D4D_ScrlBrChangePosition(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pThis), -1);
      break;
    case D4D_MSG_MOUSE_BTN_WHEEL_DOWN:
      if(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)
        D4D_ScrlBrChangePosition(D4D_GET_TEXTBOX_SCROLL_BAR_HOR(pThis), 1);
      break;
#endif

    case D4D_MSG_KEYUP:
      {
        D4D_KEY_SCANCODE tmp_key = pMsg->prm.key;
        // capture the keyboard if enter is pressed
        if(tmp_key == D4D_KEY_SCANCODE_ENTER)
          D4D_CaptureKeys(pThis);

        // exit capture
        if(tmp_key == D4D_KEY_SCANCODE_ESC)
          D4D_CaptureKeys(NULL);
      }
      break;

    case D4D_MSG_KILLFOCUS:
      D4D_CaptureKeys(NULL);
    case D4D_MSG_SETFOCUS:
    case D4D_MSG_SETCAPTURE:
    case D4D_MSG_KILLCAPTURE:
      //D4D_InvalidateObject(pThis, D4D_FALSE);
      break;


    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}
