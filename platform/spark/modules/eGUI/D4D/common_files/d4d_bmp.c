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
* @file      d4d_bmp.c
*
* @author     Petr Gargulak
*
* @version   0.0.36.0
*
* @date      Oct-3-2013
*
* @brief     D4D driver bmp decoder functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"

#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  static const D4D_CHAR imgDfltPath[] = D4D_IMG_WORK_PATH_DEFAULT;
  static D4D_CHAR* pImgWorkPath = (D4D_CHAR*)imgDfltPath;
#endif

static const D4D_EXTIMG_FUNC d4d_extImgFunc[D4D_EXTIMG_ITEMS_CNT] =
{
  { NULL, NULL, NULL, NULL, NULL },
  { D4D_ImgDecIntD4DDraw, D4D_ImgDecIntD4DGetWidth, D4D_ImgDecIntD4DGetHeight, D4D_ImgDecIntD4DGetSize, D4D_ImgDecIntD4DGetHeader },
  { D4D_ImgDecExtD4DDraw, D4D_ImgDecExtD4DGetWidth, D4D_ImgDecExtD4DGetHeight, D4D_ImgDecExtD4DGetSize, D4D_ImgDecExtD4DGetHeader },
  { D4D_ImgDecBmpDraw, D4D_ImgDecBmpGetWidth, D4D_ImgDecBmpGetHeight, D4D_ImgDecBmpGetSize, D4D_ImgDecBmpGetHeader }
};

const D4D_BMP d4d_mouseNoCursor =
{
  NULL,
  NULL
};

/******************************************************************************
* Begin of D4D_BMP public functions
*//*! @addtogroup doxd4d_bmp_func
* @{
*******************************************************************************/

/**************************************************************//*!
*
* External Bitmap support
*
******************************************************************/

#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  /**************************************************************************/ /*!
  * @brief   Function sets image external source files work directory.
  * @param   pPath - the new default IMAGE working path
  * @return  none
  * @note    This function sets the new image default working path. All relative defined image
  *          paths is using this as start folder.
  *******************************************************************************/
  void D4D_ExtImgSetWorkPath(D4D_CHAR* pPath)
  {
    if(pPath)
      pImgWorkPath = pPath;
  }

  /**************************************************************************/ /*!
  * @brief   Function gets image external source files work directory.
  * @return  The current sets working directory
  * @note    This function returns the image working path. All relative defined image
  *          paths is using this as start folder.
  *******************************************************************************/
  D4D_CHAR* D4D_ExtImgGetWorkPath(void)
  {
    return pImgWorkPath;
  }

  /**************************************************************************/ /*!
  * @brief   Function opens the ing file.
  * @param   pFileName - the image file name path (could be relative or absolute)
  * @param   mode - mode of file open
  * @return  pointer to file
  * @note    This function open the image file. In case of relative path it create
  *          absolute path with the image work path.
  *******************************************************************************/
  D4D_FILEPTR D4D_OpenImgFile(D4D_CHAR* pFileName, D4D_CHAR* mode)
  {
    // create the file name (if needed)
    if(D4D_FileIsAbsolutePath(pFileName))
    {
      // the input path is absolute and can't be changed
      return D4D_FileOpen(pFileName,mode);
    }
    else
    {
      // the input path is relative and must be updated with preselected image work paths
      if((D4D_StrLen(pImgWorkPath) + D4D_StrLen(pFileName)) >= D4D_EXTSRC_BUFF_SIZE)
        return NULL;

      D4D_StrCopy((D4D_CHAR*)d4d_extsrcBuffer, pImgWorkPath);
      D4D_StrCat((D4D_CHAR*)d4d_extsrcBuffer, pFileName);
      return D4D_FileOpen((D4D_CHAR*)d4d_extsrcBuffer,mode);
    }

  }
#endif

/**************************************************************************/ /*!
* @brief   Function returns the type of image source.
* @param   pBmp - pointer on the bitmap structure
* @return  type of image
* @note    This function find out the type of the image source. In case of not supported image it
*          returns D4D_EXTIMG_UNKNOWN.
*******************************************************************************/
D4D_EXTIMG_TYPE D4D_GetImgSrcType(D4D_BMP* pBmp)
{
  if(pBmp == NULL)
    return D4D_EXTIMG_UNKNOWN;

  #if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
    if(pBmp->pData == NULL)
    {
      D4D_CHAR pExt[3+1];
	  D4D_CHAR* pExtSrc;	  

      // run the external data dekoder
      if(pBmp->pParam == NULL)
        return D4D_EXTIMG_UNKNOWN;

      // 1. get File extension
      pExtSrc = D4D_GetFileExtension((D4D_CHAR*)pBmp->pParam);
	  
      if(D4D_StrLen(pExtSrc) > 3)
          return D4D_EXTIMG_UNKNOWN;
      
      D4D_StrCopy(pExt, pExtSrc);
      // Convert extension to upper format
      D4D_ToUpper(pExt);

      #if (D4D_BMP_EXTSRC_SUPPORT_D4D == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
        if(!D4D_CompareStrings(pExt, D4D_BMP_EXTSRC_SUPPORT_D4D_EXT))        // big or little endian of the egui picture format
          return D4D_EXTIMG_D4D; // d4d bitmap binary files
      #endif

      #if (D4D_BMP_EXTSRC_SUPPORT_BMP == D4D_TRUE) && (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
        if(!D4D_CompareStrings(pExt, "BMP"))
          return D4D_EXTIMG_BMP; // standard bitmap BMP files
      #endif

    }else

  #endif
    if((pBmp->pData != NULL))
      return D4D_EXTIMG_INTD4D;

  return D4D_EXTIMG_UNKNOWN;
}

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Interface Bitmap Functions
  #pragma mark -
#endif


/**************************************************************************/ /*!
* @brief   Function draw the bitmap on the specify coordination.
* @param   x - coordination in X axis
* @param   y - coordination in Y axis
* @param   pBmp - pointer on the bitmap structure
* @param   greyScale - drawing in greyScale
* @param   radius - round corner radius
* @return  none
* @note    This function draws bitmap on specified coordination, it can draw colors
*          in grey scale, and with round corners.
*******************************************************************************/
void D4D_DrawRBmpXY(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius)
{
  D4D_EXTIMG_TYPE imgType = D4D_GetImgSrcType((D4D_BMP*)pBmp);

  if(imgType == D4D_EXTIMG_UNKNOWN)
    return;

  if(imgType == D4D_EXTIMG_INTD4D)
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecDraw)
      d4d_extImgFunc[imgType].D4D_ImgDecDraw(x, y, (D4D_CHAR*)pBmp, greyScale, radius);

  }
#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  else
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecDraw)
      d4d_extImgFunc[imgType].D4D_ImgDecDraw(x, y, (D4D_CHAR*)(pBmp->pParam), greyScale, radius);
  }
#endif
}


/**************************************************************************/ /*!
* @brief   Function draw the bitmap on the specify coordination into rectangle.
* @param   ppt - pointer on point structure of top left corner on screen to specify rectangle
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   pBmp - pointer on the bitmap structure
* @param   bmpProp - bitmap properties
* @param   colorBkgd - background color of bitmap
* @param   greyScale - drawing in greyScale
* @return  none
* @note    This function draws bitmap in specified rectangle, it can draw colors
*          in grey scale, and with round corners. There is optin to align bitmap inside the rectangle.
*******************************************************************************/
void D4D_DrawBmpRect(D4D_POINT* ppt, D4D_SIZE* psz, const D4D_BMP* pBmp, D4D_BMP_PROPERTIES bmpProp, D4D_COLOR colorBkgd, D4D_BOOL greyScale)
{
  D4D_SIZE tmp_size = *psz;
  D4D_POINT tmp_pos = *ppt;

  D4D_COOR tmp_bmpWidth = (D4D_COOR)D4D_GetBmpWidth(pBmp);
  D4D_COOR tmp_bmpHeight = (D4D_COOR)D4D_GetBmpHeight(pBmp);

  if(!tmp_size.cx)
    tmp_size.cx = tmp_bmpWidth;

  if(!tmp_size.cy)
    tmp_size.cy = tmp_bmpHeight;

  // If the bitmap is larger than specify place, don't draw it
  if ((tmp_size.cy < tmp_bmpHeight) || (tmp_size.cx < tmp_bmpWidth))
  {
    D4D_FillRect(ppt, psz, colorBkgd);
    return;
  }

  tmp_pos.y = ppt->y;
  tmp_pos.x = ppt->x;

  // Compute the text position in rectangle in Y axis
  switch(bmpProp & D4D_ALIGN_V_MASK)
  {
    case D4D_ALIGN_V_BOTTOM_MASK:
      tmp_pos.y += (D4D_COOR)(tmp_size.cy - tmp_bmpHeight);
      break;

    case D4D_ALIGN_V_CENTER_MASK:
      tmp_pos.y += (D4D_COOR)((tmp_size.cy - tmp_bmpHeight) / 2);
      break;
  }

  // Compute the text position in rectangle in X axis
  switch(bmpProp & D4D_ALIGN_H_MASK)
  {
    case D4D_ALIGN_H_RIGHT_MASK:
      tmp_pos.x += (D4D_COOR)(tmp_size.cx - tmp_bmpWidth);
      break;

    case D4D_ALIGN_H_CENTER_MASK:
      tmp_pos.x += (D4D_COOR)((tmp_size.cx - tmp_bmpWidth) / 2);
      break;
  }

    if(tmp_size.cx > tmp_bmpWidth)
    {
      if((bmpProp & D4D_ALIGN_H_MASK) != D4D_ALIGN_H_LEFT_MASK)
      {
        // Fill the part of rect in front of the text in full height of rect
          D4D_FillRectXY(ppt->x, ppt->y, tmp_pos.x, (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
      }

      if((bmpProp & D4D_ALIGN_H_MASK) != D4D_ALIGN_H_RIGHT_MASK)
      {
        // Fill the part of rect behind the text in full height of rect
          D4D_FillRectXY((D4D_COOR)(tmp_pos.x + tmp_bmpWidth - 1), ppt->y, (D4D_COOR)(ppt->x + tmp_size.cx - 1), (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
      }
    }

    if(tmp_size.cy > tmp_bmpHeight)
    {
      if((bmpProp & D4D_ALIGN_V_MASK) != D4D_ALIGN_V_TOP_MASK)
      {
        // Fill the part of rect above the text in lenght of text
        D4D_FillRectXY(tmp_pos.x, ppt->y, (D4D_COOR)(tmp_pos.x + tmp_bmpWidth - 1), tmp_pos.y, colorBkgd);
      }

      if((bmpProp & D4D_ALIGN_V_MASK) != D4D_ALIGN_V_BOTTOM_MASK)
      {
        // Fill the part of rect under the text in lenght of text
        D4D_FillRectXY(tmp_pos.x, (D4D_COOR)(tmp_pos.y + tmp_bmpHeight - 1), (D4D_COOR)(tmp_pos.x + tmp_bmpWidth - 1), (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
      }
    }

  // Print the string itself
  D4D_DrawRBmpXY(tmp_pos.x, tmp_pos.y, pBmp, greyScale, 0); // radius, grayscale?


}

#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
/**************************************************************************/ /*!
* @brief   Function draw the bitmap on the specify coordination into rectangle with round corners.
* @param   ppt - pointer on point structure of top left corner on screen to specify rectangle
* @param   psz - pointer on the size structure that defines size of the rectangle
* @param   pBmp - pointer on the bitmap structure
* @param   bmpProp - bitmap properties
* @param   colorBkgd - background color of bitmap
* @param   greyScale - drawing in greyScale
* @param   radius - round corner radius
* @return  none
* @note    This function draws bitmap in specified rectangle, it can draw colors
*          in grey scale, and with round corners. There is optin to align bitmap inside the rectangle.
*******************************************************************************/
void D4D_DrawRBmpRect(D4D_POINT* ppt, D4D_SIZE* psz, const D4D_BMP* pBmp, D4D_BMP_PROPERTIES bmpProp, D4D_COLOR colorBkgd, D4D_BOOL greyScale, D4D_COOR radius)
{
  D4D_POINT tmp_pos = *ppt;
  D4D_SIZE  tmp_size = *psz;

  D4D_COOR tmp_bmpWidth;
  D4D_COOR tmp_bmpHeight;

  if(!radius)
  {
    D4D_DrawBmpRect(ppt, psz, pBmp, bmpProp, colorBkgd, greyScale);
    return;
  }


  tmp_bmpWidth = (D4D_COOR)D4D_GetBmpWidth(pBmp);
  tmp_bmpHeight = (D4D_COOR)D4D_GetBmpHeight(pBmp);

  // If the bitmap is larger than specify place, don't draw it
  if ((tmp_size.cy < tmp_bmpHeight) || (tmp_size.cx < tmp_bmpWidth))
  {
    D4D_FillRect(ppt, psz, colorBkgd);
    return;
  }


  // Compute the text position in rectangle in Y axis
  switch(bmpProp & D4D_ALIGN_V_MASK)
  {
    case D4D_ALIGN_V_BOTTOM_MASK:
      tmp_pos.y = (D4D_COOR)(ppt->y + (tmp_size.cy - tmp_bmpHeight));
      break;

    case D4D_ALIGN_V_CENTER_MASK:
      tmp_pos.y = (D4D_COOR)(ppt->y + ((tmp_size.cy - tmp_bmpHeight) / 2));
      break;

    default:
    case D4D_ALIGN_V_TOP_MASK:
      tmp_pos.y = ppt->y;
      break;
  }

  // Compute the text position in rectangle in X axis
  switch(bmpProp & D4D_ALIGN_H_MASK)
  {
    case D4D_ALIGN_H_RIGHT_MASK:
      tmp_pos.x = (D4D_COOR)(ppt->x + (tmp_size.cx - tmp_bmpWidth));
      break;

    case D4D_ALIGN_H_CENTER_MASK:
      tmp_pos.x = (D4D_COOR)(ppt->x + ((tmp_size.cx - tmp_bmpWidth) / 2));
      break;

    default:
    case D4D_ALIGN_H_LEFT_MASK:
      tmp_pos.x = ppt->x;
      break;
  }


  // Fill the part of rect in front of the text in full height of rect
  if((psz->cx != tmp_bmpWidth) || (psz->cy != tmp_bmpHeight))
    D4D_FillRRect(ppt, psz, colorBkgd, radius);

  // Print the bmp itself
  D4D_DrawRBmpXY(tmp_pos.x, tmp_pos.y, pBmp, greyScale, radius); // radius, grayscale?


}

#endif

/**************************************************************************/ /*!
* @brief   Function gets information about bitmap size.
* @param   pBmp - pointer on the bitmap structure
* @return  none
* @note    Function tries get the size information of bitmap handled in D4D_BMP parameter.
*          In case that there is no chance get the size, it return size 0,0.
*******************************************************************************/
D4D_SIZE D4D_GetBmpSize(const D4D_BMP* pBmp)
{
  D4D_EXTIMG_TYPE imgType = D4D_GetImgSrcType((D4D_BMP*)pBmp);

  if(imgType == D4D_EXTIMG_UNKNOWN)
    return *(D4D_SIZE*)&d4d_size_zero;

  if(imgType == D4D_EXTIMG_INTD4D)
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetSize)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetSize((D4D_CHAR*)pBmp);

  }
#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  else
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetSize)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetSize((D4D_CHAR*)(pBmp->pParam));
  }
#endif

  return *(D4D_SIZE*)&d4d_size_zero;

}

/**************************************************************************/ /*!
* @brief   Function gets information about bitmap width.
* @param   pBmp - pointer on the bitmap structure
* @return  none
* @note    Function tries get the width information of bitmap handled in D4D_BMP parameter.
*          In case that there is no chance get the width, it return width 0.
*******************************************************************************/
D4D_COOR D4D_GetBmpWidth(const D4D_BMP* pBmp)
{
  D4D_EXTIMG_TYPE imgType = D4D_GetImgSrcType((D4D_BMP*)pBmp);

  if(imgType == D4D_EXTIMG_UNKNOWN)
    return 0;

  if(imgType == D4D_EXTIMG_INTD4D)
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetWidth)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetWidth((D4D_CHAR*)pBmp);

  }
#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  else
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetWidth)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetWidth((D4D_CHAR*)(pBmp->pParam));
  }
#endif

  return 0;
}

/**************************************************************************/ /*!
* @brief   Function gets information about bitmap height.
* @param   pBmp - pointer on the bitmap structure
* @return  none
* @note    Function tries get the height information of bitmap handled in D4D_BMP parameter.
*          In case that there is no chance get the height, it return height 0.
*******************************************************************************/
D4D_COOR D4D_GetBmpHeight(const D4D_BMP* pBmp)
{
  D4D_EXTIMG_TYPE imgType = D4D_GetImgSrcType((D4D_BMP*)pBmp);

  if(imgType == D4D_EXTIMG_UNKNOWN)
    return 0;

  if(imgType == D4D_EXTIMG_INTD4D)
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetHeight)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetHeight((D4D_CHAR*)pBmp);

  }
#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  else
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetHeight)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetHeight((D4D_CHAR*)(pBmp->pParam));
  }
#endif

  return 0;
}


/**************************************************************************/ /*!
* @brief   Function gets image file header.
* @param   pBmp - pointer on the bitmap structure
* @param   pBuff - buffer in application code that is prepared to store the image header
* @param   max_size - max_buffer size
* @return  count of byte copied to buffer
* @note    Function return the image header. The size and type of image header depends on input
*          image source typeZ.
*******************************************************************************/
int D4D_GetBmpHeader(D4D_BMP* pBmp, void* pBuff, int max_size)
{
  D4D_EXTIMG_TYPE imgType = D4D_GetImgSrcType((D4D_BMP*)pBmp);

  if(imgType == D4D_EXTIMG_INTD4D)
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetHeader)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetHeader((D4D_CHAR*)pBmp, pBuff, max_size);

  }
#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  else
  {
    if(d4d_extImgFunc[imgType].D4D_ImgDecGetHeader)
      return d4d_extImgFunc[imgType].D4D_ImgDecGetHeader((D4D_CHAR*)(pBmp->pParam), pBuff, max_size);
  }
#endif

  return 0;
}


/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_bmp_func                                               */
/******************************************************************************/

/******************************************************************************
*       Private functions                                                     *
******************************************************************************/


