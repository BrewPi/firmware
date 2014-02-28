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
* @file      d4d_imgdec_bmp.h
*
* @author     Petr Gargulak
*
* @version   0.0.8.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver external bmp files decoder header file
*
******************************************************************************/

#ifndef __D4D_IMGDEC_BMP_H
#define __D4D_IMGDEC_BMP_H

/******************************************************************************
* Types
******************************************************************************/

#pragma pack (push)
#pragma pack (1)
typedef struct {
  Word    magic;      // the magic must be "BM" 66 77      0x424D
  LWord   size;       // Size of the BMP file
  Word    reserved0;  // Application specific
  Word    reserved1;  // Application specific
  LWord   dataOff;    // Offset where the Pixel Array (bitmap data) can be found
} D4D_BITMAPHEADER;


typedef struct {
  Word    header_sz;   // 12 Bytes
  Word    width;
  Word    height;
  Word    nplanes;
  Word    bitspp;
} D4D_BITMAPCOREHEADER;

typedef struct {
  LWord   headerSz;       // 40 Bytes Number of bytes in the DIB header (from this point)
  sLWord  width;          // Width of the bitmap in pixels
  sLWord  height;         // Height of the bitmap in pixels
  Word    nplanes;        // Number of color planes being used
  Word    bitspp;         // Number of bits per pixel
  LWord   compressType;   // Pixel Array compression used
  LWord   bmpBytesz;      // Size of the raw data in the Pixel Array (including padding)
  sLWord  hres;           // Horizontal resolution of the image
  sLWord  vres;           // Vertical resolution of the image
  LWord   ncolors;        // Number of colors in the palette
  LWord   nimpcolors;     // Important colors 	0 means all colors are important
} D4D_BITMAPINFOHEADER;

#pragma pack (pop)

typedef enum {
  D4D_BC_RGB = 0,
  D4D_BC_RLE8,
  D4D_BC_RLE4,
  D4D_BC_BITFIELDS, //Also Huffman 1D compression for BITMAPCOREHEADER2
  D4D_BC_JPEG,      //Also RLE-24 compression for BITMAPCOREHEADER2
  D4D_BC_PNG
} D4D_BITMAPCOMPRESSION;

typedef struct D4D_RGBA_S
{
	Byte Red;
	Byte Green;
	Byte Blue;
	Byte Alpha;
} D4D_RGBA;

typedef struct D4D_BGRA_S
{
	Byte Blue;
	Byte Green;
	Byte Red;
	Byte Alpha;
} D4D_BGRA;

typedef union D4D_LONG_S
{
  LWord complete;
  struct
  {
    Word high;
    Word low;
  }Words;
  struct
  {
    Byte hh;
    Byte hl;
    Byte lh;
    Byte ll;
  }Bytes;
}D4D_LONG;
/******************************************************************************
* Constants
******************************************************************************/

#if D4D_ENDIAN == D4D_ENDIAN_LITTLE
	#define D4D_BITMAP_SIGNATURE 0x4d42
#else
	#define D4D_BITMAP_SIGNATURE 0x424d
#endif

#define D4D_BITMAPHEADER_SIZE sizeof(D4D_BITMAPHEADER)

#define D4D_BITMAPINFOHEADER_SIZE sizeof(D4D_BITMAPINFOHEADER)

#define D4D_TMP_PBITMAPHEADER ((D4D_BITMAPHEADER*)d4d_extsrcBuffer)

#define D4D_TMP_PBITMAPINFOHEADER ((D4D_BITMAPINFOHEADER*)d4d_extsrcBuffer)

/******************************************************************************
* External source constants
******************************************************************************/




/******************************************************************************
* Global D4D bitmap drawing level functions
******************************************************************************/
#if (D4D_BMP_EXTSRC_SUPPORT_BMP == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)

  void D4D_ImgDecBmpDraw(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius);
  D4D_COOR D4D_ImgDecBmpGetWidth(D4D_CHAR* pFileName);
  D4D_COOR D4D_ImgDecBmpGetHeight(D4D_CHAR* pFileName);
  D4D_SIZE D4D_ImgDecBmpGetSize(D4D_CHAR* pFileName);
  int D4D_ImgDecBmpGetHeader(D4D_CHAR* pFileName, void* pBuff, int max_size);
  D4D_BOOL D4D_PrintScreenToBMP(D4D_CHAR* pFileName);

#else
  #define D4D_ImgDecBmpDraw       NULL
  #define D4D_ImgDecBmpGetWidth   NULL
  #define D4D_ImgDecBmpGetHeight  NULL
  #define D4D_ImgDecBmpGetSize    NULL
  #define D4D_ImgDecBmpGetHeader  NULL
  #define D4D_PrintScreenToBMP(x) (D4D_FALSE)
#endif

#endif /* __D4D_IMGDEC_BMP_H */
