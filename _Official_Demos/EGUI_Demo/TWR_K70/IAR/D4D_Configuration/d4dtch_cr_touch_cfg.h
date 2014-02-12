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
* @file      d4dtch_cr_touch_cfg.h
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - cr_touch touch screen driver configuration header file 
*
*******************************************************************************/

#ifndef __D4DTCH_CR_TOUCH_CFG_H
#define __D4DTCH_CR_TOUCH_CFG_H


  /******************************************************************************
  * includes
  ******************************************************************************/
  // include here what the driver need for run for example "derivative.h"
  // #include "derivative.h"    /* include peripheral declarations and more for S08 and CV1 */ 
 // #include "MK70F15.h"
  /******************************************************************************
  * Constants
  ******************************************************************************/
    
    #define D4D_LLD_TCH_HW_CRTOUCH d4dtchhw_crtouch_iicBareMetal
//#define D4D_LLD_TCH_HW_CRTOUCH d4dtchhw_crtouch_iicMqx
    #define D4DTCH_IIC_BAUDRATE           400000
  
    #define D4DTCH_IIC_ADDRESS           0x49
    
    #define D4DTCH_INVERSE_X               0
    #define D4DTCH_INVERSE_Y               0

    #define D4DTCH_SWAP_AXES               0

#if D4D_LLD_TCH_HW_CRTOUCH == d4dtchhw_crtouch_iicBareMetal
    #define D4D_I2C_BASE      I2C0_BASE_PTR

#define D4D_I2C_USER_INIT SIM_SCGC4 |= SIM_SCGC4_IIC2_MASK; // the index 2 is bug of K70 header file :-(
    
    #define D4D_I2C_FREQ_REG   0x17        // 400Khz for 100MHz       

    #define D4D_I2C_FREQ_BUS   50000000u

       
    #define D4D_I2C_SDA_PCR   PORTE_PCR18
    #define D4D_I2C_SCL_PCR   PORTE_PCR19

    #define D4D_I2C_SCL   19
    #define D4D_I2C_SCL_PORT  GPIOE_PDOR   // Data output register
    #define D4D_I2C_SCL_DDR   GPIOE_PDDR   // Output enable register
    
    #define D4D_I2C_SDA   18
    #define D4D_I2C_SDA_PORT  GPIOE_PDOR   // Data output register
    #define D4D_I2C_SDA_DDR   GPIOE_PDDR   // Output enable register

#endif

  // define here what you need to be configurable

#endif /* __D4DTCH_CR_TOUCH_CFG_H */