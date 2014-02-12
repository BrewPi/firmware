/*************************************************************************
*
* $FileName: usbh_hid.c$
* $Version : 4.0.1.0$
* $Date    : Jan-15-2014$
*
* Comments: Provide USB HOST HID initialization
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h> 

#include <usb.h>
#include <host_main.h>
#include <host_rcv.h>
#include <usb_host_hub_sm.h>
#include <usb_host_hid.h>

#include "hidkeyboard.h"
#include "hidmouse.h"

#include "main.h"

extern void Keyboard_Task(uint32_t param);
extern void Mouse_Task(uint32_t param);


/**************************************************************************************************
*   USB HOST HID description
**************************************************************************************************/
/* Table of driver capabilities this application wants to use */
static  USB_HOST_DRIVER_INFO DriverInfoTable[] =
{
   {
      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_HID,                /* Class code                       */
      USB_SUBCLASS_HID_BOOT,        /* Sub-Class code                   */
      USB_PROTOCOL_HID_KEYBOARD,    /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_hid_keyboard_event   /* Application call back function   */
   },
   {
      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_HID,                /* Class code                       */
      USB_SUBCLASS_HID_BOOT,        /* Sub-Class code                   */
      USB_PROTOCOL_HID_MOUSE,       /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_hid_mouse_event      /* Application call back function   */
   },
   /* USB 2.0 hub */
   {

      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_HUB,                /* Class code                       */
      USB_SUBCLASS_HUB_NONE,        /* Sub-Class code                   */
      USB_PROTOCOL_HUB_LS,          /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_hub_device_event     /* Application call back function   */
   },
   {
      {0x00,0x00},                  /* All-zero entry terminates        */
      {0x00,0x00},                  /* driver info list.                */
      0,
      0,
      0,
      0,
      NULL
   }
};

_usb_host_handle        host_handle;



 /*TASK*-----------------------------------------------------------------
*
* Function Name  : InitUsbHostHid
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/
uint32_t InitUsbHostHid(void)
{
  /* Body */
   USB_STATUS           status = USB_OK;
   
   /* _usb_otg_init needs to be done with interrupts disabled */
   _int_disable();

   
   if (MQX_OK != _usb_host_driver_install(USBCFG_DEFAULT_HOST_CONTROLLER)) {
      printf("\n Driver installation failed");
      _int_enable();
      return FALSE;
   }

   /*
   ** It means that we are going to act like host, so we initialize the
   ** host stack. This call will allow USB system to allocate memory for
   ** data structures, it uses later (e.g pipes etc.).
   */
   status = _usb_host_init (USBCFG_DEFAULT_HOST_CONTROLLER, &host_handle);

   if (status != USB_OK) 
   {
      printf("\nUSB Host Initialization failed. STATUS: %x", status);
      fflush(stdout);
      _int_enable();
      return FALSE;
   }

   /*
   ** since we are going to act as the host driver, register the driver
   ** information for wanted class/subclass/protocols
   */
   status = _usb_host_driver_info_register (
                                    host_handle,
                                    DriverInfoTable
                                    );
   if (status != USB_OK) {
      printf("\nDriver Registration failed. STATUS: %x", status);
      fflush(stdout);
      _int_enable();
      return FALSE;
   }
          
   _int_enable();

   printf("\nMQX USB HID Keyboard + Mouse is Running");
   fflush(stdout);

   _task_create(0, MOUSE_TASK, (uint32_t) host_handle);
   _task_create(0, KEYBOARD_TASK, (uint32_t) host_handle);

   /* The main task has done its job, so exit */
  return TRUE;
}