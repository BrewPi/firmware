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
* @file      d4d_imgdec_d4d.c
*
* @author     Petr Gargulak
*
* @version   0.0.13.0
*
* @date      Feb-19-2013
*
* @brief     D4D driver external pictures d4d decoder functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_imgdec_d4d.h"
#include "common_files/d4d_imgdec_d4dint.h"

#if (D4D_BMP_EXTSRC_SUPPORT_D4D == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)

#if D4D_BMP_PAL_2_ENABLE != D4D_FALSE
  extern D4D_COLOR D4D_GetPixel_BmpDecodeBmp2(D4D_D4DBMP_DECODE* pDecode);
#endif

#if D4D_BMP_PAL_16_ENABLE != D4D_FALSE
  extern D4D_COLOR D4D_GetPixel_BmpDecodeBmp16(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE)
  extern D4D_COLOR D4D_GetPixel_BmpDecodeBmp256(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
  extern D4D_COLOR D4D_GetPixel_BmpDecodeBmp65K(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
  extern D4D_COLOR D4D_GetPixel_BmpDecodeBmp256K(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE)
  extern D4D_COLOR D4D_GetPixel_BmpDecodeBmp16M(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
  static LWord D4D_GetDataLong(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment);
#endif

#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
  static Word D4D_GetDataWord(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment);
#endif

#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_16_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_2_ENABLE != D4D_FALSE)
  static Byte D4D_GetDataByte(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment);
#endif


void D4D_ImgDecExtD4DDraw(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius)
{
  D4D_FILEPTR pFile;
  D4D_D4DBMP_DECODE d4dbmp_decode;
  LWord cmode;
  D4D_COOR x2;
  D4D_COOR y2;
  D4D_COOR width;
  D4D_COOR height;
  LWord x_ix, y_ix;

  // open the picture file
  pFile = D4D_OpenImgFile(pFileName, "r");

  // check if the file is opened
  if(pFile == NULL)
    return;

  // load the header of picture file

  #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(D4D_EXTSRC_HEADER_D4D_SIZE <= D4D_EXTSRC_BUFF_SIZE);
  #endif

  if(D4D_FileRead(pFile, d4d_extsrcBuffer, D4D_EXTSRC_HEADER_D4D_SIZE) != D4D_EXTSRC_HEADER_D4D_SIZE)
  {
    // the header isn't loaded succesfully
    D4D_FileClose(pFile);
    return;
  }
  #if D4D_ENDIAN == D4D_ENDIAN_LITTLE
    if(d4d_extsrcBuffer[6] != 'L')
  #else
    if(d4d_extsrcBuffer[6] != 'B')
  #endif
  {
    //the content doesn't match the excepted content
    D4D_FileClose(pFile);
    return;
  }

  d4d_extsrcBuffer[6] = '\0';

  // check the content of file - is really D4D picture binary format?
  if(D4D_CompareStrings((const D4D_CHAR*)(&((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->ident), "D4DBMP"))
  {
    //the content doesn't match the excepted content
    D4D_FileClose(pFile);
    return;
  }

  // check the version of D4DBMP header
  if(((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->version > 1)
  {
    //the current version of D4DBMP file is not supported
    D4D_FileClose(pFile);
    return;
  }

  width = ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->width;
  height = ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->height;

  x2 = (D4D_COOR)(x + width - 1);
  y2 = (D4D_COOR)(y + height - 1);

  if(((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->version)
    cmode = ((D4D_EXTSRC_HEADER_D4D_1*)d4d_extsrcBuffer)->colorMode; // color mode in version
  else
    cmode = ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->colorMode; // color mode in version

  d4dbmp_decode.ix = 0;
  d4dbmp_decode.rptCnt = 0;
  d4dbmp_decode.curSubPxl = 0;
  d4dbmp_decode.compress = (cmode & D4D_BMP_COMPRESS_MASK);
  d4dbmp_decode.greyScale = greyScale;
  d4dbmp_decode.pBmp = NULL;
  d4dbmp_decode.dataCnt = 0;
  d4dbmp_decode.dataIx = 0;
  d4dbmp_decode.pFile = pFile;

  // There is also pallete
  if(((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->palleteOffset && ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->palleteSize)
  {
    // alloc the memory for pallete
    d4dbmp_decode.pPalette = D4D_MemAlloc(((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->palleteSize);
    if(d4dbmp_decode.pPalette == NULL)
    {
      // the Bitmap pallete isn't loaded succesfully
      D4D_FileClose(pFile);
      return;
    }

    if(D4D_FileSeek(pFile, ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->palleteOffset, D4D_SEEK_SET) != 0)
    {
      // the position in file can't be sets
      D4D_MemFree((void*)d4dbmp_decode.pPalette);
      D4D_FileClose(pFile);
      return;
    }

    // load the Bitmap pallete
    if(D4D_FileRead(pFile, (void*)d4dbmp_decode.pPalette, ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->palleteSize) != ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->palleteSize)
    {
      // the Bitmap pallete isn't loaded succesfully
      D4D_MemFree((void*)d4dbmp_decode.pPalette);
      D4D_FileClose(pFile);
      return;
    }
  }else
    d4dbmp_decode.pPalette = NULL;

  switch(cmode & (D4D_BMP_TYPE_MASK | D4D_BMP_CLR_DEPTH_MASK))
  {
#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE)
  case D4D_BMP_16MNOPAL:
    d4dbmp_decode.GetPxlDecoder = D4D_GetPixel_BmpDecodeBmp16M;
    d4dbmp_decode.GetDataFunc = (void*)D4D_GetDataLong;
    break;
#endif


#if (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
  case D4D_BMP_256KNOPAL:
    d4dbmp_decode.GetPxlDecoder = D4D_GetPixel_BmpDecodeBmp256K;
    d4dbmp_decode.GetDataFunc = (void*)D4D_GetDataLong;
    break;
#endif

#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
  case D4D_BMP_65536NOPAL:
    d4dbmp_decode.GetPxlDecoder = D4D_GetPixel_BmpDecodeBmp65K;
    d4dbmp_decode.GetDataFunc = (void*)D4D_GetDataWord;
    break;
#endif

#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE)
  case D4D_BMP_256NOPAL:
  case D4D_BMP_PAL_256:
    d4dbmp_decode.GetPxlDecoder = D4D_GetPixel_BmpDecodeBmp256;
    d4dbmp_decode.GetDataFunc = (void*)D4D_GetDataByte;
    break;
#endif

#if D4D_BMP_PAL_16_ENABLE != D4D_FALSE
  case D4D_BMP_PAL_16:
    d4dbmp_decode.GetPxlDecoder = D4D_GetPixel_BmpDecodeBmp16;
    d4dbmp_decode.GetDataFunc = (void*)D4D_GetDataByte;
    break;
#endif

#if D4D_BMP_PAL_2_ENABLE != D4D_FALSE
  case D4D_BMP_PAL_2:
    d4dbmp_decode.GetPxlDecoder = D4D_GetPixel_BmpDecodeBmp2;
    d4dbmp_decode.GetDataFunc = (void*)D4D_GetDataByte;
    break;
#endif

  default:
    // there are all non supported bitmaps
    d4dbmp_decode.GetPxlDecoder = NULL;
    d4dbmp_decode.GetDataFunc = NULL;
    break;
  }


  // set up the position in file on start of data
  if(D4D_FileSeek(pFile, ((D4D_EXTSRC_HEADER_D4D*)d4d_extsrcBuffer)->dataOffset, D4D_SEEK_SET) != 0)
  {
    if(d4dbmp_decode.pPalette)
     D4D_MemFree((void*)d4dbmp_decode.pPalette);

    D4D_FileClose(pFile);
    return;
  }

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  if(radius)
  {
    LWord non_print_len;

    if(!d4dbmp_decode.GetPxlDecoder)
    {
      D4D_RBoxXY(x, y, x2, y2, D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore, D4D_ScreenGetScheme(D4D_GetActiveScreen())->bckg, radius);
      D4D_MoveToXY((D4D_COOR)(x + radius /2), (D4D_COOR)(y + radius /2));

      D4D_LineToXY((D4D_COOR)(x2 + radius /2), (D4D_COOR)(y2 + radius /2), D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore);

      D4D_MoveToXY((D4D_COOR)(x + radius /2), (D4D_COOR)(y2 + radius /2));
      D4D_LineToXY((D4D_COOR)(x2 + radius /2), (D4D_COOR)(y + radius /2), D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore);

      if(d4dbmp_decode.pPalette)
        D4D_MemFree((void*)d4dbmp_decode.pPalette);

      D4D_FileClose(pFile);

      return;
    }
    // the main drawing function
    for (y_ix = 0; y_ix < height; ++y_ix)           // do for all lines
    {
      if(y_ix < radius)
        non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(y_ix)); // get len of non printed pixels
      else if(((y2 - y) - y_ix) < radius)
        non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)((y2 - y) - y_ix)); // get len of non printed pixels
      else
        non_print_len = 0;

      (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x + non_print_len), (D4D_COOR)(y + y_ix), (D4D_COOR)(x2 - non_print_len + 1), (D4D_COOR)(y + y_ix + 1));

      for (x_ix = 0; x_ix < width; ++x_ix)           // do for all pixels on line
      {

        if((x_ix >= non_print_len) && (x_ix <= ((x2 - x) - non_print_len)))
          D4D_LLD_LCD.D4DLCD_Send_PixelColor(d4dbmp_decode.GetPxlDecoder(&d4dbmp_decode));             // and send color to display
        else
          d4dbmp_decode.GetPxlDecoder(&d4dbmp_decode); // just dummy read from file
      }
    }
  }else
#else
  D4D_UNUSED(radius);
#endif
  {
    if(!d4dbmp_decode.GetPxlDecoder)
    {
      D4D_BoxXY(x, y, x2, y2, D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore,  D4D_ScreenGetScheme(D4D_GetActiveScreen())->bckg);
      D4D_LineToXY(x2, y2, D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore);

      D4D_MoveToXY(x, y2);
      D4D_LineToXY(x2, y, D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore);

      if(d4dbmp_decode.pPalette)
        D4D_MemFree((void*)d4dbmp_decode.pPalette);

      D4D_FileClose(pFile);
      return;
    }
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x, y, x2, y2);

    x_ix = (LWord) (width * height); // Count the overall pixel count

    // the main drawing function
    for (y_ix=0; y_ix<x_ix; ++y_ix)           // do for all pixels
      D4D_LLD_LCD.D4DLCD_Send_PixelColor(d4dbmp_decode.GetPxlDecoder(&d4dbmp_decode));   // write same pixels to display
  }
#if D4D_LLD_FLUSH_ELEMENT != D4D_FALSE
  D4D_LLD_LCD.D4DLCD_FlushBuffer(D4DLCD_FLSH_ELEMENT);
#endif
  if(d4dbmp_decode.pPalette)
    D4D_MemFree((void*)d4dbmp_decode.pPalette);

  D4D_FileClose(pFile);
}

/**************************************************************//*!
*
* Internal Bitmap support
*
******************************************************************/
#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
static LWord D4D_GetDataLong(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment)
{
  LWord ret;

  if(pDecode->dataIx >= (pDecode->dataCnt / 4))
  {
    pDecode->dataCnt = D4D_FileRead(pDecode->pFile, d4d_extsrcBuffer, D4D_EXTSRC_BUFF_SIZE);

    if(pDecode->dataCnt == 0)
      return 0;

    pDecode->dataIx = 0;
  }

  ret = ((LWord*)d4d_extsrcBuffer)[pDecode->dataIx];

  if(increment)
  {
    pDecode->ix++;
    pDecode->dataIx++;
  }

  return ret;
}
#endif

#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
static Word D4D_GetDataWord(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment)
{
  Word ret;

  if(pDecode->dataIx >= (pDecode->dataCnt / 2))
  {
    pDecode->dataCnt = D4D_FileRead(pDecode->pFile, d4d_extsrcBuffer, D4D_EXTSRC_BUFF_SIZE);

    if(pDecode->dataCnt == 0)
      return 0;

    pDecode->dataIx = 0;
  }

  ret = ((Word*)d4d_extsrcBuffer)[pDecode->dataIx];

  if(increment)
  {
    pDecode->ix++;
    pDecode->dataIx++;
  }

  return ret;
}
#endif

#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_16_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_2_ENABLE != D4D_FALSE)
static Byte D4D_GetDataByte(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment)
{
  Byte ret;

  if(pDecode->dataIx >= pDecode->dataCnt)
  {
    pDecode->dataCnt = D4D_FileRead(pDecode->pFile, d4d_extsrcBuffer, D4D_EXTSRC_BUFF_SIZE);

    if(pDecode->dataCnt == 0)
      return 0;

    pDecode->dataIx = 0;
  }

  ret = ((Byte*)d4d_extsrcBuffer)[pDecode->dataIx];

  if(increment)
  {
    pDecode->ix++;
    pDecode->dataIx++;
  }

  return ret;
}
#endif


/**************************************************************//*!
*
* Get width  - BMP image file
*
******************************************************************/
D4D_COOR D4D_ImgDecExtD4DGetWidth(D4D_CHAR* pFileName)
{
  D4D_SIZE tmp_size = D4D_ImgDecExtD4DGetSize(pFileName);

  return tmp_size.cx;
}

/**************************************************************//*!
*
* Get height  - BMP image file
*
******************************************************************/
D4D_COOR D4D_ImgDecExtD4DGetHeight(D4D_CHAR* pFileName)
{
  D4D_SIZE tmp_size = D4D_ImgDecExtD4DGetSize(pFileName);

  return tmp_size.cy;
}


/**************************************************************//*!
*
* Get size  - BMP image file
*
******************************************************************/
D4D_SIZE D4D_ImgDecExtD4DGetSize(D4D_CHAR* pFileName)
{
  D4D_FILEPTR pFile;
  D4D_SIZE tmp_size = *(D4D_SIZE*)&d4d_size_zero;
  int res;

  // open the picture file
  pFile = D4D_OpenImgFile(pFileName, "r");

  // check if the file is opened
  if(pFile == NULL)
    return tmp_size;

  // set up the position in file on start of coordination overjump the version strings (to do_ is it correct?)
  if(D4D_FileSeek(pFile, 8, D4D_SEEK_SET) != 0)
  {
    // the position in file can't be sets
    D4D_FileClose(pFile);
    return tmp_size;
  }


  res = D4D_FileRead(pFile, d4d_extsrcBuffer, 4);

  D4D_FileClose(pFile);

  if(res == 4)
  {
    tmp_size.cx = D4D_READ16L(((Word*)d4d_extsrcBuffer)[0]);
    tmp_size.cy = D4D_READ16L(((Word*)d4d_extsrcBuffer)[1]);

  }
  return tmp_size;


}

/**************************************************************//*!
*
* Get header  - BMP image file
*
******************************************************************/
int D4D_ImgDecExtD4DGetHeader(D4D_CHAR* pFileName, void* pBuff, int max_size)
{
  D4D_FILEPTR pFile;
  int res;

  if(max_size < D4D_EXTSRC_HEADER_D4D_SIZE)
    return 0;

  // open the picture file
  pFile = D4D_OpenImgFile(pFileName, "r");

  // check if the file is opened
  if(pFile == NULL)
    return 0;

  // load the header of picture file

  res = D4D_FileRead(pFile, pBuff, D4D_EXTSRC_HEADER_D4D_SIZE);

  D4D_FileClose(pFile);

  return res;
}

#endif

