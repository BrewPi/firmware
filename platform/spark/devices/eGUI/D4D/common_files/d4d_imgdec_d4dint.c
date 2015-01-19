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
* @file      d4d_imgdec_d4dint.c
*
* @author     Petr Gargulak
*
* @version   0.0.16.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver external pictures d4d decoder functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_imgdec_d4dint.h"





static D4D_BOOL D4D_BmpHeaderCheckEndianism(const D4D_BMP* pBmp);
//static void D4D_LCD_DrawRBmp(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius);

#if D4D_BMP_PAL_2_ENABLE != D4D_FALSE
  D4D_COLOR D4D_GetPixel_BmpDecodeBmp2(D4D_D4DBMP_DECODE* pDecode);
#endif

#if D4D_BMP_PAL_16_ENABLE != D4D_FALSE
  D4D_COLOR D4D_GetPixel_BmpDecodeBmp16(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE)
  D4D_COLOR D4D_GetPixel_BmpDecodeBmp256(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
  D4D_COLOR D4D_GetPixel_BmpDecodeBmp65K(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
  D4D_COLOR D4D_GetPixel_BmpDecodeBmp256K(D4D_D4DBMP_DECODE* pDecode);
#endif

#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE)
  D4D_COLOR D4D_GetPixel_BmpDecodeBmp16M(D4D_D4DBMP_DECODE* pDecode);
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



/**************************************************************//*!
*
* Draw bitmap from internal memory
*
******************************************************************/
void D4D_ImgDecIntD4DDraw(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius)
{
  //  D4D_LCD_DrawRBmp(x, y, (D4D_BMP*)pFileName, greyScale, radius);


  D4D_COOR width;
  D4D_COOR height;
  LWord cmode;

  D4D_COOR x2;
  D4D_COOR y2;
  LWord x_ix, y_ix;
  void* pData;
  D4D_BOOL endian;
  D4D_D4DBMP_DECODE d4dbmp_decode;

  // check the version of bitmap header

  if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->key == D4D_IMG_HEADER_2_KEY_D4DINT)
  {
    // bitmap header version 2
    width = (D4D_COOR)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->width;
    height = (D4D_COOR)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->height;
    cmode = D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->colorMode;
    pData = (void*)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->pData;
  }
  else
  {
    Word* pHead = (Word*)((D4D_BMP*)pFileName)->pData;

    // bitmap header original version
    #if D4D_ENDIAN == D4D_ENDIAN_BIG
      D4D_UNUSED(endian);
      width = (D4D_COOR)pHead[0];
      height = (D4D_COOR)pHead[1];
      cmode = pHead[2];
      pData = pHead+3;
     #else
      endian = D4D_BmpHeaderCheckEndianism(((D4D_BMP*)pFileName));

      width = (D4D_COOR)(endian)? D4D_SwapWord(pHead[0]) : pHead[0];
      height = (D4D_COOR)(endian)? D4D_SwapWord(pHead[1]) : pHead[1];
      cmode = (endian)? D4D_SwapWord(pHead[2]) : pHead[2];
      pData = pHead+3;
     #endif
  }

  x2 = (D4D_COOR)(x + width - 1);
  y2 = (D4D_COOR)(y + height - 1);

  d4dbmp_decode.ix = 0;
  d4dbmp_decode.rptCnt = 0;
  d4dbmp_decode.curSubPxl = 0;
  d4dbmp_decode.compress = (cmode & D4D_BMP_COMPRESS_MASK);
  d4dbmp_decode.greyScale = greyScale;
  d4dbmp_decode.pBmp = pData;
  d4dbmp_decode.pPalette = (const Word*)((D4D_BMP*)pFileName)->pParam;


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
}


/**************************************************************//*!
*
* Get width  - BMP image file
*
******************************************************************/
D4D_COOR D4D_ImgDecIntD4DGetWidth(D4D_CHAR* pFileName)
{
  D4D_BMP* pBmp = (D4D_BMP*)pFileName;
  Word* pHead;

  if(pBmp == NULL)
    return 0;

  // bitmap header version 2
  if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->key == D4D_IMG_HEADER_2_KEY_D4DINT)
    return (D4D_COOR)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->width;

  pHead = (Word*)pBmp->pData;

  #if D4D_ENDIAN == D4D_ENDIAN_BIG
    return (D4D_COOR) pHead[0];
  #else
    return (D4D_COOR) (D4D_BmpHeaderCheckEndianism(pBmp))? D4D_SwapWord(pHead[0]) : pHead[0];
  #endif
}

/**************************************************************//*!
*
* Get height  - BMP image file
*
******************************************************************/
D4D_COOR D4D_ImgDecIntD4DGetHeight(D4D_CHAR* pFileName)
{
  Word* pHead;
  D4D_BMP* pBmp = (D4D_BMP*)pFileName;

  if(pBmp == NULL)
    return 0;

  // bitmap header version 2
  if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->key == D4D_IMG_HEADER_2_KEY_D4DINT)
    return (D4D_COOR)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->height;


  pHead = (Word*)pBmp->pData;

  #if D4D_ENDIAN == D4D_ENDIAN_BIG
    return (D4D_COOR) pHead[1];
  #else
    return (D4D_COOR) (D4D_BmpHeaderCheckEndianism(pBmp))? D4D_SwapWord(pHead[1]) : pHead[1];
  #endif
}


/**************************************************************//*!
*
* Get size  - BMP image file
*
******************************************************************/
D4D_SIZE D4D_ImgDecIntD4DGetSize(D4D_CHAR* pFileName)
{
  Word* pHead;
  D4D_BMP* pBmp = (D4D_BMP*)pFileName;
  D4D_SIZE tmpSize = { 0, 0};

   // bitmap header version 2
  if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->key == D4D_IMG_HEADER_2_KEY_D4DINT)
  {
    tmpSize.cx = (D4D_COOR)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->width;
    tmpSize.cy = (D4D_COOR)D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->height;
  }
  else
  {
  #if D4D_ENDIAN == D4D_ENDIAN_BIG
    if(pBmp != NULL)
    {
      pHead = (Word*)pBmp->pData;
      tmpSize.cx = (D4D_COOR) (pHead[0]);
      tmpSize.cy = (D4D_COOR) (pHead[1]);
    }
  #else
    if(pBmp != NULL)
    {
      D4D_BOOL endian = D4D_BmpHeaderCheckEndianism(pBmp);
      pHead = (Word*)pBmp->pData;
      tmpSize.cx = (D4D_COOR)(endian)? D4D_SwapWord(pHead[0]) : pHead[0];
      tmpSize.cy = (D4D_COOR)(endian)? D4D_SwapWord(pHead[1]) : pHead[1];
    }
  #endif
  }
  return tmpSize;
}

/**************************************************************//*!
*
* Get header  - BMP image file
*
******************************************************************/
int D4D_ImgDecIntD4DGetHeader(D4D_CHAR* pFileName, void* pBuff, int max_size)
{
  Word* pHead;
  D4D_BMP* pBmp = (D4D_BMP*)pFileName;
  int size;



  if(pBmp != NULL)
    return 0;
   // bitmap header version 2
  if(D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName)->key == D4D_IMG_HEADER_2_KEY_D4DINT)
  {
    if(max_size < D4D_IMG_HEADER_2_D4DINT_SIZE)
      return 0;
    size = D4D_IMG_HEADER_2_D4DINT_SIZE;
    *((D4D_IMG_HEADER_2_D4DINT*)pBuff) = *D4D_GET_IMG_HEADER_2_FROM_FILENAME(pFileName);
  }
  else
  {
    if(max_size < D4D_IMG_HEADER_D4DINT_SIZE)
    return 0;

    size = D4D_IMG_HEADER_D4DINT_SIZE;

  #if D4D_ENDIAN == D4D_ENDIAN_BIG
      pHead = (Word*)pBmp->pData;
      ((D4D_IMG_HEADER_D4DINT*)pBuff)->width = (D4D_COOR) pHead[0];
      ((D4D_IMG_HEADER_D4DINT*)pBuff)->height = (D4D_COOR) pHead[1];
      ((D4D_IMG_HEADER_D4DINT*)pBuff)->colorMode = (Word) pHead[2];
  #else
    {
      D4D_BOOL endian = D4D_BmpHeaderCheckEndianism(pBmp);
      pHead = (Word*)pBmp->pData;
      ((D4D_IMG_HEADER_D4DINT*)pBuff)->width = (D4D_COOR)(endian)? D4D_SwapWord(pHead[0]) : pHead[0];
      ((D4D_IMG_HEADER_D4DINT*)pBuff)->height = (D4D_COOR)(endian)? D4D_SwapWord(pHead[1]) : pHead[1];
      ((D4D_IMG_HEADER_D4DINT*)pBuff)->colorMode = (D4D_COOR)(endian)? D4D_SwapWord(pHead[2]) : pHead[2];
    }
  #endif
  }
  return size;
}


/**************************************************************//*!
*
* Return next pixel color from picture source
*
******************************************************************/
#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE)
D4D_COLOR D4D_GetPixel_BmpDecodeBmp16M(D4D_D4DBMP_DECODE* pDecode)
{
  // setup the startup condition (the previos color must be different than current one)
  if(!pDecode->ix)
    pDecode->pxlClrPrv = ~GET_DATA_LONG(pDecode, D4D_FALSE);


  if(pDecode->rptCnt == 0)
  {
    pDecode->pxlClr = GET_DATA_LONG(pDecode, D4D_TRUE);           // load the color of pixel

    pDecode->pxlClrRet = D4D_COLOR_FROM_888((LWord)(pDecode->pxlClr));

    if(pDecode->greyScale)
      pDecode->pxlClrRet = D4D_GetGreyScale(pDecode->pxlClrRet);   // if needed convert it to grayscale
  }

  if(pDecode->compress)                               // case that bitmap is compressed
  {
    if (pDecode->pxlClrPrv == pDecode->pxlClr)              // check the two same colors in row
    {
      if(pDecode->rptCnt == 0)
      {

        pDecode->rptCnt = GET_DATA_LONG(pDecode, D4D_TRUE);            // load the count of repeated pixels
        if(pDecode->rptCnt)
          return pDecode->pxlClrRet;
      }

      if(pDecode->rptCnt)
        pDecode->rptCnt--;

      if(!pDecode->rptCnt)
        pDecode->pxlClr = ~GET_DATA_LONG(pDecode, D4D_FALSE);            // devalue the last information of drawed pixel
    }
    pDecode->pxlClrPrv = pDecode->pxlClr;                   // store the last drawed pixel
  }

  return pDecode->pxlClrRet;
}
#endif

/**************************************************************//*!
*
* Return next pixel color from picture source
*
******************************************************************/
#if (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
D4D_COLOR D4D_GetPixel_BmpDecodeBmp256K(D4D_D4DBMP_DECODE* pDecode)
{
  // setup the startup condition (the previos color must be different than current one)
  if(!pDecode->ix)
    pDecode->pxlClrPrv = ~GET_DATA_LONG(pDecode, D4D_FALSE);;


  if(pDecode->rptCnt == 0)
  {
    pDecode->pxlClr = GET_DATA_LONG(pDecode, D4D_TRUE);;           // load the color of pixel

    pDecode->pxlClrRet = D4D_COLOR_FROM_666((LWord)(pDecode->pxlClr));

    if(pDecode->greyScale)
      pDecode->pxlClrRet = D4D_GetGreyScale(pDecode->pxlClrRet);   // if needed convert it to grayscale
  }

  if(pDecode->compress)                               // case that bitmap is compressed
  {
    if (pDecode->pxlClrPrv == pDecode->pxlClr)              // check the two same colors in row
    {
      if(pDecode->rptCnt == 0)
      {

        pDecode->rptCnt = GET_DATA_LONG(pDecode, D4D_TRUE);;            // load the count of repeated pixels
        if(pDecode->rptCnt)
          return pDecode->pxlClrRet;
      }

      if(pDecode->rptCnt)
        pDecode->rptCnt--;

      if(!pDecode->rptCnt)
        pDecode->pxlClr = ~GET_DATA_LONG(pDecode, D4D_FALSE);            // devalue the last information of drawed pixel
    }
    pDecode->pxlClrPrv = pDecode->pxlClr;                   // store the last drawed pixel
  }

  return pDecode->pxlClrRet;
}
#endif

/**************************************************************//*!
*
* Return next pixel color from picture source
*
******************************************************************/
#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
D4D_COLOR D4D_GetPixel_BmpDecodeBmp65K(D4D_D4DBMP_DECODE* pDecode)
{
  // setup the startup condition (the previos color must be different than current one)
  if(!pDecode->ix)
    pDecode->pxlClrPrv = ~GET_DATA_WORD(pDecode, D4D_FALSE);


  if(pDecode->rptCnt == 0)
  {
    pDecode->pxlClr = GET_DATA_WORD(pDecode, D4D_TRUE);           // load the color of pixel

    pDecode->pxlClrRet = D4D_COLOR_FROM_565((Word)(pDecode->pxlClr));

    if(pDecode->greyScale)
      pDecode->pxlClrRet = D4D_GetGreyScale(pDecode->pxlClrRet);   // if needed convert it to grayscale
  }

  if(pDecode->compress)                               // case that bitmap is compressed
  {
    if (pDecode->pxlClrPrv == pDecode->pxlClr)              // check the two same colors in row
    {
      if(pDecode->rptCnt == 0)
      {

        pDecode->rptCnt = GET_DATA_WORD(pDecode, D4D_TRUE);            // load the count of repeated pixels
        if(pDecode->rptCnt)
          return pDecode->pxlClrRet;
      }

      if(pDecode->rptCnt)
        pDecode->rptCnt--;

      if(!pDecode->rptCnt)
        pDecode->pxlClr = ~GET_DATA_WORD(pDecode, D4D_FALSE);            // devalue the last information of drawed pixel
    }
    pDecode->pxlClrPrv = pDecode->pxlClr;                   // store the last drawed pixel
  }


  return pDecode->pxlClrRet;

}
#endif

/**************************************************************//*!
*
* Return next pixel color from picture source
*
******************************************************************/
#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE)
D4D_COLOR D4D_GetPixel_BmpDecodeBmp256(D4D_D4DBMP_DECODE* pDecode)
{
  // setup the startup condition (the previos color must be different than current one)
  if(!pDecode->ix)
    pDecode->pxlClrPrv = ~GET_DATA_BYTE(pDecode, D4D_FALSE);


  if(pDecode->rptCnt == 0)
  {
    pDecode->pxlClr = GET_DATA_BYTE(pDecode, D4D_TRUE);           // load the color of pixel

    if(pDecode->pPalette)
      pDecode->pxlClrRet = D4D_COLOR_FROM_565(((Word*)(pDecode->pPalette))[pDecode->pxlClr]);
    else
      pDecode->pxlClrRet = D4D_COLOR_FROM_332((Byte)pDecode->pxlClr); // convert it from 332 to d4d color system



    if(pDecode->greyScale)
      pDecode->pxlClrRet = D4D_GetGreyScale(pDecode->pxlClrRet);   // if needed convert it to grayscale
  }

  if(pDecode->compress)                               // case that bitmap is compressed
  {
    if (pDecode->pxlClrPrv == pDecode->pxlClr)              // check the two same colors in row
    {
      if(pDecode->rptCnt == 0)
      {

        pDecode->rptCnt = GET_DATA_BYTE(pDecode, D4D_TRUE);            // load the count of repeated pixels
        if(pDecode->rptCnt)
          return pDecode->pxlClrRet;
      }

      if(pDecode->rptCnt)
        pDecode->rptCnt--;

      if(!pDecode->rptCnt)
        pDecode->pxlClr = ~GET_DATA_BYTE(pDecode, D4D_FALSE);            // devalue the last information of drawed pixel
    }
    pDecode->pxlClrPrv = pDecode->pxlClr;                   // store the last drawed pixel
  }


  return pDecode->pxlClrRet;

}
#endif

/**************************************************************//*!
*
* Return next pixel color from picture source
*
******************************************************************/
#if (D4D_BMP_PAL_16_ENABLE != D4D_FALSE)

static D4D_COLOR D4D_GetPxl16(D4D_D4DBMP_DECODE* pDecode)
{
  Byte ix = pDecode->pxlCode; // this solution with pointers solve endianism issue

  if(pDecode->curSubPxl++)
    ix &= 0x0f;
  else
    ix = (Byte)((ix >> 4) & 0x0f);

  if(pDecode->pPalette)
    pDecode->pxlClrRet = D4D_COLOR_FROM_565(((Word*)(pDecode->pPalette))[ix]);
  else
    pDecode->pxlClrRet =  D4D_COLOR_BLACK;          // at the moment there isn't support 16 color bitmaps without pallete

  if(pDecode->greyScale)
    pDecode->pxlClrRet = D4D_GetGreyScale(pDecode->pxlClrRet);   // if needed convert it to grayscale



  return pDecode->pxlClrRet;
}


D4D_COLOR D4D_GetPixel_BmpDecodeBmp16(D4D_D4DBMP_DECODE* pDecode)
{
  // setup the startup condition (the previos color must be different than current one)
  if(!pDecode->ix)
    pDecode->pxlClrPrv = ~GET_DATA_BYTE(pDecode, D4D_FALSE);

  pDecode->curSubPxl &= 0x01;

  if(pDecode->curSubPxl == 0)
  {
    if(pDecode->rptCnt == 0)
    {
      pDecode->pxlClr = GET_DATA_BYTE(pDecode, D4D_TRUE);           // load the color of pixel
      pDecode->pxlCode = (Byte)pDecode->pxlClr;   // to do Kinetis
    }

    if(pDecode->compress)                               // case that bitmap is compressed
    {
      if (pDecode->pxlClrPrv == pDecode->pxlClr)              // check the two same colors in row
      {
        if(pDecode->rptCnt == 0)
        {

          pDecode->rptCnt = GET_DATA_BYTE(pDecode, D4D_TRUE);            // load the count of repeated pixels
          if(pDecode->rptCnt)
            return D4D_GetPxl16(pDecode); // to do
        }

        if(pDecode->rptCnt)
          pDecode->rptCnt--;

        if(!pDecode->rptCnt)
          pDecode->pxlClr = ~GET_DATA_BYTE(pDecode, D4D_FALSE);            // devalue the last information of drawed pixel
      }
      pDecode->pxlClrPrv = pDecode->pxlClr;                   // store the last drawed pixel
    }
  }


  return D4D_GetPxl16(pDecode);
}

#endif


/**************************************************************//*!
*
* Return next pixel color from picture source
*
******************************************************************/

#if (D4D_BMP_PAL_2_ENABLE != D4D_FALSE)

static D4D_COLOR D4D_GetPxl2(D4D_D4DBMP_DECODE* pDecode)
{
  Byte ix = pDecode->pxlCode; // this solution with pointers solve endianism issue

  ix = (Byte)((ix >> (7 - (pDecode->curSubPxl))) & 0x01);

  pDecode->curSubPxl++;

  if(pDecode->pPalette)
    pDecode->pxlClrRet = D4D_COLOR_FROM_565(((Word*)(pDecode->pPalette))[ix]);
  else
    pDecode->pxlClrRet =  D4D_COLOR_BLACK;          // at the moment there isn't support monocolor bitmaps without pallete

  if(pDecode->greyScale)
    pDecode->pxlClrRet = D4D_GetGreyScale(pDecode->pxlClrRet);   // if needed convert it to grayscale



  return pDecode->pxlClrRet;
}

D4D_COLOR D4D_GetPixel_BmpDecodeBmp2(D4D_D4DBMP_DECODE* pDecode)
{
  // setup the startup condition (the previos color must be different than current one)
  if(!pDecode->ix)
    pDecode->pxlClrPrv = ~GET_DATA_BYTE(pDecode, D4D_FALSE);

  pDecode->curSubPxl &= 0x07;

  if(pDecode->curSubPxl == 0)
  {
    if(pDecode->rptCnt == 0)
    {
      pDecode->pxlClr = GET_DATA_BYTE(pDecode, D4D_TRUE);           // load the color of pixel
      pDecode->pxlCode = (Byte)pDecode->pxlClr;   // to do Kinetis
      //Byte ix = (*(Byte*)(&pDecode->pxlClr)); // this solution with pointers solve endianism issue

    }

    if(pDecode->compress)                               // case that bitmap is compressed
    {
      if (pDecode->pxlClrPrv == pDecode->pxlClr)              // check the two same colors in row
      {
        if(pDecode->rptCnt == 0)
        {

          pDecode->rptCnt = GET_DATA_BYTE(pDecode, D4D_TRUE);            // load the count of repeated pixels
          if(pDecode->rptCnt)
            return D4D_GetPxl2(pDecode);
        }

        if(pDecode->rptCnt)
          pDecode->rptCnt--;

        if(!pDecode->rptCnt)
          pDecode->pxlClr = ~GET_DATA_BYTE(pDecode, D4D_FALSE);            // devalue the last information of drawed pixel
      }
      pDecode->pxlClrPrv = pDecode->pxlClr;                   // store the last drawed pixel
    }
  }


  return D4D_GetPxl2(pDecode);
}

#endif


static D4D_BOOL D4D_BmpHeaderCheckEndianism(const D4D_BMP* pBmp)
{
  Word* pHead = (Word*)pBmp->pData;

  if(pHead[2] & D4D_BMP_ENDIAN_FLAG)
    return D4D_FALSE;
  else
    return D4D_TRUE;
}

/**************************************************************//*!
*
* Internal Bitmap support
*
******************************************************************/
#if (D4D_BMP_16MNOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_256KNOPAL_ENABLE != D4D_FALSE)
static LWord D4D_GetDataLong(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment)
{
  LWord ret = ((LWord*)pDecode->pBmp)[pDecode->ix];

  if(increment)
    pDecode->ix++;

  return ret;
}
#endif

#if (D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE)
static Word D4D_GetDataWord(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment)
{
  Word ret = ((Word*)pDecode->pBmp)[pDecode->ix];

  if(increment)
    pDecode->ix++;

  return ret;
}
#endif

#if (D4D_BMP_256NOPAL_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_256_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_16_ENABLE != D4D_FALSE) || (D4D_BMP_PAL_2_ENABLE != D4D_FALSE)
static Byte D4D_GetDataByte(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment)
{
  Byte ret = ((Byte*)pDecode->pBmp)[pDecode->ix];

  if(increment)
    pDecode->ix++;

  return ret;
}
#endif
