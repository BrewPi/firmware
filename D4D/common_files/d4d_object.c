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
* @file      d4d_object.c
*
* @author     Petr Gargulak
*
* @version   0.0.14.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver object function c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"


/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/******************************************************************************
* Begin of D4D_OBJECT public functions
*//*! @addtogroup doxd4d_object_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function invalidate object to redraw on screen.
* @param   pObject - pointer to the object that should be invalidate.
* @param   bComplete - force complete redraw of object.
* @return  none.
* @note    This function invalidate object what in fact is that force eGUI to redraw
*          object on screen. By the bComplete parameter is select that will be
*          redraw also static parts of object.
*******************************************************************************/
void D4D_InvalidateObject(D4D_OBJECT_PTR pObject, D4D_BOOL bComplete)
{
  D4D_OBJECT_FLAGS flags = D4D_OBJECT_F_REDRAW;

  if(bComplete)
    flags |= D4D_OBJECT_F_REDRAWC;

  D4D_SetObjectFlags((D4D_OBJECT*)pObject,  flags, D4D_TRUE);

  if(pObject->pData->pScreen)
    (pObject->pData->pScreen)->pData->flags |= D4D_SCR_FINT_CHECKOBJECTS;
}

/**************************************************************************/ /*!
* @brief   Function control visibility of object on screen.
* @param   pObject - pointer to the object.
* @param   bShow - <D4D_TRUE - visible, D4D_FALSE - hidden>
* @return  none.
* @note    This function control the visibility of object on screen. The hide action
*          force invalidate of whole screen.
*******************************************************************************/
void D4D_ShowObject(D4D_OBJECT_PTR pObject, D4D_BOOL bShow)
{
  if(bShow)
  {
    pObject->pData->flags |= D4D_OBJECT_F_VISIBLE;
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }
  else
  {
    pObject->pData->flags &= ~D4D_OBJECT_F_VISIBLE;

    if(d4d_pKeysCapturer == pObject)
      D4D_CaptureKeys(NULL);


    if((D4D_OBJECT*)pObject == D4D_GetFocusedObject(pObject->pData->pScreen))
      D4D_FocusNextObject(pObject->pData->pScreen, D4D_FALSE);

    if(pObject->pRelations)
    {
      D4D_OBJECT* pParent = D4D_GetParentObject((D4D_OBJECT*)pObject);

      if(pParent)
        D4D_InvalidateObject(pParent, D4D_TRUE);
      else
        D4D_InvalidateScreen(pObject->pData->pScreen, D4D_TRUE);

    }else
      D4D_InvalidateScreen(pObject->pData->pScreen, D4D_TRUE);
  }
}

/**************************************************************************/ /*!
* @brief   Function return the object user data.
* @param   pThis - pointer to the object.
* @return  User data of object.
* @note    Each object has user data that could be used what ever user needs
*          and this function returns it.
*******************************************************************************/
void* D4D_GetUserPointer(D4D_OBJECT *pThis)
{
  if(pThis == NULL)
    return NULL;

  return pThis->userData;
}


/**************************************************************************/ /*!
* @brief   Function enables touchs screen capability and sets also mode of touch event
* @param   pObj - pointer to the object.
* @param   bEnable - <D4D_TRUE - touch event enabled, D4D_FALSE - touch event disabled>
* @param   bFastTouch - <D4D_TRUE - fast touch mode, D4D_FALSE -standard touch event>
* @return  none.
* @note    This function enables touch event for the object and also sets the mode of touch event:
*          - standard mode and fast touch mode. For example for object button the first touch just
*          focused the obejct and second one do click action.
*          - fast touch mode - do both actions at once (more user friendly for touch control).
*******************************************************************************/
void D4D_EnableTouchScreen(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable, D4D_BOOL bFastTouch)
{
  if(bEnable)
    pObj->pData->flags |= D4D_OBJECT_F_TOUCHENABLE;
  else
    pObj->pData->flags &= ~D4D_OBJECT_F_TOUCHENABLE;

  if(bFastTouch)
    pObj->pData->flags |= D4D_OBJECT_F_FASTTOUCH;
  else
    pObj->pData->flags &= ~D4D_OBJECT_F_FASTTOUCH;

}

/**************************************************************************/ /*!
* @brief   Function enable or diasble receiving timeticks for an OBJECT
* @param   pObj - pointer to the object.
* @param   bEnable - <D4D_TRUE - object will receive periodic time ticks events, D4D_FALSE - object won't receive the time ticks events>
* @return  none.
* @note    This function enables gettings the time tick events to object.
*******************************************************************************/
void D4D_EnableTimeTicks(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable)
{
  if(bEnable)
    pObj->pData->flags |= D4D_OBJECT_F_TIMETICK;
  else
    pObj->pData->flags &= ~D4D_OBJECT_F_TIMETICK;
}

/**************************************************************************/ /*!
* @brief   Function switch on capturing the keys to objects
* @param   pObj - pointer to the object. if the parameter is handled as NULL,
*                the function switch off the capturing the keys to object.
* @return  none.
* @note    This function sets the object to the capture keys state. In this state the object obtains all the
*               keys inputs including system navigation keys (escape, up, and down). In this state the object is using
*               capture colors from a color scheme. To switch off from this state the active screen has to be changed or
*               this function has to be called with the input parameter set to NULL.
*******************************************************************************/
void D4D_CaptureKeys(D4D_OBJECT_PTR  pObj)
{
    // NOTE: we need to send message, but we may just be in the middle of
    //       message processing (very likely). This may cause problem with
    //       the global d4d_msg object as we are changing it...
    //       We better use the temporary memory to instanitate the message

    D4D_MESSAGE* pMsg = (D4D_MESSAGE*) d4d_scratchPad;

    if(d4d_pKeysCapturer == pObj)
      return;

    pMsg->pScreen = D4D_GetActiveScreen();

    if(d4d_pKeysCapturer)
    {
        D4D_SetObjectFlags(d4d_pKeysCapturer, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

        pMsg->nMsgId = D4D_MSG_KILLCAPTURE;
        pMsg->pObject = d4d_pKeysCapturer;
        D4D_SendMessage(pMsg);

        d4d_pKeysCapturer = NULL;
    }

    if(pObj != NULL)
    {
      if((pObj->pData->flags & (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP)) == (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP))
      {
        if(D4D_IsEnabled((D4D_OBJECT*)pObj))
        {
          D4D_FocusSet(D4D_GetActiveScreen(), pObj);

          d4d_pKeysCapturer = (D4D_OBJECT*) pObj;

          D4D_SetObjectFlags(d4d_pKeysCapturer, D4D_OBJECT_F_REDRAWSTATE, D4D_FALSE);

          pMsg->nMsgId = D4D_MSG_SETCAPTURE;
          pMsg->pObject = d4d_pKeysCapturer;
          D4D_SendMessage(pMsg);
        }
      }
    }
}

/**************************************************************************/ /*!
* @brief   Function returns the current keys capturing object pointer
* @return  Pointer to object that is capturing keys or NULL if no object is capturing keys.
* @note    This function returns the pointer to the current object and sets it as a capture keys object. If
*       neither object is set as capture keys, the function returns NULL..
*******************************************************************************/
D4D_OBJECT* D4D_GetCapturedObject(void)
{
  return d4d_pKeysCapturer;
}

/**************************************************************************/ /*!
* @brief   Function enables TabStop on the object
* @param   pObj - pointer to the object
* @param   bEnable - <D4D_TRUE - tabStop enabled, D4D_FALSE - tabStop disabled>
* @return  none.
* @note    This function sets the tab stop property. When the tab stop is enabled, the object can be
*               focused. The initialization state is set by the parameter flags in the object declaration. This function is
*               targeted at using for run-time.
*******************************************************************************/
void D4D_EnableTabStop(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable)
{
  if(bEnable)
    pObj->pData->flags |= D4D_OBJECT_F_TABSTOP;
  else
    pObj->pData->flags &= ~D4D_OBJECT_F_TABSTOP;
}

/**************************************************************************/ /*!
* @brief   Function enables object
* @param   pObj - pointer to the object
* @param   bEnable - <D4D_TRUE - object enabled, D4D_FALSE - enabled disabled>
* @return  none.
* @note    This function sets the enabled property. When the object is enabled, it can accept all user
*       inputs and use normal colors. In the disable state the object is redrawn by grayscale colors and all user
*       inputs are ignored. The initialization state is set by parameter flags in the object declaration. This function
*       is targeted for using at run-time.
*******************************************************************************/
void D4D_EnableObject(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable)
{
  //if(((pObj->pData->flags & D4D_OBJECT_F_ENABLED) && !bEnable) || (!(pObj->pData->flags & D4D_OBJECT_F_ENABLED) && bEnable))
  if(D4D_LOG_EXOR((pObj->pData->flags & D4D_OBJECT_F_ENABLED), bEnable))
  {
    if(bEnable)
      pObj->pData->flags |= D4D_OBJECT_F_ENABLED;
    else
    {
      pObj->pData->flags &= ~D4D_OBJECT_F_ENABLED;

      if((D4D_OBJECT*)pObj == d4d_pKeysCapturer)
        D4D_CaptureKeys(NULL);

      if((D4D_OBJECT*)pObj == D4D_GetFocusedObject(pObj->pData->pScreen))
          D4D_FocusNextObject(pObj->pData->pScreen, D4D_FALSE);
    }

    D4D_InvalidateObject(pObj, D4D_TRUE);
  }
}


/**************************************************************************/ /*!
* @brief   Function find out if the object is enabled or not
* @param   pObject - pointer to the object
* @return  <D4D_TRUE - object is enabled, D4D_FALSE - object is disabled>.
* @note    This function gets the current status of object enabled / disabled. The function control the
*               object itself and also all parents objects..
*******************************************************************************/
D4D_BOOL D4D_IsEnabled(D4D_OBJECT* pObject)
{
  if(!(pObject->pData->flags & D4D_OBJECT_F_ENABLED))
    return D4D_FALSE;

  if(pObject->pRelations)
  {
    D4D_OBJECT* pParent = D4D_GetParentObject(pObject);
    if(pParent)
      return D4D_IsEnabled(pParent);
  }

  return D4D_TRUE;
}

/**************************************************************************/ /*!
* @brief   Function find out if the object is visible or not
* @param   pObject - pointer to the object
* @return  <D4D_TRUE - object is visible, D4D_FALSE - object is invisible>.
* @note    This function gets the current status of object visible / invisible. The function control the
*               object itself and also all parents objects..
*******************************************************************************/
D4D_BOOL D4D_IsVisible(D4D_OBJECT* pObject)
{
  if(!(pObject->pData->flags & D4D_OBJECT_F_VISIBLE))
    return D4D_FALSE;

  if(pObject->pRelations)
  {
    D4D_OBJECT* pParent = D4D_GetParentObject(pObject);
    if(pParent)
      return D4D_IsVisible(pParent);
  }

  return D4D_TRUE;
}


/**************************************************************************/ /*!
* @brief   Function find out if the object (including compounded objects) is focused
* @param   pObject - pointer to the object
* @return  <D4D_TRUE - object has focus, D4D_FALSE - object hasn't focus>.
* @note    This function gets the focused object and check if it belong this object or any other
*               object in compounded set.
*******************************************************************************/
D4D_BOOL D4D_IsMineFocus(D4D_OBJECT* pObject)
{
  D4D_OBJECT * pFocused;

  if(pObject == NULL)
    return D4D_FALSE;

  if((D4D_SCREEN*)(pObject->pData->pScreen) != D4D_GetActiveScreen())
    return D4D_FALSE;

  pFocused = (D4D_OBJECT*)D4D_GetFocusedObject((D4D_SCREEN*)(pObject->pData->pScreen));

  if(pFocused == pObject)
    return D4D_TRUE;

  if((pObject->pRelations) && (pFocused->pRelations))
  {
    // Lok for all parents - step by step
    while(pFocused = D4D_GetParentObject(pFocused))
    {
      if(pObject == pFocused)
        return D4D_TRUE;
    }
  }

  return D4D_FALSE;
}

/**************************************************************************/ /*!
* @brief   The function return the real client size of object
* @param   pObject - the pointer to object
* @return  client area object size
* @note    The function already handles also the compounded objects.
*******************************************************************************/
D4D_SIZE D4D_GetClientScreenSize(D4D_OBJECT* pObject)
{
  D4D_SIZE tmp_size = {0, 0};
  D4D_SCREEN* pScreen = pObject->pData->pScreen;


  if(pObject->pRelations)
  {
    D4D_OBJECT* pParent = (D4D_OBJECT*)pObject->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];

    if(pParent)
    {
      tmp_size = pParent->size;
      if(pParent->pMargin)
      {
        tmp_size.cx -= (D4D_COOR)(pParent->pMargin->left + pParent->pMargin->right);
        tmp_size.cy -= (D4D_COOR)(pParent->pMargin->top + pParent->pMargin->bottom);
      }
      return tmp_size;
    }
  }

  if(pScreen == NULL)
  {
    return tmp_size;
  }

  tmp_size.cx = pScreen->size.cx;
  tmp_size.cy = pScreen->size.cy;

  if(pScreen->flags & D4D_SCR_F_BEVEL)
  {
    tmp_size.cx -= 2 * D4D_BEVEL_WIDTH;
    tmp_size.cy -= 2 * D4D_BEVEL_WIDTH;
  }
  else if(pScreen->flags & D4D_SCR_F_OUTLINE)
  {
    tmp_size.cx -= 2;
    tmp_size.cy -= 2;
  }

  tmp_size.cy -= D4D_GetScrHeaderSize(pScreen);

  return tmp_size;
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_object_func                                              */
/******************************************************************************/

/******************************************************************************
*       Private functions                                                     *
******************************************************************************/

/**************************************************************//*!
*
* defualt message handler for all OBJECTS
*
******************************************************************/

void D4D_ObjOnMessage(D4D_MESSAGE* pMsg)
{
  if(pMsg->pObject)
  {
    if(pMsg->nMsgId == D4D_MSG_ONINIT)
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT; // just clear the non init flags for objects which doeasn't handle this message
  }

}

/*******************************************************
*
* The object default check coordinators  handler
*
*******************************************************/

D4D_BOOL D4D_ObjectCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  if((point.x >= pThis->position.x) && (point.y >= pThis->position.y))
  {
    if((point.x <= (pThis->position.x + pThis->size.cx)) && (point.y <= (pThis->position.y + pThis->size.cy)))
      return D4D_TRUE;
  }
#else
  D4D_UNUSED(pThis);
  D4D_UNUSED(point);
#endif
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Function returns TRUE if the object has children
*
******************************************************************/
D4D_BOOL D4D_HasObjectChildren(D4D_OBJECT* pObject)
{
  if(pObject == NULL)
    return D4D_FALSE;

  if(pObject->pRelations == NULL)
    return D4D_FALSE;

  if(!(pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX]))
    return D4D_FALSE;

  return D4D_TRUE;
}

/**************************************************************//*!
*
* Function returns the parent object
*
******************************************************************/
D4D_OBJECT* D4D_GetParentObject(D4D_OBJECT* pObject)
{
  if(pObject == NULL)
    return NULL;

  if(pObject->pRelations == NULL)
    return NULL;

  return (D4D_OBJECT*)pObject->pRelations[D4D_OBJECT_USR_DATA_PARENT_IX];
}

/**************************************************************//*!
*
* Function returns the next object defined in screen (top level)
*
******************************************************************/
D4D_OBJECT* D4D_FindNextObjectOnScreen(D4D_OBJECT* pObject)
{
  D4D_SCREEN* pScreen = pObject->pData->pScreen;
  const D4D_OBJECT* const* pObjects = pScreen->pObjects;
  LWord i = 0;
  while(pObjects[i])
  {
    if(pObjects[i] == pObject) {
      if(pObjects[i+1])
        return (D4D_OBJECT*)pObjects[i+1];
      else
        return (D4D_OBJECT*)pObjects[0];
    }
    i++;
  }
  // Error case
  return NULL;
}

/**************************************************************//*!
*
* Function returns the next parent sibling object
*
******************************************************************/
D4D_OBJECT* D4D_FindNextParentSiblingObject(D4D_OBJECT* pObject)
{
  D4D_OBJECT* pParent;
 // D4D_OBJECT* pSibling;

  pParent = D4D_GetParentObject(pObject);

  if(!pParent)
    return D4D_FindNextObjectOnScreen(pObject);

  // return pointer on parent sibling object or null if there is no parent next sibling
  return D4D_FindNextSiblingObject(pParent);
}

/**************************************************************//*!
*
* Function returns the next sibling object
*
******************************************************************/
D4D_OBJECT* D4D_FindNextSiblingObject(D4D_OBJECT* pObject)
{
  D4D_OBJECT* pParent;
  LWord i = D4D_OBJECT_USR_DATA_CHILD_IX;
  pParent = D4D_GetParentObject(pObject);

  if(!pParent)
    return D4D_FindNextObjectOnScreen(pObject);

  while(pParent->pRelations[i])
  {
    if(pParent->pRelations[i] == pObject)
    {
      if(pParent->pRelations[i+1])
        return (D4D_OBJECT*)pParent->pRelations[i+1];
      else
        return NULL;
    }
    i++;
  }

  return NULL;
}

/**************************************************************//*!
*
* Function returns the next object (looking for any object in object tree)
*
******************************************************************/
D4D_OBJECT* D4D_FindNextObject(D4D_OBJECT* pObject, D4D_BOOL childrenAlso)
{
  D4D_OBJECT* pLocObject;


  // Compatible mode - just only screen top level objects
  if(pObject->pRelations == NULL)
    return D4D_FindNextObjectOnScreen(pObject);

  if(childrenAlso)
    if(pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX])    // Does I have childrens?
      return (D4D_OBJECT*)pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];

  // Does I have siblings?
  pLocObject = D4D_FindNextSiblingObject(pObject);
  if(pLocObject)
    return pLocObject;

  // So my parent has to have siblings
  do
  {
    pLocObject = D4D_FindNextParentSiblingObject(pObject);
    if(pLocObject)
      return pLocObject;

    pObject = D4D_GetParentObject(pObject);

  }while(1);
}

/**************************************************************//*!
*
* Function returns the previous object defined in screen (top level)
*
******************************************************************/
D4D_OBJECT* D4D_FindPreviousObjectOnScreen(D4D_OBJECT* pObject)
{
  D4D_SCREEN* pScreen = pObject->pData->pScreen;
  const D4D_OBJECT* const* pObjects = pScreen->pObjects;
  LWord i = 0;

  while(pObjects[i])
  {
    if(pObjects[i] == pObject) {
      if(i)
        return (D4D_OBJECT*)pObjects[i-1];
      else
        return D4D_GetLastObject(pObject->pData->pScreen);
    }
    i++;
  }
  // Error case
  return NULL;
}

/**************************************************************//*!
*
* Function returns the previous parent sibling object
*
******************************************************************/
D4D_OBJECT* D4D_FindPreviousParentSiblingObject(D4D_OBJECT* pObject)
{
  D4D_OBJECT* pParent;
 // D4D_OBJECT* pSibling;

  pParent = D4D_GetParentObject(pObject);

  if(!pParent)
    return  D4D_FindPreviousObjectOnScreen(pObject);

  // return pointer on parent sibling object or null if there is no parent previous sibling
  return D4D_FindPreviousSiblingObject(pParent);
}

/**************************************************************//*!
*
* Function returns the next sibling object
*
******************************************************************/
D4D_OBJECT* D4D_FindPreviousSiblingObject(D4D_OBJECT* pObject)
{
  D4D_OBJECT* pParent;
  LWord i = D4D_OBJECT_USR_DATA_CHILD_IX;
  pParent = D4D_GetParentObject(pObject);

  if(!pParent)
    return D4D_FindPreviousObjectOnScreen(pObject);

  while(pParent->pRelations[i])
  {
    if(pParent->pRelations[i] == pObject)
    {
      if(i>D4D_OBJECT_USR_DATA_CHILD_IX)
        return (D4D_OBJECT*)pParent->pRelations[i-1];
      else
        return NULL;
    }
    i++;
  }

  return NULL;
}


/**************************************************************//*!
*
* Function returns the next object (looking for any object in object tree)
*
******************************************************************/
D4D_OBJECT* D4D_FindPreviousObject(D4D_OBJECT* pObject, D4D_BOOL childrenAlso)
{
  D4D_OBJECT* pLocObject;


  // Compatible mode - just only screen top level objects
  if(pObject->pRelations  == NULL)
  {
    pLocObject = D4D_FindPreviousObjectOnScreen(pObject);

    if(pLocObject->pRelations  == NULL)
      return pLocObject;

  }else
  {
    // find out the "latest child" of my previous sibling

    // Does I have siblings?
    pLocObject = D4D_FindPreviousSiblingObject(pObject);
  }

  if(childrenAlso)
  {
    while(pLocObject && pLocObject->pRelations)
    {
      D4D_OBJECT** pChild = (D4D_OBJECT**)&pLocObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX];
      //Check the children
      if(*pChild)
      {  while(*(pChild + 1)) //Find out the last child
        {
          pChild++;
        }
        pLocObject = *pChild;
      }else
        return pLocObject;
    }
  }else if(pLocObject)
    return pLocObject;

  pLocObject = D4D_GetParentObject(pObject);

  if(pLocObject)
    return pLocObject;

  return D4D_FindPreviousObjectOnScreen(pObject);
}


/**************************************************************//*!
*
* Function sets the flags of object and all it's childs
*
******************************************************************/
void D4D_SetObjectFlags(D4D_OBJECT* pObject, D4D_OBJECT_FLAGS flagsMask, D4D_BOOL alsoChildren)
{
  pObject->pData->flags |= flagsMask;

  if(alsoChildren && (pObject->pRelations))
  {
    D4D_OBJECT** pChild = (D4D_OBJECT**)&(pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX]);

    while(*pChild)
    {
      D4D_SetObjectFlags(*pChild, flagsMask, D4D_TRUE);
      pChild++;
    }
  }

  if(flagsMask & (D4D_OBJECT_F_REDRAW | D4D_OBJECT_F_REDRAWC | D4D_OBJECT_F_REDRAWSTATE))
  {
    if(pObject->pData->pScreen)
      (pObject->pData->pScreen)->pData->flags |= D4D_SCR_FINT_CHECKOBJECTS;
  }
}

/**************************************************************//*!
*
* Function sets the screen pointer on object and its children
*
******************************************************************/
void D4D_SetObjectScreenPointer(D4D_OBJECT* pObject, D4D_SCREEN* pScreen)
{
  pObject->pData->pScreen = pScreen;

  if(pObject->pRelations)
  {
    D4D_OBJECT** pChild = (D4D_OBJECT**)&(pObject->pRelations[D4D_OBJECT_USR_DATA_CHILD_IX]);

    while(*pChild)
    {
      D4D_SetObjectScreenPointer(*pChild, pScreen);
      pChild++;
    }
  }
}
