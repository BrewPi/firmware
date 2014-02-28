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
* @file      d4d_list_box.c
*
* @author     Petr Gargulak
*
* @version   0.0.10.0
*
* @date      Oct-16-2013
*
* @brief     D4D driver listBox object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_ListBoxOnMessage(D4D_MESSAGE* pMsg);


static void D4D_ListBoxOnInit(D4D_OBJECT* pThis);
static D4D_LIST_BOX_INDEX D4D_GetItemsCount(D4D_LIST_BOX* pListBox);

const D4D_OBJECT_SYS_FUNCTION d4d_listBoxSysFunc =
{
  D4D_DEFSTR("List Box"),
  D4D_ListBoxOnMessage,
  NULL,
  NULL
};

const D4D_MARGIN listBox_marginDefault =
{
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH
};

// temporary structure for listBox temporary value calculation
typedef struct
{
    D4D_POINT position;
    D4D_POINT contentPos;
    D4D_SIZE  contentSize;
    D4D_LIST_BOX_INDEX posCnt;
    D4D_LIST_BOX_INDEX itemsCnt;
    D4D_COOR textOff;
} D4D_LIST_BOX_TMP_VAL;

#define _calc (*((D4D_LIST_BOX_TMP_VAL*)d4d_scratchPad))




/*******************************************************
*
* LIST_BOX Helper computing routines
*
*******************************************************/
static D4D_LIST_BOX_INDEX D4D_GetItemsCount(D4D_LIST_BOX* pListBox)
{
  D4D_LIST_BOX_INDEX i = 0;

  while(pListBox->pData->pItems[i].pText)
    i++;

  return i;
}

static D4D_LIST_BOX_INDEX D4D_GetPositionCount(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  D4D_LIST_BOX_INDEX index;
  D4D_COOR fontHeight;
  D4D_COOR sizey;

  if(pListBox->posCnt)
    return pListBox->posCnt;

  sizey = pThis->size.cy;


  if(pThis->pMargin)
  {
    sizey -= (D4D_COOR)(pThis->pMargin->top + pThis->pMargin->bottom);
  }

  fontHeight = (D4D_COOR)(D4D_GetFontHeight(pListBox->itemsFontId) + 2);

  index = (D4D_LIST_BOX_INDEX)(sizey / fontHeight);


  if(sizey % fontHeight)
    index--;

  return index;
}


static void D4D_ListBoxValue2Coor(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);
  _calc.contentPos = _calc.position;
  _calc.contentSize = pThis->size;
  _calc.posCnt = D4D_GetPositionCount(pThis);
  _calc.itemsCnt = D4D_GetItemsCount(pListBox);
  _calc.textOff = pListBox->textOff;

  if(pThis->pMargin)
  {
    _calc.contentPos.x += pThis->pMargin->left;
    _calc.contentPos.y += pThis->pMargin->top;
    _calc.contentSize.cx -= (pThis->pMargin->left + pThis->pMargin->right);
    _calc.contentSize.cy -= (pThis->pMargin->top + pThis->pMargin->bottom);

  }else
  {
    if(pThis->initFlags & D4D_OBJECT_F_FOCUSRECT)
    {
      _calc.contentPos.x++;
      _calc.contentPos.y++;
      _calc.contentSize.cx -= 2;
      _calc.contentSize.cy -= 2;
    }
  }

  if(D4D_IsVisible(D4D_GET_LIST_BOX_SCROLL_BAR(pThis)))
    _calc.contentSize.cx -= D4D_LIST_BOX_SCRLBR_WIDTH;

  // if count of items position or text offsets are not declared , then use automatic values
  if(!_calc.textOff)
  {
    if(_calc.posCnt > 1)
      _calc.textOff = (D4D_COOR)( _calc.contentSize.cy / (_calc.posCnt));
    else
      _calc.textOff = (D4D_COOR)(_calc.contentSize.cy / 2);
  }
}

/*******************************************************
*
* LIST_BOX Drawing routine
*
*******************************************************/

static void D4D_ListBoxOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_POINT tmp_point;
    D4D_SIZE tmp_size;
    D4D_STRING tmp_txtbuff;
    D4D_STR_PROPERTIES tmp_str_prty;
    Byte tmpB;

    // Prepare the D4D-STRING structure to print items texts
    tmp_txtbuff.str_properties = &tmp_str_prty;
    tmp_txtbuff.printOff = 0;

    // Get background simple color
    clrT = D4D_ObjectGetForeColor(pThis, draw);
    clrB = D4D_ObjectGetBckgFillColor(pThis);

    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif

    // Compute all tempoarary values
    D4D_ListBoxValue2Coor(pThis);


    // draw just a color rectangle instead of bitmap
    if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
    {
      // Draw the background
      #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      if(pThis->radius)
        D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);
      else
      #endif
        D4D_FillRect(&_calc.position, &pThis->size, clrB);
    }

    // Draw the frame
    if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
      D4D_DrawFrame(pThis, clrT, clrB);

    // Prepare all "constant" variables to draw list
    tmp_point   = _calc.contentPos;
    tmp_size.cx = _calc.contentSize.cx;
    tmp_size.cy = _calc.textOff;

    tmp_txtbuff.fontId = pListBox->itemsFontId;
    tmp_txtbuff.buffSize = 0;
    tmp_str_prty.font_properties = D4D_LIST_BOX_ITEM_FNT_PRTY_DEFAULT;
    tmp_str_prty.text_properties = D4D_LIST_BOX_ITEM_TXT_PRTY_DEFAULT;


    // Draw the list
    for(tmpB = pListBox->pData->page_ix; tmpB < _calc.itemsCnt; tmpB++)
    {
      // Check end if list
      if((tmpB - pListBox->pData->page_ix) >= _calc.posCnt)
        break;

      if((pListBox->pData->ix != tmpB) || (!D4D_IsEnabled(pThis)))
      {
        clrT = D4D_ObjectGetForeFillColor(pThis);
        clrB = D4D_ObjectGetBckgFillColor(pThis);
      }
      else
      {
        D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pThis);
        clrT = pScheme_tmp->foreFocus;
        clrB = pScheme_tmp->bckgFocus;
      }

      // Draw listBox item text
      if(pListBox->pData->pItems[tmpB].pText != NULL)
      {
        tmp_txtbuff.pText = pListBox->pData->pItems[tmpB].pText;
        tmp_txtbuff.printLen = D4D_GetTextLength(tmp_txtbuff.pText);

        #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
        if(pThis->radius)
          D4D_DrawTextRRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB, pThis->radius);
        else
        #endif
          D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);

        tmp_point.y += _calc.textOff;
      }
    }


}

/*******************************************************
*
* LIST_BOX object focus next item routine
*
*******************************************************/
static void D4D_ListBoxFocusNextItem(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  D4D_LIST_BOX_INDEX tmpPosCnt = D4D_GetPositionCount(pThis);
  D4D_LIST_BOX_INDEX tmpItemsCnt = D4D_GetItemsCount(pListBox);

  pListBox->pData->ix++;

  if(pListBox->pData->ix >= tmpItemsCnt)
    pListBox->pData->ix = 0;

  if(pListBox->OnEvent)
    pListBox->OnEvent(pThis, D4D_EVENT_ONCHANGE);

  D4D_ListBoxEnsureVisible(pThis);

  D4D_InvalidateObject(pThis, D4D_FALSE);
}

/*******************************************************
*
* LIST_BOX object focus previous item routine
*
*******************************************************/
static void D4D_ListBoxFocusPreviousItem(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  D4D_LIST_BOX_INDEX tmpPosCnt = D4D_GetPositionCount(pThis);
  D4D_LIST_BOX_INDEX tmpItemsCnt = D4D_GetItemsCount(pListBox);

  pListBox->pData->ix--;

  if(pListBox->pData->ix < 0)
    pListBox->pData->ix = (D4D_LIST_BOX_INDEX)(tmpItemsCnt - 1);

  if(pListBox->OnEvent)
    pListBox->OnEvent(pThis, D4D_EVENT_ONCHANGE);

  D4D_ListBoxEnsureVisible(pThis);

  D4D_InvalidateObject(pThis, D4D_FALSE);
}

/*******************************************************
*
* LIST_BOX key handling routine
*
*******************************************************/

//static void D4D_ListBoxOnKeyDown(D4D_MESSAGE* pMsg)
//{

//}

/*******************************************************
*
* LIST_BOX key handling routine
*
*******************************************************/

static void D4D_ListBoxOnKeyUp(D4D_MESSAGE* pMsg)
{
  if(D4D_GetCapturedObject() == pMsg->pObject)
  {
    D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pMsg->pObject);

    switch(pMsg->prm.key)
    {
      case D4D_KEY_SCANCODE_DOWN:
        D4D_ListBoxFocusNextItem(pMsg->pObject);
      break;

      case D4D_KEY_SCANCODE_UP:
        D4D_ListBoxFocusPreviousItem(pMsg->pObject);
      break;

      case D4D_KEY_SCANCODE_ENTER:
        if(pListBox->OnEvent)
          pListBox->OnEvent(pMsg->pObject, D4D_EVENT_ONCLICK);

      case D4D_KEY_SCANCODE_ESC:
        D4D_CaptureKeys(NULL);
      break;
    }
  }else
    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    D4D_CaptureKeys(pMsg->pObject);


}

/*******************************************************
*
* LIST_BOX touch screen handling routine
*
*******************************************************/
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
static void D4D_ListBoxOnTouch(D4D_MESSAGE* pMsg, D4D_POINT* pPoint)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

  if(pThis->pData->flags & D4D_OBJECT_F_TABSTOP)
  {
    if((D4D_GetFocusedObject(pMsg->pScreen) == pThis) || (pThis->pData->flags & D4D_OBJECT_F_FASTTOUCH))
    {
      D4D_FocusSet(pMsg->pScreen, pThis);

      D4D_ListBoxValue2Coor(pThis);

      if(pPoint->x < (pThis->position.x + pThis->size.cx - ((pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX]->pData->flags & D4D_OBJECT_F_VISIBLE)? D4D_LIST_BOX_SCRLBR_WIDTH:0)))
      {
        // Select ListBox Item
        D4D_COOR tmp_y;
        Byte tmpB;
        D4D_LIST_BOX_INDEX tmpItemsCnt = D4D_GetItemsCount(pListBox);

        tmp_y = (D4D_COOR)(pPoint->y - pThis->position.y);

        // now is in tmp_y offset of y from title bar

        // Check the list
        for(tmpB = pListBox->pData->page_ix; tmpB < _calc.itemsCnt; tmpB++)
        {
          // Check end if list
          if((tmpB - pListBox->pData->page_ix) >= _calc.posCnt)
            break;

          if(tmp_y < _calc.textOff)
          {
            // founded touched line
            if(pListBox->pData->ix != tmpB)
            {
              // Touched line is not focused
              pListBox->pData->ix = (D4D_LIST_BOX_INDEX)tmpB;

              if(pListBox->OnEvent)
                pListBox->OnEvent(pThis, D4D_EVENT_ONCHANGE);
            }else
            {
              if(pListBox->OnEvent)
                pListBox->OnEvent(pThis, D4D_EVENT_ONCLICK);
            }
            break;
          }
          tmp_y -= _calc.textOff;
        }
        D4D_InvalidateObject(pThis, D4D_FALSE);
      }
    }else
    {
      D4D_FocusSet(pMsg->pScreen, pThis);
    }

  }
}
#endif

static void D4D_ListBoxOnInit(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  D4D_LIST_BOX_INDEX tmpItemsCnt = D4D_GetItemsCount(pListBox);
  D4D_BOOL showScrollBar;
  // Try to check if the ListBox needs scroll Bar
  D4D_ListBoxValue2Coor(pThis);

  pListBox->pData->ix = 0;
  pListBox->pData->page_ix = 0;

  showScrollBar = ((D4D_GetPositionCount(pThis) < tmpItemsCnt) || (pThis->initFlags & D4D_LIST_BOX_F_SIDEBAR));

  D4D_ShowObject(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], showScrollBar);

  if(showScrollBar)
  {
    if(tmpItemsCnt < D4D_GetPositionCount(pThis))
      D4D_ScrlBrSetRange(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], 0, D4D_GetPositionCount(pThis));
    else
      D4D_ScrlBrSetRange(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], 0, tmpItemsCnt);

    D4D_ScrlBrSetStep(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], D4D_GetPositionCount(pThis), D4D_LIST_BOX_SCRLBR_STEP);
    D4D_ScrlBrSetPosition(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], 0);
  }

  if(pListBox->OnEvent)
    pListBox->OnEvent(pThis, D4D_EVENT_ONRELOAD);

  // set flag to redraw screen
  D4D_InvalidateObject(pThis, D4D_TRUE);
}


void D4D_ListBoxScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX((D4D_OBJECT*)(pThis->userData));

  D4D_UNUSED(old_position);

  pListBox->pData->page_ix = (D4D_LIST_BOX_INDEX)new_position;

  D4D_InvalidateObject((D4D_OBJECT*)(pThis->userData), D4D_FALSE);
}
/******************************************************************************
* Begin of D4D_COMBO_BOX public functions
*//*! @addtogroup doxd4d_list_box_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function ensure that the selected item in list will be scrolled to visible area
* @param   pThis - pointer to the list box object
* @return  none
* @note    none
*******************************************************************************/
void D4D_ListBoxEnsureVisible(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  D4D_LIST_BOX_INDEX tmpPosCnt = D4D_GetPositionCount(pThis);
  D4D_LIST_BOX_INDEX tmpItemsCnt = D4D_GetItemsCount(pListBox);

  // Check if the selected item is already visible
  if((pListBox->pData->ix >= pListBox->pData->page_ix) && (pListBox->pData->ix < (pListBox->pData->page_ix + tmpPosCnt)))
    return;

  if(pListBox->pData->ix < pListBox->pData->page_ix)
    D4D_ScrlBrSetPosition(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], pListBox->pData->ix);
  else
    D4D_ScrlBrSetPosition(pThis->pRelations[D4D_LIST_BOX_CHILD_SCROLL_BAR_IX], (D4D_INDEX)(pListBox->pData->ix - tmpPosCnt + 1));
}

/**************************************************************************/ /*!
* @brief   Function returns the current selected item index
* @param   pThis - pointer to the list box object
* @return  index on current selct item
* @note    none
*******************************************************************************/
D4D_LIST_BOX_INDEX D4D_ListBoxGetIndex(D4D_OBJECT* pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

  if(pThis == NULL)
    return 0;

  return pListBox->pData->ix;
}

/**************************************************************************/ /*!
* @brief   Function select new item by index
* @param   pThis - pointer to the list box object
* @param   ix - index of item that should be selected
* @return  none
* @note    none
*******************************************************************************/
void  D4D_ListBoxSetIndex(D4D_OBJECT* pThis, D4D_LIST_BOX_INDEX ix)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

  if(pThis == NULL)
    return;

  if(ix > D4D_GetItemsCount(pListBox))
    ix = D4D_GetItemsCount(pListBox);

  if(pListBox->pData->ix == ix)
    return;

  pListBox->pData->ix = ix;

  D4D_InvalidateObject(pThis, D4D_FALSE);

  if(pListBox->OnEvent)
    pListBox->OnEvent(pThis, D4D_EVENT_ONCHANGE);
}

/**************************************************************************/ /*!
* @brief   Function gets the count of list box items
* @param   pThis - pointer to the list box object
* @return  count of list box items.
* @note    none
*******************************************************************************/
D4D_LIST_BOX_INDEX D4D_ListBoxGetItemCount(D4D_OBJECT_PTR pThis)
{
   D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

   return D4D_GetItemsCount(pListBox);
}

/**************************************************************************/ /*!
* @brief   Function finds the index of items with specified user data
* @param   pThis - pointer to the list box object
* @param   pUser - user data that is used to look for the item index (must match)
* @return  index of item if it's found, if not, it returns -1.
* @note    none
*******************************************************************************/
D4D_LIST_BOX_INDEX D4D_ListBoxFindUserDataItem(D4D_OBJECT_PTR pThis, void* pUser)
{
    D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
    D4D_LIST_BOX_INDEX i_max = D4D_GetItemsCount(pListBox);
    D4D_LIST_BOX_INDEX i;

    for(i=0;i< i_max;i++)
    {
      if(pListBox->pData->pItems[i].pUser == pUser)
        return i;
    }

    return -1;
}

/**************************************************************************/ /*!
* @brief   Function gets the selected item user data
* @param   pThis - pointer to the list box object
* @return  user data of selected item.
* @note    none
*******************************************************************************/
void* D4D_ListBoxGetItemUserData(D4D_OBJECT_PTR pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  return pListBox->pData->pItems[pListBox->pData->ix].pUser;
}

/**************************************************************************/ /*!
* @brief   Function gets the selected item text
* @param   pThis - pointer to the list box object
* @return  pointer to selected  item text
* @note    none
*******************************************************************************/
D4D_TCHAR* D4D_ListBoxGetItemText(D4D_OBJECT_PTR pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);
  return D4D_GetInternalStringPointer(pListBox->pData->pItems[pListBox->pData->ix].pText);
}

/**************************************************************************/ /*!
* @brief   Function gets the text of item choosed by item index
* @param   pThis - pointer to the list box object
* @param   ix - index to item array
* @return  pointer to item text
* @note    It returns none if the input index is out of list box range.
*******************************************************************************/
D4D_TCHAR* D4D_ListBoxGetItemTextIx(D4D_OBJECT_PTR pThis, D4D_LIST_BOX_INDEX ix)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

  if(ix < 0 || ix >= D4D_GetItemsCount(pListBox))
    return NULL;

  return D4D_GetInternalStringPointer(pListBox->pData->pItems[ix].pText);
}

/**************************************************************************/ /*!
* @brief   Function change the items list using by list box
* @param   pThis - pointer to the list box object
* @param   pItems - pointer to the new item list
* @return  none
* @note    none
*******************************************************************************/
void D4D_ListBoxChangeItemList(D4D_OBJECT_PTR pThis, const D4D_LIST_BOX_ITEM* pItems)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

  if(pThis == NULL || pItems == NULL)
    return;

  pListBox->pData->pItems = pItems;

  D4D_ListBoxOnInit((D4D_OBJECT*)pThis);
}

/**************************************************************************/ /*!
* @brief   Function gets the current using item list
* @param   pThis - pointer to the list box object
* @return  pointer to current using item list
* @note    none
*******************************************************************************/
D4D_LIST_BOX_ITEM* D4D_ListBoxGetItemList(D4D_OBJECT_PTR pThis)
{
  D4D_LIST_BOX* pListBox = D4D_GET_LIST_BOX(pThis);

  if(pThis == NULL)
    return NULL;

  return (D4D_LIST_BOX_ITEM*)pListBox->pData->pItems;
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_list_box_func                                            */
/******************************************************************************/
/**************************************************************//*!
*
*
*
*     THE INTERNAL API FUNCTIONS
*
*
*
******************************************************************/

/*******************************************************
*
* The LIST_BOX message handler
*
*******************************************************/

void D4D_ListBoxOnMessage(D4D_MESSAGE* pMsg)
{
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  D4D_POINT touchClickPoint;
#endif

  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_ListBoxOnDraw(pMsg);
      break;

  //  case D4D_MSG_KEYDOWN:
 //     D4D_ListBoxOnKeyDown(pMsg);
 //     break;

    case D4D_MSG_KEYUP:
      D4D_ListBoxOnKeyUp(pMsg);
      break;
#ifdef D4D_LLD_MOUSE
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
        touchClickPoint = D4D_GetMouseCoordinates(pMsg->pObject);
        D4D_ListBoxOnTouch(pMsg, &touchClickPoint);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_UP:
      D4D_ListBoxFocusPreviousItem(pMsg->pObject);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_DOWN:
      D4D_ListBoxFocusNextItem(pMsg->pObject);
      break;
#endif

#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
        touchClickPoint = D4D_GetTouchScreenCoordinates(pMsg->pObject);
        D4D_ListBoxOnTouch(pMsg, &touchClickPoint);
      break;
#endif
    case D4D_MSG_ONINIT:
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;
      D4D_ListBoxOnInit(pMsg->pObject);
      break;

    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}
