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
* @file      d4d_user_cfg.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-24-2013
* 
* @brief     D4D Driver user configuration file for Kinetis TWR-K60N512
*
*******************************************************************************/

#ifndef __D4D_USER_CFG_H
#define __D4D_USER_CFG_H

/******************************************************************************
* Desciption : This file allows user depends modification of D4D/eGUI 
*   graphical driver. 
*
*
******************************************************************************/

/**************************************************************//*!
*
* User Include header file section
*
******************************************************************/
 #include <mqx.h>
 #include <bsp.h>

/**************************************************************//*!
*
* Low Level driver specification for LCD diplay and optionally for 
* touch screen
*
******************************************************************/
// List of implemented low level LCD drivers

// d4dlcd_ssd1289 - driver for LCD displays with SSD1289 controller from Solomon Systech
// d4dlcd_fsa506 - driver for LCD displays with FSA506 controller
// d4dlcd_lgdp4531 - driver for LCD displays with LGDP4531 controller from LG Electronics
// d4dlcd_ls020 - driver for LCD displays LS020 
// d4dlcd_frame_buffer - frame buffer driver for MCU/MPU with LCD peripheral 


// Please define a used low LCD driver
//#define D4D_LLD_LCD d4dlcd_ssd1289   // the name of low level driver descriptor structure
#define D4D_LLD_LCD d4dlcd_frame_buffer   // the name of low level driver descriptor structure
// List of implemented low level LCD hw interface drivers

// d4dlcdhw_flexbus_16b - low level hw interface driver for flexbus with 16 bit witdh
// d4dlcdhw_flexbus_8b - low level hw interface driver for flexbus with 8 bit witdh
// d4dlcdhw_gpio6800_8b - low level hw interface driver for gpio drived parallel 8b 6800 bus
// d4dlcdhw_gpio8080_8b - low level hw interface driver for gpio drived parallel 8b 8080 bus
// d4dlcdhw_gpio8080_byte_8b - low level hw interface driver for gpio drived parallel 8b 8080 bus
//                              byte version (sends only bytes instead of words as normal)
// d4dlcdhw_spi_8b - low level hw interface driver for hardware SPI with 8 bit witdh for S08 MCU
// d4dlcdhw_s12_spi_16b - low level hw interface driver for hardware SPI with 8 bit witdh for S12 MCU
// d4dlcdhw_swspi_16b - low level hw interface driver for software SPI with 16 bit witdh
// d4dlcdhw_spi_swc_8b - low level hw interface driver for hardware SPI with 8 bit witdh and software 
//                          controlled control signals of LCD controller (for example FSA506)

// d4dlcdhw_mqx_spi - low level driver for SPI over MQX (version 3.6 or higher)
// d4dlcdhw_mqx_fb - low level driver for flexbus over MQX (version 3.6 or higher)
// d4dlcdhw_dragonfire_lcdc - low level driver for MCF52277 MPU LCDC peripherial
// d4dlcdhw_mqx_mpc5125_diu - low level driver for MPC5125 MPU DIU peripherial over MQX (version 3.6 or higher)
// d4dlcdhw_k70_lcdc - low level driver for Kinetis K70 MCU LCDC peripherial
// d4dlcdhw_px_dcu_fb - low level driver for PX series MCU DCU peripherial
  
// Please (if it's needed) define a used LCD hw interface driver
#define D4D_LLD_LCD_HW d4dlcdhw_k70_lcdc

/**************************************************************//*!
*
* Touch screen low level driver section
*
******************************************************************/


// List of supported low level touch screen drivers

// d4dtch_resistive - driver for resistive touch screen with direct analog connection to MCU
// d4dtch_resistive_mqx - driver for resistive touch screen with direct analog connection to MCU over MQX (version 3.6 or higher)
// d4dtch_mcf52277_asp - driver for resistive touch screen over MCF52277 ASP peripheral
// d4dtch_cr_touch - driver for touch screen over CR TOUCH external chip driven over IIC (also there is MQX IIC driver version)

// Please define a used touch screen driver if touch screen is used in project
#define D4D_LLD_TCH  d4dtch_cr_touch


// List of implemented low level Touch screen hw interface drivers

// d4dtchhw_s08_adc - low level hw interface driver for S08 ADC
// d4dtchhw_s12_adc - low level hw interface driver for S12 ADC
// d4dtchhw_mcf52259_adc - low level hw interface driver for MCF52277 ASP peripheral (interrupt driven)
// d4dtchhw_kinetis_adc - low level hw interface driver for Kinetis ADC
// d4dtchhw_px_adc_10b - low level hw interface driver for PX series ADC

// Please (if it's needed) define a used touch screen hw interface driver
//#define D4D_LLD_TCH_HW d4dtchhw_kinetis_adc


#define D4D_LLD_MOUSE d4dmouse_k70_lcdc

/******************************************************************************
*
*        ==================== D4D General ======================              
*
*               User definition of general bahaviour of D4Dvr
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/

// The D4Drv contains own standard types for cases that in the whole project missing 
// loaded standard types as Byte, Word, LWord etc.
// To disable using of D4Drv own standard types change the below define to D4D_TRUE 

#define D4D_USE_STANDARD_TYPES D4D_FALSE


//Select the MCU type that is used in this project  supported types:
//D4D_HC08  	- HC08 
//D4D_HCS08 	- HCS08	
//D4D_HC12  	- HC12
//D4D_HCS12 	- HCS12
//D4D_HCS12X	- HCS12X
//D4D_MCF51 	- ColdFire V1
//D4D_MCF52 	- ColdFire V2
//D4D_MCF53 	- ColdFire V3
//D4D_MCF54		- ColdFire V4
//D4D_MPC51 	- MobileGT
//D4D_MK 		- Kinetis

#define D4D_MCU_TYPE D4D_MK

#define D4D_MCU_BUS_CLOCK 120000000L  // Mcu bus clock in Hz mainly for few delay loops in low level


#include "../../../common_sources/d4d_user_cfg_app.h"

#endif /* __D4D_USER_CFG_H */