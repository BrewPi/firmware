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
* @file      d4dlcdhw_twrrgba.c
*
* @author     Petr Gargulak
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
#define d4dlcdhw_k70_lcdc_twr_rgbA_ID 1


// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW_K70LCDC) == d4dlcdhw_k70_lcdc_twr_rgbA_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/k70_lcdc/twr_rgb_rev_a/d4dlcdhw_twrrgba.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/
  static void lcdc_init_pins(void);
  static void lcdc_init_spi_lcd(void);
  static void spi_init(void);
  static void SpiRegSet(unsigned char reg);
  static void SpiSendDataWord(unsigned char value);
  static unsigned short SpiReadDataWord(unsigned char reg);
  static unsigned char D4DLCDHW_Init_K70LCDC_TwrRgbB(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  const D4DLCDHWFBK70LCDC_FUNCTIONS d4dlcdhw_k70_lcdc_twr_rgbA =
  {
    D4DLCDHW_Init_K70LCDC_TwrRgbA
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
unsigned char D4DLCDHW_Init_K70LCDC_TwrRgbA(void)
{
  /* Initialize LCD Screen over SPI */
  lcdc_init_spi_lcd();      // to do rewrite  this part to any universal scheme

  /* Setup LCD pin muxing */
  lcdc_init_pins();

  // Setting for TWR-LCD-RGB revA!!!

    // set LCD panning offset
  LCDC_LPOR = 0;

  // set LCD panel configuration
  LCDC_LPCR =
    LCDC_LPCR_TFT_MASK      |       //TFT Screen
    LCDC_LPCR_COLOR_MASK    |       //Color
    LCDC_LPCR_BPIX(D4DLCDHWFB_BPP)     |       //bpp
    //LCDC_LPCR_PIXPOL_MASK | // pixel polarity
    LCDC_LPCR_FLMPOL_MASK   |       //first line marker active low
    LCDC_LPCR_LPPOL_MASK    |       //line pulse active low
    //LCDC_LPCR_END_SEL_MASK  |       //Use big-endian mode (0xFFAA5500 means R=AA,G=55,B=00)
    LCDC_LPCR_SWAP_SEL_MASK |
    LCDC_LPCR_SCLKIDLE_MASK |       //Enalbe LSCLK when vsync is idle
    LCDC_LPCR_SCLKSEL_MASK  |       //Always enable clock
    LCDC_LPCR_ACD(ACD_DIV_0) |
    LCDC_LPCR_PCD(D4DLCDHWFB_PANEL_CLKDIV);             //Divide 120 PLL clock by (3+1)=4 to get 30MHz clock

   // set LCD horizontal configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LHCR =
    LCDC_LHCR_H_WIDTH(9)   |    //(9+1)=10 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(9)  |    //(9+1)=10 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(56);     //(56+3)=59 SCLK periods between end of HSYNC and beginning of OE

  // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LVCR =
    LCDC_LVCR_V_WIDTH(15)  |    //15 SCLK period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(15) |    //15 SCLK period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(15);     //15 SCLK periods between end of VSYNC and beginning of OE

  return 1;
}

static void lcdc_init_pins(void)
{
  //#define ALT2 			(0|PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK)
  //#define ALT5 			(0|PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK)
  //#define ALT7 			(0|PORT_PCR_MUX(7)|PORT_PCR_DSE_MASK)
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
  PORTF_PCR19=ALT5; // Graphic LCD D[15], Schematic PTF19
  PORTF_PCR20=ALT5; // Graphic LCD D[16], Schematic PTF20

  PORTF_PCR21=ALT7; // Graphic LCD D[17], Schematic PTF21
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


  // set LCD_CONTRAST
  PORTB_PCR4=ALT2;  // Graphic LCD CONTRAST,  Schematic PTB4
}

/*
 * SPI configuration to initialize the LCD
 */
static void lcdc_init_spi_lcd(void)
{
  spi_init();

  time_delay_ms(50);

  SpiRegSet(0x20);

  SpiRegSet(0x36);
  SpiSendDataWord(0x00);

  SpiRegSet(0x3A);
  SpiSendDataWord(0x70); // 24bit mode

  SpiRegSet(0xB1);
  SpiSendDataWord(0x06);
  SpiSendDataWord(0x43);
  SpiSendDataWord(0x0A);


  SpiRegSet(0xB2);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0xC8);

  SpiRegSet(0xB3);
  SpiSendDataWord(0x00);

  SpiRegSet(0xB4);
  SpiSendDataWord(0x04);

  SpiRegSet(0xB5);
  SpiSendDataWord(0x42);
  SpiSendDataWord(0x10);
  SpiSendDataWord(0x10);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);

  SpiRegSet(0xB6);
  SpiSendDataWord(0x0B);
  SpiSendDataWord(0x18);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x40);
  SpiSendDataWord(0x10);
  SpiSendDataWord(0x30);

  SpiRegSet(0xC3);
  SpiSendDataWord(0x07);
  SpiSendDataWord(0x04);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x04);

  time_delay_ms(500);

  SpiRegSet(0xC4);
  SpiSendDataWord(0x12);
  SpiSendDataWord(0x24);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x49);

  time_delay_ms(500);

  SpiRegSet(0xC5);
  SpiSendDataWord(0x5B);

  time_delay_ms(500);

  SpiRegSet(0xC6);
  SpiSendDataWord(0x44);
  SpiSendDataWord(0x63);

  time_delay_ms(500);

  SpiRegSet(0xF9);
  SpiSendDataWord(0x40);

  SpiRegSet(0xD0);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD1);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD2);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD3);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD4);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD5);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0x11);

  time_delay_ms(120);

  SpiRegSet(0x29);

  printf("Finished LCD SPI Init\n");
}

static void spi_init(void)
{
  /* clock gate */
  SIM_SCGC3 |=SIM_SCGC3_DSPI2_MASK ;

  //Turn on SPI2 Mux
  PORTD_PCR11 = PORT_PCR_MUX(2); //SPI2_PCS0
  PORTD_PCR12 = PORT_PCR_MUX(2); //SPI2_SCK
  PORTD_PCR13 = PORT_PCR_MUX(2); //SPI2_SOUT
  PORTD_PCR14 = PORT_PCR_MUX(2); //SPI2_SIN

  SPI2_MCR &= (~ SPI_MCR_MDIS_MASK);
  SPI2_MCR  = SPI_MCR_HALT_MASK | SPI_MCR_CLR_TXF_MASK | SPI_MCR_CLR_RXF_MASK | SPI_MCR_DIS_RXF_MASK;
  SPI2_CTAR0  = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(2);
  SPI2_CTAR1  = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(2);


  // Receive FIFO overflow disable
  SPI2_MCR |= SPI_MCR_ROOE_MASK;

  // Set CS0-7 inactive high
  SPI2_MCR |= SPI_MCR_PCSIS(0xFF);
  SPI2_MCR |= SPI_MCR_MSTR_MASK;

  // Disable interrupts
  SPI2_RSER = 0;

  // Clear all flags
  SPI2_SR |=  SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK;

  // Enable SPI
  SPI2_MCR &= (~ SPI_MCR_HALT_MASK);
}

/*
 * To set the LCD register to read/write, send out 0x70 and then the register #
 */
static void SpiRegSet(unsigned char reg)
{
  unsigned short data;

  //Set the data to write out by concatenating 0x70 with the register value.
  data=0x7000 | reg;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0, and send out on MISO
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag
}


/*
 * To write data to a LCD register, send out 0x72 and then value
 * Make sure to set the register index first via SpiRegSet()
 */
static void SpiSendDataWord(unsigned char value)
{
  unsigned short data;

  //Set the data to write out by concatenating 0x72 with the data to write
  data=0x7200 | value;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag
}


/*
 * To read data from a LCD register, this function sets the register index,
 * and then sends out 0x73 and wait for reply
 */
static unsigned short SpiReadDataWord(unsigned char reg)
{
  unsigned short value, data;
  unsigned short data2;

  //Set register pointer
  data=0x7000 | reg;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag


  //Tell it to do a read by sending out 0x73
  data=0x7300;
  data2=0x0000;

  //Halt SPI from sending out anything
  SPI2_MCR  |= SPI_MCR_HALT_MASK;

  // wait write buffer not full flag
  while (!(SPI2_SR & SPI_SR_TFFF_MASK)){};

  // Assert CS0, Use config 0, write two words at once to have CW low long enough
  //  to let data be recieved.
  SPI2_PUSHR = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data);
  SPI2_PUSHR = SPI_PUSHR_PCS(1 << (0)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)data2);


  //Send out both data words back to back using the FIFO
  SPI2_MCR &= (~ SPI_MCR_HALT_MASK);

  while (!(SPI2_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
  //while (!(SPI2_SR & SPI_SR_RCF_MASK)){};// while shift-out complete
  SPI2_SR = SPI_SR_TCF_MASK;           // clear flag

  time_delay_ms(5);

  //Data returned from LCD should be in POPR now
  value = SPI2_POPR ;  //garbage
  value = SPI2_POPR ;  //good data

  return value;
}

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_k70_lcdc_ID)

#endif
