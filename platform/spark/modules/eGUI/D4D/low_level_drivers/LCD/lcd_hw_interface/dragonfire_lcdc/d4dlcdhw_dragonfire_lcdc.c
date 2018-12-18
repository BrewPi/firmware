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
* @file      d4dlcdhw_dragonfire_lcdc.c
*
* @author     Petr Gargulak
*
* @version   0.0.9.0
*
* @date      Aug-6-2012
*
* @brief     D4D driver - dragonfire_lcdc hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_dragonfire_lcdc_ID 1


// copilation enable preprocessor condition
// the string d4dtch_dragonfire_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_dragonfire_lcdc_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    #error The eGUI low level driver "d4dlcdhw_dragonfire_lcdc" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
  #endif

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_DragonFireLCDC(void);
  static unsigned char D4DLCDHW_DeInit_DragonFireLCDC(void);
  static void D4DLCDHW_WriteData_DragonFireLCDC(unsigned long addr, D4D_COLOR value);
  static D4D_COLOR D4DLCDHW_ReadData_DragonFireLCDC(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_DragonFireLCDC(void);
  static unsigned char D4DLCDHW_PinCtl_DragonFireLCDC(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_DragonFireLCDC(D4DLCD_FLUSH_MODE mode);

	static void Configure_VEEEN_ON(void);
	static void Configure_LCD_Ports(void);
	static void Configure_Bursting(void);
	static void LCDC_Init(LCDC_PANEL_TYPE p_type, Panel_Size panel, BPP_TYPE BPP, unsigned char ACD_DIV, unsigned char PixClkDiv);
	static void LCDC_Enable(void);
	static void LCDC_Disable(void);

	extern void mcf5227_cache_invalidate(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHWFB_FUNCTIONS d4dlcdhw_dragonfire_lcdc =
  {
    D4DLCDHW_Init_DragonFireLCDC,
    D4DLCDHW_WriteData_DragonFireLCDC,
    D4DLCDHW_ReadData_DragonFireLCDC,
    D4DLCDHW_GetFbDescriptor_DragonFireLCDC,
    D4DLCDHW_PinCtl_DragonFireLCDC,
    D4DLCD_FlushBuffer_DragonFireLCDC,
    D4DLCDHW_DeInit_DragonFireLCDC
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

	//**************************************************
	/* Configure horizontal LCD timing
	   H = H_WAIT_1 + H_WIDTH + H_WAIT_2 + XMAX
	   H_WAIT1 = delay in Pixel CLK periods between OE & HSYNC
	   H_WIDTH = width of Horizontal Sync Pulse in Pixel CLK periods
	   H_WAIT2 = delay between HSYNC & first data of next line */

	/* Configure vertical LCD timing
	   V_WAIT1 = delay between OE and VSYNC (in TFT)
	   V_WIDTH = Vertical Sync pulse width in HSYNC periods
	   V_WAIT2 = delay between VSYNC and OE of the first line of next frame*/

	   unsigned char Panel_Timing[][6] =
	   /*[H_WAIT_1, H_WIDTH, H_WAIT_2, V_WAIT_1, V_WIDTH, V_WAIT_2]*/
	  // {{/*12.0SVGA*/ 22, 14,  75,  24,  4,  20 },
	    {{/*12.0SVGA*/ 22, 14,  69,  24,  4,  19 },
	    {/* 3.5QVGA*/  16,  4,  72,   16,  2,   8 },
	    {/* 5.7QVGA*/ 16,  4,  50,  40,  4,   8 },
	    {/* 6.4VGA*/ 36,  4,  96,  40,  4,   35 },
	    {/*10.4VGA */ 48,  14,  102,  32,  1,   35 },
	    {/*M52277EVB_PANEL */ 15, 1, 6, 9, 1, 7 }};

	static const D4DLCD_FRAMEBUFF_DESC d4dlcdhw_dragonfire_lcdc_desc =
	{
		D4DLCDHWFB_START_ADDRESS,
		D4DLCDHWFB_X_MAX,
		D4DLCDHWFB_Y_MAX,
		D4DLCDHWFB_Y_BPP_BYTE
	};


  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_Init_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_Init_DragonFireLCDC(void)
  {
  	Configure_VEEEN_ON();
  	Configure_LCD_Ports();

	  #if D4DLCDHWFB_MIRROWED == 1
	  	MCF_LCDC_LCD_SSAR = D4DLCDHWFB_START_ADDRESS + D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX * 2;
	  #else
			MCF_LCDC_LCD_SSAR = D4DLCDHWFB_START_ADDRESS;
	  #endif
	  MCF_LCDC_LCD_SR =   (D4DLCDHWFB_X_MAX / 16) << 20 | (D4DLCDHWFB_Y_MAX);
	  MCF_LCDC_LCD_VPW = (D4DLCDHWFB_X_MAX / 2);


  	MCF_LCDC_LCD_DCR = ( 0	| MCF_LCDC_LCD_DCR_HM(4)	| MCF_LCDC_LCD_DCR_TM(8) );

  	Configure_Bursting();

  	LCDC_Init(TFT_PANEL, D4DLCDHWFB_PANEL_TYPE, BPP16, ACD_DIV_0, D4DLCDHWFB_PANEL_CLKDIV);

  	LCDC_Enable();

  	//raise LCDC priority
    //(*(vuint32*)(0xFC004100)) = 0x76000021;

  	return 1;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_DeInit_DragonFireLCDC(void)
  {
  	LCDC_Disable();
  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD
  //
  // PARAMETERS:  unsigned long addr  		address to write data
  //							unsigned short value    variable to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_WriteData_DragonFireLCDC(unsigned long addr, D4D_COLOR value)
  {
  	*((D4D_COLOR*)addr) = value;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
  //
  // PARAMETERS:  unsigned long addr  		address to read data
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static D4D_COLOR D4DLCDHW_ReadData_DragonFireLCDC(unsigned long addr)
  {
  	return *((D4D_COLOR*)addr);
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_GetFbDescriptor_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function return the pointer on filled frame buffer descriptor
  //
  // PARAMETERS:  none
  //
  // RETURNS:     D4DLCD_FRAMEBUFF_DESC* - pointer on frame buffer descriptor
  //
  //-----------------------------------------------------------------------------
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_DragonFireLCDC(void)
  {
  	return (D4DLCD_FRAMEBUFF_DESC*) &d4dlcdhw_dragonfire_lcdc_desc;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_DragonFireLCDC(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
		(void)pinId;
		(void)setState;
		return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_DragonFireLCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_DragonFireLCDC(D4DLCD_FLUSH_MODE mode)
  {
		D4D_UNUSED(mode);
  }

	/**************************************************
	* Function name	: Configure_VEEEN_ON(void)
	* returns		: return none
	* arg1			: none
	* arg2			: none
	* Description	: enable LCDC backlight w/ VEEE using PBE0
	* Notes			: restrictions, odd modes
	**************************************************/
	static void Configure_VEEEN_ON(void)
	{
		/* I2C1 is used as LCD enable for the kitted panel */

		MCF_PAD_PAR_I2C = 0;

		MCF_GPIO_PDDR_I2C |= MCF_GPIO_PDDR_I2C_PDDR_I2C1;

		MCF_GPIO_PODR_I2C |= MCF_GPIO_PODR_I2C_PODR_I2C1;

		/* BE0 was still used as LCD enable for external LCDs.
		 * This is a 1.8V signal so it doesn't drive high enough. For now
		 * I'm jumpering BE0/SD_DQM0 to 3.3V on J1. signa. So I'm
		 * putting BE0 as an input to avoid any contention on the line.
		 */
		/* Enable BE3 and BE2 as byte enables. BE0 and BE1 are GPIO. */
		MCF_PAD_PAR_BE = ( 0
								| MCF_PAD_PAR_BE_PAR_BE2_BE2
								| MCF_PAD_PAR_BE_PAR_BE3_BE3 );

		/* Configure BE0 as input */
		MCF_GPIO_PDDR_BE &= ~MCF_GPIO_PDDR_BE_PDDR_BE0;


		/* Code below is all for the prototype boards only */
		#if 0
		/* The current rev of the boards is setup to use BE0 as an LCD enable.
		 * This is a 1.8V signal so it doesn't drive high enough. For now
		 * I'm jumpering TOUT0 as a GPIO to the BE0 signal so that I can use
		 * it as an LCD enable instead. So I'm putting BE0 as an input to
		 * avoid any contention on the line.
		 */

		/* Enable BE3 and BE2 as byte enables. BE0 and BE1 are GPIO. */
		MCF_GPIO_PAR_BE = ( 0
								| MCF_GPIO_PAR_BE_PAR_BE3
								| MCF_GPIO_PAR_BE_PAR_BE2 );

		/* Configure BE0 as input */
		MCF_GPIO_PDDR_BE &= ~MCF_GPIO_PDDR_BE_PDDR_BE0;


		/* Enable TOUT0 and TOUT1 as GPIO outputs */
		MCF_GPIO_PDDR_TIMER = ( 0
								| MCF_GPIO_PDDR_TIMER_PDDR_TIMER1
								| MCF_GPIO_PDDR_TIMER_PDDR_TIMER0
								);

		/* Set both signals high */
		MCF_GPIO_PODR_TIMER = ( 0
								| MCF_GPIO_PODR_TIMER_PODR_TIMER1
								| MCF_GPIO_PODR_TIMER_PODR_TIMER0
								);

		#endif

	} //End Configure_VEEEN_ON


/**************************************************
* Function name	: void Configure_LCD_Ports(void)
* returns		: return none
* arg1			: none
* arg2			: none
* Description	: enable LCDC pins on GPIO
* Notes			: restrictions, odd modes
**************************************************/
static void Configure_LCD_Ports(void)
{
	MCF_PAD_PAR_LCDCTL = ( 0
							| MCF_PAD_PAR_LCDCTL_PAR_ACD_OE(2)
							| MCF_PAD_PAR_LCDCTL_PAR_FLM_VSYNC
							| MCF_PAD_PAR_LCDCTL_PAR_LP_HSYNC
							| MCF_PAD_PAR_LCDCTL_PAR_LSCLK	);

	MCF_PAD_PAR_LCDH = ( 0
							| MCF_PAD_PAR_LCDH_PAR_LD17(3)
							| MCF_PAD_PAR_LCDH_PAR_LD16(3)
							| MCF_PAD_PAR_LCDH_PAR_LD15(3)
							| MCF_PAD_PAR_LCDH_PAR_LD14(3)
							| MCF_PAD_PAR_LCDH_PAR_LD13(3)
							| MCF_PAD_PAR_LCDH_PAR_LD12(3) );

	MCF_PAD_PAR_LCDL = ( 0
							| MCF_PAD_PAR_LCDL_PAR_LD11(3)
							| MCF_PAD_PAR_LCDL_PAR_LD10(3)
							| MCF_PAD_PAR_LCDL_PAR_LD9(3)
							| MCF_PAD_PAR_LCDL_PAR_LD8(3)
							| MCF_PAD_PAR_LCDL_PAR_LD7(3)
							| MCF_PAD_PAR_LCDL_PAR_LD6(3)
							| MCF_PAD_PAR_LCDL_PAR_LD5(3)
							| MCF_PAD_PAR_LCDL_PAR_LD4(3)
							| MCF_PAD_PAR_LCDL_PAR_LD3(3)
							| MCF_PAD_PAR_LCDL_PAR_LD2(3)
							| MCF_PAD_PAR_LCDL_PAR_LD1(3)
							| MCF_PAD_PAR_LCDL_PAR_LD0(3)	);

	MCF_PAD_PAR_TIMER |= (MCF_PAD_PAR_TIMER_PAR_T0IN(1)
							| MCF_PAD_PAR_TIMER_PAR_T1IN(1)
							);

	MCF_PAD_PAR_UART |= MCF_PAD_PAR_UART_PAR_U1CTS(1)
							| MCF_PAD_PAR_UART_PAR_U1RTS(1);

}//End Configure_LCD_Ports

/**************************************************
* Function name	: void Configure_Bursting(void)
* returns		: return none
* arg1			: none
* arg2			: none
* Description	: enable LCDC to burst data
* Notes			: restrictions, odd modes
**************************************************/
static void Configure_Bursting(void)
{
 MCF_SCM_BCR = MCF_SCM_BCR_SBE_BURST_EN;
}//End Configure_Bursting

/***************************************************
* Function name	: LCDC_Init(LCDC_PANEL_TYP p_type, BPP_TYPE BPP, uint8 CLK_DIV)
* returns		: return none
* arg1			: pointer to image
* arg2			: none
* Description	: detailed description
* Notes			: restrictions, odd modes
****************************************************/

static void LCDC_Init(LCDC_PANEL_TYPE p_type, Panel_Size panel, BPP_TYPE BPP, unsigned char ACD_DIV, unsigned char PixClkDiv)

{

/* Configure the panel settings that are used for all panels */
    MCF_LCDC_LCD_PCR = (unsigned long)(0
    				| MCF_LCDC_LCD_PCR_PANEL_TYPE(p_type)  // TFT enable/Color enable
    			//  |MCF_LCDC_LPCR_PBSIZ(3) // Panel Bus Width, 0=1 bit,1=2,2=4,3=8 (does not matter for TFT)
  					|MCF_LCDC_LCD_PCR_BPIX(BPP) // BPP, 0=1bpp,1=2,2=4,3=8,4=12,5=16
  				//	|MCF_LCDC_LPCR_PIXPOL // Pixel Polarity, 0 = Active High, 1 = Active Low
  					|MCF_LCDC_LCD_PCR_ACDSEL // ACD Clock Source select
  					|MCF_LCDC_LCD_PCR_ACD(ACD_DIV) // ACD Stuff
  					|MCF_LCDC_LCD_PCR_SCLKSEL // LSCLK Select, 0 = Disable OE & LSCLK when no data output in TFT mode for power savings, 1 = Always on
  				//	|MCF_LCDC_LPCR_SHARP // Sharp Panel Signals Enable
  					|MCF_LCDC_LCD_PCR_PCD(PixClkDiv) // Pixel CLK Divider, actual divider is PCD value + 1
	    );

	/* Configure the LCD control signals for the panel */

	switch(panel)
	{ case SHARP_12SVGA:
	    MCF_LCDC_LCD_PCR |= ( 0
  				    | MCF_LCDC_LCD_PCR_CLKPOL // LCD Shift Clock, 0 = Active Falling, 1 = Active Rising (reverse for TFT)
						//	|MCF_LCDC_LPCR_SHARP // Sharp Panel Signals Enable
							//|MCF_LCDC_LPCR_PIXPOL // Pixel Polarity, 0 = Active High, 1 = Active Low
  						| MCF_LCDC_LCD_PCR_END_SEL  // Output Enable Polarity, 0= Active High, 1 = Active Low
  					 #if D4DLCDHWFB_MIRROWED == 1
  						| MCF_LCDC_LCD_PCR_REV_VS
  					#endif
  					);
  		MCF_LCDC_LCD_SCR = 0x00120300;
			MCF_LCDC_LCD_PCCR = ( 0
							| MCF_LCDC_LCD_PCCR_CLS_HI_WIDTH(0xA9)
							| MCF_LCDC_LCD_PCCR_SCR(0x1)
							| MCF_LCDC_LCD_PCCR_CC_EN
							| MCF_LCDC_LCD_PCCR_PW(0xFF) );
						//MCF_GPIO_DSCR_LCD = 0x03;
	   	break;
	  case SHARP_104VGA:
	    MCF_LCDC_LCD_PCR |= (
  				    MCF_LCDC_LCD_PCR_CLKPOL // LCD Shift Clock, 0 = Active Falling, 1 = Active Rising (reverse for TFT)
					#ifdef _DIGITALPHOTOALBUM_H
  					|MCF_LCDC_LCD_PCR_END_SEL ); // Output Enable Polarity, 0= Active High, 1 = Active Low
					#else
					);	/* For some reason the example project doesn't work with the ENDSEL bit set. */
					#endif
	   	break;
	  case SHARP_64VGA:
	    MCF_LCDC_LCD_PCR |= (
  				    MCF_LCDC_LCD_PCR_CLKPOL // LCD Shift Clock, 0 = Active Falling, 1 = Active Rising (reverse for TFT)
  					|MCF_LCDC_LCD_PCR_END_SEL
  					); // Output Enable Polarity, 0= Active High, 1 = Active Low
	   	break;
	  case SHARP_57QVGA:
	    MCF_LCDC_LCD_PCR |= (
  					MCF_LCDC_LCD_PCR_LPPOL // Line Pulse Polarity, 0 = Active High, 1 = Active Low
  					|MCF_LCDC_LCD_PCR_END_SEL
  					);
	   	break;
 	  case SHARP_35QVGA:
	    MCF_LCDC_LCD_PCR |= (
  					MCF_LCDC_LCD_PCR_LPPOL // Line Pulse Polarity, 0 = Active High, 1 = Active Low
						| MCF_LCDC_LCD_PCR_OEPOL
//  					|MCF_LCDC_LPCR_ENDSEL
  					); // Output Enable Polarity, 0= Active High, 1 = Active Low
  		break;
 	  case M52277EVB_PANEL:
		if ((BPP==BPP16)||(BPP==BPP18))
		{
		    MCF_LCDC_LCD_PCR |= ( 0
						| MCF_LCDC_LCD_PCR_PIXPOL
						| MCF_LCDC_LCD_PCR_OEPOL
	  	//				| MCF_LCDC_LPCR_ACD(ACD_DIV)
	  					| MCF_LCDC_LCD_PCR_SHARP // Enable SHARP control signal timing
	  						| MCF_LCDC_LCD_PCR_END_SEL
	  						| MCF_LCDC_LCD_PCR_SWAP_SEL
// BPP16/18 don't need	| MCF_LCDC_LPCR_ENDSEL
// BPP16/18 don't need	| MCF_LCDC_LPCR_SWAP_SEL
	  					);

		}
		else
		{
		    MCF_LCDC_LCD_PCR |= ( 0
						| MCF_LCDC_LCD_PCR_PIXPOL
						| MCF_LCDC_LCD_PCR_OEPOL
	  					| MCF_LCDC_LCD_PCR_ACD(ACD_DIV)
	  					| MCF_LCDC_LCD_PCR_SHARP // Enable SHARP control signal timing
	  					| MCF_LCDC_LCD_PCR_END_SEL
	  					| MCF_LCDC_LCD_PCR_SWAP_SEL
	  					);
		}
		MCF_LCDC_LCD_SCR = 0x00120300;
		MCF_LCDC_LCD_PCCR = ( 0
							| MCF_LCDC_LCD_PCCR_CLS_HI_WIDTH(0xA9)
							| MCF_LCDC_LCD_PCCR_SCR(0x1)
							| MCF_LCDC_LCD_PCCR_CC_EN
							| MCF_LCDC_LCD_PCCR_PW(0xFF) );
	   	break;

 	default:
  	//	printf("ERR!! Unsupported panel\n");
	    break;
	}


   // Configure horizontal LCD timing
   //H = H_WIDTH + H_WAIT_2 + XMAX + H_WAIT_1
   MCF_LCDC_LCD_HCR = (unsigned long)(0
  				  |MCF_LCDC_LCD_HCR_H_WAIT_1(Panel_Timing[panel][0]) // H_WAIT1 + 1 = delay in Pixel CLK periods between OE & HSYNC
  				  |MCF_LCDC_LCD_HCR_H_WIDTH(Panel_Timing[panel][1])  // H_WIDTH + 1 = width of Horizontal Sync Pulse in Pixel CLK periods
   				  |MCF_LCDC_LCD_HCR_H_WAIT_2(Panel_Timing[panel][2]) // H_WAIT2 + 3 = delay between HSYNC & first data of next line
 					);

   // Configure vertical LCD timing
   //V = V_WIDTH + V_WAIT_2 + YMAX + V_WAIT_1
   MCF_LCDC_LCD_VCR = (unsigned long)(0
  				   |MCF_LCDC_LCD_VCR_V_WAIT_1(Panel_Timing[panel][3]) // V_WAIT1 = delay between OE and VSYNC (in TFT)
  				   |MCF_LCDC_LCD_VCR_V_WIDTH(Panel_Timing[panel][4])// V_WIDTH = Vertical Sync pulse width in HSYNC periods
  				   |MCF_LCDC_LCD_VCR_V_WAIT_2(Panel_Timing[panel][5]) // V_WAIT2 = delay between VSYNC and OE of the first line of next frame
  				   );
}

/**************************************************
* Function name	: void LCDC_Enable(void)
* returns		: return none
* arg1			: none
* arg2			: none
* Description	: enable LCDC in miscellaneous control register
* Notes			: restrictions, odd modes
**************************************************/
static void LCDC_Enable(void)
{ (MCF_CCM_MISCCR |= MCF_CCM_MISCCR_LCDCHEN);
}//End LCDC_Enable

/**************************************************
* Function name	: void LCDC_Disable(void)
* returns		: return none
* arg1			: none
* arg2			: none
* Description	: Disables LCDC in miscellaneous control register
* Notes			: restrictions, odd modes
**************************************************/
static void LCDC_Disable(void)
{ (MCF_CCM_MISCCR &= ~MCF_CCM_MISCCR_LCDCHEN);
}//End LCDC_Enable

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_dragonfire_lcdc_ID)
