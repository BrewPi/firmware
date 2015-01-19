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
* @file      d4dlcdhw_k70_lcdc.h
*
* @author     Petr Gargulak
*
* @version   0.0.9.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - k70_lcdc hardware lcd driver function header file
*
*******************************************************************************/

#ifndef __D4DLCDHW_K70_LCDC_H
#define __D4DLCDHW_K70_LCDC_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_k70_lcdc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_k70_lcdc_cfg.h"

    /******************************************************************************
    * Constants
    ******************************************************************************/

    // TWR-LCDRBG board revA - with TWR-K70 on SPI2
    #define TWR_LCDRGB_A 1

    // TWR-LCDRBG board revB
    #define TWR_LCDRGB_B 2

    #ifndef D4DLCDHWFB_DOUBLE_BUFFER
      #define D4DLCDHWFB_DOUBLE_BUFFER 0
    #endif

    #ifndef D4DLCDHWFB_DMA_ENABLE
      #define D4DLCDHWFB_DMA_ENABLE 0
    #endif

    #ifndef D4DLCDHWFB_DMA_CHANNEL
      #define D4DLCDHWFB_DMA_CHANNEL 0
    #endif

    #ifndef D4DLCDHWFB_DMA_CHPRI
      #define D4DLCDHWFB_DMA_CHPRI (DMA_DCHPRI3_ECP_MASK | DMA_DCHPRI3_DPA_MASK | (D4DLCDHWFB_DMA_CHANNEL & 0xf))
    #endif

    #define D4DLCDHWFB_DMA_INTVECT (INT_DMA0_DMA16 + (D4DLCDHWFB_DMA_CHANNEL & 0xf))

    #define D4DLCDHWFB_FRAMEBUFFER_SIZE  ((D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX * D4DLCDHWFB_BPP_BYTE) + 32) // add 32 to manage non alligned buffer at 16 byte (burst line)

    #ifndef D4DLCDHWFB_START_ADDRESS
      #define D4DLCDHWFB_START_ADDRESS 0
    #endif

    #ifndef D4DLCDHWFB_START_ADDRESS_2ND
      #define D4DLCDHWFB_START_ADDRESS_2ND 0
    #endif

    #ifndef D4DLCDHWFB_X_MAX
      #define D4DLCDHWFB_X_MAX 480
    #endif

    #ifndef D4DLCDHWFB_Y_MAX
      #define D4DLCDHWFB_Y_MAX 800
    #endif

    #ifndef D4DLCDHWFB_BPP_BYTE
      #define D4DLCDHWFB_BPP_BYTE 4
    #endif

    #ifndef D4DLCDHWFB_BPP
      #define D4DLCDHWFB_BPP BPP24
    #endif

    #ifndef D4DLCDHWFB_MIRROWED
      #define D4DLCDHWFB_MIRROWED 0
    #endif

    #ifndef D4DLCDHWFB_CACHE_FLUSH_ENABLE
      #define D4DLCDHWFB_CACHE_FLUSH_ENABLE 0
    #endif

    #ifndef D4DMOUSE_CURSOR_SIZE_X
      #define D4DMOUSE_CURSOR_SIZE_X 16
    #endif

    #ifndef D4DMOUSE_CURSOR_SIZE_Y
      #define D4DMOUSE_CURSOR_SIZE_Y 16
    #endif

    #ifndef D4DMOUSE_CURSOR_BMPFORMAT
      #define D4DMOUSE_CURSOR_BMPFORMAT (D4D_BMP_ENDIAN_FLAG | D4D_BMP_16MNOPAL)
    #endif

// List of supported LCD Panels
//  d4dlcdhw_k70_lcdc_twr_rgbA - TWR-LCD-RGB rev A; on TWR-K70 and SPI2 - it must be on TWR-PROTO board rerouted SPI2 signals
//  d4dlcdhw_k70_lcdc_twr_rgbB - TWR-LCD_RGB rev B; general (it doesn't need SPI initialization)

    #ifndef D4D_LLD_LCD_HW_K70LCDC
      #define D4D_LLD_LCD_HW_K70LCDC d4dlcdhw_k70_lcdc_twr_rgbB
    #endif




    /******************************************************************************
    * Types
    ******************************************************************************/
    typedef struct D4DLCDHWFBK70LCDC_FUNCTIONS_S
    {
      unsigned char (*D4DLCDHWFBK70LCDC_InitPanel)(void);
    }D4DLCDHWFBK70LCDC_FUNCTIONS;


    /******************************************************************************
    * Macros
    ******************************************************************************/


    /******************************************************************************
    * Global variables
    ******************************************************************************/

    extern const D4DLCDHWFBK70LCDC_FUNCTIONS D4D_LLD_LCD_HW_K70LCDC;

    /******************************************************************************
    * Global functions
    ******************************************************************************/



/************************* Structure/Union Data types ************************/

typedef enum
{ ACD_DIV_0=0,
  ACD_DIV_1,
  ACD_DIV_2,
  ACD_DIV_3,
  ACD_DIV_4,
  ACD_DIV_5,
  ACD_DIV_6,
  ACD_DIV_7,
  ACD_DIV_8,
  ACD_DIV_9,
  ACD_DIV_10,
  ACD_DIV_11,
  ACD_DIV_12,
  ACD_DIV_13,
  ACD_DIV_14,
  ACD_DIV_15
} ACD_DIV;


typedef enum
{ MONO_PANEL=0,
  CSTN_PANEL,
  PANEL_UNUSED,
  TFT_PANEL=3
} LCDC_PANEL_TYPE;

typedef enum
{
 BPP1=0,
 BPP2=1,
 BPP4=2,
 BPP8=3,
 BPP12=4,
 BPP16=5,
 BPP18=6,
 BPP24=7
} BPP_TYPE;

  #endif
#endif /* __D4DLCDHW_K70_LCDC_H */










