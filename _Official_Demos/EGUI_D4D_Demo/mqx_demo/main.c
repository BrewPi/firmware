/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved                       
*
* Copyright (c) 1989-2008 ARC International;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: main.c$
* $Version : 3.6.3.0$
* $Date    : Jan-14-2014$
*
* Comments:
*
*   This file contains the source for the eGUI example program.
*
*END************************************************************************/

// Jumper settings on LCD story - TWR-LCD-REV A:

// TWRK60N512 + TWR_LCD SW1 configuration:
// POS:   1 2 3 4 5 6 7 8
// SPI:   0 1 1 1 1 1 1 0
// FLEX:  1 0 1 1 1 1 1 0

#include <mqx.h>
#include <bsp.h> 
#include <lwtimer.h>
#include "main.h"


D4D_EXTERN_SCREEN(screen_entry)

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if !defined(BSPCFG_ENABLE_ADC0) && !defined(BSPCFG_ENABLE_ADC) 
#error This application requires BSPCFG_ENABLE_ADC0 to be not NULL. Please set BSPCFG_ENABLE_ADC1 to non-zero in user_config.h and recompile BSP with this option. 
#endif

//#if PSP_MQX_CPU_IS_KINETIS && (D4D_LLD_LCD_HW == d4dlcdhw_mqx_spi)
//	#if ! BSPCFG_ENABLE_SPI2 
//	#error This application requires BSPCFG_ENABLE_SPI2 to be not NULL. Please set BSPCFG_ENABLE_SPI2 to non-zero in user_config.h and recompile BSP with this option. 
//	#endif
//#endif

#if ! MQX_USE_LWTIMER
#error This application requires MQX_USE_LWTIMER defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


/* Task IDs */
#define LCD_TASK 1  

const ADC_INIT_STRUCT adc_init = {
    ADC_RESOLUTION_12BIT,	     /* resolution */
};

/* Initial structure for TCHRES taken from init_tchres.c */
extern TCHRES_INIT_STRUCT _bsp_tchscr_resisitve_init;

void lcd_task(uint32_t); // task function

TIME_FLAGS time;
LWord time100sm_cnt = 0;
LWTIMER_PERIOD_STRUCT lwt_ps;
LWTIMER_STRUCT lwt_st25;
 
const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,Function,Stack,Priority, Name, Attributes,Param,Time Slice */
    { LCD_TASK,   lcd_task, 3000,   9,"LCD",  MQX_AUTO_START_TASK, 0,     0 },
    {0,           0,           0,     0,   0,      0,                 }
};

/*LW Timer CallBack*---------------------------------------
* 
* Function Name    : Time_event25ms
* Comments     :
*    This function do: 	periodical events in 25ms
*
*END*-----------------------------------------------------*/
static void Time_event25ms(void* pUser)
{
  static Byte actual_time = 0;
  TIME_FLAGS t_flags;

  D4D_TimeTickPut();
		
  /* set the internal time flags */
  actual_time++;
  t_flags.all = (LWord)(actual_time ^ (actual_time - 1));

  time.all |= t_flags.all;
  
  if(t_flags.bits.b50ms)
    D4D_CheckTouchScreen();
  
  if(t_flags.bits.b100ms)
    time100sm_cnt++;		
}

/*TASK*-----------------------------------------------------
* 
* Task Name    : lcd_task
* Comments     :
*    This task handle the D4D display
*
*END*-----------------------------------------------------*/

void lcd_task(uint32_t initial_data)
{
 // MQX_FILE_PTR   adc_file;
  TCHRES_INSTALL_PARAM_STRUCT install_params;
  
  printf("Hello from LCD demo\n\r");     

#if (defined BSP_TCHRES_ADC_XPLUS_DEVICE) && (defined BSP_TCHRES_ADC_YPLUS_DEVICE)
    printf("Opening ADC device for XPLUS electrode ... " BSP_TCHRES_ADC_XPLUS_DEVICE " ");
    if (fopen(BSP_TCHRES_ADC_XPLUS_DEVICE, (const char*)&adc_init) == NULL) {
        printf("failed\n");
        _task_block();
    } else {
        install_params.ADC_XPLUS_DEVICE = BSP_TCHRES_ADC_XPLUS_DEVICE;
        printf("done\n");
    }

    printf("Opening ADC device for YPLUS electrode ... " BSP_TCHRES_ADC_YPLUS_DEVICE " ");
    if (fopen(BSP_TCHRES_ADC_YPLUS_DEVICE, (const char*)&adc_init) == NULL) {
        printf("failed\n");
        _task_block();
    } else {
        install_params.ADC_YPLUS_DEVICE = BSP_TCHRES_ADC_YPLUS_DEVICE;
        printf("done\n");
    }
#elif (defined BSP_TCHRES_ADC_DEVICE)
    printf("Opening ADC device for XPLUS and YPLUS electrodes ... " BSP_TCHRES_ADC_DEVICE " ");
    if (fopen(BSP_TCHRES_ADC_DEVICE, (const char*)&adc_init) == NULL) {
        printf("failed\n");
        _task_block();
    } else {
        install_params.ADC_XPLUS_DEVICE = install_params.ADC_YPLUS_DEVICE = BSP_TCHRES_ADC_DEVICE;
        printf("done\n");
    }
#else
    printf("This demo application requires ADC devices for TCHRES to be defined!\n");
    _task_block();
#endif
  
  #if ADC_IOCTL_CALIBRATE   
//    ioctl( adc_file, ADC_IOCTL_CALIBRATE, NULL );
  #endif

  _io_tchres_install("tchscr:", &_bsp_tchscr_resisitve_init, &install_params );

  // Create the lwtimer component
  _lwtimer_create_periodic_queue(&lwt_ps, 25 / 5, 40);
  _lwtimer_add_timer_to_queue(&lwt_ps, &lwt_st25, 0, Time_event25ms, 0);

  if(!D4D_Init(&screen_entry))
  {
    // D4D initialization failed
    printf("eGUI/D4D initialization failed\n\r");   
    _mqx_exit(0);
  }

  D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);

  D4D_CheckTouchScreen();

  for(;;)
  {
    D4D_Poll();
    _time_delay(10);
  }
}

/* EOF */
