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
* @file      hw_iic.c
* @author    Pavel Michlicek, Pavel Krenek
* @version   1.0.6.0
* @date      Oct-2-2013
* @brief     Hard I2C routines, SINGLE MASTER ONLY!
*
******************************************************************************/

#include "low_level_drivers/touch_screen/touch_screen_drivers/cr_touch/hw_iic_cfg.h"



#if D4D_I2C_DISABLE_COMPILATION != 0

#include "low_level_drivers/touch_screen/touch_screen_drivers/cr_touch/iic/hw_iic.h"
#include "low_level_drivers/touch_screen/touch_screen_drivers/cr_touch/iic/hw_iic_prv.h"

  /******************************************************************************
  * Macros
  ******************************************************************************/
  #define D4D_I2C_ADR_WRITE(i2c_address)  ((Byte)((i2c_address<<1) & 0xFE))
  #define D4D_I2C_ADR_READ(i2c_address)   ((Byte)((i2c_address<<1) | 0x01))

  #define HwIIC_StopCndtn() D4D_I2C_C1 = D4D_I2C_C1_I2CEN_MASK;       // stop
  #define HwIIC_RepeatStartCndtn() D4D_I2C_C1 |= D4D_I2C_C1_RSTA_MASK; // repeat start
  #define HwIIC_ReadMode() D4D_I2C_C1 &= ~D4D_I2C_C1_TX_MASK; (void)D4D_I2C_D; // read mode
  #define HwIIC_WriteMode() D4D_I2C_C1 |= D4D_I2C_C1_TX_MASK;      //write mode

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/
  static void HwIIC_RestoreBus(void);
  static Byte HwIIC_StartCndtn(void);
  //static void HwIIC_RepeatStartCndtn(void);
  //static void HwIIC_StopCndtn(void);
  //static void HwIIC_ReadMode(void);
  //static void HwIIC_WriteMode(void);
  static Byte HwIIC_Write(Byte val);
  static Byte HwIIC_Read(Byte* pData, Byte ack);
  static LWord HwIIC_Abs(sLWord in);
  static void HwIIC_CpuDelay10US(unsigned short us10);

  /**************************************************************//*!
  * Global variables
  ******************************************************************/

  /**************************************************************//*!
  * Local variables
  ******************************************************************/
  static Byte chip_address = 0;
  static Byte doStopIfnack = 1;
  static Byte i2c_f = D4D_I2C_FREQ_REG;



  /**************************************************************//*!
  *
  * API Public Functions bodies
  *
  ******************************************************************/

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_Init
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function initialiaze I2C peripheral and pins
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_Init(void)
  {
    #ifdef D4D_I2C_USER_INIT
      D4D_I2C_USER_INIT
    #endif

    D4D_I2C_INIT_SCL
    D4D_I2C_INIT_SDA

    D4D_I2C_F = i2c_f;
    D4D_I2C_C1 = D4D_I2C_C1_I2CEN_MASK;        // I2C en.

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_DeInit
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function switch off I2C peripheral and pins
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_DeInit(void)
  {
    D4D_I2C_C1 = 0;
    D4D_I2C_SDA_PIN_ENABLE(D4D_I2C_PCR_PIN_GPIO)
    D4D_I2C_SCL_PIN_ENABLE(D4D_I2C_PCR_PIN_GPIO)

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_SetBaudRate
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function that try to compute and set the I2C frequency
  //
  // PARAMETERS:  baudrate - final baudrate frequency
  //
  // RETURNS:     differencies of real frequency to requested
  //
  //-----------------------------------------------------------------------------
  sLWord HwIIC_SetBaudRate(LWord baudRate)
  {
    const Word scl_divider[] = {
    20 ,  22 ,  24 ,  26 ,  28 ,  30 ,  34 ,  40 ,  28 ,  32 ,  36 ,  40 ,  44 ,
    48 ,  56 ,  68 ,  48 ,  56 ,  64 ,  72 ,  80 ,  88 ,  104,  128,  80 ,  96 ,
    112,  128,  144,  160,  192,  240,  160,  192,  224,  256,  288,  320,  384,
    480,  320,  384,  448,  512,  576,  640,  768,  960,  640,  768,  896,  1024,
    1152,  1280,  1536,  1920,  1280,  1536,  1792,  2048,  2304,  2560,  3072,
    3840
    };

    sLWord bestCaseVal = 0x80000000;
    sLWord tmpVal;
    Byte mul, icr;


    for(mul = 0; mul < 3; mul++)
      for(icr = 0; icr < 0x40; icr++)
      {
        tmpVal = (baudRate - (D4D_I2C_FREQ_BUS / ((1 << mul) * scl_divider[icr])));

        if(HwIIC_Abs(tmpVal) < HwIIC_Abs(bestCaseVal))
        {
          bestCaseVal = tmpVal;
          i2c_f = ((mul)?(0x40 << mul) : 0) | icr;
        }
      }
    HwIIC_DeInit();
    HwIIC_Init();

    return bestCaseVal;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_SetAddress
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function sets the I2C address that will be used
  //
  // PARAMETERS:  address - new I2C address that will be used for communication
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  void HwIIC_SetAddress(Byte address)
  {
    chip_address = address;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_GetAddress
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function return the I2C address that is used by driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     address - I2C address that is used for communication
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_GetAddress(void)
  {
    return chip_address;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_ReadByte
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read one byte from I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer where will be stored read byte
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_ReadByte(Byte regAdr, Byte* pData)
  {
    // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send repeat start condition
    HwIIC_RepeatStartCndtn();

    // Send the chip address with read flag
    if(HwIIC_Write(D4D_I2C_ADR_READ(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    //Switch peripharel to read mode
    HwIIC_ReadMode();

    // Read Byte from bus & send NACk to notify slave that this is last one
    if(HwIIC_Read(pData, D4D_I2C_NACK) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send stop condition
    //HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_WriteByte
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write one byte to I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              data - data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_WriteByte(Byte regAdr, Byte data)
  {
    // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of written register
    if(HwIIC_Write(regAdr) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the data
    if(HwIIC_Write(data) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send stop condition
    HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write multiple bytes to I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_MultiWriteBytes(Byte regAdr, Byte *pData, Byte cnt)
  {
    // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of written register
    if(HwIIC_Write(regAdr) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the data
    while(cnt--)
    {
      if(HwIIC_Write(*pData) !=  D4D_I2C_OK)
        return D4D_I2C_FALSE;

      pData++;
    }

    // Send stop condition
    HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read multiple bytes from I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_MultiReadBytes(Byte regAdr, Byte *pData, Byte cnt)
  {
   // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send repeat start condition
    HwIIC_RepeatStartCndtn();

    // Send the chip address with read flag
    if(HwIIC_Write(D4D_I2C_ADR_READ(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // check if there is requested something to read
    if(cnt)
    {
      //Switch peripharel to read mode
      HwIIC_ReadMode();

      // Read Byte from bus
      while(cnt--)
      {
        // Read Byte from bus , send NACK on last read byte
        if(HwIIC_Read(pData++, ((cnt)? D4D_I2C_ACK : D4D_I2C_NACK)) != D4D_I2C_OK)
          return D4D_I2C_FALSE;
      }
    }else
      HwIIC_StopCndtn(); // Send stop condition in normal end of function is Stop
    // send by read function


    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_LookForDevice
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function return if the device with specific address is
  //              present on the BUS
  //
  // PARAMETERS:  adr - address of device taht should be checked
  //
  // RETURNS:     result - result of I2C check operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_LookForDevice(Byte adr)
  {
   // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(adr)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send stop condition
    HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }
  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_ReadArrayDelayed
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read multiple bytes from I2C device (8 bit address)
  //              It can wait for device with some delay
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_ReadArrayDelayed(Byte regAdr, Byte* pData, Byte cnt, Word attemps)
  {
   // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // disable functionality of write function that end communication after NACK
    doStopIfnack = 0;
    while(attemps--)
    {
      // Send repeat start condition
      HwIIC_RepeatStartCndtn();

      // Send the chip address with read flag
      if(HwIIC_Write(D4D_I2C_ADR_READ(chip_address)) == D4D_I2C_OK)
        break;
    }
    // enable functionality of write function that end communication after NACK
    doStopIfnack = 1;

    if(!attemps)
    {
      // Send stop condition
      HwIIC_StopCndtn();

      return D4D_I2C_FALSE;
    }

    // check if there is requested something to read
    if(cnt)
    {
      //Switch peripharel to read mode
      HwIIC_ReadMode();

      // Read Byte from bus
      while(cnt--)
      {
        // Read Byte from bus , send NACK on last read byte
        if(HwIIC_Read(pData++, ((cnt)? D4D_I2C_ACK : D4D_I2C_NACK)) != D4D_I2C_OK)
          return D4D_I2C_FALSE;
      }
    }else
      HwIIC_StopCndtn(); // Send stop condition in normal end of function is Stop
    // send by read function

    return D4D_I2C_OK;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_ReadByte16
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read one byte from I2C device (16 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer where will be stored read byte
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_ReadByte16(D4D_I2C_WORD_BYTE regAdr, Byte* pData)
  {
    // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr.b.h) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    if(HwIIC_Write(regAdr.b.l) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send repeat start condition
    HwIIC_RepeatStartCndtn();

    // Send the chip address with read flag
    if(HwIIC_Write(D4D_I2C_ADR_READ(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    //Switch peripharel to read mode
    HwIIC_ReadMode();

    // Read Byte from bus & send NACk to notify slave that this is last one
    if(HwIIC_Read(pData, D4D_I2C_NACK) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send stop condition
    //HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_WriteByte16
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write one byte to I2C device (16 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              data - data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_WriteByte16(D4D_I2C_WORD_BYTE regAdr, Byte data)
  {
    // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr.b.h) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    if(HwIIC_Write(regAdr.b.l) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the data
    if(HwIIC_Write(data) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send stop condition
    HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write multiple bytes to I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_MultiWriteBytes16(D4D_I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt)
  {
    // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr.b.h) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    if(HwIIC_Write(regAdr.b.l) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the data
    while(cnt--)
    {
      if(HwIIC_Write(*pData) !=  D4D_I2C_OK)
        return D4D_I2C_FALSE;

      pData++;
    }

    // Send stop condition
    HwIIC_StopCndtn();

    return D4D_I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read multiple bytes from I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte HwIIC_MultiReadBytes16(D4D_I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt)
  {
   // Send start condition
    if(HwIIC_StartCndtn() != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the chip address with write flag
    if(HwIIC_Write(D4D_I2C_ADR_WRITE(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send the address of read register
    if(HwIIC_Write(regAdr.b.h) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    if(HwIIC_Write(regAdr.b.l) !=  D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // Send repeat start condition
    HwIIC_RepeatStartCndtn();

    // Send the chip address with read flag
    if(HwIIC_Write(D4D_I2C_ADR_READ(chip_address)) != D4D_I2C_OK)
      return D4D_I2C_FALSE;

    // check if there is requested something to read
    if(cnt)
    {
      //Switch peripharel to read mode
      HwIIC_ReadMode();

      // Read Byte from bus
      while(cnt--)
      {
        // Read Byte from bus , send NACK on last read byte
        if(HwIIC_Read(pData++, ((cnt)? D4D_I2C_ACK : D4D_I2C_NACK)) != D4D_I2C_OK)
          return D4D_I2C_FALSE;
      }
    }else
      HwIIC_StopCndtn(); // Send stop condition in normal end of function is Stop
    // send by read function

    return D4D_I2C_OK;
  }

  /**************************************************************//*!
  *
  * Private Functions bodies
  *
  ******************************************************************/

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_Abs
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function make absolute reselt from signed
  //
  // PARAMETERS:  in - signed input
  //
  // RETURNS:     unsigned output
  //
  //-----------------------------------------------------------------------------
  static LWord HwIIC_Abs(sLWord in)
  {
    if(in < 0)
      return (LWord)(in * -1);

    return (LWord)in;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_StartCndtn
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function sets START condition on I2C bus. It also check bus
  //              Busy status and sets the peripheral to Master mode and transmit
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result - result of I2C read operation D4D_I2C_OK / D4D_I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  static Byte HwIIC_StartCndtn(void)
  {
    Word tmp = 1;

    // wait to free I2C bus - none BUSY
    while((D4D_I2C_S & D4D_I2C_S_BUSY_MASK) && (++tmp)){};   // wait for bus free

    // check the timeout
    if(tmp)
    {
      // if all is fine, do start condition and set the transmit for address
      D4D_I2C_C1 = (D4D_I2C_C1_I2CEN_MASK | D4D_I2C_C1_MST_MASK | D4D_I2C_C1_TX_MASK);

      // notify that all end succesfully
      return D4D_I2C_OK;
    }

    // in case of timeout restore I2C BUS
    HwIIC_RestoreBus();

    // and notify
    return D4D_I2C_FALSE;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_StopCndtn
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function sets STOP condition on I2C bus.
  //              It's implemented by MACRO
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  /*
  static void HwIIC_StopCndtn(void)
  {
    D4D_I2C_C1 = D4D_I2C_C1_I2CEN_MASK;        // - I2C mstr .
  }
  */

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_RepeatStartCndtn
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function sets REPEAT START condition on I2C bus.
  //              It's implemented by MACRO
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  /*
  static void HwIIC_RepeatStartCndtn(void)
  {
    D4D_I2C_C1 |= D4D_I2C_C1_RSTA_MASK;
  }
  */

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_ReadMode
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function switch I2C peripheral to read mode
  //              It's implemented by MACRO
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  /*
  static void HwIIC_ReadMode(void)
  {

    volatile Byte test;
    D4D_I2C_C1 &= ~D4D_I2C_C1_TX_MASK;      //read mode
    test = D4D_I2C_D;             //dummy
  }
  */

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_WriteMode
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function switch I2C peripheral to write mode
  //              It's implemented by MACRO
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  /*
  static void HwIIC_WriteMode(void)
  {
     D4D_I2C_C1 |= D4D_I2C_C1_TX_MASK;      //write mode
  }
  */

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_RestoreBus
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function restore/reset the I2C bus in case if its stucked
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  static void HwIIC_RestoreBus(void)
  {
    Byte tmp = 0;

    HwIIC_StopCndtn();
    HwIIC_DeInit();

    // restore action MUST be in same defined baudrate for normal communication

    D4D_SET_INPUT_I2C_SDA //SDA pin set to input
    D4D_SET_OUTPUT_I2C_SCL //SCL Pin set to output

    // nine clock for data
    for(tmp = 0; tmp <9; tmp ++)
    {
      D4D_RESET_I2C_SCL
      HwIIC_CpuDelay10US(1);

      D4D_SET_I2C_SCL
      HwIIC_CpuDelay10US(1);
    }

    D4D_RESET_I2C_SCL

    SET_D4D_I2C_SDA    //negative ackowledge
    D4D_SET_OUTPUT_I2C_SDA //SDA pin set to output

    HwIIC_CpuDelay10US(1);

    D4D_SET_I2C_SCL
    HwIIC_CpuDelay10US(1);

    D4D_RESET_I2C_SCL
    HwIIC_CpuDelay10US(1);

    RESET_D4D_I2C_SDA    //stop
    HwIIC_CpuDelay10US(1);

    D4D_SET_I2C_SCL
    HwIIC_CpuDelay10US(1);

    HwIIC_Init();
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_Write
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function send one byte on bus
  //
  // PARAMETERS:  data - data to send
  //
  // RETURNS:     result of write operation
  //                      D4D_D4D_I2C_ - byte successfuly sent and receive ACK
  //                      D4D_I2C_NACK - byte successfuly sent but not acknowledged.
  //                                 Also was did STOP condition on bus
  //                      D4D_I2C_FALSE - can't sent byte (called restore bus)
  //
  //-----------------------------------------------------------------------------
  static Byte HwIIC_Write(Byte data)
  {
    Word tmp = 1;
    // write data to I2C data register
    D4D_I2C_D = data;

    // wait to finished send sequence
    while (((D4D_I2C_S & D4D_I2C_S_I2CIF_MASK) == 0) && (++tmp)){};

    // check if the wait operation finished in time
    if(tmp)
    {
      // Clear interrupt flag
      D4D_I2C_S |= D4D_I2C_S_I2CIF_MASK;

      // return the status of acknowledge
      if(D4D_I2C_S & D4D_I2C_S_RXAK_MASK)
      {
        // End communication ?
        if(doStopIfnack)
          HwIIC_StopCndtn();
        return D4D_I2C_NACK;
      }

      // return OK
      return D4D_I2C_OK;
    }

    // Try to restore I2C BUS
    HwIIC_RestoreBus();

    // And notify
    return D4D_I2C_FALSE;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_Read
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function read one byte on bus
  //
  // PARAMETERS:  pData - pointer where should be stored read byte
  //              ack - acknowledge after read byte (for last reading should be NACK)
  //
  // RETURNS:     result of write operation
  //                      D4D_D4D_I2C_ - byte successfuly read
  //                      D4D_I2C_FALSE - can't read byte (called restore bus)
  //
  //-----------------------------------------------------------------------------
  static Byte HwIIC_Read(Byte* pData, Byte ack)
  {
    Word tmp = 1;

    // start reading with final ACk or NACK
    if(!ack)
      D4D_I2C_C1 &= ~D4D_I2C_C1_TXAK_MASK;
    else
      D4D_I2C_C1 |= D4D_I2C_C1_TXAK_MASK;

    // wait for transfer is complete
    while (((D4D_I2C_S & D4D_I2C_S_I2CIF_MASK) == 0) && (++tmp)){};

    if(tmp)
    {
      // the operation ends on time
      if(ack)
        HwIIC_StopCndtn();

      // read the readed byte
      *pData = D4D_I2C_D;

      // clear the interupt flag
      D4D_I2C_S |= D4D_I2C_S_I2CIF_MASK;

      return D4D_I2C_OK;
    }

    // operation failed - do restoration of BUS
    HwIIC_RestoreBus();

    // and notify that operation failed
    return D4D_I2C_FALSE;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    HwIIC_CpuDelay10US
  // SCOPE:       HW I2C peripheral driver private function
  // DESCRIPTION: The function do delay ~ 10us
  //
  // PARAMETERS:  us10 - count of 10 us delays
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  #define MCU_BUS_CYCLES_10US (D4D_I2C_FREQ_BUS / 100000)

    #if (D4D_I2C_MCU_TYPE == D4D_I2C_MCU_TYPE_S08)
    // S08
    #error "S08 is currently unssuported MCU type for delay loop in loe level common driver!"
    static void HwIIC_CpuDelay10US(unsigned short us10)
    {

    }
    #elif (D4D_I2C_MCU_TYPE == D4D_I2C_MCU_TYPE_CV)
    //ColdFire
    #error "ColdFire is currently unssuported MCU type for delay loop in loe level common driver!"
    static void HwIIC_CpuDelay10US(unsigned short us10)
    {

    }
   // Kinetis
   #elif (D4D_I2C_MCU_TYPE == D4D_I2C_MCU_TYPE_MK)
    static void HwIIC_CpuDelay10US(unsigned short us10)
    {
      unsigned int i;
      while((us10--))
      {
        for(i=0; i < (MCU_BUS_CYCLES_10US/5); i++)
        {// 10 cycles delay
          asm("NOP");
          asm("NOP");
          asm("NOP");
        }
      }
    }


   #else
    #error "Unsupported MCU type for delay loop in low level common driver!"



   #endif
#endif
