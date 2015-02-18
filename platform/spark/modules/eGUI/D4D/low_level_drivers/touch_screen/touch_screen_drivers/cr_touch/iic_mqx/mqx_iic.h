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
* @file      mqx_iic.h
* @author    Petr Gargulak
* @version   1.0.3.0
* @date      Mar-19-2012
* @brief     MqxI2C definitions
*
******************************************************************************/

#ifndef _MQX_I2C_H
#define _MQX_I2C_H

// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW_CRTOUCH) == d4dtchhw_crtouch_iicMqx_ID)

  /******************************************************************************
  * Constants
  ******************************************************************************/
#ifndef I2C_OK
  #define I2C_OK 0
#endif
  #define I2C_FALSE  1
  #define I2C_ACK  I2C_OK
  #define I2C_NACK 2


  /******************************************************************************
  * Macros
  ******************************************************************************/

  #ifndef MQX_I2C_DEV
    #define MQX_I2C_DEV "i2c0:"
  #endif

  /******************************************************************************
  * Types
  ******************************************************************************/
  typedef union
  {
    Word w;
    struct
    {
      Byte h;
      Byte l;
    }b;
  }I2C_WORD_BYTE;

  /******************************************************************************
  * Global functions
  ******************************************************************************/

  // Init Denit functions
  Byte MqxIIC_Init(void);
  Byte MqxIIC_DeInit(void);
  // Address related functions
  void MqxIIC_SetAddress(Byte address);
  Byte MqxIIC_GetAddress(void);
  // Single Byte read Write functions - 8 bit device address mode
  Byte MqxIIC_ReadByte(Byte regAdr, Byte* pData);
  Byte MqxIIC_WriteByte(Byte regAdr, Byte data);
  // Multi Byte read Write functions - 8 bit device address mode
  Byte MqxIIC_MultiReadBytes(Byte regAdr, Byte *pData, Byte cnt);
  Byte MqxIIC_MultiWriteBytes(Byte regAdr, Byte *pData, Byte cnt);
  Byte MqxIIC_ReadArrayDelayed(Byte regAdr, Byte* pData, Byte cnt, Word attemps);
  // Single Byte read Write functions - 16 bit device address mode
  Byte MqxIIC_ReadByte16(I2C_WORD_BYTE regAdr, Byte* pData);
  Byte MqxIIC_WriteByte16(I2C_WORD_BYTE regAdr, Byte data);
  // Multi Byte read Write functions - 16 bit device address mode
  Byte MqxIIC_MultiReadBytes16(I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt);
  Byte MqxIIC_MultiWriteBytes16(I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt);

  //Help functions
  Byte MqxIIC_LookForDevice(Byte adr);
  sLWord MqxIIC_SetBaudRate(LWord baudRate);


  #endif
#endif
