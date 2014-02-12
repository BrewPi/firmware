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
* $FileName: hidmouse.c$
* $Version : 3.8.1.0$
* $Date    : Jan-15-2014$
*
* Comments:
*
*   This file is an example of device drivers for a Mouse device.
*   It has been tested in a Dell and logitech USB 3 button wheel Mouse. The program
*   queues transfers on Interrupt USB pipe and waits till the data comes
*   back. It prints the data and queues another transfer on the same pipe.
*   See the code for details.
*
*END************************************************************************/

#include <mqx.h>
#include <lwevent.h>

#include <bsp.h>

#include <usb.h>
#include <hostapi.h>
#include <usb_host_hid.h>


#include "hidmouse.h"
#include "d4d.h"

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/************************************************************************************
**
** Globals
************************************************************************************/
LWEVENT_STRUCT USB_Mouse_Event;

#define USB_EVENT_CTRL           (0x01)
#define USB_EVENT_DATA           (0x02)
#define USB_EVENT_DATA_CORRUPTED (0x04)

#define MAIN_TASK          (10)

USB_MOUSE_DEVICE_STRUCT  mouse_hid_device = { 0 };  /* structure defined by this driver */

extern void Mouse_Task(uint32_t param);
void process_mouse_buffer(unsigned char *buffer);

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : main (Main_Task if using MQX)
* Returned Value : none
* Comments       :
*     Execution starts here
*
*END*--------------------------------------------------------------------*/

void Mouse_Task( uint32_t param )
{
    USB_STATUS              status = USB_OK;
    TR_INIT_PARAM_STRUCT    tr;
    HID_COMMAND_PTR         hid_com;
    unsigned char               *buffer;
    PIPE_STRUCT_PTR         pipe;
    uint32_t                 e;
    _usb_host_handle        mouse_host_handle = (_usb_host_handle) param;
   
    hid_com = (HID_COMMAND_PTR) _mem_alloc(sizeof(HID_COMMAND));

    /* event for USB callback signaling */
    _lwevent_create(&USB_Mouse_Event, LWEVENT_AUTO_CLEAR);

    printf("\nMQX USB HID Mouse Demo\nWaiting for USB Mouse to be attached...\n");
    fflush(stdout);
   
    /*
    ** Infinite loop, waiting for events requiring action
    */
    for ( ; ; ) {
    
        // Wait for insertion or removal event
        _lwevent_wait_ticks(&USB_Mouse_Event, USB_EVENT_CTRL, FALSE, 0);
        
        switch ( mouse_hid_device.DEV_STATE ) {
            case USB_DEVICE_IDLE:
                break;
            case USB_DEVICE_ATTACHED:
                printf("\nMouse device attached\n");
                fflush(stdout);
                mouse_hid_device.DEV_STATE = USB_DEVICE_SET_INTERFACE_STARTED;
                status = _usb_hostdev_select_interface(mouse_hid_device.DEV_HANDLE, mouse_hid_device.INTF_HANDLE, (void *)&mouse_hid_device.CLASS_INTF);
                if (status != USB_OK) {
                    printf("\nError in _usb_hostdev_select_interface: %x", status);
                    fflush(stdout);
                    _task_block();
                } /* Endif */
                break;
            case USB_DEVICE_SET_INTERFACE_STARTED:
                break;

            case USB_DEVICE_INTERFACED:
                pipe = _usb_hostdev_find_pipe_handle(mouse_hid_device.DEV_HANDLE, mouse_hid_device.INTF_HANDLE, USB_INTERRUPT_PIPE, USB_RECV);
                if (pipe == NULL) {
                    printf("\nError getting interrupt pipe.");
                    fflush(stdout);
                    _task_block();
                }
                _usb_hostdev_get_buffer(mouse_hid_device.DEV_HANDLE, pipe->MAX_PACKET_SIZE, (void **) &buffer);
                if (buffer == NULL) {
                    printf("\nMemory allocation failed. STATUS: %x", status);
                    fflush(stdout);
                    _task_block();
                }

                printf("Mouse interfaced, setting protocol...\n");
                /* now we will set the USB Hid standard boot protocol */
                mouse_hid_device.DEV_STATE = USB_DEVICE_SETTING_PROTOCOL;
            
                hid_com->CLASS_PTR = (CLASS_CALL_STRUCT_PTR)&mouse_hid_device.CLASS_INTF;
                hid_com->CALLBACK_FN = usb_host_hid_mouse_ctrl_callback;
                hid_com->CALLBACK_PARAM = 0;
            
                status = usb_class_hid_set_protocol(hid_com, USB_PROTOCOL_HID_MOUSE);
         
                if (status != USB_STATUS_TRANSFER_QUEUED) {
                      printf("\nError in usb_class_hid_set_protocol: %x", status);
                      fflush(stdout);
                }
                break;
            case USB_DEVICE_INUSE:
                printf("Mouse device ready, try to move the mouse\n");
                while (1) {
                    /******************************************************************
                    Initiate a transfer request on the interrupt pipe
                    ******************************************************************/
                    usb_hostdev_tr_init(&tr, usb_host_hid_mouse_recv_callback, NULL);
                    tr.G.RX_BUFFER = buffer;
                    tr.G.RX_LENGTH = pipe->MAX_PACKET_SIZE;                     
                        
                    status = _usb_host_recv_data(mouse_host_handle, pipe, &tr);
                        
                    if (status != USB_STATUS_TRANSFER_QUEUED) {
                        printf("\nError in _usb_host_recv_data: %x", status);
                        fflush(stdout);
                    }
                    
                    /* Wait untill we get the data from keyboard. */
                    _lwevent_wait_ticks(&USB_Mouse_Event, USB_EVENT_CTRL | USB_EVENT_DATA | USB_EVENT_DATA_CORRUPTED, FALSE, 0);
                        
                    e = _lwevent_get_signalled();
                    if (USB_EVENT_DATA == e) {
                        if(mouse_hid_device.DEV_STATE == USB_DEVICE_INUSE) {
                            process_mouse_buffer((unsigned char *)buffer);
                        }
                    }
                    else if (USB_EVENT_CTRL == e) {
                        /* kick the outer loop again to handle the CTRL event */
                        _lwevent_set(&USB_Mouse_Event, USB_EVENT_CTRL);
                        break;
                    }
                }
                break;
            case USB_DEVICE_DETACHED:
                printf("Going to idle state\n");
                mouse_hid_device.DEV_STATE = USB_DEVICE_IDLE;
                break;
            case USB_DEVICE_OTHER:
                break;
             default:
                printf("Unknown Mouse Device State = %d\n", mouse_hid_device.DEV_STATE);
                fflush(stdout);
                break;
        } /* Endswitch */
    } /* Endfor */
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_mouse_event
* Returned Value : None
* Comments       :
*     Called when HID device has been attached, detached, etc.
*END*--------------------------------------------------------------------*/

void usb_host_hid_mouse_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint32_t                          event_code
   )
{ /* Body */

   INTERFACE_DESCRIPTOR_PTR intf_ptr = (INTERFACE_DESCRIPTOR_PTR)intf_handle;
      
   switch (event_code) {
      case USB_ATTACH_EVENT:
            printf("----- Attach Event -----\n");
            /* Drop through into attach, same processing */
      case USB_CONFIG_EVENT:           
            printf("State = %d", mouse_hid_device.DEV_STATE);
            printf("  Class = %d", intf_ptr->bInterfaceClass);
            printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
            printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
            fflush(stdout);

         if (mouse_hid_device.DEV_STATE == USB_DEVICE_IDLE) {
            mouse_hid_device.DEV_HANDLE = dev_handle;
            mouse_hid_device.INTF_HANDLE = intf_handle;
            mouse_hid_device.DEV_STATE = USB_DEVICE_ATTACHED;
         } else {
            printf("HID device already attached - DEV_STATE = %d\n", mouse_hid_device.DEV_STATE);
            fflush(stdout);
         } /* Endif */
         
         break;
      case USB_INTF_EVENT:
         printf("----- Interfaced Event -----\n");
         mouse_hid_device.DEV_STATE = USB_DEVICE_INTERFACED;
         break ;
      case USB_DETACH_EVENT:
         /* Use only the interface with desired protocol */
         printf("----- Detach Event -----\n");
         printf("State = %d", mouse_hid_device.DEV_STATE);
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);
         
         mouse_hid_device.DEV_HANDLE = NULL;
         mouse_hid_device.INTF_HANDLE = NULL;
         mouse_hid_device.DEV_STATE = USB_DEVICE_DETACHED;
         break;

      default:
         printf("HID Device state = %d??\n", mouse_hid_device.DEV_STATE);
         fflush(stdout);
         mouse_hid_device.DEV_STATE = USB_DEVICE_IDLE;
         break;
   } /* EndSwitch */

    /* notify application that status has changed */
    _lwevent_set(&USB_Mouse_Event, USB_EVENT_CTRL);
   
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_ctrl_callback
* Returned Value : None
* Comments       :
*     Called when a command is completed
*END*--------------------------------------------------------------------*/

void usb_host_hid_mouse_ctrl_callback
   (
      /* [IN] pointer to pipe */
      _usb_pipe_handle  pipe_handle,

      /* [IN] user-defined parameter */
      void             *user_parm,

      /* [IN] buffer address */
      unsigned char         *buffer,

      /* [IN] length of data transferred */
      uint32_t           buflen,

      /* [IN] status, hopefully USB_OK or USB_DONE */
      uint32_t           status
   )
{ /* Body */
   if (status == USBERR_ENDPOINT_STALLED) {
      printf("\nHID Set_Protocol Request BOOT is not supported!\n");
      fflush(stdout);
   }
   else if (status) {
      printf("\nHID Set_Protocol Request BOOT failed!: 0x%x ... END!\n", status);
      fflush(stdout);
   } /* Endif */

    if(mouse_hid_device.DEV_STATE == USB_DEVICE_SETTING_PROTOCOL)
        mouse_hid_device.DEV_STATE = USB_DEVICE_INUSE;
    
    /* notify application that status has changed */
    _lwevent_set(&USB_Mouse_Event, USB_EVENT_CTRL);
   
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_mouse_host_hid_recv_callback
* Returned Value : None
* Comments       :
*     Called when data is received on a pipe
*END*--------------------------------------------------------------------*/

void usb_host_hid_mouse_recv_callback
   (
      /* [IN] pointer to pipe */
      _usb_pipe_handle  pipe_handle,

      /* [IN] user-defined parameter */
      void             *user_parm,

      /* [IN] buffer address */
      unsigned char         *buffer,

      /* [IN] length of data transferred */
      uint32_t           buflen,

      /* [IN] status, hopefully USB_OK or USB_DONE */
      USB_STATUS        status
   )
{ /* Body */

    if (status == USB_OK) {
        /* notify application that data are available */
        _lwevent_set(&USB_Mouse_Event, USB_EVENT_DATA);
    }
    else {
        /* notify application that data are available */
        _lwevent_set(&USB_Mouse_Event, USB_EVENT_DATA_CORRUPTED);
    }  
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : process_mouse_buffer
* Returned Value : None
* Comments       :
*   Process the data from mouse buffer
*END*--------------------------------------------------------------------*/
void process_mouse_buffer(unsigned char *buffer)
{
    static uint8_t lastBtnState = 0;
    uint8_t newAction = lastBtnState ^ buffer[0];
    
    lastBtnState = buffer[0];
    
    
//  
//    if (buffer[0] & 0x01) 
//        printf("Left Click ");
//    else    
//        printf("           ");
//    if (buffer[0] & 0x02) 
//        printf("Right Click ");
//    else 
//        printf("            ");
//    if (buffer[0] & 0x04) 
//        printf("Middle Click ");
//    else 
//        printf("             ");

    if(buffer[1] || buffer[2])
      D4D_NewMouseEvent((int8_t)buffer[1], (int8_t)buffer[2], D4D_MOUSE_EVENT_MOVE);
    
    if(newAction & 0x01)
    {
      if(buffer[0] & 0x01)
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_DOWN_L);
      else
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_UP_L);
    }
    
    if(newAction & 0x02)
    {
      if(buffer[0] & 0x02)
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_DOWN_R);
      else
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_UP_R);
    }
    
    if(newAction & 0x04)
    {
      if(buffer[0] & 0x04)
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_DOWN_M);
      else
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_UP_M);
    }
    
    if(buffer[3])
    {
      if(buffer[3] > 127)
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_WHEEL_DOWN);
      else
        D4D_NewMouseEvent(0,0, D4D_MOUSE_EVENT_WHEEL_UP);
    }
    
//    if(buffer[1]){
//        if(buffer[1] > 127) 
//            printf("Left  ");
//        else 
//            printf("Right ");
//    }
//    else { 
//        printf("      ");
//    }
//
//    if(buffer[2]){
//        if(buffer[2] > 127) 
//            printf("UP   ");
//        else 
//            printf("Down ");
//    }
//    else { 
//        printf("     ");
//    }

//    if(buffer[3]){
//        if(buffer[3] > 127) 
//            printf("Wheel Down");
//        else 
//            printf("Wheel UP  ");
//    }
//    else { 
//        printf("          ");
//    }
//
//    printf("\n");
//    fflush(stdout); 
//    
}

/* EOF */

