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
* @file      d4d_imgdec_d4d.h
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Feb-19-2013
*
* @brief     D4D driver external pictures d4d decoder functions header file
*
******************************************************************************/

#ifndef __D4D_IMGDEC_D4D_H
#define __D4D_IMGDEC_D4D_H

/******************************************************************************
* Types
******************************************************************************/

#pragma pack (push)
#pragma pack (1)

typedef struct D4D_EXTSRC_HEADER_D4D_S
{
  char            ident[7];     // identification string of file "D4DBMP" include termination char
  unsigned char   version;      // version of d4d bitmap header version (at the moment 0)
  unsigned short  width;        // width of bitmap
  unsigned short  height;       // height of bitmap
  unsigned short  colorMode;    // color mode of stored bitmap
  unsigned short  reserved;     // allign  2 bytes to 32 bit shape
  unsigned long   dataOffset;   // offset of first byte of bitmap data
  unsigned long   dataSize;     // size of bitmap data
  unsigned long   palleteOffset;// offset of first byte of pallete data. If 0 there is no pallete
  unsigned long   palleteSize;  // size of pallete data
}D4D_EXTSRC_HEADER_D4D;

typedef struct D4D_EXTSRC_HEADER_D4D_1_S
{
  char            ident[7];     // identification string of file "D4DBMP" include termination char
  unsigned char   version;      // version of d4d bitmap header version (at the moment 0)
  unsigned short  width;        // width of bitmap
  unsigned short  height;       // height of bitmap
  unsigned long  colorMode;    // color mode of stored bitmap
  unsigned long   dataOffset;   // offset of first byte of bitmap data
  unsigned long   dataSize;     // size of bitmap data
  unsigned long   palleteOffset;// offset of first byte of pallete data. If 0 there is no pallete
  unsigned long   palleteSize;  // size of pallete data
}D4D_EXTSRC_HEADER_D4D_1;


#pragma pack (pop)

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* External source constants
******************************************************************************/


#define D4D_EXTSRC_HEADER_D4D_SIZE sizeof(D4D_EXTSRC_HEADER_D4D)



/******************************************************************************
* Global D4D bitmap drawing level functions
******************************************************************************/

#if (D4D_BMP_EXTSRC_SUPPORT_D4D == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
  void D4D_ImgDecExtD4DDraw(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius);
  D4D_COOR D4D_ImgDecExtD4DGetWidth(D4D_CHAR* pFileName);
  D4D_COOR D4D_ImgDecExtD4DGetHeight(D4D_CHAR* pFileName);
  D4D_SIZE D4D_ImgDecExtD4DGetSize(D4D_CHAR* pFileName);
  int D4D_ImgDecExtD4DGetHeader(D4D_CHAR* pFileName, void* pBuff, int max_size);

  void D4D_LCD_Bmp65kNoPalExt(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,
                       D4D_FILEPTR pFile, LWord dataOffset,
                       D4D_BOOL Compressed, D4D_BOOL GreyScale);
#else

  #define D4D_ImgDecExtD4DDraw       NULL
  #define D4D_ImgDecExtD4DGetWidth   NULL
  #define D4D_ImgDecExtD4DGetHeight  NULL
  #define D4D_ImgDecExtD4DGetSize    NULL
  #define D4D_ImgDecExtD4DGetHeader  NULL

#endif

#endif /* __D4D_IMGDEC_D4D_H */
