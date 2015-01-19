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
* $FileName: main.c$
*
* @author    Petr Gargulak
*
* $Version : 3.6.1.0$
*
* $Date    : Aug-14-2012$
*
* Comments:
*
*   This file contains the main c code for the eGUI example project.
*
*END************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "main.h" /* include declarations */

D4D_EXTERN_SCREEN(screen_entry)

/**************************************************************//*!
*
* Global variables
*
******************************************************************/
  
TIME_FLAGS time;
LWord time100sm_cnt = 0;


// MCU Initialization 

void MCU_Init(void) 
{

  /* Common initialization of the write once registers */
  /* SOPT1: COPT=0,STOPE=0,WAITE=1,??=0,??=0,??=0,??=0 */
  SOPT1=0x10;                 
  /* SOPT2: COPCLKS=0,COPW=0,USB_BIGEND=0,CLKOUT_EN=0,CMT_CLK_SEL=0,SPI1FE=1,SPI2FE=1,ACIC=0 */
  SOPT2= 0x06;                 
  /* SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,??=0,BGBE=0 */
  SPMSC1= 0x1C;                
  /* SPMSC2: ??=0,??=0,LVDV=0,LVWV=0,PPDF=0,PPDACK=0,??=0,PPDC=0 */
  SPMSC2= 0x00;                
  /* Initialization of CPU registers */

               
  /*  System clock initialization */
  /* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
  MCGC2= 0x36;                /* Set MCGC2 register */ 
  /* MCGC3: DIV32=1 */
  MCGC3=0x10;             
  /* MCGC1: CLKS=2,RDIV=2,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCGC1= 0x92;                /* Set MCGC1 register */ 
  /* MCGC3: LOLIE=0,PLLS=0,CME=0,DIV32=1,VDIV=8 */
  MCGC3= 0x18;                /* Set MCGC3 register */ 
  /* MCGC4: ??=0,??=0,DMX32=0,??=0,??=0,??=0,DRST_DRS=0 */
  MCGC4= 0x00;                /* Set MCGC4 register */ 
  while(!MCGSC_OSCINIT) {              /* Wait until external reference is stable */
  }
  while(MCGSC_IREFST) {                /* Wait until external reference is selected */
  }
  while((MCGSC & 0x0C) != 0x08) {      /* Wait until external clock is selected as a bus clock reference */
  }
  /* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=1,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
  MCGC2=0x3E;                /* Set MCGC2 register */ 
  /* MCGC1: CLKS=2,RDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCGC1=0x9A;                /* Set MCGC1 register */ 
  /* MCGC3: LOLIE=0,PLLS=0,CME=0,DIV32=0,VDIV=8 */
  MCGC3=0x08;                /* Set MCGC3 register */ 
  /* MCGC3: LOLIE=0,PLLS=1,CME=0,DIV32=0,VDIV=8 */
  MCGC3= 0x48;                /* Set MCGC3 register */ 
  while(!MCGSC_PLLST) {                /* Wait until PLL is selected */
  }
  /* MCGC2: LP=0 */

  MCGC2&=~0x08;             
  while(!MCGSC_LOCK) {                 /* Wait until PLL is locked */
  }
  /* MCGC1: CLKS=0,RDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCGC1=0x1A;                /* Set MCGC1 register */ 
  while((MCGSC & 0x0C) != 0x0C) {      /* Wait until PLL clock is selected as a bus clock reference */
  }

 
    /* SCGC1: CMT=1,TPM2=1,TPM1=1,ADC=1,IIC2=1,IIC1=1,SCI2=1,SCI1=1 */
  SCGC1= 0xFF;                 
  /* SCGC2: USB=1,FLS=1,IRQ=1,KBI=1,ACMP=1,RTC=1,SPI2=1,SPI1=1 */
  SCGC2=0xFF;                 
  /* SCGC3: ??=1,??=1,??=1,??=1,??=1,??=1,??=1,RNGA=1 */
  SCGC3=0xFF; 
 
}

// Timer initialization

void Timer_Init(void)
{
   (void)TPM1SC;
   TPM1SC = TPM1SC_TOIE_MASK | TPM1SC_CLKSA_MASK | TPM1SC_PS_MASK;
   TPM1MOD = 4688; // 25ms time base   
}

 /* 25ms periodic interrupt */
__interrupt VectorNumber_Vtpm1ovf void TPM1_Interrupt(void)
{
    static Byte actual_time = 0;
  TIME_FLAGS flags;
  
  actual_time++;
  flags.all = (LWord)(actual_time ^ (actual_time - 1));
  
  time.all |= flags.all;
  
  if(flags.bits.b50ms)
  {    
  //  Key_CheckKeys();
    D4D_CheckTouchScreen();
  }
    
  
  if(flags.bits.b100ms)
  {
    time100sm_cnt++;
    
  }
  
  (void)TPM1SC;
  TPM1SC_TOF = 0;

}



void main(void) {
  
  MCU_Init();   // MCU Initialization Clock, WatchDog etc
  Timer_Init(); // Periodic Timer interrupt initialization - 25ms
 
  D4D_Init(&screen_entry);    
  D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
  D4D_CalibrateTouchScreen();
  EnableInterrupts; /* enable interrupts */
  
  for(;;) {
  
    D4D_Poll(); // D4D poll loop
    
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
