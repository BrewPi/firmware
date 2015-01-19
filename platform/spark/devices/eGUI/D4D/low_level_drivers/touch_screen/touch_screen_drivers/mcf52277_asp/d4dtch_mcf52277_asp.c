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
* @file      d4dtch_mcf52277_asp.c
*
* @author     Petr Gargulak
*
* @version   0.0.5.0
*
* @date      Jun-29-2012
*
* @brief     D4D driver - mcf52277_asp touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_mcf52277_asp_ID 1


// copilation enable preprocessor condition
// the string d4dtch_mcf52277_asp_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_mcf52277_asp_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\touch_screen\touch_screen_drivers\mcf52277_asp\d4dtch_mcf52277_asp.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DTCH_Init_Mcf52277_asp(void);
  static unsigned char D4DTCH_DeInit_Mcf52277_asp(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Mcf52277_asp(void);
  static unsigned char D4DTCH_GetPositionRaw_Mcf52277_asp(unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_mcf52277_asp =
  {
     D4DTCH_Init_Mcf52277_asp,
     D4DTCH_GetPositionRaw_Mcf52277_asp,
     D4DTCH_GetRawLimits_Mcf52277_asp,
     D4DTCH_DeInit_Mcf52277_asp
  };

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

	const D4D_TOUCHSCREEN_LIMITS d4dtch_mcf52277_asp_limits =
  {
    D4DTCH_FULL_SCALE,
    D4DTCH_X_TOUCH_MIN,
    D4DTCH_Y_TOUCH_MIN,
    D4DTCH_X_TOUCH_OFFMAX,
    D4DTCH_Y_TOUCH_OFFMAX
  };

	static uint32 touched = 0;
	static uint16 coordinationX = 0;
	static uint16 coordinationY = 0;


  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

	static void mcf5xxx_set_handler (int vector, int (*handler) (void));
  int aspc_handler(void);
  void read_aspc_fifo(void);
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_Init_Mcf52277_asp
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_Init_Mcf52277_asp(void)
  {
		mcf5xxx_set_handler(128+61, aspc_handler);

		MCF_INTC1_ICR61 |= 4;
		MCF_INTC1_IMRH &= ~(MCF_INTC_IMRH_INT_MASK61);		//enable interrupt


		MCF_ASP_CR = ( 0  // <--don't need &~'s below
						&~MCF_ASP_CR_MDIS // clear module disable
						&~MCF_ASP_CR_CALA // no cal
		//				| MCF_ASP_CR_CALA	// enable cal
						| MCF_ASP_CR_PENE	// pen detect enable
						| MCF_ASP_CR_TSE	// touch detect enable
						& ~MCF_ASP_CR_AZE // no auto zero
		//				| MCF_ASP_CR_AZE 	// auto zero enable
						&~(MCF_ASP_CR_TSTYPE(0x00))  //4 wire
		//				| MCF_ASP_CR_TSTYPE(0x01)  //5 wire
		//				| MCF_ASP_CR_TSTYPE(0x10)  //7 wire
		//				| MCF_ASP_CR_TSTYPE(0x11)  //8 wire
						&~MCF_ASP_CR_AUTO			//turn off auto
		//				| MCF_ASP_CR_AUTO
						&~(MCF_ASP_CR_MODE(00)) // clear mode (set mode zero)
						| MCF_ASP_CR_MODE(0x01)	// set mode one
		//				| MCF_ASP_CR_MODE(0x10)	// set mode two
		//				| MCF_ASP_CR_MODE(0x11)	// set mode three
						);// | MCF_ASP_CR_ASPE

		MCF_ASP_CLKD =  MCF_ASP_CLKD_CLKD(0x4E);	//1Mhz At 80MHz Core
		//MCF_ASP_CLKD =  MCF_ASP_CLKD_CLKD(0x27);	//2Mhz At 80MHz Core
		//MCF_ASP_CLKD =  MCF_ASP_CLKD_CLKD(0x14);	//4Mhz At 80MHz Core
		//MCF_ASP_CLKD =  MCF_ASP_CLKD_CLKD(0x0A);	//8Mhz At 80MHz Core

		MCF_ASP_ICR = ( 0
		//			|MCF_ASP_ICR_PDRIE  // FIFO data avail
					|MCF_ASP_ICR_PFFIE	// FIFO full
					|MCF_ASP_ICR_PFLIE	// FIFO level
		//			|MCF_ASP_ICR_PFLDE	// FIFO DMA / INT req
					|MCF_ASP_ICR_PDIE 	// pen dwn
					|MCF_ASP_ICR_PUIE 	// pen up
					|MCF_ASP_ICR_POVIE	// FIFO Overflow
					|MCF_ASP_ICR_FIFO_WM(16)
					); // set WM / enable all interrupt sources

		//MCF_ASP_TIM = 0x00010111;		//setup ASPC -> PANNEL timing
		//MCF_ASP_TIM = 0x0800ff88;		//setup ASPC -> PANNEL timing
		MCF_ASP_TIM = 0x0F00ffFF;		//setup ASPC -> PANNEL timing

		//temp = MCF_ASP_SFIFO; //clear the pointers.
		MCF_ASP_FIFOP = 0; //clear the pointers.
		MCF_ASP_ISR = 0x000000B7; // clear all interrupts

		MCF_ASP_CR |= ( MCF_ASP_CR_AUTO);	//enable auto scan| MCF_ASP_CR_AZE

		return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_DeInit_Mcf52277_asp
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_DeInit_Mcf52277_asp(void)
  {
  	MCF_ASP_CR =  0x00080000; // Disable ASP module
  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetRawLimits_Mcf52277_asp
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits
  //              structure.
  // PARAMETERS:  none
  //
  // RETURNS:     pointer on touch screen raw limit structure
  //
  //-----------------------------------------------------------------------------
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Mcf52277_asp(void)
  {
  	return (D4D_TOUCHSCREEN_LIMITS*)&d4dtch_mcf52277_asp_limits;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetPositionRaw_Mcf52277_asp
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y
  //              coordinates if screen touched
  // PARAMETERS:  unsigned short *TouchPositionX    Pointer to X coordinate
  //              unsigned short *TouchPositionY    Pointer to Y ccordinate
  // RETURNS:     0   no screen touch
  //              1   screen touch,
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_GetPositionRaw_Mcf52277_asp (unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY)
  {

    if(touched == 0)
     	return 0;

    *TouchPositionX = coordinationX;
    *TouchPositionY = coordinationY;


    return 1;
  }

  static void mcf5xxx_set_handler (int vector, int (*handler) (void))
	{
    extern uint32 D4DTCHHW_VECTOR_RAM[];

    D4DTCHHW_VECTOR_RAM[vector] = (uint32)handler;
	}

	/********************************************************************/
//
/********************************************************************/
void read_aspc_fifo(void)
{
	uint8 incount=0;
	uint8 avgcount=0;
	uint32 MCF_ASP_SFIFO_temp;

	uint16 coorX[32];
	uint16 coorY[32];


	while (((MCF_ASP_FIFOP & 0x003F0000)>>16) != (MCF_ASP_FIFOP & 0x0000003F))
	{
		MCF_ASP_SFIFO_temp=MCF_ASP_SFIFO;

		switch(MCF_ASP_SFIFO_temp & 0x0000F000)
		{
			case MCF_ASP_SFIFO_CHANID_X_COORD:
				coorX[incount] = (uint16)(MCF_ASP_SFIFO_temp & 0x00000FFF);
				break;

			case MCF_ASP_SFIFO_CHANID_Y_COORD:
				coorY[incount] = (uint16)(MCF_ASP_SFIFO_temp & 0x00000FFF);
				incount++;
				break;

			default:
				break;
		}
	}

	for (avgcount=1;avgcount<=incount-1;avgcount++){
		coorX[0] += coorX[avgcount];
		coorY[0] += coorY[avgcount];
		}

	if(avgcount>=1){
		coorX[0] /= avgcount;
		coorY[0] /= avgcount;

	}
	#if D4DTCH_MIRROVED_X == 1
		coordinationX = (uint16)(4096 - coorX[0]);
	#else
		coordinationX = coorX[0];
	#endif

	#if D4DTCH_MIRROVED_Y == 1
		coordinationY = (uint16)(4096 - coorY[0]);
	#else
		coordinationY = coorY[0];
	#endif

	#if D4DTCH_SWAP_AXES == 1
		coorX[0] = coordinationX;
		coordinationX = coordinationY;
		coordinationY = coorX[0];
	#endif

}

	/********************************************************************/
	//
	// The calls in the handler below are in a specific order as flags
	//  for events will be set even if interrupts are disabled.
	//  (just does not cause an int to ocurr)
	//
	/********************************************************************/
	__declspec(interrupt)	int aspc_handler(void)
	{

		if(MCF_ASP_ISR & MCF_ASP_ISR_PUF){			// pen up detected
			//printf("Pen Up\n");
			MCF_ASP_ISR = MCF_ASP_ISR_PUF | MCF_ASP_ISR_PDF;  // clear pen flags
	//	    MCF_ASP_CR &= (~MCF_ASP_CR_ASPE); 	//stop convertion
			MCF_ASP_ICR |= MCF_ASP_ICR_PDIE;	// turn on PenDwn int.

		MCF_GPIO_PCLRR_TIMER = 0xf7;
		touched = 0;
		#if D4DTCH_ASYNCH_EVENTS == 1
			D4D_PutRawTouchScreen(0, 0, 0);
		#endif
		}
		if(MCF_ASP_ISR & MCF_ASP_ISR_PFFF){			//	FIFO Full (should not be here)
			//printf("FIFO Full\n");
			MCF_ASP_FIFOP = 0;					// clear FIFo pointers.
		}
		if(MCF_ASP_ISR & MCF_ASP_ISR_PFLF){
			//printf("PFLF\n");
		    MCF_ASP_CR &= (~MCF_ASP_CR_ASPE); 	//stop convertion
			read_aspc_fifo();
			MCF_GPIO_PPDSDR_TIMER = 0x08;
			touched = 1;
			#if D4DTCH_ASYNCH_EVENTS == 1
				D4D_PutRawTouchScreen(1, coordinationX, coordinationY);
			#endif
		}
		if(MCF_ASP_ISR & MCF_ASP_ISR_PDRF){			// FIFO has data
			//printf("FIFO has data\n");
			MCF_ASP_ISR = MCF_ASP_ISR_PDRF; 		// clear flag
		}
		if(MCF_ASP_ISR & MCF_ASP_ISR_PDF){			// pen down detected
			//printf("Pen Dwn\n");
			MCF_ASP_ISR = MCF_ASP_ISR_PDF; 		// clear flag
				MCF_ASP_ICR &= ~MCF_ASP_ICR_PDIE;	// turn off PenDwn int.
				MCF_ASP_CR |= MCF_ASP_CR_ASPE;		// start convertion
				MCF_ASP_FIFOP = 0;					// clear FIFo pointers.

		}
		if(MCF_ASP_ISR & MCF_ASP_ISR_POVF){
			//printf("FIFO Overflow\n");
			MCF_ASP_FIFOP = 0;					// clear FIFo pointers.
			MCF_ASP_ISR = MCF_ASP_ISR_POVF;
		}

		return( 1) ;
	}

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_mcf52277_asp_ID)
