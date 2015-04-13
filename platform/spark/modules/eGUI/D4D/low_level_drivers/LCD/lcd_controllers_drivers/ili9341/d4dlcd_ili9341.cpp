/**************************************************************************
 * Copyright 2015 by Elco Jacobs, BrewPi
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
* @file      d4dlcd_ili9341.c
*
* @author    Elco Jacobs
*
* @version   0.0.1.0
*
* @date      Feb-2015
*
* @brief     D4D driver - ili9341 lcd driver source c file
*
******************************************************************************/

extern "C" {
#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR
}

/******************************************************************************
 * D4D LCD Driver setting  constants
 *
 *//*! @addtogroup doxd4d_lcd_const
* @{
*******************************************************************************/

/*! @brief  Identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
 it is used for enable the code for compilation   */
#define d4dlcd_ili9341_ID 1

/*! @} End of doxd4d_lcd_const                                             */

// compilation enable preprocessor condition
// the string d4dlcd_ili9341_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ili9341_ID)
//  #if (D4D_LLD_LCD == d4dlcd_ili9341_ID)
// include of low level driver header file
// it will be included into whole project only in case that this driver is selected in main D4D configuration file
#include "low_level_drivers/LCD/lcd_controllers_drivers/ili9341/d4dlcd_ili9341.h"
extern "C" {
#include "low_level_drivers/LCD/lcd_hw_interface/common_drivers/d4dlcdhw_common.h"
}

/******************************************************************************
 * Macros
 ******************************************************************************/

/******************************************************************************
 * Internal function prototypes
 ******************************************************************************/

static unsigned char D4DLCD_Init_ili9341(void);
static unsigned char D4DLCD_SetWindow_ili9341(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
static unsigned char D4DLCD_SetOrientation_ili9341(D4DLCD_ORIENTATION new_orientation);
static void D4DLCD_Send_PixelColor_ili9341(D4D_COLOR color);
static D4D_COLOR D4DLCD_Read_PixelColor_ili9341(void);
static void D4DLCD_Flush_ili9341(D4DLCD_FLUSH_MODE mode);
static void D4DLCD_Delay_ms_ili9341(unsigned short period);
static unsigned char D4DLCD_DeInit_ili9341(void);

/**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

/******************************************************************************
 * D4D LCD Driver setting  variables
 *
 *//*! @addtogroup doxd4d_lcd_variable
  * @{
  *******************************************************************************/
/*! @brief the main structure that contains low level driver api functions
 the name of this structure is used for recognizing of configured low level driver of whole D4D
 so this name has to be used in main configuration header file of D4D driver to enable this driver
 */
const D4DLCD_FUNCTIONS d4dlcd_ili9341 = {
    D4DLCD_Init_ili9341, ///< The pointer to driver initialization function
    D4DLCD_SetWindow_ili9341, ///< The pointer to driver set drawing window function
    D4DLCD_SetOrientation_ili9341, ///< The pointer to driver set screen orientation function
    D4DLCD_Send_PixelColor_ili9341, ///< The pointer to driver send pixel to LCD function
    D4DLCD_Read_PixelColor_ili9341, ///< The pointer to driver get pixel from LCD function
    D4DLCD_Flush_ili9341, ///< The pointer to driver flush written pixels to LCD function
    D4DLCD_Delay_ms_ili9341, ///< The pointer to driver delay function
    D4DLCD_DeInit_ili9341, ///< The pointer to driver deinitialization function
};
/*! @} End of doxd4d_lcd_variable                                           */
/**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

static D4DLCD_ORIENTATION d4dlcd_orientation = Landscape;

/**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/
/******************************************************************************
 * Begin of D4D_LCD public functions
 *//*! @addtogroup doxd4d_lcd_func
  * @{
  *******************************************************************************/

/**************************************************************************/ /*!
  * @brief   The function is used for initialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should initialize all necessary things to run the ili9341 lcd driver.
  *******************************************************************************/

static unsigned char D4DLCD_Init_ili9341(void) {
    D4D_LLD_LCD_HW.D4DLCDHW_Init(); // init low level hardware driver (e.g. SPI)
    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xEF);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x03);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x80);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x02);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xCF);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0XC1);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0X30);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xED);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x64);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x03);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0X12);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0X81);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xE8);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x85);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x78);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xCB);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x39);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x2C);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x34);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x02);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xF7);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x20);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xEA);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_PWCTR1); //Power control 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x23); //VRH[5:0] 

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_PWCTR2); //Power control 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x10); //SAP[2:0];BT[3:0] 

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_VMCTR1); //VCM control 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x3e);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x28);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_VMCTR2); //VCM control2 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x86); //--

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_MADCTL); // Memory Access Control 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_PIXFMT);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x55);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_FRMCTR1);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x18);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_DFUNCTR); // Display Function Control 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x08);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x82);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x27);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xF2); // 3Gamma Function Disable 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_GAMMASET); //Gamma curve selected 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x01);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_GMCTRP1); //Set Gamma 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0F);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x31);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x2B);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0C);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0E);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x08);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x4E);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0xF1);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x37);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x07);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x10);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x03);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0E);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x09);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_GMCTRN1); //Set Gamma 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0E);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x14);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x03);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x11);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x07);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x31);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0xC1);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x48);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x08);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0F);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0C);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x31);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x36);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(0x0F);

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_SLPOUT); //Exit Sleep 
    D4DLCD_Delay_ms_ili9341(120);
    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_DISPON); //Display on 


    return 1;

}

/**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run ili9341 lcd driver.
  *******************************************************************************/

static unsigned char D4DLCD_DeInit_ili9341(void) {
    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function sets the logic drawing window in memory of LCD driver
  * @param   x1 - left cordination of logic window
  * @param   y1 - top cordination of logic window
  * @param   x2 - right cordination of logic window
  * @param   y2 - bottom cordination of logic window
  * @return  result: 1 - Success; 0 - Failed
  * @note    After this function one can write/read pixels to/from LCD panel
  *******************************************************************************/

static unsigned char D4DLCD_SetWindow_ili9341(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1) {
    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_CASET); // Column addr set
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(x0 >> 8);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(x0 & 0xFF); // XSTART 
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(x1 >> 8);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(x1 & 0xFF); // XEND

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_PASET); // Row addr set
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(y0 >> 8);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(y0); // YSTART
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(y1 >> 8);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(y1); // YEND

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_RAMWR); // write to RAM
    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function set the new orientation of LCD
  * @param   new_orientation - new orientation of the LCD
  * @return  result: 1 - Success; 0 - Failed
  * @note    This function changes the orientation of LCD
  *******************************************************************************/

static unsigned char D4DLCD_SetOrientation_ili9341(D4DLCD_ORIENTATION new_orientation) {

    unsigned short width, height;
    d4dlcd_orientation = new_orientation;

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(ILI9341_MADCTL);

    switch (new_orientation) {
        default: // Invalid! Fall through to landscape mode
        case Landscape:
            D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(
                    ILI9341_MADCTL_MV | ILI9341_MADCTL_MY | ILI9341_MADCTL_MX
                    | ILI9341_MADCTL_BGR);
            width = D4D_SCREEN_SIZE_LONGER_SIDE;
            height = D4D_SCREEN_SIZE_SHORTER_SIDE;
            break;
        case Portrait:
            D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
            width = D4D_SCREEN_SIZE_SHORTER_SIDE;
            height = D4D_SCREEN_SIZE_LONGER_SIDE;
            break;
        case Landscape180:
            D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
            width = D4D_SCREEN_SIZE_LONGER_SIDE;
            height = D4D_SCREEN_SIZE_SHORTER_SIDE;
            break;
        case Portrait180:
            D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
            width = D4D_SCREEN_SIZE_SHORTER_SIDE;
            height = D4D_SCREEN_SIZE_LONGER_SIDE;
            break;
    }
    (void) D4DLCD_SetWindow_ili9341(0, 0, height, width); // x is shorter side in display
    return 1;
}

/**************************************************************************/ /*!
  * @brief   The function send the one pixel into LCD drawing window
  * @param   color - value of pixel color
  * @return  none
  * @note    This function writes the one pixel to the opened drawing window by \ref  D4DLCD_SetWindow_ili9341 function.
  *           If the window is lager then 1 pixel the driver MUST autmatically increment the next pixel address.
  *******************************************************************************/

static void D4DLCD_Send_PixelColor_ili9341(D4D_COLOR value) {
#ifdef D4D_COLOR_TRANSPARENT
    if (value == D4D_COLOR_TRANSPARENT)
        D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
    else
#endif
    {
#if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
        Word color = D4D_COLOR_RGB565(D4D_COLOR_GET_R(value), D4D_COLOR_GET_G(value), D4D_COLOR_GET_B(value));
        D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(color);
#else
        D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(value >> 8);
        D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(value & 0xff);
#endif
    }
}

/**************************************************************************/ /*!
  * @brief   The function reads the one Pixel from LCD (if this function is supported)
  * @return  color - value of pixel color
  * @note    This function reads the one pixel from the opened drawing window by \ref  D4DLCD_SetWindow_ili9341 function.
  *           If the window is lager then 1 pixel the driver MUST automatically increment the next pixel address.
  *******************************************************************************/

static D4D_COLOR D4DLCD_Read_PixelColor_ili9341(void) {
#if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    Word color = D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
    return D4D_COLOR_RGB(D4D_COLOR565_GET_R(color), D4D_COLOR565_GET_G(color), D4D_COLOR565_GET_B(color));
#else
    return D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
#endif
}

/**************************************************************************/ /*!
  * @brief   For buffered low level interfaces is used to inform
  *            driver the complete object is drawn and pending pixels should be flushed
  * @param   mode - mode of Flush
  * @return  none
  * @note    This function just notify the driver that eGUI finish drawing element/object/screen to allow driver handle any kind of caching memmory.
  *******************************************************************************/

static void D4DLCD_Flush_ili9341(D4DLCD_FLUSH_MODE mode) {
    D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer(mode);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCD_Delay_ms_ili9341
// SCOPE:       Low Level Driver API function
// DESCRIPTION: For do some small delays in ms
//
// PARAMETERS:  period - count of ms
//
// RETURNS:     none
//-----------------------------------------------------------------------------
/**************************************************************************/ /*!
  * @brief   For do some small delays in ms
  * @param   period - 1ms periods time
  * @return  none
  * @note    This function is just used to do some delays of eGUI (just for initialization purposes, not for run)
  *******************************************************************************/

static void D4DLCD_Delay_ms_ili9341(unsigned short period) {
    D4DLCD_Delay_ms_Common(period);
}

/*! @} End of doxd4d_lcd_func                                           */

#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ili9341_ID)
