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
* @file      hw_iic_prv.h
* @author    Pavel Michlicek, Pavel Krenek
* @version   1.0.4.0
* @date      Oct-2-2013
* @brief     HardI2C private definitions
*
******************************************************************************/

#ifndef _HW_D4D_I2C_PRV_H
#define _HW_D4D_I2C_PRV_H



/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/

#define SET(x)       (x##_PORT) |= (1 << (x))
#define RESET(x)     (x##_PORT) &= ~(1 << (x))

#define GET(x)      ((x##_PORT) & ~(1 << (x)))


#define OUTPUT(x)    (x##_DDR) |= (1 << (x)) ;
#define INPUT(x)     (x##_DDR) &= ~(1 << (x));

#ifndef D4D_I2C_MCU_TYPE
  #define D4D_I2C_MCU_TYPE D4D_I2C_MCU_TYPE_MK
#endif

#ifndef D4D_I2C_FREQ_REG
  #define D4D_I2C_FREQ_REG   0x17
#endif

#ifndef D4D_I2C_FREQ_BUS
  #define D4D_I2C_FREQ_BUS   50000000u
#endif

#ifndef D4D_I2C_PCR_PIN_GPIO
  #define D4D_I2C_PCR_PIN_GPIO  1
#endif

#ifndef D4D_I2C_PCR_PIN_I2C
  #define D4D_I2C_PCR_PIN_I2C   4
#endif

#ifndef D4D_I2C_SCL_PIN_ENABLE
  #ifndef D4D_I2C_SCL_PCR
    #error "The Macro D4D_I2C_SCL_PCR must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_I2C_SCL_PIN_ENABLE(alt)  (D4D_I2C_SCL_PCR = PORT_PCR_MUX(alt) | PORT_PCR_ODE_MASK);
  #endif
#endif

#ifndef D4D_I2C_INIT_SCL
    #define D4D_I2C_INIT_SCL  D4D_I2C_SCL_PIN_ENABLE(D4D_I2C_PCR_PIN_I2C);
#endif

#ifndef D4D_RESET_I2C_SCL
    #ifndef D4D_I2C_SCL
    #error "The Macro D4D_I2C_SCL must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_RESET_I2C_SCL RESET(D4D_I2C_SCL);
  #endif
#endif

#ifndef D4D_SET_I2C_SCL
  #ifndef D4D_I2C_SCL
    #error "The Macro D4D_I2C_SCL must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_SET_I2C_SCL SET(D4D_I2C_SCL);
  #endif
#endif

#ifndef D4D_SET_OUTPUT_I2C_SCL
  #ifndef D4D_I2C_SCL
    #error "The Macro D4D_I2C_SCL must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_SET_OUTPUT_I2C_SCL OUTPUT(D4D_I2C_SCL);
  #endif
#endif

#ifndef SET_INPUT_D4D_I2C_SCL
  #ifndef D4D_I2C_SCL
    #error "The Macro D4D_I2C_SCL must be set in Touch Screen low level driver configuration file."
  #else
    #define SET_INPUT_D4D_I2C_SCL INPUT(D4D_I2C_SCL);
  #endif
#endif

#ifndef GET_D4D_I2C_SCL
  #ifndef D4D_I2C_SCL
    #error "The Macro D4D_I2C_SCL must be set in Touch Screen low level driver configuration file."
  #else
    #define GET_D4D_I2C_SCL GET(D4D_I2C_SCL);
  #endif
#endif

#ifndef D4D_I2C_SDA_PIN_ENABLE
  #ifndef D4D_I2C_SDA_PCR
    #error "The Macro D4D_I2C_SDA_PCR must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_I2C_SDA_PIN_ENABLE(alt) (D4D_I2C_SDA_PCR = PORT_PCR_MUX(alt) | PORT_PCR_ODE_MASK);
  #endif
#endif


#ifndef D4D_I2C_INIT_SDA
    #define D4D_I2C_INIT_SDA  D4D_I2C_SDA_PIN_ENABLE(D4D_I2C_PCR_PIN_I2C)
#endif

#ifndef RESET_D4D_I2C_SDA
    #ifndef D4D_I2C_SDA
    #error "The Macro D4D_I2C_SDA must be set in Touch Screen low level driver configuration file."
  #else
    #define RESET_D4D_I2C_SDA RESET(D4D_I2C_SDA);
  #endif
#endif

#ifndef SET_D4D_I2C_SDA
  #ifndef D4D_I2C_SDA
    #error "The Macro D4D_I2C_SDA must be set in Touch Screen low level driver configuration file."
  #else
    #define SET_D4D_I2C_SDA SET(D4D_I2C_SDA);
  #endif
#endif

#ifndef SET_D4D_I2C_SDA
  #ifndef D4D_I2C_SDA
    #error "The Macro D4D_I2C_SDA must be set in Touch Screen low level driver configuration file."
  #else
    #define SET_D4D_I2C_SDA SET(D4D_I2C_SDA);
  #endif
#endif

#ifndef D4D_SET_OUTPUT_I2C_SDA
  #ifndef D4D_I2C_SDA
    #error "The Macro D4D_I2C_SDA must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_SET_OUTPUT_I2C_SDA OUTPUT(D4D_I2C_SDA);
  #endif
#endif

#ifndef D4D_SET_INPUT_I2C_SDA
  #ifndef D4D_I2C_SDA
    #error "The Macro D4D_I2C_SDA must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_SET_INPUT_I2C_SDA INPUT(D4D_I2C_SDA);
  #endif
#endif

#ifndef D4D_GET_I2C_SDA
  #ifndef D4D_I2C_SDA
    #error "The Macro D4D_I2C_SDA must be set in Touch Screen low level driver configuration file."
  #else
    #define D4D_GET_I2C_SDA GET(D4D_I2C_SDA);
  #endif
#endif


#ifndef D4D_I2C_DISABLE_COMPILATION
  #define D4D_I2C_DISABLE_COMPILATION 0
#endif

#ifndef D4D_I2C_C1_I2CEN_MASK
  #define D4D_I2C_C1_I2CEN_MASK                         0x80u
#endif

#ifndef D4D_I2C_C1_MST_MASK
  #define D4D_I2C_C1_MST_MASK                           0x20u
#endif

#ifndef D4D_I2C_C1_TX_MASK
  #define D4D_I2C_C1_TX_MASK                            0x10u
#endif

#ifndef D4D_I2C_C1_TXAK_MASK
  #define D4D_I2C_C1_TXAK_MASK                          0x08u
#endif

#ifndef D4D_I2C_C1_RSTA_MASK
  #define D4D_I2C_C1_RSTA_MASK                          0x04u
#endif


#ifndef D4D_I2C_S_BUSY_MASK
  #define D4D_I2C_S_BUSY_MASK                           0x20u
#endif

#ifndef D4D_I2C_S_I2CIF_MASK
  #define D4D_I2C_S_I2CIF_MASK                          0x02u
#endif

#ifndef D4D_I2C_S_RXAK_MASK
  #define D4D_I2C_S_RXAK_MASK                           0x01u
#endif

#if (D4D_I2C_MCU_TYPE == D4D_I2C_MCU_TYPE_S08)
  // S08
  #error "S08 is currently unssuported MCU type for delay loop in loe level common driver!"
#elif (D4D_I2C_MCU_TYPE == D4D_I2C_MCU_TYPE_CV)
  //ColdFire
  #error "ColdFire is currently unssuported MCU type for delay loop in loe level common driver!"
#elif (D4D_I2C_MCU_TYPE == D4D_I2C_MCU_TYPE_MK)
  // Kinetis
  #define D4D_I2C_A1                                  I2C_A1_REG(D4D_I2C_BASE)
  #define D4D_I2C_F                                   I2C_F_REG(D4D_I2C_BASE)
  #define D4D_I2C_C1                                  I2C_C1_REG(D4D_I2C_BASE)
  #define D4D_I2C_S                                   I2C_S_REG(D4D_I2C_BASE)
  #define D4D_I2C_D                                   I2C_D_REG(D4D_I2C_BASE)
  #define D4D_I2C_C2                                  I2C_C2_REG(D4D_I2C_BASE)
  #define D4D_I2C_FLT                                 I2C_FLT_REG(D4D_I2C_BASE)
  #define D4D_I2C_RA                                  I2C_RA_REG(D4D_I2C_BASE)
  #define D4D_I2C_SMB                                 I2C_SMB_REG(D4D_I2C_BASE)
  #define D4D_I2C_A2                                  I2C_A2_REG(D4D_I2C_BASE)
  #define D4D_I2C_SLTH                                I2C_SLTH_REG(D4D_I2C_BASE)
  #define D4D_I2C_SLTL                                I2C_SLTL_REG(D4D_I2C_BASE)
#else
  #error "Unsupported MCU type for delay loop in low level common driver!"
#endif

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

#endif //_HW_D4D_I2C_PRV_H
