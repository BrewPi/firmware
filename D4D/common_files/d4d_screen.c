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
* @file      d4d_screen.c
*
* @author     Petr Gargulak
*
* @version   0.0.44.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver screen functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_lldapi.h"



static D4D_COOR D4D_GetScrHeaderTitleOffset(D4D_SCREEN* pScreen);
static D4D_BOOL D4D_GetScrHeaderExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* resultPos, D4D_SIZE* resultSize);
static void D4D_DrawScreenNC(D4D_SCREEN* pScreen, D4D_BOOL active);
static D4D_OBJECT* D4D_FindObject(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject);
static void D4D_ChangeScreen(D4D_SCREEN* pNewScreen, D4D_SCREEN* pOldScreen);


D4D_MARGIN d4d_screenMarginDflt =
{
  0,    //left
  0,    //top
  0,    //right
  0     //bottom
};

/******************************************************************************
* Begin of D4D_SCREEN public functions
*//*! @addtogroup doxd4d_screen_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Returns pointer to current active screen
* @return  pointer to current active screen
* @note    none
*******************************************************************************/
D4D_SCREEN* D4D_GetActiveScreen(void)
{
    // no screen yet shown
    if(d4d_screenHistoryIndex <= 0)
        return NULL;

    return d4d_screenHistory[d4d_screenHistoryIndex - 1];
}

/**************************************************************************/ /*!
* @brief   The function activate the new screen
* @param   pNewScreen - the pointer to new screen
* @param   bReplaceCurrent - the flag to tell function if the new screen should replace previous one in screen buffer
* @return  None
* @note    Enter new screen, remember the history if not only replacing
*******************************************************************************/
void D4D_ActivateScreen(D4D_SCREEN* pNewScreen, D4D_BOOL bReplaceCurrent)
{
  D4D_SCREEN* pOldScreen;

  // can not activate screen (no space left in history array)
  if(d4d_screenHistoryIndex >= D4D_SCREEN_HISTORY && !bReplaceCurrent)
      return ;

  if(!pNewScreen)
    return;

  // cancel key capture
  D4D_CaptureKeys(NULL);

  // deactivate current screen (if any)
  pOldScreen = D4D_GetActiveScreen();

  if(pOldScreen != NULL)
  {
      // replacing this screen in history?
      if(bReplaceCurrent)
          d4d_screenHistoryIndex--; // note that we are sure this is >0
  }

  // set the new screen as the active one
  d4d_screenHistory[d4d_screenHistoryIndex] = pNewScreen;
  d4d_screenHistoryIndex++;

  D4D_ChangeScreen(pNewScreen, pOldScreen);
}

/**************************************************************************/ /*!
* @brief   The function return to previous screen recorded in the history
* @return  None
* @note    Recall the previous screen in screen history buffer
*******************************************************************************/
void D4D_EscapeScreen(void)
{
    D4D_SCREEN* pOldScreen;

    // can not escape current screen, it is the top one
    if(d4d_screenHistoryIndex <= 1)
      return;

    // cancel key capture
    D4D_CaptureKeys(NULL);

    pOldScreen = D4D_GetActiveScreen();

    // pop the history stack
    if(d4d_screenHistoryIndex > 1)
      d4d_screenHistoryIndex--;

    D4D_ChangeScreen(D4D_GetActiveScreen(), pOldScreen);

}

/**************************************************************************/ /*!
* @brief   The function return to base screen recorded in the history
* @return  None
* @note    Recall the base (first) screen in screen history buffer
*******************************************************************************/
void D4D_EscapeToBaseScreen(void)
{
    D4D_SCREEN* pOldScreen;

    // can not escape current screen, it is the top one
    if(d4d_screenHistoryIndex == 0)
        return;

    // cancel key capture
    D4D_CaptureKeys(NULL);


    // I can be sure this is not NULL
    pOldScreen = D4D_GetActiveScreen();

    // pop the history stack
    d4d_screenHistoryIndex = 1;

    D4D_ChangeScreen(D4D_GetActiveScreen(), pOldScreen);
}

/**************************************************************************/ /*!
* @brief   The function inits the screen and its objects for first time case
* @param   pScreen - the pointer to screen
* @return  None
* @note    Initialize the the screen. This is keep as a public API to allow user application
*          initialize the screen before it native first use.
*******************************************************************************/
void D4D_InitScreen(D4D_SCREEN* pScreen)
{
  D4D_SCREEN_DATA* pData = pScreen->pData;
  D4D_OBJECT** pObj;
  D4D_MESSAGE localMsg;


  if(!pScreen)
    return;

  // prepare message
  localMsg.pScreen = pScreen;
  localMsg.nMsgId = D4D_MSG_ONINIT;

  pObj = (D4D_OBJECT**) pScreen->pObjects;

  // init objects
  while(*pObj != NULL)
  {

    localMsg.pObject = *pObj;

    // initialize the pointers on this screen in all screen objects
    D4D_SetObjectScreenPointer(*pObj, pScreen);

    // send the ON INIT message
    D4D_SendMessageMask(&localMsg, 0, D4D_OBJECT_F_NOTINIT);

    pObj++;
  }

  if(!(pData->flags & D4D_SCR_FINT_INITDONE))
  {
    pData->flags |= D4D_SCR_FINT_INITDONE;

    D4D_FocusNextObject(pScreen, D4D_TRUE);

    // user's screen initialization
    if(pScreen->OnInit)
        pScreen->OnInit();

    // Enable natively touch scren for screens if exit button is enabled
    if(pScreen->flags & D4D_SCR_F_EXIT)
      pData->flags |= D4D_SCR_FINT_TOUCHENABLE;
  }
}

/**************************************************************************/ /*!
* @brief   The function mark the screen and its abject as "redraw pending"
* @param   pScreen - the pointer to screen that should be invalidate
* @param   bComplete - flag to mark the the screen and object MUST be redrawed completely, not only the active areas
* @return  None
* @note    Invalidate screen and its object in two ways - complete or active areas only.
*******************************************************************************/
void D4D_InvalidateScreen(D4D_SCREEN* pScreen, D4D_BOOL bComplete)
{
    if(!pScreen)
      return;

    pScreen->pData->flags |= D4D_SCR_FINT_CHECKOBJECTS;

    if(bComplete)
    {
        pScreen->pData->flags |= D4D_SCR_FINT_REDRAWC;
    }
    else
    {
      D4D_OBJECT** pObj = (D4D_OBJECT**)pScreen->pObjects;
      while(*pObj != NULL)
      {
        D4D_SetObjectFlags(*pObj,  D4D_OBJECT_F_REDRAW, D4D_TRUE);
        pObj++;
      }
    }
}

/**************************************************************************/ /*!
* @brief   The function returns pointer to object that is focused in given screen
* @param   pScreen - the pointer to screen
* @return  pointer of focused object in given screen
* @note    None.
*******************************************************************************/
D4D_OBJECT_PTR D4D_GetFocusedObject(D4D_SCREEN* pScreen)
{
  if(!pScreen)
    return NULL;

  // just return the object on a focus index
  return pScreen->pData->focusedObject;
}





/**************************************************************************/ /*!
* @brief   The function change focus to the next object in the given screen
* @param   pScreen - the pointer to screen
* @param   bInitialSearch - flag force start looking from the first object in screen object table
* @return  None
* @note    In case that there is no other usable object (visible, enable ...) the focus is not changed
*******************************************************************************/
void D4D_FocusNextObject(D4D_SCREEN* pScreen, D4D_BOOL bInitialSearch)
{
  D4D_SCREEN_DATA* pData = pScreen->pData;
  const D4D_OBJECT* const* pObjects = pScreen->pObjects;
  D4D_OBJECT* pFocusedObj = pData->focusedObject;

  if(!pScreen)
    return;

  // sanity check of list of objects - contains Screen any object?
  if(*pObjects == NULL)
    return;

  // currently focused object already has a tabstop
  if(bInitialSearch)
  {
    //SetUp start object
    pData->focusedObject = (D4D_OBJECT*)pObjects[0];
    pFocusedObj = (D4D_OBJECT*)pObjects[0];
    if((pObjects[0]->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED)) == (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED))
      return ;
  }

  do
  {

      // get next object
      pFocusedObj = D4D_FindNextObject(pFocusedObj, (((pFocusedObj->pData->flags) & (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)) == (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)));

      // object with focus enabled?
      if((pFocusedObj->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)) == (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE))
          break;

    // avoid endless loop if no focused object can be found
  }while(((D4D_OBJECT*)pFocusedObj) != pData->focusedObject);



  if(((D4D_OBJECT*)pFocusedObj) != pData->focusedObject)
  {
    // invalidate object which is loosing focus
    D4D_SetObjectFlags(pData->focusedObject, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

    // prepare message KILLFOCUS
    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_KILLFOCUS;
    d4d_msg.pObject = pData->focusedObject;
    D4D_SendMessage(&d4d_msg);


    // invalidate object which is getting focus
    D4D_SetObjectFlags((D4D_OBJECT*)pFocusedObj, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

    // move the focus
    pData->focusedObject = ((D4D_OBJECT*)pFocusedObj);

    // prepare message
    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_SETFOCUS;
    d4d_msg.pObject = ((D4D_OBJECT*)pFocusedObj);
    D4D_SendMessage(&d4d_msg);
  }
}

/**************************************************************************/ /*!
* @brief   The function change focus to the previous object in the given screen
* @param   pScreen - the pointer to screen
* @return  None
* @note    In case that there is no other usable object (visible, enable ...) the focus is not changed
*******************************************************************************/
void D4D_FocusPrevObject(D4D_SCREEN* pScreen)
{
    D4D_SCREEN_DATA* pData = pScreen->pData;
    const D4D_OBJECT* const* pObjects = pScreen->pObjects;
    D4D_OBJECT* pFocusedObj = pData->focusedObject;

    if(!pScreen)
      return;

    // sanity check of list of objects - contains Screen any object?
    if(*pObjects == NULL)
      return;

    do
    {

        // just get previous object
        pFocusedObj = D4D_FindPreviousObject(pFocusedObj, (((pFocusedObj->pData->flags) & (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)) == (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)));

        // object with focus enabled?
        if((pFocusedObj->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)) == (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE))
        {
          D4D_OBJECT * pParent = pFocusedObj;
          D4D_BOOL couldBeFocused = D4D_TRUE;

          // Take care that the parents objects are also visible and enabled
          while(pParent = D4D_GetParentObject(pParent))
          {
            if((pParent->pData->flags & (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE)) != (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_VISIBLE))
            {
              couldBeFocused = D4D_FALSE;
              break;
            }
          }

          if(couldBeFocused)
            break;
        }

      // avoid endless loop if no focused object can be found
    }while(pFocusedObj != pData->focusedObject);

    if(pFocusedObj != pData->focusedObject)
    {
      // invalidate object which is loosing focus
      D4D_SetObjectFlags(pData->focusedObject, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

      // prepare message KILLFOCUS
      d4d_msg.pScreen = pScreen;
      d4d_msg.nMsgId = D4D_MSG_KILLFOCUS;
      d4d_msg.pObject = pData->focusedObject;
      D4D_SendMessage(&d4d_msg);

      // invalidate object which is getting focus
      D4D_SetObjectFlags((D4D_OBJECT*)pFocusedObj, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

      // move the focus
      pData->focusedObject = pFocusedObj;

      // prepare message
      d4d_msg.pScreen = pScreen;
      d4d_msg.nMsgId = D4D_MSG_SETFOCUS;
      d4d_msg.pObject = pFocusedObj;
      D4D_SendMessage(&d4d_msg);
    }
}

/**************************************************************************/ /*!
* @brief   The function set the obejct focus to new object
* @param   pScreen - the pointer to screen
* @param   pObject - the pointer to object that should be focused
* @return  None
* @note    In case that there is no other issue (obejct exists, visible, enable ...) the focus is changed to given one
*******************************************************************************/
void D4D_FocusSet(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject)
{
    D4D_SCREEN_DATA* pData = pScreen->pData;
    D4D_OBJECT* pFocusedObj = pData->focusedObject;
    D4D_OBJECT* pNewFocus;

    if(pScreen == NULL)
      return;

    // check if object is really item of the current screen
    pNewFocus = D4D_FindObject(pScreen, pObject);
    if(pNewFocus == NULL)
      return;

    if(pNewFocus == pFocusedObj) // is selected object same as focused?
      return;

   if((pNewFocus->pData->flags & (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED)) != (D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_ENABLED)) // is this object selectable?
      return;

    if(!D4D_IsEnabled(pNewFocus))
      return;

    // invalidate object which is loosing focus
    D4D_SetObjectFlags(pFocusedObj, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

    // invalidate object which is getting focus
    D4D_SetObjectFlags(pNewFocus, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

    // move the focus
    pData->focusedObject = pNewFocus;

    // prepare message
    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_KILLFOCUS;
    d4d_msg.pObject = pFocusedObj;
    D4D_SendMessage(&d4d_msg);



    // prepare message
    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_SETFOCUS;
    d4d_msg.pObject = pNewFocus;
    D4D_SendMessage(&d4d_msg);

}


#ifdef D4D_LLD_TCH
  /**************************************************************************/ /*!
  * @brief   The function enables/disables touch screen capability of screen
  * @param   pScr - the pointer to screen
  * @param   bEnable - enable/disable touch screen support
  * @return  None
  * @note    None
  *******************************************************************************/
  void D4D_EnableScrTouchScreen(const D4D_SCREEN* pScr, D4D_BOOL bEnable)
  {
    if(bEnable)
      pScr->pData->flags |= D4D_SCR_FINT_TOUCHENABLE;
    else
      pScr->pData->flags &= ~D4D_SCR_FINT_TOUCHENABLE;
  }
#endif

/**************************************************************************/ /*!
* @brief   The function sets the screen font properties
* @param   pScreen - the pointer to screen
* @param   property - font property
* @return  None
* @note    Sets the font property for text used natively by screen (title)
*******************************************************************************/
void D4D_SetScreenFontProperties(D4D_SCREEN* pScreen, D4D_FONT_PROPERTIES property)
{
  if(pScreen->textBuff.str_properties->font_properties == property)
    return;     // There is no change needed

  pScreen->textBuff.str_properties->font_properties = property;
  D4D_InvalidateScreen(pScreen, D4D_FALSE);
}

/**************************************************************************/ /*!
* @brief   The function sets the screen text properties
* @param   pScreen - the pointer to screen
* @param   property - text property
* @return  None
* @note    Sets the text property for text used natively by screen (title)
*******************************************************************************/
void D4D_SetScreenTextProperties(D4D_SCREEN* pScreen, D4D_TEXT_PROPERTIES property)
{
  if(pScreen->textBuff.str_properties->text_properties == property)
    return;     // There is no change needed

  pScreen->textBuff.str_properties->text_properties = property;
  D4D_InvalidateScreen(pScreen, D4D_FALSE);

}

/**************************************************************************/ /*!
* @brief   The function convert client point on the screen to the global screen point
* @param   pObject - the pointer to object
* @param   nClientPoint - pointer to client point structure
* @return  global screen point
* @note    The function already handles also the compounded objects.
*******************************************************************************/
D4D_POINT D4D_GetClientToScreenPoint(D4D_OBJECT* pObject, D4D_POINT* nClientPoint)
{
  D4D_POINT tmp_point;
  D4D_SCREEN* pScreen = pObject->pData->pScreen;


  if(pScreen == NULL)
  {
    return *nClientPoint;
  }

  tmp_point.x = (D4D_COOR)(pScreen->position.x + nClientPoint->x);
  tmp_point.y = (D4D_COOR)(pScreen->position.y + nClientPoint->y);

  if(pScreen->flags & D4D_SCR_F_BEVEL)
    tmp_point.x += D4D_BEVEL_WIDTH;
  else if(pScreen->flags & D4D_SCR_F_OUTLINE)
  	tmp_point.x++;

  tmp_point.y += D4D_GetScrHeaderSize(pScreen);

  if(pObject->pRelations)
  {
    D4D_OBJECT* pParent = (D4D_OBJECT*)pObject->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];

    while(pParent)
    {
      tmp_point.x += pParent->position.x;
      tmp_point.y += pParent->position.y;

      if(pParent->pMargin)
      {
        tmp_point.x += pParent->pMargin->left;
        tmp_point.y += pParent->pMargin->top;
      }

      pParent = (D4D_OBJECT*)pParent->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];
    }
  }

  return tmp_point;
}

/**************************************************************************/ /*!
* @brief   The function convert global screen point on the screen to the client point
* @param   pObject - the pointer to object
* @param   nScreenPoint - pointer to global screen point structure
* @return  client screen point
* @note    The function already handles also the compounded objects.
*******************************************************************************/
D4D_POINT D4D_GetScreenToClientPoint(D4D_OBJECT* pObject, D4D_POINT* nScreenPoint)
{
  D4D_POINT tmp_point;
  D4D_SCREEN* pScreen = pObject->pData->pScreen;

  if(pScreen == NULL)
  {
    return *nScreenPoint;
  }

  tmp_point.x = (D4D_COOR)(nScreenPoint->x - pScreen->position.x);
  tmp_point.y = (D4D_COOR)(nScreenPoint->y - pScreen->position.y);

  if(pScreen->flags & D4D_SCR_F_BEVEL)
    tmp_point.x -= D4D_BEVEL_WIDTH;
  else if(pScreen->flags & D4D_SCR_F_OUTLINE)
    tmp_point.x--;

  tmp_point.y -= D4D_GetScrHeaderSize(pScreen);

  if(pObject->pRelations)
  {
    D4D_OBJECT* pParent = (D4D_OBJECT*)pObject->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];

    while(pParent)
    {
      tmp_point.x -= pParent->position.x;
      tmp_point.y -= pParent->position.y;
      if(pParent->pMargin)
      {
        tmp_point.x -= pParent->pMargin->left;
        tmp_point.y -= pParent->pMargin->top;
      }
      pParent = (D4D_OBJECT*)pParent->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];
    }
  }


  return tmp_point;
}

/**************************************************************************/ /*!
* @brief   The function check if the coordination are in screen area
* @param   pScreen - the pointer to screen
* @param   point - pointer to input point structure
* @return  D4D_TRUE - in case that the coordination fits to the screen area, D4D_FALSE if not
* @note    None
*******************************************************************************/
D4D_BOOL D4D_ScrCheckCoor(D4D_SCREEN* pScreen, D4D_POINT* point)
{
  if((point->x >= pScreen->position.x) && (point->y >= (pScreen->position.y + D4D_GetScrHeaderSize(pScreen))))
  {
    if((point->x <= (pScreen->position.x + pScreen->size.cx)) && (point->y <= (pScreen->position.y + pScreen->size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}


/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_screen_func                                              */
/******************************************************************************/

/**************************************************************************/ /*!
* @brief   The function returns the pointer to last object in the screen
* @param   pScreen - the pointer to screen
* @return  pointer to last screen object
* @note    It's useful if the last object should be find
*******************************************************************************/
D4D_OBJECT* D4D_GetLastObject(D4D_SCREEN* pScreen)
{
    D4D_OBJECT** pObject;

    if(!pScreen)
      return NULL;

    pObject = (D4D_OBJECT**)pScreen->pObjects;

    while(*(pObject + 1))
      pObject++;

    return *pObject;
}

/**************************************************************//*!
*
* Find object's index
*
******************************************************************/
static D4D_OBJECT* D4D_FindObject(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject)
{
    D4D_OBJECT* pLocObject = pScreen->pData->focusedObject;

    if(!pScreen || !pLocObject)
      return NULL;

    do
    {
      // get next object
      pLocObject = D4D_FindNextObject(pLocObject, D4D_TRUE);

      if(pLocObject == pObject)
        return pLocObject;

    }while(pLocObject != pScreen->pData->focusedObject);

    // not found
    return NULL;
}

/**************************************************************//*!
*
* Change the active screen
*
******************************************************************/
static void D4D_ChangeScreen(D4D_SCREEN* pNewScreen, D4D_SCREEN* pOldScreen)
{
    D4D_SCREEN_DATA* pData;
    D4D_MESSAGE tmp_msg;

    if(pNewScreen == NULL)
      return;

    if(pOldScreen == pNewScreen)
      return;

    tmp_msg.pScreen = pOldScreen;

    if(pOldScreen != NULL)
    {
      D4D_SetObjectFlags(pOldScreen->pData->focusedObject, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

      tmp_msg.nMsgId = D4D_MSG_KILLFOCUS;
      tmp_msg.pObject = pOldScreen->pData->focusedObject;
      D4D_SendMessage(&tmp_msg);

      // Draw NC screen area as an inactivate
      D4D_DrawScreenNC(pOldScreen, D4D_FALSE);

      // call de-activate event
      if(pOldScreen->OnDeactivate != NULL)
          pOldScreen->OnDeactivate();
    }

    // invalidate the new screen (global complete redraw, not individual objects)
    D4D_InvalidateScreen(pNewScreen, D4D_TRUE);

    // if this is the first time activating
    pData = pNewScreen->pData;

    // Init the screen
    D4D_InitScreen(pNewScreen);

    D4D_SetObjectFlags(pData->focusedObject, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

    // Send to the object Focus message
    tmp_msg.pScreen = pNewScreen;
    tmp_msg.nMsgId = D4D_MSG_SETFOCUS;
    tmp_msg.pObject = pData->focusedObject;
    D4D_SendMessage(&tmp_msg);

    // inform the screen it has been activated
    if(pNewScreen->OnActivate)
        pNewScreen->OnActivate();

    D4D_ClearKeysBuffer();

  #ifdef D4D_LLD_TCH
    d4d_LastTouchedObj = NULL;
  #endif

    // finish all action for previous screen
    D4D_MouseChangedScreen();
}

/**************************************************************//*!
*
* Compute size of screen header
*
******************************************************************/
D4D_COOR D4D_GetScrHeaderSize(D4D_SCREEN* pScreen)
{
  D4D_COOR tmp_size = 0;

  if(pScreen == NULL)
    return 0;

  if((pScreen->flags & D4D_SCR_F_TITLEBAR) || pScreen->textBuff.pText)
  {
    tmp_size = (D4D_COOR)(D4D_SCR_TITLE_OFF_Y * 2 + D4D_GetFontHeight(pScreen->textBuff.fontId));
  }

  if(pScreen->pIcon != NULL)
    if(tmp_size < (D4D_GetBmpHeight(pScreen->pIcon) + D4D_SCR_TITLE_OFF_Y * 2 + 1))
      tmp_size = (D4D_COOR)(D4D_GetBmpHeight(pScreen->pIcon) + D4D_SCR_TITLE_OFF_Y * 2 + 1);

  if(pScreen->flags & D4D_SCR_F_TITLEBAR)
    if(tmp_size < D4D_SCR_HEADER_SIZE_MIN_SIZE)
      tmp_size = D4D_SCR_HEADER_SIZE_MIN_SIZE;

  return tmp_size;
}

/**************************************************************//*!
*
* Compute offset of screen header title
*
******************************************************************/
static D4D_COOR D4D_GetScrHeaderTitleOffset(D4D_SCREEN* pScreen)
{
  D4D_COOR tmp_off = D4D_SCR_TITLE_OFF_X;

  if(pScreen == NULL)
    return 0;

  if(pScreen->pIcon != NULL)
    tmp_off = (D4D_COOR)(D4D_GetBmpWidth(pScreen->pIcon) + D4D_SCR_TITLE_OFF_X * 2);

  #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    tmp_off += (pScreen->radius * 2);
  #endif

  return tmp_off;
}


/**************************************************************//*!
*
* Compute position of exit button on screen header title
*
******************************************************************/
static D4D_BOOL D4D_GetScrHeaderExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* resultPos, D4D_SIZE* resultSize)
{
  D4D_COOR tmp_coor = D4D_GetScrHeaderSize(pScreen);

  resultPos->x = 0;
  resultPos->y = 0;

  resultSize->cx = 0;
  resultSize->cy = 0;

  if(pScreen == NULL)
    return D4D_FALSE;

  if((tmp_coor - D4D_SCR_TITLE_EXITBTN_OFFSET * 2) <= 0)
    return D4D_FALSE;

  tmp_coor -= D4D_SCR_TITLE_EXITBTN_OFFSET * 2;

  if(tmp_coor < D4D_SCR_TITLE_EXITBTN_MIN_SIZE)
    return D4D_FALSE;

  resultSize->cx = tmp_coor;
  resultSize->cy = tmp_coor;

  resultPos->x = (D4D_COOR)(pScreen->position.x + pScreen->size.cx - tmp_coor - D4D_SCR_TITLE_EXITBTN_OFFSET);
  resultPos->y = (D4D_COOR)(pScreen->position.y + D4D_SCR_TITLE_EXITBTN_OFFSET);

  if(pScreen->flags & D4D_SCR_F_BEVEL)
  {
    resultPos->x -= D4D_BEVEL_WIDTH;
    resultPos->y += D4D_BEVEL_WIDTH;

  }else if(pScreen->flags & D4D_SCR_F_OUTLINE)
  {
    resultPos->x--;
    resultPos->y++;
  }

  return D4D_TRUE;
}

/**************************************************************//*!
*
* Redraw non client screen area - Header, background, title, etc.
*
******************************************************************/
static void D4D_DrawScreenNC(D4D_SCREEN* pScreen, D4D_BOOL active)
{
  D4D_CLR_SCHEME *pScheme = D4D_ScreenGetScheme(pScreen);
  D4D_COLOR clr;
  D4D_GEOMETRY contentGeom;
  D4D_POINT tmpPoint;
  D4D_SIZE tmpSize;

  contentGeom.pnt = pScreen->position;
  contentGeom.sz = pScreen->size;

  if(pScreen->flags & D4D_SCR_F_BEVEL)
  {
    contentGeom.pnt.x += D4D_BEVEL_WIDTH;
    contentGeom.pnt.y += D4D_BEVEL_WIDTH;
    contentGeom.sz.cx -= 2*D4D_BEVEL_WIDTH;
    contentGeom.sz.cy -= 2*D4D_BEVEL_WIDTH;

  }else if(pScreen->flags & D4D_SCR_F_OUTLINE)
  {
    contentGeom.pnt.x++;
    contentGeom.pnt.y++;
    contentGeom.sz.cx -= 2;
    contentGeom.sz.cy -= 2;
  }

  if(active)
  {
    if(pScreen->flags & D4D_SCR_F_BCKG)
    {
      D4D_FillRRect(&contentGeom.pnt, &contentGeom.sz, pScheme->screen.desktop, pScreen->radius);
    }

    if(pScreen->flags & D4D_SCR_F_BEVEL)
    {
      D4D_Bevel(&(pScreen->position), &(pScreen->size), pScheme->screen.outline, D4D_BEVEL_RAISED, pScreen->radius);
    }else if(pScreen->flags & D4D_SCR_F_OUTLINE)
    {
      D4D_RRect(&(pScreen->position), &(pScreen->size), D4D_LINE_THIN, pScheme->screen.outline, pScreen->radius);
    }
  }

  if(pScreen->flags & D4D_SCR_F_TITLEBAR)
  {

    clr = pScheme->screen.title_bar;

    if(!active)
      clr = D4D_GetGreyScale(clr);

    tmpSize = contentGeom.sz;
    tmpSize.cy = D4D_GetScrHeaderSize(pScreen);

    if(tmpSize.cy)
      tmpSize.cy--;

    D4D_FillRRect(&contentGeom.pnt, &tmpSize, clr, pScreen->radius);
  }

  if(pScreen->pIcon != NULL)
  {
    tmpPoint = contentGeom.pnt;
    tmpPoint.x += D4D_SCR_TITLE_OFF_X;
    tmpPoint.y += (D4D_COOR)((D4D_GetScrHeaderSize(pScreen) - D4D_GetBmpHeight(pScreen->pIcon)) / 2);

    D4D_DrawRBmp(&tmpPoint, pScreen->pIcon, (D4D_BOOL)!active, pScreen->radius);
  }

  if(pScreen->textBuff.pText)
  {
    tmpPoint = contentGeom.pnt;
    tmpPoint.x += D4D_GetScrHeaderTitleOffset(pScreen);
    tmpPoint.y += (D4D_COOR)((D4D_GetScrHeaderSize(pScreen) - D4D_GetFontHeight(pScreen->textBuff.fontId)) / 2);

    clr = pScheme->screen.title_text;

    if(!active)
      clr = D4D_GetGreyScale(clr);

    D4D_DrawText(&tmpPoint, &pScreen->textBuff, clr, 0);
    }

  if(pScreen->flags & D4D_SCR_F_EXIT)
  {
    clr = pScheme->screen.exitBtnFore;

    if(!active)
      clr = D4D_GetGreyScale(clr);

    if(D4D_GetScrHeaderExitBtnCoor(pScreen, &tmpPoint, &tmpSize))
    {
      D4D_RBox(&tmpPoint, &tmpSize, D4D_LINE_THIN, clr, pScheme->screen.exitBtnBckg, pScreen->radius);

      tmpPoint.x += D4D_SCR_EXITBTN_CROSS_SIZE;
      tmpPoint.y += D4D_SCR_EXITBTN_CROSS_SIZE;
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      tmpPoint.x += (D4D_COOR)(pScreen->radius / 2);
      tmpPoint.y += (D4D_COOR)(pScreen->radius / 2);
#endif

      D4D_MoveTo(&tmpPoint);
      tmpPoint.x += tmpSize.cx - (2 * D4D_SCR_EXITBTN_CROSS_SIZE);
      tmpPoint.y += tmpSize.cy - (2 * D4D_SCR_EXITBTN_CROSS_SIZE);
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      tmpPoint.x -= (D4D_COOR)(pScreen->radius);
      tmpPoint.y -= (D4D_COOR)(pScreen->radius);
#endif

      D4D_LineTo(&tmpPoint, D4D_LINE_THIN, clr);

      tmpPoint.y -= tmpSize.cy - (2 * D4D_SCR_EXITBTN_CROSS_SIZE);
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      tmpPoint.y += (D4D_COOR)(pScreen->radius);
#endif
      D4D_MoveTo(&tmpPoint);
      tmpPoint.x -= tmpSize.cx - (2 * D4D_SCR_EXITBTN_CROSS_SIZE);
      tmpPoint.y += tmpSize.cy - (2 * D4D_SCR_EXITBTN_CROSS_SIZE);
#if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      tmpPoint.x += (D4D_COOR)(pScreen->radius);
      tmpPoint.y -= (D4D_COOR)(pScreen->radius);
#endif
      D4D_LineTo(&tmpPoint, D4D_LINE_THIN, clr);
    }
  }
}

/**************************************************************//*!
*
* Redraw all objects that need it on given screen
*
******************************************************************/
static void D4D_RedrawScreenObject(D4D_OBJECT* pObject, D4D_BOOL complete)
{
  D4D_MESSAGE msg;
  D4D_OBJECT_FLAGS flags = pObject->pData->flags;

  pObject->pData->flags &= ~(D4D_OBJECT_F_REDRAW | D4D_OBJECT_F_REDRAWC | D4D_OBJECT_F_REDRAWSTATE);

  if(!(flags & D4D_OBJECT_F_VISIBLE))
    return;

  msg.pScreen = pObject->pData->pScreen;
  msg.pObject = pObject;
  msg.prm.draw = 0;


  if(complete || (flags & (D4D_OBJECT_F_REDRAW | D4D_OBJECT_F_REDRAWC | D4D_OBJECT_F_REDRAWSTATE))) /* object does need to redraw */
  {
    /* forced complete redraw of all objects or object wants to be completely redrawn*/
    if(complete || (flags & D4D_OBJECT_F_REDRAWC))
      msg.prm.draw |= D4D_OBJECT_DRAWFLAGS_COMPLETE;

    /* notify object that state of object has been changed (focus, capture etc)*/
    if(flags & D4D_OBJECT_F_REDRAWSTATE)
      msg.prm.draw |= D4D_OBJECT_DRAWFLAGS_STATE;


    // draw special?
    if((pObject == msg.pScreen->pData->focusedObject) && (pObject->pData->flags & D4D_OBJECT_F_TABSTOP))
    {
        msg.prm.draw |= D4D_OBJECT_DRAWFLAGS_FOCUSED;

        if(pObject == d4d_pKeysCapturer)
            msg.prm.draw |= D4D_OBJECT_DRAWFLAGS_CAPTURING;
    }

    // will draw now
    // send the DRAW message
    msg.nMsgId = D4D_MSG_DRAW;
    D4D_SendMessage(&msg);
    // send the DRAWDONE message
    msg.nMsgId = D4D_MSG_DRAWDONE;
    D4D_SendMessage(&msg);
  }


  if(pObject->pRelations)
  {
    D4D_OBJECT** pChild = (D4D_OBJECT**)&(pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX]);

    while(*pChild)
    {
      D4D_RedrawScreenObject(*pChild, complete);
      pChild++;
    }
  }
}


/**************************************************************//*!
*
* Redraw the screen based on redraw flags
*
******************************************************************/
void D4D_RedrawScreen(D4D_SCREEN* pScreen)
{
  D4D_SCREEN_DATA data = *pScreen->pData;
  D4D_OBJECT** pObj;

  pScreen->pData->flags &= ~(D4D_SCR_FINT_REDRAWC | D4D_SCR_FINT_CHECKOBJECTS);

  // prepare message
  d4d_msg.pScreen = pScreen;

  if(data.flags & (D4D_SCR_FINT_CHECKOBJECTS | D4D_SCR_FINT_REDRAWC))
    D4D_LLD_LCD.D4DLCD_FlushBuffer(D4DLCD_FLSH_SCR_START);

  // redraw screen non client area if necessary
  if(data.flags & D4D_SCR_FINT_REDRAWC)
  {
    d4d_msg.pObject = NULL;
    d4d_msg.nMsgId = D4D_MSG_DRAW;
    D4D_SendMessage(&d4d_msg);

    D4D_DrawScreenNC(pScreen, D4D_TRUE);
  }

  if(data.flags & D4D_SCR_FINT_CHECKOBJECTS)
  {
    // redraw objects
    pObj = (D4D_OBJECT**)pScreen->pObjects;
    // Go through the all top level objects
    while(*pObj != NULL)
    {
      // redraw all top level objects includes it children
      D4D_RedrawScreenObject(*pObj, data.flags & D4D_SCR_FINT_REDRAWC);
      pObj++;
    }

    d4d_msg.pObject = NULL;
    d4d_msg.nMsgId = D4D_MSG_DRAWDONE;
    D4D_SendMessage(&d4d_msg);
  }

  if(data.flags & (D4D_SCR_FINT_CHECKOBJECTS | D4D_SCR_FINT_REDRAWC))
    D4D_LLD_LCD.D4DLCD_FlushBuffer(D4DLCD_FLSH_SCR_END);
}

/**************************************************************//*!
*
* Handle the timeticks events of eGUI
*
******************************************************************/
void D4D_HandleTimeTicks(D4D_SCREEN* pScreen)
{
  if(d4d_systemFlags & D4D_SYSTEM_F_TIMETICK)
  {
    D4D_OBJECT** pObjects = (D4D_OBJECT**)pScreen->pObjects;
    d4d_systemFlags &= ~D4D_SYSTEM_F_TIMETICK;

    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_TIMETICK;

    // Go through the all top level objects
    while(*pObjects != NULL)
    {
      d4d_msg.pObject = *pObjects;
      // send timeticks to all top level objects includes it children
      D4D_SendMessageMask(&d4d_msg, D4D_OBJECT_F_VISIBLE , D4D_OBJECT_F_TIMETICK);
      pObjects++;
    }

  }
}

/**************************************************************//*!
*
* Check if the coordination are in range of exit button
*
******************************************************************/
D4D_BOOL D4D_ScrCheckExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* point)
{
  D4D_POINT tmp_pos;
  D4D_SIZE tmp_size;


  if(D4D_GetScrHeaderExitBtnCoor(pScreen, &tmp_pos, &tmp_size))
  {
    if((point->x >= tmp_pos.x) && (point->y >= tmp_pos.y))
    {
      if((point->x <= (tmp_pos.x + tmp_size.cx)) && (point->y <= (tmp_pos.y + tmp_size.cy)))
        return D4D_TRUE;
    }
  }
  return D4D_FALSE;
}

