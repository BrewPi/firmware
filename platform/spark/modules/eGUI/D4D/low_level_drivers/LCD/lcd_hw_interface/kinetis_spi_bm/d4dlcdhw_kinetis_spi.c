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
* @file      d4dlcdhw_kinetis_spi.c
*
* @author    b01800
*
* @version   0.0.10.0
*
* @date      Aug-6-2012
*
* @brief     D4D driver - kinetis_spi hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_kinetis_spi_ID 1


// copilation enable preprocessor condition
// the string d4dtch_kinetis_spi_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_kinetis_spi_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/
  #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    #error The eGUI low level driver "d4dlcdhw_kinetis_spi" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
  #endif
  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_Kinetis_Spi(void);
  static unsigned char D4DLCDHW_DeInit_Kinetis_Spi(void);
  static void D4DLCDHW_SendDataWord_Kinetis_Spi(unsigned short value);
  static void D4DLCDHW_SendCmdWord_Kinetis_Spi(unsigned short cmd);
  static unsigned short D4DLCDHW_ReadDataWord_Kinetis_Spi(void);
  static unsigned short D4DLCDHW_ReadCmdWord_Kinetis_Spi(void);
  static unsigned char D4DLCDHW_PinCtl_Kinetis_Spi(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_Kinetis_Spi(D4DLCD_FLUSH_MODE mode);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHW_FUNCTIONS d4dlcdhw_kinetis_spi =
  {
    D4DLCDHW_Init_Kinetis_Spi,
    D4DLCDHW_SendDataWord_Kinetis_Spi,
    D4DLCDHW_SendCmdWord_Kinetis_Spi,
    D4DLCDHW_ReadDataWord_Kinetis_Spi,
    D4DLCDHW_ReadCmdWord_Kinetis_Spi,
    D4DLCDHW_PinCtl_Kinetis_Spi,
    D4DLCD_FlushBuffer_Kinetis_Spi,
    D4DLCDHW_DeInit_Kinetis_Spi
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_Init_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_Init_Kinetis_Spi(void)
  {

    // defined in spi_cfg.h
    #ifdef D4DLCD_DISPLAY_MCU_USER_INIT
      D4DLCD_DISPLAY_MCU_USER_INIT
    #endif

      D4DLCD_SPI_PORT_INIT;

      D4DLCD_ASSERT_DC;
      D4DLCD_INIT_DC;

    // Enable and clear SPI
    D4DLCD_SPI_MCR &= (~ SPI_MCR_MDIS_MASK);
    D4DLCD_SPI_MCR = SPI_MCR_HALT_MASK | SPI_MCR_CLR_TXF_MASK | SPI_MCR_CLR_RXF_MASK;

    // 15+1 = 16-bit transfers, Fclk = Fsys/4
    D4DLCD_SPI_CTAR0 = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(0);

    #ifdef D4DLCD_SPI_DBL_BRATE
      D4DLCD_SPI_CTAR0 |= SPI_CTAR_DBR_MASK;   // Dual Baudrate enable in config file
    #endif

    // Set CS0-7 inactive high
    D4DLCD_SPI_MCR |= (SPI_MCR_PCSIS(1 << (D4DLCD_SPI_PCS_ID))|SPI_MCR_MSTR_MASK);

    // Disable all IRQs
    D4DLCD_SPI_RSER = 0;
    // clear Flag
    D4DLCD_SPI_SR = SPI_SR_TFFF_MASK;
    D4DLCD_SPI_SR = SPI_SR_TCF_MASK;

    // Enable SPI
    D4DLCD_SPI_MCR &= (~ SPI_MCR_HALT_MASK);
      return 1;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_DeInit_Kinetis_Spi(void)
  {
    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD
  //
  // PARAMETERS:  unsigned short value    variable to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_SendDataWord_Kinetis_Spi(unsigned short value)
  {

    // wait write buffer not full flag
    while (!(D4DLCD_SPI_SR & SPI_SR_TFFF_MASK)){};

    // Assert CS0, Use config 0
    D4DLCD_SPI_PUSHR = SPI_PUSHR_PCS(1 << (D4DLCD_SPI_PCS_ID)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)value);

    while (!(D4DLCD_SPI_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
    D4DLCD_SPI_SR = SPI_SR_TCF_MASK;           // clear flag
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendCmdWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit command into LCD
  //
  // PARAMETERS:  unsigned short cmd    command to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_SendCmdWord_Kinetis_Spi(unsigned short cmd)
  {
    D4DLCD_ASSERT_DC;                        // DataCmd := 1
    D4DLCDHW_SendDataWord_Kinetis_Spi(cmd);
    D4DLCD_DEASSERT_DC;                      // DataCmd := 0
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadDataWord_Kinetis_Spi(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadCmdWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadCmdWord_Kinetis_Spi(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------

  static unsigned char D4DLCDHW_PinCtl_Kinetis_Spi(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
    switch(pinId)
    {
      case D4DLCD_RESET_PIN:
        switch(setState)
        {
          #ifdef D4DLCD_RESET

            case D4DHW_PIN_OUT:
              OUTPUT(D4DLCD_RESET);
              break;
            case D4DHW_PIN_IN:
              INPUT(D4DLCD_RESET);
              break;
            case D4DHW_PIN_SET_1:
              D4DLCD_DEASSERT_RESET
              break;
            case D4DHW_PIN_SET_0:
              D4DLCD_ASSERT_RESET;
              break;

          #endif
        }
        break;
      case D4DLCD_BACKLIGHT_PIN:
        switch(setState)
        {

          #ifdef D4DLCD_BACKLIGHT

            case D4DHW_PIN_OUT:
              OUTPUT(D4DLCD_BACKLIGHT);
              break;
            case D4DHW_PIN_IN:
              INPUT(D4DLCD_BACKLIGHT);
              break;
            case D4DHW_PIN_SET_1:
              D4DLCD_DEASSERT_BACKLIGHT
              break;
            case D4DHW_PIN_SET_0:
              D4DLCD_ASSERT_BACKLIGHT;
              break;

         #endif

        }
        break;
    }
    return 1;

  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_Kinetis_Spi(D4DLCD_FLUSH_MODE mode)
  {
     D4D_UNUSED(mode);
  }

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_kinetis_Spi_ID)
