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
* @file      d4dlcdhw_sqm4_eb.c
*
* @author    b01119
*
* @version   0.0.4.0
*
* @date      Jun-29-2012
*
* @brief     D4D driver - k70_lcdc hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_k70_lcdc_ID 1


// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_k70_lcdc_ID)

#include "low_level_drivers/LCD/lcd_hw_interface/k70_lcdc/d4dlcdhw_k70_lcdc.h"

// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_k70_lcdc_sqm4EB_ID 1


// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW_K70LCDC) == d4dlcdhw_k70_lcdc_sqm4EB_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/k70_lcdc/sqm4_eb/d4dlcdhw_sqm4_eb.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/
  static void lcdc_init_pins(void);
  static unsigned char D4DLCDHW_Init_K70LCDC_Sqm4Eb(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  const D4DLCDHWFBK70LCDC_FUNCTIONS d4dlcdhw_k70_lcdc_sqm4EB =
  {
    D4DLCDHW_Init_K70LCDC_Sqm4Eb
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
  // FUNCTION:    D4DLCDHW_Init_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_Init_K70LCDC_Sqm4Eb(void)
{
  /* Setup LCD pin muxing */
  lcdc_init_pins();

    // set LCD panning offset
  LCDC_LPOR = 0;

  // set LCD panel configuration
  LCDC_LPCR =
    LCDC_LPCR_TFT_MASK      |       //TFT Screen
    LCDC_LPCR_COLOR_MASK    |       //Color
    LCDC_LPCR_BPIX(D4DLCDHWFB_BPP)     |       //bpp
    //LCDC_LPCR_PIXPOL_MASK   | // pixel polarity
    LCDC_LPCR_FLMPOL_MASK   |       //first line marker active low
    LCDC_LPCR_LPPOL_MASK    |       //line pulse active low
    LCDC_LPCR_END_SEL_MASK  |       //Use big-endian mode (0xFFAA5500 means R=AA,G=55,B=00)
    //LCDC_LPCR_SWAP_SEL_MASK |
    LCDC_LPCR_SCLKIDLE_MASK |       //Enalbe LSCLK when vsync is idle
    LCDC_LPCR_SCLKSEL_MASK  |       //Always enable clock
    LCDC_LPCR_CLKPOL_MASK  |       //Active on positive edge of LSCLK. In TFT mode, active on negative edge of LSCLK.
 //   LCDC_LPCR_ACD(ACD_DIV_7) |  // It isn't used in TFT mode
    LCDC_LPCR_PCD(D4DLCDHWFB_PANEL_CLKDIV);             //Divide 120 PLL clock by (12+1)=13 to get 9.23MHz clock

   // set LCD horizontal configuration based on panel data (Figure 3-3 in Seiko datasheet)

#if defined(FRD7040TPT)       // -----------------------------------------------
  LCDC_LHCR =
    LCDC_LHCR_H_WIDTH(5)   |    //(47+1)=48 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(0x20)  |    //(39+1)=40 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(0x10);     //(85+3)=88 SCLK periods between end of HSYNC and beginning of OE

  // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LVCR =
    LCDC_LVCR_V_WIDTH(0x30)  |   //3 lines period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(0x30) |    //13 line period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(0x30);    //32 line periods between end of VSYNC and beginning of OE

#elif defined(FRD5040TPT)     // -----------------------------------------------
  LCDC_LHCR =
    LCDC_LHCR_H_WIDTH(47)   |    //(47+1)=48 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(39)  |    //(39+1)=40 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(85);     //(85+3)=88 SCLK periods between end of HSYNC and beginning of OE

  // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LVCR =
    LCDC_LVCR_V_WIDTH(3)  |   //3 lines period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(13) |    //13 line period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(32);    //32 line periods between end of VSYNC and beginning of OE

#else                         // -----------------------------------------------
  LCDC_LHCR =
    LCDC_LHCR_H_WIDTH(40)   |    //(40+1)=41 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(0)  |    //(0+1)=1 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(0);     //(0+3)=3 SCLK periods between end of HSYNC and beginning of OE

  // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LVCR =
    LCDC_LVCR_V_WIDTH(10)  |    //10 SCLK period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(2) |    //2 SCLK period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(2);     //2 SCLK periods between end of VSYNC and beginning of OE

#endif                        // -----------------------------------------------

  return 1;
}

static void lcdc_init_pins(void)
{
  #define ALT1 		(0|PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK)
  #define ALT2 		(0|PORT_PCR_MUX(2))
  #define ALT5 		(0|PORT_PCR_MUX(5))
  #define ALT7 		(0|PORT_PCR_MUX(7))

  PORTF_PCR4 =ALT7; // Graphic LCD D[0],  Schematic PTF4
  PORTF_PCR5 =ALT7; // Graphic LCD D[1],  Schematic PTF5
  PORTF_PCR6 =ALT7; // Graphic LCD D[2],  Schematic PTF6
  PORTF_PCR7 =ALT7; // Graphic LCD D[3],  Schematic PTF7
  PORTF_PCR8 =ALT7; // Graphic LCD D[4],  Schematic PTF8
  PORTF_PCR9 =ALT7; // Graphic LCD D[5],  Schematic PTF9
  PORTF_PCR10=ALT7; // Graphic LCD D[6],  Schematic PTF10
  PORTF_PCR11=ALT7; // Graphic LCD D[7],  Schematic PTF11
  PORTF_PCR12=ALT7; // Graphic LCD D[8],  Schematic PTF12
  PORTF_PCR13=ALT7; // Graphic LCD D[9],  Schematic PTF13
  PORTF_PCR14=ALT7; // Graphic LCD D[10], Schematic PTF14
  PORTF_PCR15=ALT7; // Graphic LCD D[11], Schematic PTF15

  PORTF_PCR16=ALT5; // Graphic LCD D[12], Schematic PTF16
  PORTF_PCR17=ALT5; // Graphic LCD D[13], Schematic PTF17
  PORTF_PCR18=ALT5; // Graphic LCD D[14], Schematic PTF18
  PORTE_PCR26=ALT5; // Graphic LCD D[15], Schematic PTF19 PORTF_PCR19=ALT5; // Graphic LCD D[15], Schematic PTF19
  PORTE_PCR27=ALT5; // Graphic LCD D[16], Schematic PTF20 PORTF_PCR20=ALT5; // Graphic LCD D[16], Schematic PTF20

  PORTE_PCR28=ALT5; // Graphic LCD D[17], Schematic PTF21 PORTF_PCR21=ALT7; // Graphic LCD D[17], Schematic PTF21
  PORTF_PCR22=ALT7; // Graphic LCD D[18], Schematic PTF22
  PORTF_PCR23=ALT7; // Graphic LCD D[19], Schematic PTF23
  PORTF_PCR24=ALT7; // Graphic LCD D[20], Schematic PTF24
  PORTF_PCR25=ALT7; // Graphic LCD D[21], Schematic PTF25
  PORTF_PCR26=ALT7; // Graphic LCD D[22], Schematic PTF26
  PORTF_PCR27=ALT7; // Graphic LCD D[23], Schematic PTF27

  PORTF_PCR0=ALT7;  // Graphic LCD PCLK,  Schematic PTF0
  PORTF_PCR1=ALT7;  // Graphic LCD DE,    Schematic PTF1
  PORTF_PCR2=ALT7;  // Graphic LCD HSYNC, Schematic PTF2
  PORTF_PCR3=ALT7;  // Graphic LCD VSYNC, Schematic PTF3

  // First of all enable and switch on Backlight
  PORTD_PCR11 = ALT1;    // Backlight,  Schematic PTC3
  GPIOD_PDDR |= 1<<11;
  GPIOD_PDOR |= 1<<11;

  // Then enable display itself by dedicated signal
  PORTE_PCR8 = ALT1;  // Graphic LCD On/Off,  Schematic PTE8
  GPIOE_PDDR |= 1<<8;
  GPIOE_PDOR |= 1<<8;



  // set LCD_CONTRAST
  //PORTB_PCR4=ALT2;  // Graphic LCD CONTRAST,  Schematic PTB4
}

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_k70_lcdc_ID)
#endif