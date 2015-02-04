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
* @file      d4d_imgdec_bmp.c
*
* @author     Petr Gargulak
*
* @version   0.0.17.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver external bmp files decoder functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_imgdec_bmp.h"

#if (D4D_BMP_EXTSRC_SUPPORT_BMP == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
static void D4D_PrintPixel(D4D_COLOR color, D4D_BOOL greyScale);

static void D4D_PrintPixel(D4D_COLOR color, D4D_BOOL greyScale)
{
  if(greyScale)
    color = D4D_GetGreyScale(color);   // if needed convert it to grayscale
  D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);             // and send color to display
}


/**************************************************************//*!
*
* Draw BMP image file
*
******************************************************************/
void D4D_ImgDecBmpDraw(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius)
{
  D4D_FILEPTR pFile;
  D4D_BITMAPINFOHEADER bitmapInfo;
  D4D_BGRA* pPalleteBgra = NULL;
  D4D_COOR locX, locY;
  D4D_COLOR color;
  LWord dataLineLength;
  LWord dataLoadedCnt;
  LWord dataIx, dataBuffIx, i;
  LWord dataOffset;
  D4D_LONG tmpLong;
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
  LWord non_print_len;
  D4D_BOOL printPixel;
#endif
  // open the picture file
  pFile =  D4D_OpenImgFile(pFileName, "r");

  // check if the file is opened
  if(pFile == NULL)
    return;

  // load the header of picture file

  #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(D4D_BITMAPHEADER_SIZE <= D4D_EXTSRC_BUFF_SIZE);
  #endif

  /********************************************************************************************
  *
  *      Load the bitmap headers
  *
  ********************************************************************************************/

  // read the file info
  if(D4D_FileRead(pFile, d4d_extsrcBuffer, D4D_BITMAPHEADER_SIZE) != D4D_BITMAPHEADER_SIZE)
  {
    // the header isn't loaded succesfully
    D4D_FileClose(pFile);
    return;
  }

  // check if this is bitmap and if it's supported
  if(D4D_TMP_PBITMAPHEADER->magic != D4D_BITMAP_SIGNATURE)
  {
    //this is not the bitmap file in supported format
    D4D_FileClose(pFile);
    return;
  }

  // save the data offset
  dataOffset = D4D_READ32L(D4D_TMP_PBITMAPHEADER->dataOff);

  // load the Bitmap info one of base header bitmap type (it should be enough for eGUI purposes)
  if(D4D_FileRead(pFile,(void*)&bitmapInfo, D4D_BITMAPINFOHEADER_SIZE) != D4D_BITMAPINFOHEADER_SIZE)
  {
    // the header isn't loaded succesfully
    D4D_FileClose(pFile);
    return;
  }

#if D4D_ENDIAN == D4D_ENDIAN_BIG
  // Fill up right the bmp info (solve endianism)
  bitmapInfo.headerSz = D4D_READ32L(bitmapInfo.headerSz);
  bitmapInfo.width = D4D_READ32L(bitmapInfo.width);
  bitmapInfo.height = D4D_READ32L(bitmapInfo.height);
  bitmapInfo.nplanes = D4D_READ16L(bitmapInfo.nplanes);
  bitmapInfo.bitspp = D4D_READ16L(bitmapInfo.bitspp);
  bitmapInfo.compressType = D4D_READ32L(bitmapInfo.compressType);
  bitmapInfo.bmpBytesz = D4D_READ32L(bitmapInfo.bmpBytesz);
  bitmapInfo.hres = D4D_READ32L(bitmapInfo.hres);
  bitmapInfo.vres = D4D_READ32L(bitmapInfo.vres);
  bitmapInfo.ncolors = D4D_READ32L(bitmapInfo.ncolors);
  bitmapInfo.nimpcolors = D4D_READ32L(bitmapInfo.nimpcolors);
#endif

  // check the size parameters of bitmap - Could be in negative
  if(bitmapInfo.width < 0)
    bitmapInfo.width *= -1;

  if(bitmapInfo.height < 0)
    bitmapInfo.height *= -1;

  // check the number of planes - MUST be 1
  if(bitmapInfo.nplanes != 1)
  {
    // the count of planes doesn't match the standard
    D4D_FileClose(pFile);
    return;
  }


  /********************************************************************************************
  *
  *      Load the bitmap color pallete
  *
  ********************************************************************************************/

  // setup position of bitmap pallete
  if(D4D_FileSeek(pFile, D4D_BITMAPHEADER_SIZE + bitmapInfo.headerSz, D4D_SEEK_SET) != 0)
  {
    // the position in file can't be sets
    D4D_FileClose(pFile);
    return;
  }
    if(!bitmapInfo.ncolors)
    {
      if((D4D_BITMAPCOMPRESSION)(bitmapInfo.compressType) == D4D_BC_RGB)
      {
        switch(bitmapInfo.bitspp)
        {
          case 1:
            bitmapInfo.ncolors = 2;
            break;
          case 4:
            bitmapInfo.ncolors = 16;
            break;
          case 8:
            bitmapInfo.ncolors = 256;
            break;
        }
      }else if((D4D_BITMAPCOMPRESSION)(bitmapInfo.compressType) == D4D_BC_RLE8)
      {
        bitmapInfo.ncolors = 256;
      }
    }


    if(bitmapInfo.ncolors)
    {
      LWord palleteSize = sizeof(D4D_BGRA) * bitmapInfo.ncolors;

      // alloc the memory for pallete
      pPalleteBgra = D4D_MemAlloc(palleteSize);
      if(pPalleteBgra == NULL)
      {
        // the Bitmap pallete isn't loaded succesfully
        D4D_FileClose(pFile);
        return;
      }

      // load the Bitmap pallete
      if(D4D_FileRead(pFile, pPalleteBgra, palleteSize) != palleteSize)
      {
        // the Bitmap pallete isn't loaded succesfully
        if(pPalleteBgra)
          D4D_MemFree(pPalleteBgra);
        D4D_FileClose(pFile);
        return;
      }

      if(greyScale)
      {
        int c,i;
        for(i = 0; i < bitmapInfo.ncolors; i++)
        {
          c = pPalleteBgra[i].Red + pPalleteBgra[i].Green + pPalleteBgra[i].Blue;
          c /= 3;

          pPalleteBgra[i].Red = c;
          pPalleteBgra[i].Green = c;
          pPalleteBgra[i].Blue = c;

        }
      }

    }


  /********************************************************************************************
  *
  *      Start Drawing the picture on screen
  *
  ********************************************************************************************/

  // setup position of bitmap data
  if(D4D_FileSeek(pFile, dataOffset, D4D_SEEK_SET) != 0)
  {
    // the position in file can't be sets
    D4D_FileClose(pFile);
    if(pPalleteBgra)
      D4D_MemFree(pPalleteBgra);
    return;
  }



  // Check the compression type and select the right decode method
  switch((D4D_BITMAPCOMPRESSION)(bitmapInfo.compressType))
  {
    // Uncompressed BMP type
    case D4D_BC_RGB:
      /********************************************************************************************
      *
      *      Not compressed BMP types
      *
      ********************************************************************************************/

      // Compute the one line length in source file
      dataLineLength = (int)((((bitmapInfo.width * bitmapInfo.bitspp) / 8) + 3) & ~3);

      // do all lines
      for(locY=0; locY < bitmapInfo.height; locY++)
      {
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
        if(locY < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(locY)); // get len of non printed pixels
        else if((bitmapInfo.height - 1 - locY) < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(bitmapInfo.height - 1 - locY)); // get len of non printed pixels
        else
          non_print_len = 0;
#endif

        // Define window on screen
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
        (void)D4D_LLD_LCD.D4DLCD_SetWindow (x + non_print_len, (D4D_COOR)(y + bitmapInfo.height - locY - 1), (D4D_COOR)(x + bitmapInfo.width - non_print_len), (D4D_COOR)(y + bitmapInfo.height - locY - 1));
#else
        (void)D4D_LLD_LCD.D4DLCD_SetWindow (x, (D4D_COOR)(y + bitmapInfo.height - locY - 1), (D4D_COOR)(x + bitmapInfo.width), (D4D_COOR)(y + bitmapInfo.height - locY - 1));
#endif
        // setup position of bitmap data
        if(D4D_FileSeek(pFile, dataOffset + locY * dataLineLength, D4D_SEEK_SET) != 0)
        {
          // the position in file can't be sets
          D4D_FileClose(pFile);
          if(pPalleteBgra)
            D4D_MemFree(pPalleteBgra);
          return;
        }

        // reset the index counter of line data
        dataIx = 0;
        dataLoadedCnt = 0;
        dataBuffIx = 0;          // just to notify that the buffer is empty

        // do all pixels on line
        for(locX=0; locX < bitmapInfo.width; locX++)
        {
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
          printPixel = D4D_TRUE;
          if(non_print_len)
          {
            if((locX < non_print_len) || ((bitmapInfo.width - 1 - locX) < non_print_len))
              printPixel = D4D_FALSE;
          }
#endif

          //if(dataIx % D4D_EXTSRC_BUFF_SIZE > (bitmapInfo.bitspp / 8))
          if(dataBuffIx + (bitmapInfo.bitspp / 8) > dataLoadedCnt )
          {

            // setup position of bitmap data
            if(D4D_FileSeek(pFile, -1 * (dataLoadedCnt - dataBuffIx), D4D_SEEK_CUR) != 0)
            {
              // the position in file can't be sets
              D4D_FileClose(pFile);
              if(pPalleteBgra)
                D4D_MemFree(pPalleteBgra);
              return;
            }

            dataBuffIx = 0;

            if((dataLineLength - dataIx) <= D4D_EXTSRC_BUFF_SIZE)
              dataLoadedCnt = (dataLineLength - dataIx);
            else
              dataLoadedCnt = D4D_EXTSRC_BUFF_SIZE;


            // load the Bitmap pallete
            if(D4D_FileRead(pFile, d4d_extsrcBuffer, dataLoadedCnt) != dataLoadedCnt)
            {
              // the Bitmap data isn't loaded succesfully
              D4D_FileClose(pFile);
                if(pPalleteBgra)
                  D4D_MemFree(pPalleteBgra);
              return;
            }
          }



          // check the color depth
          switch(bitmapInfo.bitspp)
          {
            /********************************************************************************************
            *      MonoChromatic colors
            ********************************************************************************************/
            case 1:
              tmpLong.complete = 0;

              i = d4d_extsrcBuffer[dataBuffIx];

              for(; tmpLong.complete < 8; tmpLong.complete++)
              {
                if(locX < bitmapInfo.width)
  						  {
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
                  if(printPixel)
#endif
                  {
                    color = D4D_COLOR_RGB(pPalleteBgra[(i & 0x80)? 1:0].Red, pPalleteBgra[(i & 0x80)? 1:0].Green, pPalleteBgra[(i & 0x80)? 1:0].Blue);
                      D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);             // and send color to display
                  }

  						    locX++;
  						    i <<= 1;

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
  						    printPixel = D4D_TRUE;
                  if(non_print_len)
                  {
                    if((locX < non_print_len) || ((bitmapInfo.width - 1 - locX) < non_print_len))
                      printPixel = D4D_FALSE;
                  }
#endif
  						  }else
  						    break;

              }

              locX--;


              dataIx++;
              dataBuffIx++;
              break;

            /********************************************************************************************
            *      16 Color depth of pallete
            ********************************************************************************************/
            case 4:
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
              if(printPixel)
#endif
  						{
                tmpLong.Bytes.ll = d4d_extsrcBuffer[dataBuffIx];

                tmpLong.Bytes.ll >>= 4;

                color = D4D_COLOR_RGB(pPalleteBgra[tmpLong.Bytes.ll].Red, pPalleteBgra[tmpLong.Bytes.ll].Green, pPalleteBgra[tmpLong.Bytes.ll].Blue);
                D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);             // and send color to display
  						}

  						locX++;

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
  						printPixel = D4D_TRUE;
              if(non_print_len)
              {
                if((locX < non_print_len) || ((bitmapInfo.width - 1 - locX) < non_print_len))
                  printPixel = D4D_FALSE;
              }
#endif
  						if(locX < bitmapInfo.width)
  						{
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
                if(printPixel)
#endif
  						  {
    						  tmpLong.Bytes.ll = d4d_extsrcBuffer[dataBuffIx];

                  tmpLong.Bytes.ll &= 0x0f;

                  color = D4D_COLOR_RGB(pPalleteBgra[tmpLong.Bytes.ll].Red, pPalleteBgra[tmpLong.Bytes.ll].Green, pPalleteBgra[tmpLong.Bytes.ll].Blue);
                  D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);             // and send color to display
  						  }
  						}


              dataIx++;
              dataBuffIx++;
              break;

            /********************************************************************************************
            *      256 Color depth of pallete
            ********************************************************************************************/
            case 8:
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
              if(printPixel)
#endif
  						{
                color = D4D_COLOR_RGB(pPalleteBgra[d4d_extsrcBuffer[dataBuffIx]].Red, pPalleteBgra[d4d_extsrcBuffer[dataBuffIx]].Green, pPalleteBgra[d4d_extsrcBuffer[dataBuffIx]].Blue);
                D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);             // and send color to display
  						}

  						dataIx++;
              dataBuffIx++;
              break;

            /********************************************************************************************
            *      565 Colors depth
            ********************************************************************************************/
            case 16:
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
              if(printPixel)
#endif
  						{
                tmpLong.Words.low = D4D_READ16L(*((Word*)(&d4d_extsrcBuffer[dataBuffIx])));

                // the BGR 555 system 15 bits is probably default 16 bit system used in bitmap - PROBABLY - Who knows :-)
                color = D4D_COLOR_RGB(D4D_COLOR_RGB555_GET_R(tmpLong.Words.low), D4D_COLOR_RGB555_GET_G(tmpLong.Words.low), D4D_COLOR_RGB555_GET_B(tmpLong.Words.low));
                D4D_PrintPixel(color, greyScale);
  						}

              dataIx +=2;
              dataBuffIx +=2;
              break;

            /********************************************************************************************
            *      24- bit Colors depth
            *******************************************************************************************/
            case 24:
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
              if(printPixel)
#endif
  						{
                tmpLong.complete = *((LWord*)(&d4d_extsrcBuffer[dataBuffIx]));

                color = D4D_COLOR_RGB(((D4D_BGRA*)&(tmpLong.complete))->Red, ((D4D_BGRA*)&(tmpLong.complete))->Green, ((D4D_BGRA*)&(tmpLong.complete))->Blue);

                D4D_PrintPixel(color, greyScale);
  						}

              dataIx +=3;
              dataBuffIx +=3;
              break;

            /********************************************************************************************
            *      32- bit Colors depth
            ********************************************************************************************/
            case 32:
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
              if(printPixel)
#endif
  						{
                tmpLong.complete = *((LWord*)(&d4d_extsrcBuffer[dataBuffIx]));

                color = D4D_COLOR_RGB(((D4D_BGRA*)&(tmpLong.complete))->Red, ((D4D_BGRA*)&(tmpLong.complete))->Green, ((D4D_BGRA*)&(tmpLong.complete))->Blue);

                D4D_PrintPixel(color, greyScale);
  						}
              dataIx +=4;
              dataBuffIx +=4;
              break;
          }
        }
      }


      break;

    case D4D_BC_RLE8:
      /********************************************************************************************
      *
      *      RLE 8 Compression type
      *
      ********************************************************************************************/
      {

        #define COUNTER 0
        #define COLOR_IX 1

        #define OFFSET_X 2
        #define OFFSET_Y 3
        #define COLOR 4

        locX = x;
        locY = y + bitmapInfo.height - 1;

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
        non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, 0); // get len of non printed pixels for first line
#endif
        while(D4D_FileRead(pFile, d4d_extsrcBuffer, 2) == 2)
        {

          dataOffset +=2;

          if(d4d_extsrcBuffer[COUNTER] > 0)
          {
            D4D_COOR x1, x2;

            color = D4D_COLOR_RGB(pPalleteBgra[d4d_extsrcBuffer[COLOR_IX]].Red, pPalleteBgra[d4d_extsrcBuffer[COLOR_IX]].Green, pPalleteBgra[d4d_extsrcBuffer[COLOR_IX]].Blue);

            x1 = locX;
            x2 = (D4D_COOR)(locX + d4d_extsrcBuffer[COUNTER]);

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
            if(x1 < x + non_print_len)
              x1 = x + non_print_len;

            if(x2 > (x + bitmapInfo.width - 1 - non_print_len))
              x2 = (x + bitmapInfo.width - 1 - non_print_len);

            if(x2 > x1)
#endif
            {
              // Define window on screen
              (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, locY, x2, locY);

              for(i=0;i<(x2-x1);i++)
                D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);             // and send color to display
            }
            locX += d4d_extsrcBuffer[COUNTER];

          }
          else
          {
            /********************************************************************************************
            *		  Second byte           Meaning
            *
            *    				 0              End of line
            *    				 1              End of bitmap
            *    				 2              Delta. The next two bytes are the horizontal
            *    								          and vertical offsets from the current position
            *    								          to the next pixel.
            *    			 3-255            Switch to absolute mode
            ********************************************************************************************/

            switch(d4d_extsrcBuffer[COLOR_IX])
            {
              case 0:
                locX = x;
                locY--;
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
                if((locY - y) < radius)
                  non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(locY - y)); // get len of non printed pixels
                else if((bitmapInfo.height - 1 - (locY - y)) < radius)
                  non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(bitmapInfo.height - 1 - (locY - y))); // get len of non printed pixels
                else
                  non_print_len = 0;
#endif
                break;

              case 1:
                // the bitmap is completelly drawed
                D4D_FileClose(pFile);
                if(pPalleteBgra)
                  D4D_MemFree(pPalleteBgra);
                return;

              case 2:
                // change offset of cursor position
                if(D4D_FileRead(pFile, &d4d_extsrcBuffer[OFFSET_X], 2) != 2)
                {
                  // the header isn't loaded succesfully
                  D4D_FileClose(pFile);
                  if(pPalleteBgra)
                    D4D_MemFree(pPalleteBgra);
                  return;
                }

                dataOffset +=2;

                locX += d4d_extsrcBuffer[OFFSET_X];
                locY -= d4d_extsrcBuffer[OFFSET_Y];

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
                if((locY - y) < radius)
                  non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(locY - y)); // get len of non printed pixels
                else if((bitmapInfo.height - 1 - (locY - y)) < radius)
                  non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(bitmapInfo.height - 1 - (locY - y))); // get len of non printed pixels
                else
                  non_print_len = 0;
#endif
                break;

              default:


                for(i=0;i<d4d_extsrcBuffer[COLOR_IX];i++)
                {
                  if(D4D_FileRead(pFile, &d4d_extsrcBuffer[COLOR], 1) != 1)
                  {
                    // the header isn't loaded succesfully
                    D4D_FileClose(pFile);
                    if(pPalleteBgra)
                      D4D_MemFree(pPalleteBgra);
                    return;
                  }

                  dataOffset++;

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
                  printPixel = D4D_TRUE;
                  if(non_print_len)
                  {
                    if((locX < non_print_len + x) || ((bitmapInfo.width - 1 - (locX - x)) < non_print_len))
                      printPixel = D4D_FALSE;
                  }

                  if(printPixel)
#endif
                  {
                    color = D4D_COLOR_RGB(pPalleteBgra[d4d_extsrcBuffer[COLOR]].Red, pPalleteBgra[d4d_extsrcBuffer[COLOR]].Green, pPalleteBgra[d4d_extsrcBuffer[COLOR]].Blue);

                    D4D_LCD_PutPixel(locX, locY, D4D_LINE_THIN, color);
                  }
                  locX++;

                }

                if(dataOffset & 0x01)
                {
                  // setup position of bitmap data
                  if(D4D_FileSeek(pFile, 1, D4D_SEEK_CUR) != 0)
                  {
                    // the position in file can't be sets
                    D4D_FileClose(pFile);
                    if(pPalleteBgra)
                      D4D_MemFree(pPalleteBgra);
                    return;
                  }

                  dataOffset++;
                }

                break;
            }


          }
        }

      }
      break;

    case D4D_BC_RLE4:
    case D4D_BC_BITFIELDS:
    case D4D_BC_JPEG:
    case D4D_BC_PNG:
    default:
      // there are all non supported bitmaps
      #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
        D4D_RBoxXY(x, y, (D4D_COOR)(x + bitmapInfo.width - 1), (D4D_COOR)(y + bitmapInfo.height - 1), D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore, D4D_ScreenGetScheme(D4D_GetActiveScreen())->bckg, radius);
      #else
        D4D_BoxXY(x, y, (D4D_COOR)(x + bitmapInfo.width - 1), (D4D_COOR)(y + bitmapInfo.height - 1), D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore, D4D_ScreenGetScheme(D4D_GetActiveScreen())->bckg);
      #endif
      break;
      // not supported compressions
  }

  // the Bitmap is drawed, just close opened file
  D4D_FileClose(pFile);
  if(pPalleteBgra)
    D4D_MemFree(pPalleteBgra);

#if D4D_LLD_FLUSH_ELEMENT != D4D_FALSE
  D4D_LLD_LCD.D4DLCD_FlushBuffer(D4DLCD_FLSH_ELEMENT);
#endif
  // end


}


/**************************************************************//*!
*
* Get width  - BMP image file
*
******************************************************************/
D4D_COOR D4D_ImgDecBmpGetWidth(D4D_CHAR* pFileName)
{
  D4D_SIZE tmp_size = D4D_ImgDecBmpGetSize(pFileName);

  return tmp_size.cx;
}

/**************************************************************//*!
*
* Get height  - BMP image file
*
******************************************************************/
D4D_COOR D4D_ImgDecBmpGetHeight(D4D_CHAR* pFileName)
{
  D4D_SIZE tmp_size = D4D_ImgDecBmpGetSize(pFileName);

  return tmp_size.cy;
}


/**************************************************************//*!
*
* Get size  - BMP image file
*
******************************************************************/
D4D_SIZE D4D_ImgDecBmpGetSize(D4D_CHAR* pFileName)
{
  D4D_BITMAPINFOHEADER bitmapInfo;

  if(D4D_ImgDecBmpGetHeader(pFileName, (void*)&bitmapInfo, D4D_BITMAPINFOHEADER_SIZE) != D4D_BITMAPINFOHEADER_SIZE)
    return *(D4D_SIZE*)&d4d_size_zero;
  else
  {
    D4D_SIZE tmp_size;
    // check the size parameters of bitmap - Could be in negative
    if(bitmapInfo.width < 0)
      bitmapInfo.width *= -1;

    if(bitmapInfo.height < 0)
      bitmapInfo.height *= -1;

    tmp_size.cx = (D4D_COOR)bitmapInfo.width;
    tmp_size.cy = (D4D_COOR)bitmapInfo.height;

    return tmp_size;

  }
}

/**************************************************************//*!
*
* Get header  - BMP image file
*
******************************************************************/
int D4D_ImgDecBmpGetHeader(D4D_CHAR* pFileName, void* pBuff, int max_size)
{
  D4D_FILEPTR pFile;
  int res;

  if(max_size < D4D_BITMAPINFOHEADER_SIZE)
    return 0;

  // open the picture file
  pFile =  D4D_OpenImgFile(pFileName, "r");

  // check if the file is opened
  if(pFile == NULL)
    return 0;

  // load the header of picture file

  #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(D4D_BITMAPHEADER_SIZE <= D4D_EXTSRC_BUFF_SIZE);
  #endif

  /********************************************************************************************
  *
  *      Load the bitmap headers
  *
  ********************************************************************************************/

  // read the file info
  if(D4D_FileRead(pFile, d4d_extsrcBuffer, D4D_BITMAPHEADER_SIZE) != D4D_BITMAPHEADER_SIZE)
  {
    // the header isn't loaded succesfully
    D4D_FileClose(pFile);
    return 0;
  }

  // check if this is bitmap and if it's supported
  if(D4D_TMP_PBITMAPHEADER->magic != D4D_BITMAP_SIGNATURE)
  {
    //this is not the bitmap file in supported format
    D4D_FileClose(pFile);
    return 0;
  }

  // load the Bitmap info one of base header bitmap type (it should be enough for eGUI purposes)
  res = D4D_FileRead(pFile, pBuff, D4D_BITMAPINFOHEADER_SIZE);

  D4D_FileClose(pFile);

#if D4D_ENDIAN == D4D_ENDIAN_BIG
  if(res == D4D_BITMAPINFOHEADER_SIZE)
  {

    // Fill up right the bmp info (solve endianism)
    ((D4D_BITMAPINFOHEADER*)pBuff)->headerSz = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->headerSz);
    ((D4D_BITMAPINFOHEADER*)pBuff)->width = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->width);
    ((D4D_BITMAPINFOHEADER*)pBuff)->height = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->height);
    ((D4D_BITMAPINFOHEADER*)pBuff)->nplanes = D4D_READ16L(((D4D_BITMAPINFOHEADER*)pBuff)->nplanes);
    ((D4D_BITMAPINFOHEADER*)pBuff)->bitspp = D4D_READ16L(((D4D_BITMAPINFOHEADER*)pBuff)->bitspp);
    ((D4D_BITMAPINFOHEADER*)pBuff)->compressType = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->compressType);
    ((D4D_BITMAPINFOHEADER*)pBuff)->bmpBytesz = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->bmpBytesz);
    ((D4D_BITMAPINFOHEADER*)pBuff)->hres = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->hres);
    ((D4D_BITMAPINFOHEADER*)pBuff)->vres = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->vres);
    ((D4D_BITMAPINFOHEADER*)pBuff)->ncolors = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->ncolors);
    ((D4D_BITMAPINFOHEADER*)pBuff)->nimpcolors = D4D_READ32L(((D4D_BITMAPINFOHEADER*)pBuff)->nimpcolors);

  }
#endif

  return res;
}


/**************************************************************************/ /*!
* @brief   The function convert screen into BMP file
* @param   pFileName - pointer file name string
* @return  D4D_TRUE - The BMP file has been saved, D4D_FALSE - error in creating BMP file
* @note    The eGUI external buffer size MUST be >= 64
*******************************************************************************/
D4D_BOOL D4D_PrintScreenToBMP(D4D_CHAR* pFileName)
{
#if D4D_EXTSRC_BUFF_SIZE < 64
  D4D_UNUSED(pFileName);
  return D4D_FALSE;
#else
  D4D_FILEPTR pFile = NULL;
  D4D_COLOR clr;
  D4D_COOR x, y, x_max, y_max;
  LWord     dataIx = 0;

  if((D4D_LCD_GetOrientation() == D4D_ORIENT_PORTRAIT) || (D4D_LCD_GetOrientation() == D4D_ORIENT_PORTRAIT180))
  {
    x_max = D4D_SCREEN_SIZE_SHORTER_SIDE;
    y_max = D4D_SCREEN_SIZE_LONGER_SIDE;
  }else
  {
    x_max = D4D_SCREEN_SIZE_LONGER_SIDE;
    y_max = D4D_SCREEN_SIZE_SHORTER_SIDE;
  }

  // 1. Open File for Write
  pFile = D4D_OpenImgFile(pFileName, "w");

  if(pFile == NULL)
    return D4D_FALSE;

  // 2. Create BMP Header
  d4d_extsrcBuffer[0] = 'B';
  d4d_extsrcBuffer[1] = 'M';
  // File Size 54B header + 24bits color (3Bytes) * width * height
  *((LWord*)&(d4d_extsrcBuffer[2])) = D4D_WRITE32L(54 + 3 * D4D_SCREEN_SIZE_LONGER_SIDE * D4D_SCREEN_SIZE_SHORTER_SIDE);
  // Bitmap header reserved bytes
  *((LWord*)&(d4d_extsrcBuffer[6])) = D4D_WRITE32L(0);
  // Bitmap bitmap data offset
  *((LWord*)&(d4d_extsrcBuffer[10])) = D4D_WRITE32L(54);

  // 3. Create BMP Information Data BID
  //size of this information
  *((LWord*)&(d4d_extsrcBuffer[14])) = D4D_WRITE32L(40);
  // Width of bitmap
  *((LWord*)&(d4d_extsrcBuffer[18])) = D4D_WRITE32L(x_max);
  // Height of bitmap
  *((LWord*)&(d4d_extsrcBuffer[22])) = D4D_WRITE32L(y_max);
  // Number of color planes being used.
  *((Word*)&(d4d_extsrcBuffer[26])) = D4D_WRITE16L(1);
  // The number of bits/pixel.
  *((Word*)&(d4d_extsrcBuffer[28])) = D4D_WRITE16L(24);
  //BI_RGB, No compression used
  *((LWord*)&(d4d_extsrcBuffer[30])) = D4D_WRITE32L(0);
  // Bitmap data size  24bits color (3Bytes) * width * height
  *((LWord*)&(d4d_extsrcBuffer[34])) = D4D_WRITE32L( 3 * D4D_SCREEN_SIZE_LONGER_SIDE * D4D_SCREEN_SIZE_SHORTER_SIDE);
  //The horizontal resolution of the image   2,835 pixels/meter
  *((LWord*)&(d4d_extsrcBuffer[38])) = D4D_WRITE32L(2835);
  //The vertical resolution of the image     2,835 pixels/meter
  *((LWord*)&(d4d_extsrcBuffer[42])) = D4D_WRITE32L(2835);
  //Number of colors in the palette
  *((LWord*)&(d4d_extsrcBuffer[46])) = D4D_WRITE32L(0);
  //	Means all colors are important
  *((LWord*)&(d4d_extsrcBuffer[50])) = D4D_WRITE32L(0);

  if(D4D_FileWrite(pFile, d4d_extsrcBuffer, 54) != 54)
  {
    D4D_FileClose(pFile);
    return D4D_FALSE;
  }

  // 4. Write the Bitmap Data

  for(y = 0; y < y_max; y++)
  {

    (void)D4D_LLD_LCD.D4DLCD_SetWindow(0, (D4D_COOR)(y_max - 1 - y), (D4D_COOR)(x_max - 1), (D4D_COOR)(y_max - 1 - y));
    for(x = 0; x < x_max; x++)
    {
      clr = D4D_LLD_LCD.D4DLCD_Read_PixelColor();
      d4d_extsrcBuffer[dataIx++] = D4D_COLOR_GET_B(clr);
      d4d_extsrcBuffer[dataIx++] = D4D_COLOR_GET_G(clr);
      d4d_extsrcBuffer[dataIx++] = D4D_COLOR_GET_R(clr);

      // Check if data buffer is full
      if(dataIx > (D4D_EXTSRC_BUFF_SIZE - 3))
      {
        // If it full, flush data to file
        if(D4D_FileWrite(pFile, d4d_extsrcBuffer, dataIx) != dataIx)
        {
          D4D_FileClose(pFile);
          return D4D_FALSE;
        }
        dataIx = 0;
      }
    }
  }

  // check the last chunk of data
  if(dataIx)
  {
    if(D4D_FileWrite(pFile, d4d_extsrcBuffer, dataIx) != dataIx)
    {
      D4D_FileClose(pFile);
      return D4D_FALSE;
    }
  }

  D4D_FileClose(pFile);
  return D4D_TRUE;
#endif
}

#endif
