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
* @file      d4dlcdhw_dragonfire_lcdc.h
*
* @author     Petr Gargulak
*
* @version   0.0.2.0
*
* @date      Aug-30-2010
*
* @brief     D4D driver - dragonfire_lcdc hardware lcd driver function header file
*
*******************************************************************************/

#ifndef __D4DLCDHW_DRAGONFIRE_LCDC_H
#define __D4DLCDHW_DRAGONFIRE_LCDC_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_dragonfire_lcdc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_dragonfire_lcdc_cfg.h"

    /******************************************************************************
    * Constants
    ******************************************************************************/

		#ifndef D4DLCDHWFB_START_ADDRESS
			#define D4DLCDHWFB_START_ADDRESS 0x40000000
		#endif

		#ifndef D4DLCDHWFB_X_MAX
			#define D4DLCDHWFB_X_MAX 240
		#endif

		#ifndef D4DLCDHWFB_Y_MAX
			#define D4DLCDHWFB_Y_MAX 320
		#endif

		#ifndef D4DLCDHWFB_Y_BPP_BYTE
			#define D4DLCDHWFB_Y_BPP_BYTE 2
		#endif


		#ifndef D4DLCDHWFB_MIRROWED
			#define D4DLCDHWFB_MIRROWED 0
		#endif

		#ifndef D4DLCDHWFB_PANEL_TYPE
			#define D4DLCDHWFB_PANEL_TYPE M52277EVB_PANEL
		#endif

		#ifndef D4DLCDHWFB_PANEL_CLKDIV
			#define D4DLCDHWFB_PANEL_CLKDIV 7
		#endif

    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros
    ******************************************************************************/

    #define MCF_LCDC_LCD_PCR_PANEL_TYPE(x)     (((x)&0x00000003)<<30)

    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/



/************************* Structure/Union Data types ************************/
#define VDDEN_ON (*(unsigned long*)(0xC0080000))
//#define USE_m5329evb


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
{ SHARP_12SVGA=0,
  SHARP_35QVGA,
  SHARP_57QVGA,
  SHARP_64VGA,
  SHARP_104VGA,
  M52277EVB_PANEL
} Panel_Size ;


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
 RESERVED=7
} BPP_TYPE;

  #endif
#endif /* __D4DLCDHW_DRAGONFIRE_LCDC_H */










