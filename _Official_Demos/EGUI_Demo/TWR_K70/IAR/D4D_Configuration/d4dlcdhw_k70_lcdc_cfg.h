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
* @file      d4dlcdhw_k70_lcdc_cfg.h
*
* @author    Petr Gargulak
* 
* @version   0.0.5.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - k70_lcdc hardware lcd driver configuration header file 
*
*******************************************************************************/

#ifndef __D4DLCDHW_K70_LCDC_CFG_H
#define __D4DLCDHW_K70_LCDC_CFG_H


  /******************************************************************************
  * includes
  ******************************************************************************/
  // include here what the driver need for run for example "derivative.h"
  // #include "derivative.h"    /* include peripheral declarations and more for S08 and CV1 */ 
 // #include "MK70F15.h"
  #include <mqx.h>  
  #include <psp.h>  
  #include <bsp.h>     
  /******************************************************************************
  * Constants
  ******************************************************************************/
  
  #define D4D_LLD_LCD_HW_K70LCDC d4dlcdhw_k70_lcdc_twr_rgbB

  
  #define D4DLCDHWFB_X_MAX                      480  
  #define D4DLCDHWFB_Y_MAX                      272 
  #define D4DLCDHWFB_BPP_BYTE                   4
  #define D4DLCDHWFB_BPP BPP24
  #define D4DLCDHWFB_MIRROWED                   0
  #define D4DLCDHWFB_CACHE_FLUSH_ENABLE         1
  #define D4DLCDHWFB_DOUBLE_BUFFER              1
  #define D4DLCDHWFB_DMA_ENABLE                 1
  #define D4DLCDHWFB_DMA_CHANNEL                0 
  
//#define D4DLCDHWFB_DMA_CHPRI ((D4DLCDHWFB_DMA_CHANNEL & 0xf))

//#define D4DLCDHWFB_START_ADDRESS 0x70400000

//#define LCDC_DEBUG      0  
    
  // define here what you need to be configurable

#endif /* __D4DLCDHW_K70_LCDC_CFG_H */