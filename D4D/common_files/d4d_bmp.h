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
* @file      d4d_bmp.h
*
* @author     Petr Gargulak
*
* @version   0.0.22.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver bmp decoder header file
*
******************************************************************************/

#ifndef __D4D_BMP_H
#define __D4D_BMP_H

/******************************************************************************
* Include
******************************************************************************/

#include "common_files/d4d_imgdec_d4dint.h"
#include "common_files/d4d_imgdec_d4d.h"
#include "common_files/d4d_imgdec_bmp.h"


/******************************************************************************
* D4D Bitmaps setting  constants
*
*//*! @addtogroup doxd4d_bmp_const
* @{
*******************************************************************************/
/*! @brief D4D_BMP_EXTSRC_SUPPORT_D4D_EXT constant default declaration        */
/*! @note D4D_BMP_EXTSRC_SUPPORT_D4D_EXT reflects the endianism settings and selecting the
    right format of file to optimize the speed of drawing of pictures */
#if D4D_ENDIAN == D4D_ENDIAN_LITTLE
  #define D4D_BMP_EXTSRC_SUPPORT_D4D_EXT "D4L"  ///< D4L file extension will be used for eGUI own format
#else
  #define D4D_BMP_EXTSRC_SUPPORT_D4D_EXT "D4B"  ///< D4B file extension will be used for eGUI own format
#endif

/**************************************************************************/ /*!
* @brief   Macro that contains name of empty bitmap structure
* @note    This define is used for cases where is need to specify any bitmap structure
*          , but the bitmap should not be valid. For example for mouse cursor definition.
*******************************************************************************/
#define D4D_BMP_CLEAR d4d_mouseNoCursor

/*! @} End of doxd4d_bmp_const                                              */


/******************************************************************************
* D4D Bmp types
*
*//*! @addtogroup doxd4d_bmp_type
* @{
*******************************************************************************/
/*! @brief D4D_EXTIMG_TYPE eGUI supported image type enumeration              */
typedef enum
{
  D4D_EXTIMG_UNKNOWN = 0,       ///< Uknown image format - do nothing
  D4D_EXTIMG_INTD4D,            ///< Internal memory eGUI format of image
  D4D_EXTIMG_D4D,               ///< External memory eGUI format of image
  D4D_EXTIMG_BMP,               ///< External memory MicroSoft BMP format of image
  D4D_EXTIMG_ITEMS_CNT          ///< Just last item where is stored the count of the image decoder. Keep it on last position.
}D4D_EXTIMG_TYPE;

/*! @brief D4D_BMP eGUI main image structure             */
typedef struct
{
    const void *pData;          ///< Pointer to data to internal memory. In case of external source of image this must be NULL
    const void *pParam;         ///< Pointer to pallete to internal memory. In case of external source of image is used as a pointer to file path.
} D4D_BMP;

/*! @brief D4D_EXTIMG_FUNC API structure for eGUI image decoders             */
typedef struct
{
  void (*D4D_ImgDecDraw)(D4D_COOR x, D4D_COOR y, D4D_CHAR* pFileName, D4D_BOOL greyScale, D4D_COOR radius);     ///< Image draw function
  D4D_COOR (*D4D_ImgDecGetWidth)(D4D_CHAR* pFileName);                                                          ///< Get image width function
  D4D_COOR (*D4D_ImgDecGetHeight)(D4D_CHAR* pFileName);                                                         ///< Get image height function
  D4D_SIZE (*D4D_ImgDecGetSize)(D4D_CHAR* pFileName);                                                           ///< Get image size function
  int (*D4D_ImgDecGetHeader)(D4D_CHAR* pFileName, void* pBuff, int max_size);                                   ///< Get image header function
}D4D_EXTIMG_FUNC;

/*! @brief D4D_BMP_PROPERTIES type of image properties             */
typedef Byte D4D_BMP_PROPERTIES;

/*! @} End of doxd4d_bmp_type                                               */

/******************************************************************************
* D4D Bitmaps macros
*
*//*! @addtogroup doxd4d_bmp_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that define the bitmap inside of internal memory
* @param   name - name of the bitmap structure
* @param   pbmp - pointer to bitmap data
* @param   ppal - pointer on pallete, in case that the picture is not provided with pallete, let this field NULL
* @note    This define D4D_BMP structure that is used in whole eGUI to draw image to screen.
*******************************************************************************/
#define D4D_DECLARE_BMP(name, pbmp, ppal) \
    const D4D_BMP name = \
    { \
        (const void*)pbmp, (const void*)ppal \
    };

/**************************************************************************/ /*!
* @brief   Macro that define the bitmap in external memory
* @param   name - name of the bitmap structure
* @param   filename - pointer to filename (relative to default image folder or absolute)
* @note    This define D4D_BMP structure for external image.
*******************************************************************************/
#define D4D_DECLARE_BMPFILE(name, filename) \
    const D4D_BMP name = \
    { \
        NULL, filename \
    };

/**************************************************************************/ /*!
* @brief   Macro that externs the image structure
* @param   name - name of the bitmap structure
* @note    Just help macro to extern the image structure.
*******************************************************************************/
#define D4D_EXTERN_BMP(name) \
    extern const D4D_BMP name

/*! @} End of doxd4d_bmp_macro                                              */


/******************************************************************************
* Global D4D bitmap drawing level functions
******************************************************************************/


#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  void D4D_ExtImgSetWorkPath(D4D_CHAR* pPath);
  D4D_CHAR* D4D_ExtImgGetWorkPath(void);
  D4D_FILEPTR D4D_OpenImgFile(D4D_CHAR* pFileName, D4D_CHAR* mode);
#endif

D4D_EXTIMG_TYPE D4D_GetImgSrcType(D4D_BMP* pBmp);



#endif /* __D4D_BMP_H */
