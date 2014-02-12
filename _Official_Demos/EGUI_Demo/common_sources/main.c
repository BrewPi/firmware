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
* $Version : 3.6.4.0$
* $Date    : Jan-14-2014$
*
* Comments:
*
*   This file contains the source for the eGUI example program.
*
*END************************************************************************/

/****************************************************************************
*       Includes header files
****************************************************************************/
#include <mqx.h>
#include <bsp.h> 
#include <lwtimer.h>
#include <usb.h>
#include <host_main.h>
#include <host_rcv.h>
#include <usb_host_hub_sm.h>
#include <usb_host_hid.h>

#include "hidkeyboard.h"
#include "hidmouse.h"

#include "main.h"

/****************************************************************************
*       MQX checks
****************************************************************************/
#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if ! MQX_USE_LWTIMER
#error This application requires MQX_USE_LWTIMER defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#if !MQX_HAS_TIME_SLICE
#error This application requires that MQX_HAS_TIME_SLICE is set to 1
#endif

/****************************************************************************
*       Local functions declarations
***************************************************************************/
static void lcd_task(uint32_t); // task function
static D4D_FILEPTR My_FileOpen(char* filename, char* mode); // THE MODE WILL BE ALWAYS READ ONLY 'r'
static int My_FileClose(D4D_FILEPTR p_file);
static int My_FileRead(D4D_FILEPTR p_file, void* buffer, int size);
static int My_FileWrite(D4D_FILEPTR p_file, const void* buffer, int size);
static int My_FileFseek(D4D_FILEPTR p_file, long offset, int origin);
static long My_FileFtell(D4D_FILEPTR p_file);
extern uint32_t InitSdCard(void);

D4D_EXTERN_SCREEN(screen_main)
D4D_EXTERN_SCREEN(screen_scrsvr)
D4D_EXTERN_OBJECT(grpBxScrsvr_enable)

extern void Keyboard_Task(uint32_t param);
extern void Mouse_Task(uint32_t param);
extern uint32_t InitUsbHostHid(void);



/****************************************************************************
*       Local variables declarations
***************************************************************************/
TIME_FLAGS time;
LWord time100sm_cnt = 0;
LWord screenSaverTimeout = 0;
LWord screenSaverTimeoutPreset = SCREENSAVER_TIMEOUT_100MS;
LWord screenSaverTouchProtection = 0;

// The backup of the calibration values takes 3 longs from RVBAT area
#define MY_CALIBRATION_BACKUP (*((D4D_TOUCHSCREEN_CALIB*)&RFVBAT_REG(0)))

/****************************************************************************
*       MQX tasks definition
***************************************************************************/
const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,Function,Stack,Priority, Name, Attributes,Param,Time Slice */
    { LCD_TASK,         lcd_task,       4000L,   9,     "LCD",          MQX_AUTO_START_TASK},    
    { KEYBOARD_TASK,    Keyboard_Task,  3000L,  10L,    "Keyboard",     MQX_TIME_SLICE_TASK},
    { MOUSE_TASK,       Mouse_Task,     3000L,  10L,    "Mouse",        MQX_TIME_SLICE_TASK},
    { 0,                 0,              0,      0,      0,              0,                 }
};

/**************************************************************************************************
*   D4D file operation API
**************************************************************************************************/

D4D_DECLARE_FILE_FUNCTIONS(My_FileOpen, My_FileClose, My_FileRead, My_FileWrite, My_FileFseek, My_FileFtell)  

/**********************
* D4D open file
**********************/
static D4D_FILEPTR My_FileOpen(char* filename, char* mode) // THE MODE WILL BE ALWAYS READ ONLY 'r'
{    
  return (D4D_FILEPTR)fopen(filename, mode);
}

/**********************
* D4D close file
**********************/
static int My_FileClose(D4D_FILEPTR p_file)
{
  return fclose((MQX_FILE_PTR)p_file);
}

/**********************
* D4D read file
**********************/
static int My_FileRead(D4D_FILEPTR p_file, void* buffer, int size)
{   
  return fread(buffer, 1, size, (MQX_FILE_PTR)p_file);
}

/**********************
* D4D write file
**********************/
static int My_FileWrite(D4D_FILEPTR p_file, const void* buffer, int size)
{
  return fwrite((void*)buffer, 1, size, (MQX_FILE_PTR)p_file);
}

/**********************
* D4D seek file
**********************/
static int My_FileFseek(D4D_FILEPTR p_file, long offset, int origin)
{ 
  return fseek((MQX_FILE_PTR)p_file, offset, origin);
}

/**********************
* D4D tell file
**********************/
static long My_FileFtell(D4D_FILEPTR p_file)
{ 
  return ftell((MQX_FILE_PTR)p_file);
} 


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
  
  D4D_UNUSED(pUser);
  
  
  
  // Notify eGUI about new time event
  D4D_TimeTickPut();
  
  // Check the touches, the protection is here to protect short moment after leaving the screen saver
  if(!screenSaverTouchProtection)
    D4D_CheckTouchScreen();
  
  if(screenSaverTouchProtection)
    screenSaverTouchProtection--;
  
  // set the internal time flags
  actual_time++;
  t_flags.all = (uint32_t)(actual_time ^ (actual_time - 1));

  time.all |= t_flags.all;
  
  if(t_flags.bits.b100ms)
  {
    time100sm_cnt++;    
    // manage start of screen saver
    if(screenSaverTimeout && D4D_CheckBoxGetValue(&grpBxScrsvr_enable))
    {
      screenSaverTimeout--;
      if(screenSaverTimeout == 0)
      {
        // Timeout occur - Start Screen Saver
        D4D_ActivateScreen(&screen_scrsvr, D4D_FALSE);
      }
    }
  }
}

/**************************************************************//*!
* D4D Input event call back function
******************************************************************/
void D4D_InputEventCB(void)
{
  if(screenSaverTimeout == 0)
  {
    // Escape from Screen Saver screen
    if(D4D_GetActiveScreen() == &screen_scrsvr)
    {
      D4D_EscapeScreen();
      D4D_ClearKeysBuffer();
      D4D_ClearTouchScreenEvents();    
      
      screenSaverTouchProtection = 8;
    }
  }
 
  screenSaverTimeout = screenSaverTimeoutPreset;   
}

/*TASK*-----------------------------------------------------
* 
* Task Name    : lcd_task
* Comments     :
*    This task handle the D4D display
*
*END*-----------------------------------------------------*/

static void lcd_task(uint32_t initial_data)

{
  LWTIMER_PERIOD_STRUCT lwt_ps;
  LWTIMER_STRUCT lwt_st25;
  
  
#ifdef BSP_BUTTON2
  LWGPIO_STRUCT button;
#endif
  
  
#if 0 //defined(BSPCFG_ENABLE_IODEBUG) && BSPCFG_ENABLE_IODEBUG 
   MQX_FILE_PTR      fh_ptr;
   if(NULL == (fh_ptr = fopen("iodebug:", NULL))) { 
      printf("Cannot open the debug output\n"); 
   } else { 
      _io_set_handle(IO_STDOUT, fh_ptr); 
      printf("The debug print output is enabled.\n"); 
   }
#endif
   
   // Just for sure if we are running on MQX :-)
  _int_install_unexpected_isr();
  
  printf("Hello from LCD demo\n\r"); 
 
  // Init USB Host - for this demo application 
  if(!InitUsbHostHid())
  {
    printf("USB HOST HID is not ready!.\n\r"); 
  }else
    _time_delay(100);
  
  // Init SD card - for this demo application 
  if(InitSdCard() == 0)
  {
    printf("SD Card is not ready!.\n\r"); 
  }

  // Create the lwtimer component
  _lwtimer_create_periodic_queue(&lwt_ps, 25 / 5, 40);
  _lwtimer_add_timer_to_queue(&lwt_ps, &lwt_st25, 0, Time_event25ms, 0);

  if(!D4D_Init(&screen_main))
  {
    // D4D initialization failed
    printf("eGUI/D4D initialization failed\n\r");   
    _mqx_exit(0);
  }

  D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
  
  //Change used language to default english
  D4D_ChangeStringTable(STRING_LANG_ENGLISH);
  
  if(MY_CALIBRATION_BACKUP.ScreenCalibrated)  
  {
    D4D_SetTouchScreenCalibration(MY_CALIBRATION_BACKUP);
  }else
  {
    D4D_CalibrateTouchScreen();
    MY_CALIBRATION_BACKUP = D4D_GetTouchScreenCalibration();
  }

  screenSaverTimeout = screenSaverTimeoutPreset;

  // The print screen button inicialization
#ifdef BSP_BUTTON2
  /* opening pins for input */
  if (!lwgpio_init(&button, BSP_BUTTON2, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE))
  {
      printf("Initializing button GPIO as input failed.\n");
      _task_block();
  }
  lwgpio_set_functionality(&button, BSP_BUTTON2_MUX_GPIO);
  lwgpio_set_attribute(&button, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
#endif

  while(1)
  {
    D4D_Poll();
    _time_delay(10);
    
    // The print screen handler
#ifdef BSP_BUTTON2    
    if (LWGPIO_VALUE_LOW == lwgpio_get_value(&button))
      D4D_PrintScreenToBMP("A:\\test.bmp");    
#endif
  }
  
}

/* EOF */
