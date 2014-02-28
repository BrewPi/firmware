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
* @file      d4d_imgdec_d4dint.h
*
* @author     Petr Gargulak
*
* @version   0.0.8.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver external pictures d4d decoder functions header file
*
******************************************************************************/

#ifndef __D4D_IMGDEC_D4DINT_H
#define __D4D_IMGDEC_D4DINT_H




#define GET_DATA_BYTE(pBmp_dec, inc) ((GET_DATA_BYTE_FUNC)pBmp_dec->GetDataFunc)(pBmp_dec, inc)
#define GET_DATA_WORD(pBmp_dec, inc) ((GET_DATA_WORD_FUNC)pBmp_dec->GetDataFunc)(pBmp_dec, inc)
#define GET_DATA_LONG(pBmp_dec, inc) ((GET_DATA_LONG_FUNC)pBmp_dec->GetDataFunc)(pBmp_dec, inc)

// the unique ID to recognize new header of internal bitmaps
#define D4D_IMG_HEADER_2_KEY_D4DINT     0x50455452


#define D4D_BMP_COMPRESS_MASK   0x00000001

#define D4D_BMP_COMPRESSED      0x00000001
#define D4D_BMP_UNCOMPRESSED    0x00000000

#define D4D_BMP_TYPE_MASK       0x0000001E

#define D4D_BMP_16MNOPAL        0x00000014
#define D4D_BMP_256KNOPAL       0x00000012
#define D4D_BMP_65536NOPAL      0x00000000
#define D4D_BMP_4096NOPAL       0x00000002
#define D4D_BMP_256NOPAL        0x00000004


#define D4D_BMP_PAL256_32       0x00000006
#define D4D_BMP_PAL16_8         0x00000008
#define D4D_BMP_PAL4            0x0000000A
#define D4D_BMP_PAL2            0x0000000C
#define D4D_BMP_MON_VER         0x00000010

#define D4D_BMP_CLR_DEPTH_MASK  0x00000700

#define D4D_BMP_CLR_DEPTH256    0x00000000
#define D4D_BMP_CLR_DEPTH128    0x00000100
#define D4D_BMP_CLR_DEPTH64     0x00000200
#define D4D_BMP_CLR_DEPTH32     0x00000300
#define D4D_BMP_CLR_DEPTH16     0x00000400
#define D4D_BMP_CLR_DEPTH8      0x00000500
#define D4D_BMP_CLR_DEPTH4      0x00000600
#define D4D_BMP_CLR_DEPTH2      0x00000700

#define D4D_BMP_ENDIAN_FLAG     0x00008000

#define D4D_BMP_PAL_256         (D4D_BMP_PAL256_32 | D4D_BMP_CLR_DEPTH256)
#define D4D_BMP_PAL_16          (D4D_BMP_PAL16_8 | D4D_BMP_CLR_DEPTH16)
#define D4D_BMP_PAL_2           (D4D_BMP_PAL2 | D4D_BMP_CLR_DEPTH2)


#ifndef D4D_BMP_16MNOPAL_ENABLE
  #define D4D_BMP_16MNOPAL_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMP_256KNOPAL_ENABLE
  #define D4D_BMP_256KNOPAL_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMP_65536NOPAL_ENABLE
  #define D4D_BMP_65536NOPAL_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMP_256NOPAL_ENABLE
  #define D4D_BMP_256NOPAL_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMP_PAL_256_ENABLE
  #define D4D_BMP_PAL_256_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMP_PAL_16_ENABLE
  #define D4D_BMP_PAL_16_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMP_PAL_2_ENABLE
  #define D4D_BMP_PAL_2_ENABLE D4D_FALSE
#endif




typedef struct D4D_IMG_HEADER_D4DINT_S
{
  unsigned short  width;        // width of bitmap
  unsigned short  height;       // height of bitmap
  unsigned short  colorMode;    // color mode of stored bitmap
}D4D_IMG_HEADER_D4DINT;

typedef struct D4D_IMG_HEADER_2_D4DINT_S
{
  unsigned long   key;          // unique key that points to new format of internal bitmaps
  unsigned short  width;        // width of bitmap
  unsigned short  height;       // height of bitmap
  unsigned long  colorMode;    // color mode of stored bitmap
  const void * pData;           // pointer on picture data
}D4D_IMG_HEADER_2_D4DINT;


/******************************************************************************
* Private Types
******************************************************************************/

typedef struct D4D_D4DBMP_DECODE_S
{
  LWord       pxlClr;
  LWord       pxlClrPrv;
  Byte        pxlCode;
  D4D_COLOR   pxlClrRet;
  LWord       ix;
  LWord       rptCnt;
  Byte        curSubPxl;
  D4D_BOOL    compress;
  D4D_BOOL    greyScale;
  const void* pBmp;
  const void* pPalette;
  void*       GetDataFunc;
  D4D_COLOR   (*GetPxlDecoder)(struct D4D_D4DBMP_DECODE_S* pDecode);

#if (D4D_BMP_EXTSRC_SUPPORT_D4D == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
  LWord       dataCnt;
  LWord       dataIx;
  D4D_FILEPTR pFile;
#endif
}D4D_D4DBMP_DECODE;


typedef Byte (*GET_DATA_BYTE_FUNC)(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment);
typedef Word (*GET_DATA_WORD_FUNC)(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment);
typedef LWord (*GET_DATA_LONG_FUNC)(D4D_D4DBMP_DECODE* pDecode, D4D_BOOL increment);

/**************************************************************************/ /*!
* @brief   Macro that define the size of original header of bitmap
*******************************************************************************/
#define D4D_IMG_HEADER_D4DINT_SIZE sizeof(D4D_IMG_HEADER_D4DINT)

/**************************************************************************/ /*!
* @brief   Macro that define the size of version 2 header of bitmap
*******************************************************************************/
#define D4D_IMG_HEADER_2_D4DINT_SIZE sizeof(D4D_IMG_HEADER_2_D4DINT)

#define D4D_DECLARE_IMG_HEADER_2_D4DINT(name, size_x, size_y, c_mode, pData) \
  const D4D_IMG_HEADER_2_D4DINT name = { \
    D4D_IMG_HEADER_2_KEY_D4DINT, \
    size_x, \
    size_y, \
    c_mode, \
    &pData };



/******************************************************************************
* Private Macro's
******************************************************************************/

#define D4D_GET_IMG_HEADER_2_FROM_FILENAME(file) ((D4D_IMG_HEADER_2_D4DINT*)(((D4D_BMP*)file)->pData))

#define D4D_GET_IMG_HEADER_FROM_FILENAME(file) ((D4D_IMG_HEADER_D4DINT*)(((D4D_BMP*)file)->pData))

/******************************************************************************
* Global D4D bitmap drawing level functions
******************************************************************************/

#if 1
  void D4D_ImgDecIntD4DDraw(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius);
  D4D_COOR D4D_ImgDecIntD4DGetWidth(D4D_CHAR* pFileName);
  D4D_COOR D4D_ImgDecIntD4DGetHeight(D4D_CHAR* pFileName);
  D4D_SIZE D4D_ImgDecIntD4DGetSize(D4D_CHAR* pFileName);
  int D4D_ImgDecIntD4DGetHeader(D4D_CHAR* pFileName, void* pBuff, int max_size);
#else
  #define D4D_ImgDecIntD4DDraw       NULL
  #define D4D_ImgDecIntD4DGetWidth   NULL
  #define D4D_ImgDecIntD4DGetHeight  NULL
  #define D4D_ImgDecIntD4DGetSize    NULL
  #define D4D_ImgDecIntD4DGetHeader  NULL
#endif

#endif /* __D4D_IMGDEC_D4DINT_H */
