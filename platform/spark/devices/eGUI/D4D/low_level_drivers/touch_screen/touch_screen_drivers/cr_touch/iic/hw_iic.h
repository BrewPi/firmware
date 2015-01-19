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
* @file      hw_iic.h
* @author    Pavel Michlicek, Pavel Krenek
* @version   1.0.2.0
* @date      Oct-2-2013
* @brief     HardI2C definitions
*
******************************************************************************/

#ifndef _HW_D4D_I2C_H
#define _HW_D4D_I2C_H


/******************************************************************************
* Constants
******************************************************************************/

#define D4D_I2C_OK 0
#define D4D_I2C_FALSE  1
#define D4D_I2C_ACK  D4D_I2C_OK
#define D4D_I2C_NACK 2


#define D4D_I2C_MCU_TYPE_S08 1
#define D4D_I2C_MCU_TYPE_CV 2
#define D4D_I2C_MCU_TYPE_MK 3


/******************************************************************************
* Macros
******************************************************************************/



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
}D4D_I2C_WORD_BYTE;

/******************************************************************************
* Global functions
******************************************************************************/

// Init Denit functions
Byte HwIIC_Init(void);
Byte HwIIC_DeInit(void);
// Address related functions
void HwIIC_SetAddress(Byte address);
Byte HwIIC_GetAddress(void);
// Single Byte read Write functions - 8 bit device address mode
Byte HwIIC_ReadByte(Byte regAdr, Byte* pData);
Byte HwIIC_WriteByte(Byte regAdr, Byte data);
// Multi Byte read Write functions - 8 bit device address mode
Byte HwIIC_MultiReadBytes(Byte regAdr, Byte *pData, Byte cnt);
Byte HwIIC_MultiWriteBytes(Byte regAdr, Byte *pData, Byte cnt);
Byte HwIIC_ReadArrayDelayed(Byte regAdr, Byte* pData, Byte cnt, Word attemps);
// Single Byte read Write functions - 16 bit device address mode
Byte HwIIC_ReadByte16(D4D_I2C_WORD_BYTE regAdr, Byte* pData);
Byte HwIIC_WriteByte16(D4D_I2C_WORD_BYTE regAdr, Byte data);
// Multi Byte read Write functions - 16 bit device address mode
Byte HwIIC_MultiReadBytes16(D4D_I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt);
Byte HwIIC_MultiWriteBytes16(D4D_I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt);

//Help functions
Byte HwIIC_LookForDevice(Byte adr);
sLWord HwIIC_SetBaudRate(LWord baudRate);



#endif
