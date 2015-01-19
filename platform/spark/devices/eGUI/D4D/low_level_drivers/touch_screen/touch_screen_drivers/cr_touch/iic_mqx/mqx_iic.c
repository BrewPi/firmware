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
* @file      mqx_iic.c
* @author    Petr Gargulak
* @version   1.0.5.0
* @date      Jun-29-2012
* @brief     Hard I2C routines, SINGLE MASTER ONLY!
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR

// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_cr_touch_ID 1


// copilation enable preprocessor condition
// the string d4dtch_cr_touch_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_cr_touch_ID)

#include "low_level_drivers\touch_screen\touch_screen_drivers\cr_touch\d4dtch_cr_touch.h"

// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtchhw_crtouch_iicMqx_ID 1




// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW_CRTOUCH) == d4dtchhw_crtouch_iicMqx_ID)

  #include <mqx.h>
  #include <bsp.h>
  #include <i2c.h>

  #include "low_level_drivers\touch_screen\touch_screen_drivers\cr_touch\iic_mqx\mqx_iic.h"

  /******************************************************************************
  * Macros
  ******************************************************************************/
  #define I2C_ADR_WRITE(i2c_address)  ((Byte)((i2c_address<<1) & 0xFE))
  #define I2C_ADR_READ(i2c_address)   ((Byte)((i2c_address<<1) | 0x01))


  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  /**************************************************************//*!
  * Global variables
  ******************************************************************/
  const D4DTCHHWCRTOUCH_FUNCTIONS d4dtchhw_crtouch_iicMqx =
  {
    MqxIIC_Init,
    MqxIIC_DeInit,
    MqxIIC_SetAddress,
    MqxIIC_SetBaudRate,
    MqxIIC_MultiReadBytes,
    MqxIIC_MultiWriteBytes,
    I2C_OK
  };
  /**************************************************************//*!
  * Local variables
  ******************************************************************/
  static Byte chip_address = 0;

  static FILE_PTR fd;
  static LWSEM_STRUCT lock;



  /**************************************************************//*!
  *
  * API Public Functions bodies
  *
  ******************************************************************/

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_Init
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function initialiaze I2C peripheral and pins
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_Init(void)
  {
    /* Open the I2C driver, and assign a I2C device handler*/
    fd = fopen (MQX_I2C_DEV, NULL);
    if (fd == NULL)
    {
      //printf ("Failed to open the I2C driver!\n");
      return I2C_FALSE;
    }

    /* Set I2C into Master mode */
    ioctl (fd, IO_IOCTL_I2C_SET_MASTER_MODE, NULL);

    return I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_DeInit
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function switch off I2C peripheral and pins
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_DeInit(void)
  {
    if(fd)
      fclose(fd);
    fd = NULL;

    return I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_SetBaudRate
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function that try to compute and set the I2C frequency
  //
  // PARAMETERS:  baudrate - final baudrate frequency
  //
  // RETURNS:     differencies of real frequency to requested
  //
  //-----------------------------------------------------------------------------
  sLWord MqxIIC_SetBaudRate(LWord baudRate)
  {
    LWord i2c_device_baudrate = baudRate;
    /* Set the destination address */

    return ioctl (fd, IO_IOCTL_I2C_SET_BAUD, &i2c_device_baudrate);
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_SetAddress
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function sets the I2C address that will be used
  //
  // PARAMETERS:  address - new I2C address that will be used for communication
  //
  // RETURNS:     none
  //
  //-----------------------------------------------------------------------------
  void MqxIIC_SetAddress(Byte address)
  {
    chip_address = address;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_GetAddress
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function return the I2C address that is used by driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     address - I2C address that is used for communication
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_GetAddress(void)
  {
    return chip_address;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_ReadByte
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read one byte from I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer where will be stored read byte
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_ReadByte(Byte regAdr, Byte* pData)
  {
    return MqxIIC_MultiReadBytes(regAdr, pData, 1);
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_WriteByte
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write one byte to I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              data - data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_WriteByte(Byte regAdr, Byte data)
  {
    uint_8 tmpdata = data;
    return MqxIIC_MultiWriteBytes(regAdr, &tmpdata, 1);
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write multiple bytes to I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_MultiWriteBytes(Byte regAdr, Byte *pData, Byte cnt)
  { /* Body */

     uint_32       param;
     uint_32       result;
     uint_8        mem;

     /* Protect I2C transaction in multitask environment */
     _lwsem_wait (&lock);

     //printf ("Writing %d bytes to address 0x%08x ...\n", cnt, regAdr);

        /* I2C bus address also contains memory block index */
       param = chip_address;
       //printf ("  Set I2C bus address to 0x%02x ... ", param);
       if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
       {
          //printf ("ERROR\n");
          /* Release the transaction lock */
           _lwsem_post (&lock);
          return I2C_FALSE;
       }

      /* Initiate start and send I2C bus address */
      //printf ("  I2C start, send address and get ack ... ");
      (void)fwrite (&mem, 1, 0, fd);

        /* Check ack (device exists) */
        if (I2C_OK == ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
        {
           //printf ("OK ... ack == %d\n", param);
           if (param)
           {
              /* Stop I2C transfer */
              //printf ("  Stop transfer ... ");
              if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
                 //printf ("ERROR\n");
              /* Release the transaction lock */
              _lwsem_post (&lock);
              return I2C_FALSE;
           }
        } else {
           //printf ("ERROR\n");
           /* Release the transaction lock */
           _lwsem_post (&lock);
           return I2C_FALSE;
        }

        /* Write address within memory block */
        mem = regAdr;
        //printf ("  Write to address 0x%02x ... ", mem);
        result = fwrite (&mem, 1, 1, fd);
        if (1 != result)
        {
           //printf ("ERROR\n");
           /* Release the transaction lock */
           _lwsem_post (&lock);
           return I2C_FALSE;
        }

        /* Page write of data */
        //printf ("  Page write %d bytes ... ", cnt);
        result = fwrite (pData, 1, cnt, fd);
        if (result != cnt)
        {
           //printf ("ERROR\n");
           /* Release the transaction lock */
           _lwsem_post (&lock);
           return I2C_FALSE;
        }

        /* Wait for completion */
        //printf ("  Flush ... ");
        result = fflush (fd);
        if (MQX_OK != result)
        {
           //printf ("ERROR\n");
           /* Release the transaction lock */
           _lwsem_post (&lock);
           return I2C_FALSE;
        }

        /* Stop I2C transfer - initiate EEPROM write cycle */
        //printf ("  Stop transfer ... ");
        if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
        {
           //printf ("ERROR\n");
           /* Release the transaction lock */
           _lwsem_post (&lock);
           return I2C_FALSE;
        }

        /* Wait for EEPROM write cycle finish - acknowledge */
  #if 0
        result = 0;
        do
        {  /* Sends just I2C bus address, returns acknowledge bit and stops */
           fwrite (&result, 1, 0, fd);

           if (I2C_OK != ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
           {
              //printf ("  ERROR during wait (flush)\n");
           }

           if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
           {
              //printf ("  ERROR during wait (stop)\n");
           }
           result++;
        } while (param & 1);

        //printf ("  EEPROM polled %d times.\n", result);
  #endif

     /* Release the transaction lock */
     _lwsem_post (&lock);

     return I2C_OK;
  } /* Endbody */


  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read multiple bytes from I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_MultiReadBytes(Byte regAdr, Byte *pData, Byte cnt)
  { /* Body */
     uint_32       param;
     uint_32       result;
     uint_8        mem;

     //printf ("Reading %d bytes from address 0x%08x ...\n", cnt, regAdr);
     if (0 == cnt)
     {
        //printf ("  Nothing to do.\n");
        return I2C_OK;
     }

     /* Protect I2C transaction in multitask environment */
     _lwsem_wait (&lock);

     /* I2C bus address also contains memory block index */
     param = chip_address;
     //printf ("  Set I2C bus address to 0x%02x ... ", param);
     if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Initiate start and send I2C bus address */
     //printf ("  I2C start, send address and get ack ... ");
     (void)fwrite (&mem, 1, 0, fd);

     /* Check ack (device exists) */
     if (I2C_OK == ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
     {
        //printf ("OK ... ack == %d\n", param);
        if (param)
        {
           /* Stop I2C transfer */
           //printf ("  Stop transfer ... ");
           if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
           {
              //printf ("ERROR\n");
              /* Release the transaction lock */
              _lwsem_post (&lock);
              return I2C_FALSE;
           }

           /* Release the transaction lock */
           _lwsem_post (&lock);

           return I2C_FALSE;
        }
     } else {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Write address within memory block */
     mem = regAdr;
     //printf ("  Write address 0x%02x to read from ... ", mem);
     result = fwrite (&mem, 1, 1, fd);
     if (1 != result)
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Wait for completion */
     //printf ("  Flush ... ");
     result = fflush (fd);
     if (MQX_OK != result)
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Restart I2C transfer for reading */
     //printf ("  Initiate repeated start ... ");
     if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_REPEATED_START, NULL))
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Set read request */
     param = cnt;
     //printf ("  Set number of bytes requested to %d ... ", param);
     if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_RX_REQUEST, &param))
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Read all data */
     //printf ("  Read %d bytes ... ", cnt);
     result = fread (pData, 1, cnt, fd);
     if (result != cnt)
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Wait for completion */
     //printf ("  Flush ... ");
     result = fflush (fd);
     if (MQX_OK != result)
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Stop I2C transfer - initiate EEPROM write cycle */
     //printf ("  Stop transfer ... ");
     if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
     {
        //printf ("ERROR\n");
        /* Release the transaction lock */
        _lwsem_post (&lock);
        return I2C_FALSE;
     }

     /* Release the transaction lock */
     _lwsem_post (&lock);
     return I2C_OK;
  } /* Endbody */

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_LookForDevice
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function return if the device with specific address is
  //              present on the BUS
  //
  // PARAMETERS:  adr - address of device taht should be checked
  //
  // RETURNS:     result - result of I2C check operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_LookForDevice(Byte adr)
  {
    return I2C_FALSE;
  }
  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_ReadArrayDelayed
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read multiple bytes from I2C device (8 bit address)
  //              It can wait for device with some delay
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_ReadArrayDelayed(Byte regAdr, Byte* pData, Byte cnt, Word attemps)
  {


    return I2C_OK;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_ReadByte16
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read one byte from I2C device (16 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer where will be stored read byte
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_ReadByte16(I2C_WORD_BYTE regAdr, Byte* pData)
  {

    return I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_WriteByte16
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write one byte to I2C device (16 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              data - data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_WriteByte16(I2C_WORD_BYTE regAdr, Byte data)
  {


    return I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function write multiple bytes to I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of written register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_MultiWriteBytes16(I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt)
  {


    return I2C_OK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    MqxIIC_MultiWriteBytes
  // SCOPE:       HW I2C peripheral low level driver function - Master mode
  // DESCRIPTION: The function read multiple bytes from I2C device (8 bit address)
  //
  // PARAMETERS:  regAdr - address of readed register
  //              pData - pointer on data that should be written
  //              cnt - count of data that should be written
  //
  // RETURNS:     result - result of I2C read operation I2C_OK / I2C_FALSE
  //
  //-----------------------------------------------------------------------------
  Byte MqxIIC_MultiReadBytes16(I2C_WORD_BYTE regAdr, Byte *pData, Byte cnt)
  {

    return I2C_OK;
  }

  /**************************************************************//*!
  *
  * Private Functions bodies
  *
  ******************************************************************/
#endif
#endif

