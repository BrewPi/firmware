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
* @file      d4d_string.c
*
* @author     Petr Gargulak
*
* @version   0.0.3.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver font c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"

#if D4D_STRINGTABLE_PREFIX

// Current used language
static D4D_INDEX d4d_strTableIndex = 0;

static int D4D_StringToInt(const D4D_TCHAR* pText);

#endif

/******************************************************************************
* Begin of D4D_STRING public functions
*//*! @addtogroup doxd4d_string_func
* @{
*******************************************************************************/

#if D4D_STRINGTABLE_PREFIX

/**************************************************************************/ /*!
* @brief   The function changes the current language
* @param   stringId - the id of new string
* @return  none
* @note    The function change to new string table if it's available.
*******************************************************************************/
void D4D_ChangeStringTable(LWord stringId)
{
  LWord i = 0;

  while(d4d_StringTable[i].stringTable != NULL)
  {
    if(d4d_StringTable[i].stringId == stringId)
    {
      d4d_strTableIndex = i;
      D4D_InvalidateScreen(D4D_GetActiveScreen(), D4D_TRUE);
      return;
    }
    i++;
  }
}

/**************************************************************************/ /*!
* @brief   The function returns the current used string table id
* @param   None
* @return  current used string Id
* @note    None.
*******************************************************************************/
LWord D4D_GetCurrentStringId(void)
{
  return d4d_StringTable[d4d_strTableIndex].stringId;
}
#endif

/**************************************************************************/ /*!
* @brief   The function gets the real internal pointer to string
* @param   originTxt - the defined string in application
* @return  pointer to real string - in case of string table pointer to right table
*          to right place, in case of non string table texts it return s same as input one
* @note    None.
*******************************************************************************/
D4D_TCHAR* D4D_GetInternalStringPointer(const D4D_TCHAR* originTxt)
{
#if D4D_STRINGTABLE_PREFIX
  int tableIx;
#endif
  if(originTxt == NULL)
    return NULL;

#if D4D_STRINGTABLE_PREFIX
  if(*originTxt == D4D_STRINGTABLE_PREFIX)
  {
    if((tableIx = D4D_StringToInt(&originTxt[1])) < 0)
      return NULL;

    if(tableIx > d4d_StringTable[d4d_strTableIndex].stringCount)
      return NULL;

    return d4d_StringTable[d4d_strTableIndex].stringTable[tableIx];
  }
#endif

  return (D4D_TCHAR*)originTxt;
}

/**************************************************************************/ /*!
* @brief   The function convert decimal unsigned 32 bit number to string
* @param   val - the input numerical value
* @param   pText - pointer to output text buffer
* @param   fill - the fill character for non number characters (before number)
* @return  count of printed chars
* @note    If fill parameter is 0 {'\0'}, only the numerical characters are printed.
*******************************************************************************/
Byte D4D_SprintDecU32(LWord val, D4D_TCHAR *pText, D4D_TCHAR fill)
{
  LWord value;
  LWord divider;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  Byte tmp_cnt;
  value = val;
  divider =  1000000000;

  for(tmp_cnt = 0; tmp_cnt < 9; tmp_cnt++)
  {
    if((value > divider - 1) || (cnt > 1))
    {
      if(pText != NULL)
        *pText++ = (D4D_TCHAR)((value/divider)+'0');
      cnt++;
    } else if((pText != NULL) && (fill))
    {
      fill_cnt++;
      *pText++ = fill;
    }

    value %= divider;
    divider /= 10;
  }

	if(pText != NULL)
    *pText++ = (D4D_TCHAR)((value)+'0');

  return (Byte)(cnt + fill_cnt);
}

/**************************************************************************/ /*!
* @brief   The function convert decimal signed 32 bit number to string
* @param   val - the input numerical value
* @param   pText - pointer to output text buffer
* @param   fill - the fill character for non number characters (before number)
* @return  count of printed chars
* @note    If fill parameter is 0 {'\0'}, only the numerical characters are printed.
*******************************************************************************/
Byte D4D_SprintDecS32(sLWord val, D4D_TCHAR *pText, D4D_TCHAR fill)
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4D_SprintDecU32((LWord)val, pText, fill));
}

/**************************************************************************/ /*!
* @brief   The function convert decimal unsigned 16 bit number to string
* @param   val - the input numerical value
* @param   pText - pointer to output text buffer
* @param   fill - the fill character for non number characters (before number)
* @return  count of printed chars
* @note    If fill parameter is 0 {'\0'}, only the numerical characters are printed.
*******************************************************************************/
Byte D4D_SprintDecU16(Word val, D4D_TCHAR *pText, D4D_TCHAR fill)
{
  Word value;
  Word divider;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  Byte tmp_cnt;
  value = val;
  divider =  10000;

  for(tmp_cnt = 0; tmp_cnt < 4; tmp_cnt++)
  {
    if((value > divider - 1) || (cnt > 1))
    {
      if(pText != NULL)
        *pText++ = (D4D_TCHAR)((value/divider)+'0');
      cnt++;
    }else if((pText != NULL) && (fill))
    {
      fill_cnt++;
      *pText++ = fill;
    }

    value %= divider;
    divider /= 10;
  }

	if(pText != NULL)
    *pText++ = (D4D_TCHAR)((value)+'0');

  return (Byte)(cnt + fill_cnt);
}

/**************************************************************************/ /*!
* @brief   The function convert decimal signed 16 bit number to string
* @param   val - the input numerical value
* @param   pText - pointer to output text buffer
* @param   fill - the fill character for non number characters (before number)
* @return  count of printed chars
* @note    If fill parameter is 0 {'\0'}, only the numerical characters are printed.
*******************************************************************************/
Byte D4D_SprintDecS16(sWord val, D4D_TCHAR *pText, D4D_TCHAR fill)
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4D_SprintDecU16((Word)val, pText, fill));
}

/**************************************************************************/ /*!
* @brief   The function convert decimal unsigned 8 bit number to string
* @param   val - the input numerical value
* @param   pText - pointer to output text buffer
* @param   fill - the fill character for non number characters (before number)
* @return  count of printed chars
* @note    If fill parameter is 0 {'\0'}, only the numerical characters are printed.
*******************************************************************************/
Byte D4D_SprintDecU8(Byte val, D4D_TCHAR *pText, D4D_TCHAR fill)
{
  Byte value;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  value = val;

  if(value > 99)
  {
    cnt++;
    if(pText != NULL)
      *pText++ = (D4D_TCHAR)((value/100)+'0');
    value %= 100;

  }else if((pText != NULL) && (fill))
  {
    fill_cnt++;
    *pText++ = fill;
  }

  if((value > 9) || (cnt > 1))
  {
    cnt++;
    if(pText != NULL)
      *pText++ = (D4D_TCHAR)((value/10)+'0');
    value %= 10;

  }else if((pText != NULL) && (fill))
  {
    fill_cnt++;
    *pText++ = fill;
  }



	if(pText != NULL)
    *pText++ = (D4D_TCHAR)((value)+'0');

  return (Byte)(cnt + fill_cnt);
}

/**************************************************************************/ /*!
* @brief   The function convert decimal signed 8 bit number to string
* @param   val - the input numerical value
* @param   pText - pointer to output text buffer
* @param   fill - the fill character for non number characters (before number)
* @return  count of printed chars
* @note    If fill parameter is 0 {'\0'}, only the numerical characters are printed.
*******************************************************************************/
Byte D4D_SprintDecS8(sByte val, D4D_TCHAR *pText, D4D_TCHAR fill)
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4D_SprintDecU8((Byte)val, pText, fill));
}

/**************************************************************************/ /*!
* @brief   The function returns lenght of text
* @param   pText - pointer to text
* @return  count of characters in text
* @note    None.
*******************************************************************************/
D4D_INDEX D4D_GetTextLength(D4D_TCHAR* pText)
{
  D4D_INDEX cnt = 0;
  D4D_TCHAR* pTextInt;
  if(pText == NULL)
    return 0;

  pTextInt = D4D_GetInternalStringPointer(pText);

#if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE
  if(pTextInt == NULL)
    cnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_GetTranslateStrLength(pText);
  else
#endif
  {
    while (*pTextInt++)
      cnt++;
  }
  return cnt;
}

/**************************************************************************/ /*!
* @brief   The function returns width of text in pixels
* @param   ix - font index
* @param   pText - pointer to text
* @return  width of text in pixels
* @note    The function find out the width of string printed in given font.
*******************************************************************************/
D4D_COOR D4D_GetTextWidth(D4D_FONT ix, D4D_TCHAR* pText){

  D4D_COOR width = 0;
  D4D_TCHAR* pTextInt;

  if(pText == NULL)
    return 0;

  pTextInt = D4D_GetInternalStringPointer(pText);

#if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE

  if(pTextInt == NULL)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = 0;
    D4D_INDEX charCntTmp;

    do
    {
      charOff += charCnt;

      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pText, (D4D_TCHAR*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);

      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
        width += D4D_GetCharWidth(ix, ((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp]);

    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);


  }else
#endif
  {
    while(*(pTextInt) != 0)
      width += D4D_GetCharWidth(ix, *pTextInt++);
  }
  return width;
}

/**************************************************************************/ /*!
* @brief   The function returns width of text in pixels, also with tab table
* @param   text_buffer - pointer to full string descriptor (MUST be full filled)
* @param   pTab - pointer to teb table
* @return  width of text in pixels
* @note    The function find out the width of string printed with all specified properties in string descriptor.
*******************************************************************************/
D4D_COOR D4D_GetTextBuffWidthTab(D4D_STRING* text_buffer, D4D_TAB* pTab){

  D4D_COOR width = 0;
  D4D_COOR tab;
  D4D_INDEX char_ix = 0;
  D4D_TCHAR* pText = text_buffer->pText;
  D4D_TCHAR* pTextInt;

  if(pText == NULL)
    return 0;

  pTextInt = D4D_GetInternalStringPointer(pText);

#if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE

  if(pTextInt == NULL)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = text_buffer->printOff;
    D4D_INDEX charCntTmp;

    do
    {
      charOff += charCnt;

      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pText, (D4D_TCHAR*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);

      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if(++char_ix > text_buffer->printLen)
          return width;

        if(((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp] == '\t')
        {
          tab = D4D_GetNextTab(pTab, width);

          if(tab)
          {
            width = (D4D_COOR)(tab - (pTab->tabOffset));
            continue;
          }
        }

        width += D4D_GetCharWidth(text_buffer->fontId, ((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp]);
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);


  }else

#endif

  {
    pTextInt += text_buffer->printOff;

    while(*pTextInt != 0)
    {

      if(++char_ix > text_buffer->printLen)
        break;

      if(*pTextInt == '\t')
      {
        tab = D4D_GetNextTab(pTab, width);

        if(tab)
        {
          width = (D4D_COOR)(tab - (pTab->tabOffset));
          pTextInt++;
          continue;
        }
      }

      width += D4D_GetCharWidth(text_buffer->fontId, *pTextInt);
      pTextInt++;
    }
  }


  return width;
}

/**************************************************************************/ /*!
* @brief   The function find out the maximal text length that fits to maximal pixel width
* @param   pString - pointer to full string descriptor (MUST be full filled)
* @param   maxWidth - maximal width where the text should fits
* @return  length of text that's fits to given maximal width
* @note    None.
*******************************************************************************/
D4D_INDEX D4D_GetTextMaxLength(D4D_STRING* pString, D4D_COOR maxWidth)
{
  D4D_TCHAR* pText;
  D4D_COOR width;
  D4D_INDEX cnt = 0;
  D4D_TCHAR* pTextInt;

  if(pString == NULL)
    return 0;

  pText = pString->pText;

  if(pText == NULL)
    return 0;

  pTextInt = D4D_GetInternalStringPointer(pText);

#if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE
  if(pTextInt == NULL)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = pString->printOff;
    D4D_INDEX charCntTmp;

    do
    {
      charOff += charCnt;

      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pText, (D4D_TCHAR*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);

      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if(((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp] == 0)
          return cnt;

        if(cnt >= pString->printLen)
          return cnt;

        width = D4D_GetCharWidth(pString->fontId, *pText);

        if(width > maxWidth)
          return cnt;

        maxWidth -= width;
        cnt++;
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);


  }
  else
#endif
  {
    pTextInt += pString->printOff;

    while(*pTextInt != 0)
    {
      if(cnt >= pString->printLen)
        break;

      width = D4D_GetCharWidth(pString->fontId, *pTextInt);

      if(width > maxWidth)
        break;

      maxWidth -= width;
      pTextInt++;
      cnt++;
    }
  }

  return cnt;
}

/**************************************************************************/ /*!
* @brief   The function change the text in main object text buffer
* @param   pObject - pointer to object
* @param   pText - pointer to new string
* @return  None
* @note    The function change the text in the object text buffer if the object has it.
*******************************************************************************/
void D4D_SetText(D4D_OBJECT_PTR pObject, D4D_TCHAR* pText)
{
  D4D_STRING* p_TextBuff = NULL;

  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4D_OBJECT*)pObject);

  if(p_TextBuff)
  {
    D4D_ChangeText(p_TextBuff, pText, 0);
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }

}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_string_func                                              */
/******************************************************************************/

/**************************************************************//*!
*
* change text in the buffer
*
******************************************************************/

void D4D_ChangeText(D4D_STRING* pBuff, D4D_TCHAR* pNewText, D4D_TCHAR fillChar)
{
    D4D_INDEX n = pBuff->buffSize;
    D4D_TCHAR* pDest = pBuff->pText;

    // preserve one byte in the buffer
    if(pDest != pNewText)
      while(n > 1 && *pNewText)
      {
        *pDest = *pNewText;
        pNewText++;
        pDest++;
        n--;
      } else
      {
        D4D_INDEX i;
        i = D4D_GetTextLength(pDest);
        n -= i;
        pDest += i;
      }

    while(n > 1 && fillChar)
    {
      *(pDest++) = fillChar;
      n--;
    }

    // terminate string always
    *pDest = 0;
}

/**************************************************************//*!
*
* Print the String in ASCII in simple format up to 8 columns
*
******************************************************************/
void D4D_LCD_PrintStr(D4D_PRINT_DESC* p_StrDes)
{
  D4D_INDEX str_len, ch_ix;

  D4D_COOR orig_x = p_StrDes->x;
  D4D_COOR base_y, base_x;
  D4D_FONT_TYPE* pFontType = (D4D_FONT_TYPE*)p_StrDes->pFontType;
  D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);
  const D4D_TCHAR pLongTextEnd[] = D4D_DEFSTR("...");
  D4D_BOOL longText = D4D_FALSE;
  D4D_COOR tmp_MaxLen = p_StrDes->maxWidth;
  D4D_COOR longTextLen;
  D4D_TCHAR* pTextInt;
  // check if fontdescriptor exist and check the font scale value
  if((pFontDescriptor == NULL) || (pFontType->scale.width == 0) || (pFontType->scale.height == 0))
      return;

  // long strings restriction
  if(tmp_MaxLen)
  {
    // check if the text fits into enabled width
    D4D_STRING tmpString;
    D4D_COOR tmp_txtLen;

    tmpString.pText = p_StrDes->pText;
    tmpString.buffSize = 0;
    tmpString.fontId = p_StrDes->pFontType->ix_font;
    tmpString.printLen = p_StrDes->textLength;
    tmpString.printOff = p_StrDes->textOffset;
    tmp_txtLen = D4D_GetTextBuffWidthTab(&tmpString, p_StrDes->pTab);

    if(p_StrDes->pTab)
    {
      if(tmp_txtLen > p_StrDes->pTab->tabOffset)
        tmp_txtLen -= p_StrDes->pTab->tabOffset;
      else
        return;
    }

    longTextLen = D4D_GetTextWidth(tmpString.fontId, (D4D_TCHAR*)pLongTextEnd);

    if(tmp_txtLen > p_StrDes->maxWidth)
    {
      if(tmp_txtLen < longTextLen)
        return;

      tmp_MaxLen -= longTextLen;
      longText = D4D_TRUE;
    }
  }

  pTextInt = D4D_GetInternalStringPointer(p_StrDes->pText);

#if D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE

  if(pTextInt == NULL)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = p_StrDes->textOffset;
    D4D_INDEX charCntTmp;

    do
    {
      charOff += charCnt;

      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(p_StrDes->pText, (D4D_TCHAR*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);

        // get the lenght of string
      str_len = charCnt;

      if(p_StrDes->textLength)
        if(str_len > p_StrDes->textLength)
          str_len = p_StrDes->textLength;

      str_len += (charOff - 1);

      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if((!((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp]) || (str_len < (charCntTmp + charOff)))
        {
          charCnt = 0;
          break;
        }
        if(longText)
        {
          D4D_COOR tmp_Len;
          tmp_Len = D4D_GetCharWidth(p_StrDes->pFontType->ix_font, ((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp]);

          if(tmp_MaxLen - tmp_Len <= 0)
          {
            charCnt = 0;
            break;
          }
          tmp_MaxLen -= tmp_Len;
        }

        p_StrDes->x += D4D_LCD_PrintChr(((D4D_TCHAR*)d4d_extsrcBuffer)[charCntTmp], p_StrDes); // print the char and automatically update the coordintaion of next char
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);


  }else
#endif
  {
    pTextInt = &pTextInt[p_StrDes->textOffset];

    // get the lenght of string
    str_len = D4D_GetTextLength(pTextInt);

    if(p_StrDes->textLength)
      if(str_len > p_StrDes->textLength)
        str_len = p_StrDes->textLength;

    // Draw the text
    for (ch_ix=0; ch_ix<str_len; ++ch_ix)	// For each character in the string
    {
      if(longText)
      {
        D4D_COOR tmp_Len;
        tmp_Len = D4D_GetCharWidth(p_StrDes->pFontType->ix_font, *pTextInt);

        if(tmp_MaxLen - tmp_Len <= 0)
        {
          break;
        }
        tmp_MaxLen -= tmp_Len;
      }

      p_StrDes->x += D4D_LCD_PrintChr(*pTextInt++, p_StrDes); // print the char and automatically update the coordintaion of next char
    }
  }



  if(longText)
  {
    ch_ix=0;
    while(pLongTextEnd[ch_ix])
      p_StrDes->x += D4D_LCD_PrintChr(pLongTextEnd[ch_ix++], p_StrDes); // print the char and automatically update the coordintaion of next char

    if(!(p_StrDes->properties & D4D_FNT_PRTY_TRANSPARENT_MASK))
      D4D_FillRectXY(p_StrDes->x, p_StrDes->y, (D4D_COOR)(orig_x + p_StrDes->maxWidth - 1), (D4D_COOR)(p_StrDes->y + D4D_GetFontHeight(p_StrDes->pFontType->ix_font) - 1), p_StrDes->colorBack);


  }

	// Draw the Underline option
	switch(p_StrDes->properties & D4D_FNT_PRTY_UNDERLINE_MASK)
	{
	  case D4D_FNT_PRTY_UNDERLINE_LINE_MASK:  // Draw the full line
	    base_y = (D4D_COOR)(p_StrDes->y + pFontDescriptor->charBaseLine * pFontType->scale.height);
	    base_x = p_StrDes->x;
	    D4D_FillRectXY(orig_x, base_y, (D4D_COOR)(base_x - 1), (D4D_COOR)(base_y + pFontType->scale.height - 1), p_StrDes->colorText);
	    break;

	  case D4D_FNT_PRTY_UNDERLINE_DOT_MASK:  // Draw the dot line
	    base_y = (D4D_COOR)(p_StrDes->y + pFontDescriptor->charBaseLine * pFontType->scale.height);

	    ch_ix = 0;
	    for(base_x = orig_x; base_x < p_StrDes->x; base_x += pFontType->scale.height)
	    {
	      if(++ch_ix % 2)
	        D4D_FillRectXY(base_x, base_y, (D4D_COOR)(base_x + pFontType->scale.width - 1), (D4D_COOR)(base_y + pFontType->scale.height - 1), p_StrDes->colorText);
	    }
	    break;
	  default:
	    break;
	}

	// Draw the Strike Through option
	if(p_StrDes->properties & D4D_FNT_PRTY_STRIKETHROUGH_MASK)
	{
	    Byte line_cnt = (Byte)((p_StrDes->properties & D4D_FNT_PRTY_STRIKETHROUGH_MASK) >> D4D_FNT_PRTY_STRIKETHROUGH_SHIFT);

      base_y = (D4D_COOR)(p_StrDes->y + (pFontDescriptor->charBaseLine - (pFontDescriptor->charSize / 3)) * pFontType->scale.height);
	    base_x = p_StrDes->x;
	    // Draw Strike Through line as many as specified by parameter
	    while(line_cnt)
	    {
	      D4D_FillRectXY(orig_x, base_y, (D4D_COOR)(base_x - 1), (D4D_COOR)(base_y - 1), p_StrDes->colorText);
	      base_y -=2;
              line_cnt--;
	    }

	}
#if D4D_LLD_FLUSH_ELEMENT != D4D_FALSE
	D4D_LLD_LCD.D4DLCD_FlushBuffer(D4DLCD_FLSH_ELEMENT);
#endif

}

#if D4D_STRINGTABLE_PREFIX
  /*-----------------------------------------------------------------------------
  * FUNCTION:    Strings_ChangeLanguage
  * SCOPE:       language strings related function
  * DESCRIPTION: Function changes the current language
  *
  * PARAMETERS:  ix - index of language
  *
  * RETURNS:     none
  *-----------------------------------------------------------------------------*/
  static int D4D_StringToInt(const D4D_TCHAR* pText)
  {
    int res = 0;
    int tens = 1;
    // simple implementation of string to integer
    // just for example  of external strings

    int i = D4D_GetTextLength((D4D_TCHAR*)pText);

    if(i > D4D_STR_TABLE_MAX_CNT)
      return -1;

    if(i == 0)
      return -1;

    // check if the chars a really numbers
    while(i--)
    {
      if(!D4D_IsDigit(pText[i]))
        return -1;

      res += (pText[i] - '0') * tens;
      tens *= 10;
    }

    return res;
  }
#endif
