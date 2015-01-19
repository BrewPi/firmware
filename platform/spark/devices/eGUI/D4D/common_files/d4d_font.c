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
* @file      d4d_font.c
*
* @author     Petr Gargulak
*
* @version   0.0.48.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver font c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"

#if (D4D_EXTSRC_FILE_ENABLE != D4D_FALSE) && (D4D_FNT_EXTSRC_SUPPORT != D4D_FALSE)
  static D4D_CHAR* pFntWorkPath = D4D_FNT_WORK_PATH_DEFAULT;
  static D4D_FONT_TYPE * d4d_FontTableRam = NULL;
  static Byte D4D_GetFontTableEntries(D4D_FONT_TYPE* fontTable);
#endif

 typedef struct
  {
    LWord row;
    LWord clm;
    LWord char_width;
    D4D_FONT_DATA* pFntData;
    D4D_FONT_DESCRIPTOR* pFontDescriptor;
  }CHAR_PXL_DESC;

 typedef struct
 {
   D4D_FONT_DESCRIPTOR * pFontDescriptor;
   D4D_FONT_IX index;
 }FNT_CHAR_PLACE;


static FNT_CHAR_PLACE D4D_LCD_GetCharPlace(const D4D_FONT_DESCRIPTOR * pFontDescriptorStart, D4D_TCHAR ch);

static D4D_INDEX_DELTA D4D_GetCharIndex(D4D_FONT_DESCRIPTOR* pFontDescriptor, D4D_TCHAR ch);
static D4D_FONT_DATA* D4D_GetCharData(FNT_CHAR_PLACE* char_place);

#if (D4D_FONT_TABLE_DISABLED == D4D_FALSE)
  static D4D_FONT_TYPE* d4d_pFontTable =  (D4D_FONT_TYPE*)&d4d_FontTable;
#endif

#if (D4D_EXTSRC_FILE_ENABLE != D4D_FALSE) && (D4D_FNT_EXTSRC_SUPPORT != D4D_FALSE)

  /**************************************************************//*!
  *
  * Init external font table
  *
  ******************************************************************/
  static Byte D4D_GetFontTableEntries(D4D_FONT_TYPE* fontTable)
  {
    Byte i = 0;

    if(fontTable != NULL)


    while(fontTable[i].pFontDescriptor || fontTable[i].fileName)
      i++;

    return i;
  }

  /**************************************************************//*!
  *
  * Init external font table
  *
  ******************************************************************/
  D4D_BOOL D4D_ExtFntInit(void)
  {
    LWord i,j;

    // Free possible previous font table
    if(d4d_FontTableRam)
    {
      i = 0;
      while(d4d_pFontTable[i].pFontDescriptor || d4d_pFontTable[i].fileName)
      {
        // Standard local memory based font?
        if(d4d_pFontTable[i].fileName != NULL)
        {
          // external file that should be loaded into RAM to be able to use it

          // 1. clear possible previous allocated RAM
          if(d4d_FontTableRam[i].pFontDescriptor)
            D4D_MemFree((void*)d4d_FontTableRam[i].pFontDescriptor);
        }
        i++;
      }

      D4D_MemFree(d4d_FontTableRam);
    }
    j = (sizeof(D4D_FONT_TYPE) * (D4D_GetFontTableEntries(d4d_pFontTable) + 1));

    // Allocate the new place for the font table
    d4d_FontTableRam = D4D_MemAlloc(j);

    if(d4d_FontTableRam == NULL)
      return D4D_FALSE;

    //Clear up the d4d_FontTableRam structure
    for(i = 0; i < j; i++)
      ((Byte*)d4d_FontTableRam)[i] = 0;

    // Load to RAM the font tables
    D4D_ExtFntSetWorkPath(pFntWorkPath);

    d4d_pFontTable = d4d_FontTableRam;

    return D4D_TRUE;
  }

  /**************************************************************//*!
  *
  * Set Font external source files work directory
  *
  ******************************************************************/
  void D4D_ExtFntSetWorkPath(D4D_CHAR* pPath)
  {
    Byte i=0;
    D4D_FILEPTR pFile;
    LWord fileSize;
    D4D_FONT_DESCRIPTOR* pFntDesc;
    if(pPath)
      pFntWorkPath = pPath;

    // create the new font table from template and also copy the new fonts to the RAM

    while(d4d_pFontTable[i].pFontDescriptor || d4d_pFontTable[i].fileName)
    {
      // Standard local memory based font?
      if(d4d_pFontTable[i].fileName == NULL)
      {
        // just copy to RAM version of font table
        d4d_FontTableRam[i] = d4d_pFontTable[i];
      }else
      {
        // external file that should be loaded into RAM to be able to use it

        // 1. clear possible previous allocated RAM
        if(d4d_FontTableRam[i].pFontDescriptor)
          D4D_MemFree((void*)d4d_FontTableRam[i].pFontDescriptor);

        // 2. just copy to RAM version of font table
        d4d_FontTableRam[i] = d4d_pFontTable[i];

        // 3. try to open the font file
        pFile = D4D_OpenFntFile((void*)d4d_pFontTable[i].fileName);

        // 4. if the file can't be opened, just go to next font
        if(pFile == NULL)
        {
          i++;
          continue;
        }
        // 5. Get the size of file
        if(D4D_FileSeek(pFile, 0, D4D_SEEK_END) != 0)
        {
          D4D_FileClose(pFile);
          i++;
          continue;
        }

        fileSize = D4D_FileTell(pFile);

        // Return the file location to start
        if(D4D_FileSeek(pFile, 0, D4D_SEEK_SET) != 0)
        {
          D4D_FileClose(pFile);
          i++;
          continue;
        }

        // 6. Alloc the memory on heap for the data
        d4d_FontTableRam[i].pFontDescriptor = D4D_MemAlloc(fileSize + 16);

        if(d4d_FontTableRam[i].pFontDescriptor == NULL)
        {
          D4D_FileClose(pFile);
          i++;
          continue;
        }

        // 7. Copy the content of file to RAM
        if(D4D_FileRead(pFile, d4d_FontTableRam[i].pFontDescriptor, fileSize) != fileSize)
        {
          D4D_MemFree(d4d_FontTableRam[i].pFontDescriptor);
          d4d_FontTableRam[i].pFontDescriptor = NULL;
          D4D_FileClose(pFile);
          i++;
          continue;
        }

        // 8. Adjust the used pointers in all loaded structures
        pFntDesc = d4d_FontTableRam[i].pFontDescriptor;
#ifdef D4D_UNICODE
        while((pFntDesc != NULL) && (pFntDesc - d4d_FontTableRam[i].pFontDescriptor) <= fileSize)
        {
#endif
          if(pFntDesc->pIxTable)
            *(LWord*)&(pFntDesc->pIxTable) += (LWord)pFntDesc;

          if(pFntDesc->pOffTable)
            *(LWord*)&(pFntDesc->pOffTable) += (LWord)pFntDesc;

          if(pFntDesc->pSizeTable)
            *(LWord*)&(pFntDesc->pSizeTable) += (LWord)pFntDesc;

          if(pFntDesc->pFontData)
            *(LWord*)&(pFntDesc->pFontData) += (LWord)pFntDesc;
#ifdef D4D_UNICODE
          if(pFntDesc->pNext)
          {
            *(LWord*)&(pFntDesc->pNext) += (LWord)pFntDesc;
            pFntDesc = (D4D_FONT_DESCRIPTOR*)pFntDesc->pNext;
          }
          else
            pFntDesc = NULL;
        }
#endif
      }
      i++;
    }
  }

  /**************************************************************//*!
  *
  * Set Font external source files work directory
  *
  ******************************************************************/
  void D4D_ExtFntSetDfltWorkPath(void)
  {
    D4D_ExtFntSetWorkPath(D4D_FNT_WORK_PATH_DEFAULT);
  }
  /**************************************************************//*!
  *
  * Get Font external source files work directory
  *
  ******************************************************************/
  D4D_CHAR* D4D_ExtFntGetWorkPath(void)
  {
    return pFntWorkPath;
  }

  /**************************************************************//*!
  *
  * Open font external source file
  *
  ******************************************************************/
  D4D_FILEPTR D4D_OpenFntFile(D4D_CHAR* pFileName)
  {
    // create the file name (if needed)
    if(D4D_FileIsAbsolutePath(pFileName))
    {
      // the input path is absolute and can't be changed
      return D4D_FileOpen(pFileName,"r");
    }
    else
    {
      // the input path is relative and must be updated with preselected image work paths
      if(D4D_StrLen(pFntWorkPath) + D4D_StrLen(pFileName) >= D4D_EXTSRC_BUFF_SIZE)
        return NULL;

      D4D_StrCopy((D4D_CHAR*)d4d_extsrcBuffer, pFntWorkPath);
      D4D_StrCat((D4D_CHAR*)d4d_extsrcBuffer, pFileName);
      return D4D_FileOpen((D4D_CHAR*)d4d_extsrcBuffer,"r");
    }

  }
#endif

/**************************************************************//*!
*
* Change the Font Table
*
******************************************************************/
void D4D_SetFontTable(D4D_FONT_TYPE* pFontTable)
{
  if(pFontTable == d4d_pFontTable)
    return;

  if(pFontTable == NULL)
    d4d_pFontTable = (D4D_FONT_TYPE*)&d4d_FontTable;
  else
    d4d_pFontTable = pFontTable;

#if (D4D_EXTSRC_FILE_ENABLE != D4D_FALSE) && (D4D_FNT_EXTSRC_SUPPORT != D4D_FALSE)
  D4D_ExtFntInit();
#endif

}

D4D_FONT_TYPE* D4D_GetFontTable(void)
{
  return d4d_pFontTable;
}
/**************************************************************//*!
*
* Get selected font information
*
******************************************************************/

D4D_FONT_TYPE* D4D_GetFont(D4D_FONT ix)
{

#if (D4D_FONT_TABLE_DISABLED == D4D_TRUE)
    D4D_UNUSED(ix);
    return NULL;
#else
    Byte i=0;

    while(d4d_pFontTable[i].pFontDescriptor != NULL)
    {
      if(d4d_pFontTable[i].ix_font == ix)
        return (D4D_FONT_TYPE*) &(d4d_pFontTable[i]);
      i++;
    }
    return NULL;
#endif
}

/**************************************************************//*!
*
* Get selected font size information
*
******************************************************************/

D4D_FONT_SIZES D4D_GetFontSize(D4D_FONT ix)
{
    D4D_FONT_SIZES tmpFontSize;

    tmpFontSize.height = D4D_GetFontHeight(ix);
    tmpFontSize.width = D4D_GetFontWidth(ix);
    return tmpFontSize;
}


/**************************************************************//*!
*
* Get selected font height information
*
******************************************************************/

D4D_FONT_SIZE D4D_GetFontHeight(D4D_FONT ix)
{
    D4D_FONT_TYPE* pFontType = D4D_GetFont(ix);
    D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);

    if(pFontType != NULL)
      return (D4D_FONT_SIZE)((pFontType->scale.height * pFontDescriptor->charFullSize.height) + pFontType->charSpacing.height);

    return (D4D_FONT_SIZE) 0;
}

/**************************************************************//*!
*
* Get selected font width information
*
******************************************************************/

D4D_FONT_SIZE D4D_GetFontWidth(D4D_FONT ix)
{
    D4D_FONT_TYPE* pFontType = D4D_GetFont(ix);
    D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);

    if(pFontType != NULL)
      return (D4D_FONT_SIZE)((pFontType->scale.width * pFontDescriptor->charFullSize.width) + pFontType->charSpacing.width);

    return (D4D_FONT_SIZE) 0;
}

/**************************************************************//*!
*
* Get selected font width information
*
******************************************************************/

D4D_FONT_SIZE D4D_GetCharWidth(D4D_FONT ix, D4D_TCHAR ch)
{
    D4D_FONT_TYPE* pFontType = D4D_GetFont(ix);
    D4D_FONT_SIZE tmp_width = 0;
    FNT_CHAR_PLACE char_place;
    if(NULL == pFontType)
      return 0;

    if(ch < ' ')
      return 0;

    if(pFontType->pFontDescriptor == NULL)
      return 0;

    char_place = D4D_LCD_GetCharPlace(pFontType->pFontDescriptor, ch);

    if(char_place.pFontDescriptor == NULL)
      return 0;

    if(char_place.pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL)
    {
      // Get the raw width of char
      tmp_width = char_place.pFontDescriptor->pSizeTable[char_place.index];
    }else
      tmp_width = char_place.pFontDescriptor->charFullSize.width;

    // Apply the scale for the char width
    tmp_width *= pFontType->scale.width;
    // Add the char spacing
    tmp_width += pFontType->charSpacing.width;

    // return the char width
    return tmp_width;
}



/**************************************************************//*!
*
* Get "real" index in data font table of used font
*
******************************************************************/

static D4D_INDEX_DELTA D4D_GetCharIndex(D4D_FONT_DESCRIPTOR* pFontDescriptor, D4D_TCHAR ch)
{
  D4D_FONT_IX char_ix = (D4D_FONT_IX)(ch -  pFontDescriptor->startChar);

  // is the indes table linear or nonlinear?
  if(pFontDescriptor->flags & D4D_FONT_FLAGS_IX_STYLE_NONLINEAR)
  {
    // Index table is used?
    if(pFontDescriptor->pIxTable != NULL)
    {
      if(pFontDescriptor->flags & D4D_FONT_FLAGS_IX_TYPE_MAP)
      {
        // Map style of Index Table
        D4D_FONT_IX i;
        // Look for the used char in index map table
        for(i = 0; i < pFontDescriptor->charCnt; i++)
        {
          if(ch == pFontDescriptor->pIxTable[i])
            return i;
        }
      }else
      {
         // check if the char is set into range of used font
        if(char_ix < pFontDescriptor->charCnt)
          // Look up style of Index Table
          return pFontDescriptor->pIxTable[char_ix];
      }
    }
  }else
  {
     // check if the char is set into range of used font
    if(char_ix < pFontDescriptor->charCnt)
      return char_ix;
  }

  // return the "real table index" of used car
  return -1;
}

/**************************************************************//*!
  *
  * Find the exact descriptor and char ix
  *
  ******************************************************************/
  static FNT_CHAR_PLACE D4D_LCD_GetCharPlace(const D4D_FONT_DESCRIPTOR * pFontDescriptorStart, D4D_TCHAR ch)
  {
    FNT_CHAR_PLACE result = { NULL, 0};
    D4D_FONT_DESCRIPTOR * pFontDescriptor = (D4D_FONT_DESCRIPTOR *)pFontDescriptorStart;
    D4D_INDEX_DELTA char_ix;

    if(!pFontDescriptor)
      return result;

    do{
      #ifdef D4D_UNICODE
        while (pFontDescriptor)
        {
          if((ch & 0xff00) == (pFontDescriptor->startChar & 0xff00))
            break;

          pFontDescriptor = (D4D_FONT_DESCRIPTOR *)pFontDescriptor->pNext;
        }
      #endif

      if(pFontDescriptor && ((char_ix = D4D_GetCharIndex((D4D_FONT_DESCRIPTOR*)pFontDescriptor, ch)) >= 0 ))
      {
        // The char was found , so everything is perfect
        result.pFontDescriptor = pFontDescriptor;
        result.index = (D4D_FONT_IX)char_ix;
        break;
      }else
      {
        // The char is not part of this page

        // check if the the char what we are looking for isn't default one
        // if yes and we not able to find it go out
        if(ch == pFontDescriptorStart->charDefault)
          break;

        // we have to look for default char
        ch = pFontDescriptorStart->charDefault;
        pFontDescriptor = (D4D_FONT_DESCRIPTOR *)pFontDescriptorStart;
      }

    }while(1);

    return result;
  }


/**************************************************************//*!
*
* Get pointer on bitmap of selected char of used font
*
******************************************************************/

D4D_FONT_DATA* D4D_GetCharData(FNT_CHAR_PLACE* char_place) {

  if(!char_place)
   return NULL;

  if(!char_place->pFontDescriptor)
    return NULL;

  // check the style of font monospace / proportional
  if(char_place->pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL) {
    // Proportinal font is used

    // Check if Offset table exist
    if(char_place->pFontDescriptor->pOffTable == NULL)
      return NULL;

    return (D4D_FONT_DATA*)(char_place->pFontDescriptor->pFontData + char_place->pFontDescriptor->pOffTable[char_place->index]);
  }

  // Monospace font is used
  return (D4D_FONT_DATA*)(char_place->pFontDescriptor->pFontData + char_place->index * char_place->pFontDescriptor->charBmpSize);
}

/**************************************************************//*!
*
* Get "real" index in data font table of used font
*
******************************************************************/



static D4D_BOOL D4D_LCD_GetFntBit(CHAR_PXL_DESC * pPxlDesc) {

  LWord bit_cnt;
  Byte data;

  // This is decoder only for fonts that are row scan based and preffered !!!
  if(pPxlDesc->pFontDescriptor->pack & D4D_FONT_PACK_SCANPREFERRED_COLUMN)
    return 0;

  if(pPxlDesc->pFontDescriptor->pack & D4D_FONT_PACK_SCANBASED_COLUMN)
    return 0;


  if(pPxlDesc->pFontDescriptor->pack & D4D_FONT_PACK_COMPRESSED_ON) {
    // data are packed in bit stream
    bit_cnt = (Word)(pPxlDesc->char_width * pPxlDesc->row + pPxlDesc->clm);
    data = pPxlDesc->pFntData[bit_cnt / 8];
  }else
  {
    // data aren't packed
    LWord byte_cnt;
    byte_cnt = (Word)((((pPxlDesc->char_width - 1) / 8) + 1) * pPxlDesc->row);  // get the start of row index of data
    byte_cnt += (pPxlDesc->clm / 8);  // add the collumn index move
    data = pPxlDesc->pFntData[byte_cnt];  // get the data byte
    bit_cnt = (Byte)(pPxlDesc->clm % 8); // get position of bit in byte for this pixel
  }

  if(pPxlDesc->pFontDescriptor->pack & D4D_FONT_PACK_BITORDER_LITTLEEND)
    return (D4D_BOOL)((data >> (bit_cnt % 8)) & 0x01);   // little endian decoding
  else
    return (D4D_BOOL)((data << (bit_cnt % 8)) & 0x80);   // big endian decoding
}

D4D_COOR D4D_GetNextTab(D4D_TAB* pTab, D4D_COOR pos)
{
  D4D_INDEX ix = 0;

  if(pTab == NULL)
    return 0;

  if(pTab->pTabTable == NULL)
    return 0;

  while(pTab->pTabTable[ix])
  {
    if((pos + pTab->tabOffset) < pTab->pTabTable[ix])
      return (D4D_COOR)(pTab->pTabTable[ix] /*- pTab->tabOffset*/);

    ix++;
  }

  return 0;

}

/**************************************************************//*!
*
* Print the Char in ASCII in simple format up to 8 columns
*
******************************************************************/
D4D_COOR D4D_LCD_PrintChr(D4D_TCHAR ch, D4D_PRINT_DESC* p_CharDes)
{
  D4D_FONT_SIZE xScale, yScale;
  LWord row_cnt;
  LWord bit_cnt;
  LWord px_cnt;
  LWord yScale_cnt;
  D4D_COLOR clr;
  D4D_FONT_TYPE* pFontType = (D4D_FONT_TYPE*)p_CharDes->pFontType;
  CHAR_PXL_DESC pxlDesc;
  FNT_CHAR_PLACE char_place;

  if(pFontType == NULL)
    return 0;

  pxlDesc.pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);

  if(!pxlDesc.pFontDescriptor)
    return 0;

  if(ch < ' ')
  {
    if((ch == '\t') && (p_CharDes->pTab != NULL))
    {
      D4D_COOR nextTab;
      // print tabulator
      nextTab = D4D_GetNextTab(p_CharDes->pTab, p_CharDes->x);

      if(nextTab)
      {
        D4D_LCD_Box(p_CharDes->x, p_CharDes->y, nextTab,\
                       (D4D_COOR)(p_CharDes->y + D4D_GetFontHeight(p_CharDes->pFontType->ix_font) - 1),\
                       p_CharDes->colorBack);
        return (D4D_COOR)(nextTab - p_CharDes->x);
      }
    }else
      return 0;  // none printable char
  }
  xScale = p_CharDes->pFontType->scale.width;
  yScale = p_CharDes->pFontType->scale.height;

  char_place = D4D_LCD_GetCharPlace(pxlDesc.pFontDescriptor, ch);

  if(!char_place.pFontDescriptor)
    return 0;

  pxlDesc.pFontDescriptor = char_place.pFontDescriptor;


  pxlDesc.pFntData = D4D_GetCharData(&char_place);	 // Get pointer to char bmp data

  if(pxlDesc.pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL)
    pxlDesc.char_width = pxlDesc.pFontDescriptor->pSizeTable[char_place.index];
  else
    pxlDesc.char_width = pxlDesc.pFontDescriptor->charFullSize.width;



#ifndef D4D_COLOR_TRANSPARENT

  if(p_CharDes->properties & D4D_FNT_PRTY_TRANSPARENT_MASK)
  {
    for (pxlDesc.row=0; pxlDesc.row<pxlDesc.pFontDescriptor->charFullSize.height; ++pxlDesc.row)			// For each vertical row of this character
    {
      for(pxlDesc.clm = 0; pxlDesc.clm < pxlDesc.char_width; pxlDesc.clm++)
      {
        if(D4D_LCD_GetFntBit(&pxlDesc))
        {
          D4D_LCD_Box((D4D_COOR)(p_CharDes->x + pFontType->charSpacing.width + pxlDesc.clm * xScale),\
            (D4D_COOR)(p_CharDes->y + pFontType->charSpacing.height + (pxlDesc.row * yScale)),\
            (D4D_COOR)(p_CharDes->x + pFontType->charSpacing.width + pxlDesc.clm * xScale + xScale - 1),\
            (D4D_COOR)(p_CharDes->y + pFontType->charSpacing.height + (pxlDesc.row * yScale) + yScale - 1),\
            p_CharDes->colorText);
        }
      }
    }
  }
  else  // if(Transparent)
#else
  if(p_CharDes->properties & D4D_FNT_PRTY_TRANSPARENT_MASK)
    p_CharDes->colorBack = D4D_COLOR_TRANSPARENT;

#endif
  {
    (void)D4D_LLD_LCD.D4DLCD_SetWindow(p_CharDes->x, p_CharDes->y,
      (D4D_COOR)(p_CharDes->x + D4D_GetCharWidth(p_CharDes->pFontType->ix_font, ch) - 1),
      (D4D_COOR)(p_CharDes->y + D4D_GetFontHeight(p_CharDes->pFontType->ix_font) - 1)); // Set character window

    // add on background box on bottom of character to provide line spacing
    for(row_cnt=0; row_cnt < pFontType->charSpacing.height; ++row_cnt)
    {
      for(bit_cnt = 0; bit_cnt < (D4D_COOR)(pxlDesc.char_width * xScale + pFontType->charSpacing.width); ++bit_cnt)
        D4D_LLD_LCD.D4DLCD_Send_PixelColor(p_CharDes->colorBack);
    }


    for (pxlDesc.row=0; pxlDesc.row < pxlDesc.pFontDescriptor->charFullSize.height; ++pxlDesc.row)			// For each vertical row of this character
    {
      for (yScale_cnt=1; yScale_cnt<=yScale; ++yScale_cnt)		// As often as needed by the Y-scale factor (e.g. draws complete line twice if double height font is requested)
      {
        for (pxlDesc.clm=1; pxlDesc.clm<=pFontType->charSpacing.width; ++pxlDesc.clm)
        {
          // empty column = always background color
          D4D_LLD_LCD.D4DLCD_Send_PixelColor(p_CharDes->colorBack);								// draw pixel
        }

        for(pxlDesc.clm = 0; pxlDesc.clm < pxlDesc.char_width; ++pxlDesc.clm)
        {
          if(D4D_LCD_GetFntBit(&pxlDesc))
            clr = p_CharDes->colorText;
          else
            clr = p_CharDes->colorBack;

          for (px_cnt=1; px_cnt<=xScale; ++px_cnt)			// as often as needed by the horizotal scale factor (e.g. draws pixel twice if double wide font is requested)
            D4D_LLD_LCD.D4DLCD_Send_PixelColor(clr);			// draw pixel
        }
      }											// draw this line again if needed for scaling
    }
  }// next row of this character

  return (D4D_COOR)(pxlDesc.char_width * xScale + pFontType->charSpacing.width);
}



