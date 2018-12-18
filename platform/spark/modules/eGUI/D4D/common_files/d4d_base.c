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
* @file      d4d_base.c
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.117.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver core and base function c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_lldapi.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

Byte d4d_scratchPad[D4D_SCRATCHPAD_SIZE];

/**************************************************************//*!
*
* Local variables
*
******************************************************************/

// screen activation hisotry array & index
D4D_SCREEN* d4d_screenHistory[D4D_SCREEN_HISTORY];
Byte d4d_screenHistoryIndex;

// if not NULL, keys are captured by this object
D4D_OBJECT* d4d_pKeysCapturer = NULL;

// one and only message object being used to route information to objects
D4D_MESSAGE d4d_msg;

const D4D_SIZE d4d_size_zero = {0, 0}; // zero size structure for automatic function capability
const D4D_POINT d4d_point_zero = {0, 0}; // zero point structure

const D4D_MARGIN d4d_marginDefault =
{
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH
};

// The buffer of input keys
static D4D_KEYS_BUFFER d4d_KeysBuff;

// The D4D system flags
D4D_SYSTEM_FLAGS d4d_systemFlags;

#ifdef D4D_LLD_TCH
  static D4D_TOUCHSCREEN_STATUS d4d_TouchScreen_Status;
  D4D_OBJECT* d4d_LastTouchedObj;
  static D4D_POINT d4d_TouchScreenCoor = {0, 0};
  static D4D_TOUCHSCREEN_EVENT d4d_touchEvent = { 0, {0, 0}};

  static void D4D_HandleTouchScreen(D4D_SCREEN* pScreen);
  static void D4D_ManageTouchScreenEvents(void);
  static D4D_BOOL D4D_GetTouchScreen(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);
#endif

#ifdef __CWCC__
  #pragma mark -
  #pragma mark D4D System Functions
  #pragma mark -
#endif

/******************************************************************************
* Begin of D4D_BASE public functions
*//*! @addtogroup doxd4d_base_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function inits the eGUI itself including all low level drivers.
* @param   pInitScreen - the first initialized screen. Could be NULL in case that no screen should be initialized after startup.
* @return  result of init operation. D4D_TRUE - everything runs OK, D4D_FALSE initialization failed.
* @note    This function initialized complete eGUI, after this function runs,
*           the HW is ready and all internal variables are also prepared to run eGUI.
*          Must be run as a First function of eGUI in project.
*******************************************************************************/
D4D_BOOL D4D_Init(const D4D_SCREEN* pInitScreen)
{
  if(!D4D_LCD_Init()) // Init hardware of LCD
    return D4D_FALSE;

  d4d_screenHistoryIndex = 0;

  #ifdef D4D_LLD_TCH
    d4d_TouchScreen_Status = 0;
    d4d_LastTouchedObj = NULL;
  #endif

  #ifdef D4D_LLD_MOUSE
    D4D_MouseInit();
  #endif

  // In case that the external font is enabled, just initialize it
  #if (D4D_EXTSRC_FILE_ENABLE != D4D_FALSE) && (D4D_FNT_EXTSRC_SUPPORT != D4D_FALSE)
    // one time initialization
    if(D4D_ExtFntInit() == D4D_FALSE)
      return D4D_FALSE;
  #endif

  d4d_systemFlags = 0;

  D4D_ClearKeysBuffer();
  if(pInitScreen)
    D4D_ActivateScreen(pInitScreen, D4D_FALSE);

  return D4D_TRUE;
}

/**************************************************************************/ /*!
* @brief   Main eGUI function. Must be periodically called in main loop/task loop.
* @return  none
* @note    This is the main function that is handling all eGUI inputs and also drawings.
*******************************************************************************/
void D4D_Poll(void)
{
    // get active screen
    D4D_SCREEN* pScreen;

    // handle keys (may change active screen)
    D4D_HandleKeys();

  #ifdef D4D_LLD_TCH
    // handle Touch screen events - if any
    if(d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_EVENTS)
    {
      D4D_ManageTouchScreenEvents();
      d4d_TouchScreen_Status &= ~D4D_TCHSCR_STATUS_EVENTS;
    }

    pScreen = D4D_GetActiveScreen();
    if(pScreen == NULL)
      return;
    // Handle Touch screen to screen and objects
    D4D_HandleTouchScreen(pScreen);
  #endif

  #ifdef D4D_LLD_MOUSE
    pScreen = D4D_GetActiveScreen();
    if(pScreen == NULL)
      return;

    // handle mouse events
    D4D_HandleMouse();
  #endif

    pScreen = D4D_GetActiveScreen();
    if(pScreen == NULL)
      return;
    // call screen's main function
    if(pScreen->OnMain)
        pScreen->OnMain();

    // Handle the timeticks of D4D
    pScreen = D4D_GetActiveScreen();
    D4D_HandleTimeTicks(pScreen);

    // redraw all objects on active screen
    pScreen = D4D_GetActiveScreen();
    D4D_RedrawScreen(pScreen);
}

/**************************************************************************/ /*!
* @brief   Set screen orientation function.
* @param   orient - the requested new orientation
* @return  none
* @note    Function change the orientation of screen, invoke complete redraw
*******************************************************************************/
void D4D_SetOrientation(D4D_ORIENTATION orient)
{
  // get active screen
  D4D_SCREEN* pScreen = D4D_GetActiveScreen();

  D4D_LCD_SetOrientation(orient);

  #ifdef D4D_LLD_MOUSE
    D4D_MouseCenterCursor();
  #endif

  if(pScreen)
    D4D_InvalidateScreen(pScreen, D4D_TRUE);
}

/**************************************************************************/ /*!
* @brief   Notify eGui about new tick tick occur
* @return  none
* @note    Function is used to periodical notify eGUI that time tick occur.
*          It usually call from any periodic source. It also could be called from interrupt.
*******************************************************************************/
void D4D_TimeTickPut(void)
{
  d4d_systemFlags |= D4D_SYSTEM_F_TIMETICK;
}

/**************************************************************************/ /*!
* @brief   Clear all rest information about pushed Keys in buffer
* @return  none
* @note    Function clears the input keys buffer.
*******************************************************************************/
void D4D_ClearKeysBuffer(void)
{
  d4d_KeysBuff.readPos = 0;
  d4d_KeysBuff.writePos = 0;
}

/**************************************************************************/ /*!
* @brief   Function enable or disable handling system keys (ESC, UP/DOWN, LEFT/RIGHT) automatically
* @param   bEnable - <D4D_TRUE - eGUI will handle system keys automatically, D4D_FALSE - the system keys will be ignored and it will be normally handled to screen and focused object
* @return  none.
* @note    This function enables handling the system keys.
*******************************************************************************/
void D4D_EnableSystemKeys(D4D_BOOL bEnable)
{
  if(bEnable)
    d4d_systemFlags &= ~D4D_SYSTEM_F_SYSTEM_KEYS_DISABLE;
  else
    d4d_systemFlags |= D4D_SYSTEM_F_SYSTEM_KEYS_DISABLE;
}


/**************************************************************************/ /*!
* @brief   Place to keys buffer new keys events by binary mask
* @param   keys - keys mask
* @return  none
* @note    This is obsolete function, kept here just for backward application compatibility.
*          It's recommended to use void D4D_NewKeyEvent(D4D_KEY_SCANCODE scanCode).
*******************************************************************************/
void D4D_KeysChanged(D4D_KEYS keys)
{
  static D4D_KEYS keysLastState = 0;

 if(keys != keysLastState)
 {
    D4D_KEYS kChange;
    D4D_KEY_SCANCODE tmp_scanCode;

    kChange = (Byte)(keys ^ keysLastState);

    if(kChange & D4D_KEY_UP)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_UP;
      if((keys & D4D_KEY_UP) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;

      D4D_NewKeyEvent(tmp_scanCode);
    }

    if(kChange & D4D_KEY_DOWN)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_DOWN;
      if((keys & D4D_KEY_DOWN) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;

      D4D_NewKeyEvent(tmp_scanCode);
    }

    if(kChange & D4D_KEY_LEFT)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_LEFT;
      if((keys & D4D_KEY_LEFT) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;

      D4D_NewKeyEvent(tmp_scanCode);
    }

    if(kChange & D4D_KEY_RIGHT)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_RIGHT;
      if((keys & D4D_KEY_RIGHT) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;

      D4D_NewKeyEvent(tmp_scanCode);
    }

    if(kChange & D4D_KEY_ENTER)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_ENTER;
      if((keys & D4D_KEY_ENTER) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;

      D4D_NewKeyEvent(tmp_scanCode);
    }

    if(kChange & D4D_KEY_ESC)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_ESC;
      if((keys & D4D_KEY_ESC) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;

      D4D_NewKeyEvent(tmp_scanCode);
    }

    keysLastState = keys;
 }
}

/**************************************************************************/ /*!
* @brief   Place to key event into key buffer
* @param   scanCode - new key scan event
* @return  none
* @note    This function puts the new key event. It's recommanded fuction for input the keys.
*******************************************************************************/
void D4D_NewKeyEvent(D4D_KEY_SCANCODE scanCode)
{
  // Is there place for new record?
  if((d4d_KeysBuff.writePos + 1 == d4d_KeysBuff.readPos) || ((d4d_KeysBuff.readPos == 0) && (d4d_KeysBuff.writePos + 1 == D4D_KEYS_BUFF_LENGTH)))
  {
    return;
  }

  // place new record to buffer
  d4d_KeysBuff.buff[d4d_KeysBuff.writePos++] = scanCode;
  if(d4d_KeysBuff.writePos >= D4D_KEYS_BUFF_LENGTH)
    d4d_KeysBuff.writePos = 0;

  #ifdef D4D_INPUT_EVENT_CALLBACK
    D4D_INPUT_EVENT_CALLBACK();
  #endif
}

#ifdef D4D_LLD_TCH

/**************************************************************************/ /*!
* @brief   Gets the touch screen coordinates in client area
* @param   pObject - pointer to object that is asking for touch screen coordination
* @return  touch coordination in client area
* @note    This function returns the touch screen client coordination.
*******************************************************************************/
D4D_POINT D4D_GetTouchScreenCoordinates(D4D_OBJECT* pObject)
{
  return D4D_GetScreenToClientPoint(pObject, &d4d_TouchScreenCoor);
}

/**************************************************************************/ /*!
* @brief   This fuction notify eGUI that should check the touch screen
* @return  none
* @note    On the period of calling this function depends period of checking touch screen by eGUI.
*          The function could be called from interrupt routine.
*******************************************************************************/
void D4D_CheckTouchScreen(void)
{
  d4d_TouchScreen_Status |= D4D_TCHSCR_STATUS_CHECKTOUCH;
}

/**************************************************************************/ /*!
* @brief   Puts the new touch event into the eGUI, directly
* @param   touched - state of touch
* @param   x - X touch coordination
* @param   y - Y touch coordination
* @return  none
* @note    The other way how to put new touch event into eGUI system. It's designed
*          for non eGUI drivers to force simple add touch events.
*******************************************************************************/
void D4D_PutTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y)
{
  if((d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_EVENTS) == D4D_TCHSCR_STATUS_NEWEVENT)
    return;

  d4d_touchEvent.touched = touched;
  d4d_touchEvent.point.x = x;
  d4d_touchEvent.point.y = y;

  d4d_TouchScreen_Status |= D4D_TCHSCR_STATUS_NEWEVENT;
}

/**************************************************************************/ /*!
* @brief   Puts the new touch event into the eGUI, directly
* @param   touched - state of touch
* @param   x - raw X touch coordination
* @param   y - raw Y touch coordination
* @return  none
* @note    The other way how to put new touch event into eGUI system. It's designed
*          for non eGUI drivers to force simple add touch events.
*          All the data are non calibrated.
*******************************************************************************/
void D4D_PutRawTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y)
{
  if((d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_EVENTS) == D4D_TCHSCR_STATUS_NEWRAWEVENT)
    return;

  d4d_touchEvent.touched = touched;
  d4d_touchEvent.point.x = x;
  d4d_touchEvent.point.y = y;

  d4d_TouchScreen_Status |= D4D_TCHSCR_STATUS_NEWRAWEVENT;
}

/**************************************************************************/ /*!
* @brief   Clears all pending events in touch screen event buffer
* @return  none
* @note    Claers the event buffer
*******************************************************************************/
void D4D_ClearTouchScreenEvents(void)
{
  d4d_TouchScreen_Status = 0;
}


/**************************************************************************/ /*!
* @brief   Function invokes calibrate screen of touch screen
* @return  none
* @note    Clears the event buffer
*******************************************************************************/
void D4D_CalibrateTouchScreen(void)
{
  D4D_MouseShow(D4D_FALSE);
  D4D_TCH_Calibrate(D4D_COLOR_SYSTEM_FORE, D4D_COLOR_SYSTEM_BCKG);
  D4D_MouseShow(D4D_TRUE);
  D4D_InvalidateScreen(D4D_GetActiveScreen(), D4D_TRUE);
}

/**************************************************************************/ /*!
* @brief   Function return current using touch screen calibration data
* @return  Touch screen calibration data structure
* @note    Is handled complete structure , not only pointer.
*******************************************************************************/
D4D_TOUCHSCREEN_CALIB D4D_GetTouchScreenCalibration(void)
{
  return D4D_TCH_GetCalibration();
}

/**************************************************************************/ /*!
* @brief   Function sets newt touch screen calibration data
* @param   newCalib - new calibration data structure
* @return  none
* @note    Is handled complete structure , not only pointer.
*******************************************************************************/
void D4D_SetTouchScreenCalibration(D4D_TOUCHSCREEN_CALIB newCalib)
{
  D4D_TCH_SetCalibration(newCalib);
}
#endif

/**************************************************************************/ /*!
* @brief   Function sets object text font properties
* @param   pObject - pointer to object that has to have changed font properties
* @param   property - new font properties
* @return  none
* @note    Function sets the font properties of major object text.
*******************************************************************************/
void D4D_SetFontProperties(D4D_OBJECT_PTR pObject, D4D_FONT_PROPERTIES property)
{

  D4D_STRING* p_TextBuff = NULL;

  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4D_OBJECT*)pObject);

  if(p_TextBuff)
  {
    if(p_TextBuff->str_properties->font_properties == property)
      return;     // There is no change needed

    p_TextBuff->str_properties->font_properties = property;
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }

}

/**************************************************************************/ /*!
* @brief   Function sets object text properties
* @param   pObject - pointer to object that has to have changed text properties
* @param   property - new text properties
* @return  none
* @note    Function sets the text properties of major object string.
*******************************************************************************/
void D4D_SetTextProperties(D4D_OBJECT_PTR pObject, D4D_TEXT_PROPERTIES property)
{
  D4D_STRING* p_TextBuff = NULL;

  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4D_OBJECT*)pObject);

  if(p_TextBuff)
  {
    if(p_TextBuff->str_properties->text_properties == property)
      return;     // There is no change needed

    p_TextBuff->str_properties->text_properties = property;
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }
}

/**************************************************************************/ /*!
* @brief   Function swap the point coordination
* @param   point - input size coordination
* @return  swapped X&Y point structure
* @note    none
*******************************************************************************/
D4D_POINT D4D_SwapCoor(D4D_POINT point)
{
  D4D_POINT tmp_point;

  tmp_point.x = point.y;
  tmp_point.y = point.x;

  return tmp_point;
}

/**************************************************************************/ /*!
* @brief   Function returns longer side of size structure
* @param   pSz - input size coordination
* @return  longer size of size input structure
* @note    none
*******************************************************************************/
D4D_COOR D4D_GetLongerSide(D4D_SIZE* pSz)
{
  if(pSz->cx > pSz->cy)
    return pSz->cx;

  return pSz->cy;
}

/**************************************************************************/ /*!
* @brief   Function force flush output to LCD
* @return  none
* @note    It usable in system with cache etc.
*******************************************************************************/
void D4D_FlushOutput(void)
{
  D4D_LLD_LCD.D4DLCD_FlushBuffer(D4DLCD_FLSH_FORCE);
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_base_func                                               */
/******************************************************************************/

/******************************************************************************
*       Private functions                                                     *
******************************************************************************/

/**************************************************************//*!
*
* Handle the keys (part of main poll call)
*
******************************************************************/
void D4D_HandleKeys(void)
{
    // get active screen
    D4D_SCREEN* pScreen = D4D_GetActiveScreen();
    D4D_OBJECT* pFocus;
    D4D_KEY_SCANCODE scanCode;
    D4D_BOOL tmp_release;

    // TODO what about sending keys to Child objects??

    // Check the buffer of input keys changes
    if(d4d_KeysBuff.writePos == d4d_KeysBuff.readPos)
      return;

    // Read the latest record in the buffer of input key event
    scanCode = d4d_KeysBuff.buff[d4d_KeysBuff.readPos++];

    // Check the overflow of the read pointer of readed input keys
    if(d4d_KeysBuff.readPos >= D4D_KEYS_BUFF_LENGTH)
        d4d_KeysBuff.readPos = 0;

    tmp_release = (D4D_BOOL)(scanCode & D4D_KEY_SCANCODE_RELEASEMASK);
    scanCode &= ~D4D_KEY_SCANCODE_RELEASEMASK;

    // do we handle the keys ourselves (to navigate across the screen) ?
    if(!d4d_pKeysCapturer && !(d4d_systemFlags & D4D_SYSTEM_F_SYSTEM_KEYS_DISABLE))
    {
        if((scanCode & D4D_KEY_SCANCODE_RELEASEMASK) == 0)
        {
          // escape the screen?
          if(scanCode == D4D_KEY_SCANCODE_ESC)
          {
            if(tmp_release == 0)
              D4D_EscapeScreen();
            return;
          }
          // focus previous object on the screen?
          else if(scanCode ==  D4D_KEY_FUNC_FOCUS_PREV)
          {
            if(tmp_release == 0)
              D4D_FocusPrevObject(pScreen);
            return;
          }
          // focus next object on the screen?
          else if(scanCode ==  D4D_KEY_FUNC_FOCUS_NEXT)
          {
            if(tmp_release == 0)
              D4D_FocusNextObject(pScreen, D4D_FALSE);
            return;
          }
        }
    }

    // does the object get the key events?
    // yes, invoke the key events to the active object
    pFocus = (D4D_OBJECT*) (d4d_pKeysCapturer ? d4d_pKeysCapturer : D4D_GetFocusedObject(pScreen));
    if((pFocus->pData->flags & (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP)) == (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP))
    {
      if(D4D_IsEnabled(pFocus))
      {
        // prepare the message
        d4d_msg.pScreen = pScreen;
        d4d_msg.pObject = pFocus;

        if(tmp_release == 0)
          d4d_msg.nMsgId = D4D_MSG_KEYDOWN;   // if key was pressed down?
        else
          d4d_msg.nMsgId = D4D_MSG_KEYUP;     // if key was released up?

        d4d_msg.prm.key = (D4D_KEY_SCANCODE)(scanCode & D4D_KEY_SCANCODE_KEYMASK);

        D4D_SendMessageBack(&d4d_msg);
      }
    }
    else
    {
        // The object cannot receive the KEY_EVENT, send it at least to screen
        // prepare the message
        d4d_msg.pScreen = pScreen;
        d4d_msg.pObject = NULL;

        if(tmp_release == 0)
          d4d_msg.nMsgId = D4D_MSG_KEYDOWN;   // if key was pressed down?
        else
          d4d_msg.nMsgId = D4D_MSG_KEYUP;     // if key was released up?

        d4d_msg.prm.key = (D4D_KEY_SCANCODE)(scanCode & D4D_KEY_SCANCODE_KEYMASK);

        D4D_SendMessage(&d4d_msg);
    }
}

/**************************************************************//*!
*
* helper to invoke OBJECT's message handler
*
* all information need to be already prepared in d4d_msg
*
******************************************************************/
void D4D_SendMessage(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pObject = pMsg->pObject;

    // screen gets the first chance
    if(pMsg->pScreen && pMsg->pScreen->OnObjectMsg)
    {
        // screen may discard the message by returning TRUE
        if(pMsg->pScreen->OnObjectMsg(pMsg))
            return ;
    }

    if(pObject)
    {
      // now invoke the object's handler
      if(pObject->OnUsrMessage)
      {
        if(pObject->OnUsrMessage(pMsg))
          return;
      }

      if(pObject->pObjFunc->OnSysMessage)
          pObject->pObjFunc->OnSysMessage(pMsg);
    }
}

/**************************************************************//*!
*
* helper to invoke OBJECT's message handler
*
* all information need to be already prepared in d4d_msg
*
******************************************************************/
void D4D_SendMessageMask(D4D_MESSAGE* pMsg, D4D_OBJECT_FLAGS parentFlagsMask, D4D_OBJECT_FLAGS endFlagMask)
{
    D4D_OBJECT* pObject = pMsg->pObject;

    // Check if the object
    if((pObject->pData->flags & parentFlagsMask) != parentFlagsMask)
      return;

    // screen gets the first chance
    if((pObject->pData->flags & endFlagMask) == endFlagMask)
      if(pMsg->pScreen->OnObjectMsg)
      {
          // screen may discard the message by returning TRUE
          if(pMsg->pScreen->OnObjectMsg(pMsg))
              return ;
      }

    if(pObject)
    {
      // now invoke the object's handler
      if((pObject->pData->flags & endFlagMask) == endFlagMask)
      {
        if(pObject->OnUsrMessage)
        {
          if(pObject->OnUsrMessage(pMsg))
            return;
        }

        // Invoke to go the message to system handler of object
        if(pObject->pObjFunc->OnSysMessage)
            pObject->pObjFunc->OnSysMessage(pMsg);
      }

      if(pObject->pRelations)
      {
        D4D_OBJECT** pChildObj = (D4D_OBJECT**)&pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];
        while(*pChildObj)
        {
          pMsg->pObject = *pChildObj;
          D4D_SendMessageMask(pMsg, parentFlagsMask, endFlagMask);
          pChildObj++;
        }
      }

    }
}


/**************************************************************//*!
*
* helper to invoke OBJECT's message handler
*
* all information need to be already prepared in d4d_msg
*
******************************************************************/
void D4D_SendMessageBack(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pObject = pMsg->pObject;

  if(pObject && pObject->pRelations)
  {
    D4D_OBJECT* pParentObj = (D4D_OBJECT*)pObject->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];
    if(pParentObj)
    {
      pMsg->pObject = pParentObj;
      D4D_SendMessageBack(pMsg);
    }
  }

  pMsg->pObject = pObject;
  D4D_SendMessage(pMsg);
}

#ifdef D4D_LLD_TCH
/**************************************************************//*!
*
* Find the touched object function
*
******************************************************************/
static D4D_BOOL D4D_FindTouchedObject(D4D_OBJECT* pObject, D4D_POINT point)
{
  D4D_BOOL validCoor;

  if((pObject->pData->flags & (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE)) != (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE))
    return D4D_FALSE;

  if(pObject->pObjFunc->CheckCoordinates == NULL)
    validCoor = D4D_ObjectCheckCoordinates(pObject, D4D_GetScreenToClientPoint(pObject, &point));
  else
    // Check the coordination if its valid
    validCoor = pObject->pObjFunc->CheckCoordinates(pObject, D4D_GetScreenToClientPoint(pObject, &point));

  // if the coordinations are valid or autorepeat is enabled
  if(validCoor)
  {
    // This is new touch
    d4d_LastTouchedObj = pObject;
    d4d_msg.pObject = pObject;
    d4d_msg.nMsgId = D4D_MSG_TOUCHED;
    // send the D4D_MSG_TOUCHED message
    D4D_SendMessage(&d4d_msg);

    if(pObject->pRelations)
    {
      D4D_OBJECT** pChildren = (D4D_OBJECT**)&pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];

      while(*pChildren && !D4D_FindTouchedObject(*pChildren, point))
      {
        pChildren++;
      }
    }
    return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Handle the TouchScreen (part of main poll call)
*
******************************************************************/
static void D4D_HandleTouchScreen(D4D_SCREEN* pScreen)
{
  D4D_SCREEN_DATA* pData = pScreen->pData;
  D4D_BOOL validCoor;
  D4D_OBJECT** pObjects;
  //D4D_POINT tmpClientTouch = D4D_GetScreenToClientPoint(pScreen, &d4d_TouchScreenCoor);

  // prepare message
  d4d_msg.pScreen = pScreen;
  d4d_msg.prm.draw = 0;

  //1. Check if this is new touch or not
  if(d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_NEWTOUCH)
  {
    // new touch operation
    d4d_TouchScreen_Status &= ~(D4D_TCHSCR_STATUS_NEWTOUCH | D4D_TCHSCR_STATUS_LOSTTOUCH);

    // Handle Screen events
    if(pData->flags & D4D_SCR_FINT_TOUCHENABLE)
    {
      if(pScreen->flags & D4D_SCR_F_EXIT)
      {
        if(D4D_ScrCheckExitBtnCoor(pScreen, &d4d_TouchScreenCoor))
        {
          D4D_EscapeScreen();
          d4d_TouchScreen_Status &= ~D4D_TCHSCR_STATUS_NEWTOUCH;
          return;
        }
      }

      if(D4D_ScrCheckCoor(pScreen, &d4d_TouchScreenCoor))
      {
        d4d_msg.pObject = NULL;
        d4d_msg.nMsgId = D4D_MSG_TOUCHED;
        // send the D4D_MSG_TOUCHED message
        D4D_SendMessage(&d4d_msg);
      }
    }

    pObjects = (D4D_OBJECT**)pScreen->pObjects;

    // check all objects
    while(*pObjects)
    {
      if(D4D_FindTouchedObject(*pObjects, d4d_TouchScreenCoor))
        break;
      pObjects++;
    }

  }else
  {
    if(d4d_LastTouchedObj)
    {
      //Untouch operation
      if(!(d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_TOUCHED))
      {
        // prepare message
        d4d_msg.nMsgId = D4D_MSG_UNTOUCHED;
        d4d_msg.prm.draw = 0;

        if(pData->flags & D4D_SCR_FINT_TOUCHENABLE)
        {
          if(D4D_ScrCheckCoor(pScreen, &d4d_TouchScreenCoor))
          {
            d4d_msg.pObject = NULL;
            // send the D4D_MSG_UNTOUCHED message to screen
            D4D_SendMessage(&d4d_msg);  // To screen
          }
        }

        if(d4d_LastTouchedObj)
        {
          d4d_msg.pObject = d4d_LastTouchedObj;
          d4d_LastTouchedObj = NULL;
          // send the D4D_MSG_UNTOUCHED message to Object
        }else
          return;
      }
      else
      {
        //Autorepeat operations

        // Handle Screen events
        if(pData->flags & D4D_SCR_FINT_TOUCHENABLE)
        {
          if(D4D_ScrCheckCoor(pScreen, &d4d_TouchScreenCoor))
          {
            d4d_msg.pObject = NULL;
            d4d_msg.nMsgId = D4D_MSG_TOUCH_AUTO;
            // send the D4D_MSG_TOUCH_AUTO message
            D4D_SendMessage(&d4d_msg);  // To screen
          }
        }

        d4d_msg.pObject = d4d_LastTouchedObj;

        // Check the coordination if its valid
        if(d4d_LastTouchedObj->pObjFunc->CheckCoordinates)
          validCoor = d4d_LastTouchedObj->pObjFunc->CheckCoordinates(d4d_LastTouchedObj, D4D_GetScreenToClientPoint(d4d_LastTouchedObj, &d4d_TouchScreenCoor));
        else
          validCoor = D4D_ObjectCheckCoordinates(d4d_LastTouchedObj, D4D_GetScreenToClientPoint(d4d_LastTouchedObj, &d4d_TouchScreenCoor));

        // autorepeat touch
        if(!validCoor && (d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_LOSTTOUCH))
          return;

        // Check if the autorepeat touch is valid or no (LOST TOUCH EVENT)
        if(!validCoor)
        {
          d4d_TouchScreen_Status |= D4D_TCHSCR_STATUS_LOSTTOUCH;
          d4d_msg.nMsgId = D4D_MSG_TOUCH_LOST;
        }else
        {
          d4d_TouchScreen_Status &= ~D4D_TCHSCR_STATUS_LOSTTOUCH;
          d4d_msg.nMsgId = D4D_MSG_TOUCH_AUTO;
        }
      }
        // Send prepared message
        D4D_SendMessageBack(&d4d_msg);
      }
  }
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4D_GetTouchScreenPosition
// SCOPE:       Global
// DESCRIPTION: Reads touch screen and returns X, Y coorinates if screen
//              touched
// PARAMETERS:  LWord *TouchPositionX    Pointer to X coordinate
//              LWord *TouchPositionY    Pointer to Y ccordinate
// RETURNS:     0   no screen touch
//              1   screen touch,
//-----------------------------------------------------------------------------
static D4D_BOOL D4D_GetTouchScreen(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY)
{
    // Declare and initialise local variables
    D4D_BOOL ScreenTouch;


    if(d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_CHECKTOUCH)
    {
	    // Read raw touch position into *TouchPositionX and *TouchPositionY
	    ScreenTouch = D4D_TCH_GetRawPosition(TouchPositionX, TouchPositionY);
    }
    else
    {
    	ScreenTouch = d4d_touchEvent.touched;
    	*TouchPositionX = d4d_touchEvent.point.x;
    	*TouchPositionY = d4d_touchEvent.point.y;
    }


    if(ScreenTouch)
    {
      if(!(d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_NEWEVENT))
        D4D_TCH_GetCalibratedPosition(TouchPositionX, TouchPositionY);
    }
    // Return flag to indicate if screen has been touched
    return ScreenTouch;
}

/**************************************************************//*!
*
* Manage touch screen events (part of main poll call)
*
******************************************************************/
static void D4D_ManageTouchScreenEvents(void)
{
  D4D_COOR x,y;
  D4D_BOOL tmp_res;

  if(D4D_TCH_GetCalibrationStatus() == 0)
  {
    D4D_CalibrateTouchScreen();
    return;
  }

  if(d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_NEWTOUCH)
    return;

  tmp_res = D4D_GetTouchScreen(&x, &y);

  if(((d4d_TouchScreen_Status & D4D_TCHSCR_STATUS_TOUCHED) ^ (tmp_res)))
  {
    if(tmp_res)
      d4d_TouchScreen_Status |= D4D_TCHSCR_STATUS_TOUCHED;
    else
      d4d_TouchScreen_Status &= ~D4D_TCHSCR_STATUS_TOUCHED;

    if(tmp_res)
      d4d_TouchScreen_Status |= D4D_TCHSCR_STATUS_NEWTOUCH;
  }

  if(tmp_res)
  {
    d4d_TouchScreenCoor.x = (D4D_COOR)x;
    d4d_TouchScreenCoor.y = (D4D_COOR)y;

    #ifdef D4D_INPUT_EVENT_CALLBACK
      D4D_INPUT_EVENT_CALLBACK();
    #endif
  }

}
#endif

/**************************************************************//*!
*
* Compute object content geometry
*
******************************************************************/
void D4D_ComputeGeometry(D4D_GEOMETRY * pGeometry, D4D_OBJECT* pObject)
{
  pGeometry->pnt = D4D_GetClientToScreenPoint(pObject, &pObject->position);
  pGeometry->sz = pObject->size;

  if(pObject->pMargin)
  {
    pGeometry->pnt.x += pObject->pMargin->left;
    pGeometry->pnt.y += pObject->pMargin->top;

    if(pGeometry->sz.cx >= (pObject->pMargin->left + pObject->pMargin->right))
      pGeometry->sz.cx -= (pObject->pMargin->left + pObject->pMargin->right);
    if(pGeometry->sz.cy >= (pObject->pMargin->top + pObject->pMargin->bottom))
      pGeometry->sz.cy -= (pObject->pMargin->top + pObject->pMargin->bottom);

  }else if(pObject->pData->flags & D4D_OBJECT_F_BEVEL_MASK)
  {
    pGeometry->pnt.x += D4D_BEVEL_WIDTH;
    pGeometry->pnt.y += D4D_BEVEL_WIDTH;

    if(pGeometry->sz.cx >= 2 * D4D_BEVEL_WIDTH)
      pGeometry->sz.cx -= 2 * D4D_BEVEL_WIDTH;

    if(pGeometry->sz.cy >= 2 * D4D_BEVEL_WIDTH)
      pGeometry->sz.cy -= 2 * D4D_BEVEL_WIDTH;

  }else if(pObject->pData->flags & D4D_OBJECT_F_FOCUSRECT)
  {
    pGeometry->pnt.x++;
    pGeometry->pnt.y++;
    if(pGeometry->sz.cx >= 2)
      pGeometry->sz.cx -= 2;
    if(pGeometry->sz.cy >= 2)
      pGeometry->sz.cy -= 2;
  }
}
