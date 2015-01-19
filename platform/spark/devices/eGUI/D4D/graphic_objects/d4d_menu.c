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
* @file      d4d_menu.c
*
* @author     Petr Gargulak
*
* @version   0.0.51.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver menu object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
void D4D_MenuOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_MenuGetTextBuffer(D4D_OBJECT* pThis);

static void D4D_MenuOnInit(D4D_MESSAGE* pMsg);
static D4D_MENU_INDEX D4D_GetItemsCount(D4D_MENU* pMenu);

const D4D_OBJECT_SYS_FUNCTION d4d_menuSysFunc =
{
  D4D_DEFSTR("Menu"),
  D4D_MenuOnMessage,
  NULL,
  D4D_MenuGetTextBuffer
};

// temporary structure for menu temporary value calculation
typedef struct
{
    D4D_POINT position;
    D4D_GEOMETRY contentGeom;
    D4D_COOR titleBar_y;
    D4D_SIZE maxIcon;
    D4D_MENU_INDEX posCnt;
    D4D_COOR textOff;
    D4D_TCHAR index_txt[8];
    Byte index_txt_len;
    D4D_MENU_INDEX itemsCnt;
} D4D_MENU_TMP_VAL;

#define _calc (*((D4D_MENU_TMP_VAL*)d4d_scratchPad))




/*******************************************************
*
* MENU Helper computing routines
*
*******************************************************/

static D4D_SIZE D4D_GetIconsMaxSize(D4D_MENU *pMenu)
{
  Byte tmpB;
  D4D_SIZE tmpSize =  {0, 0};
  D4D_MENU_INDEX itemsCnt = D4D_GetItemsCount(pMenu);


  // compute max width of used Icons
  for(tmpB = 0; tmpB < itemsCnt; tmpB++)
  {
    if(pMenu->pItems[tmpB].pIcon != NULL)
    {
      if(D4D_GetBmpWidth(pMenu->pItems[tmpB].pIcon) > tmpSize.cx)
        tmpSize.cx = D4D_GetBmpWidth(pMenu->pItems[tmpB].pIcon);

      if(D4D_GetBmpHeight(pMenu->pItems[tmpB].pIcon) > tmpSize.cy)
        tmpSize.cy = D4D_GetBmpHeight(pMenu->pItems[tmpB].pIcon);
    }
  }
  return tmpSize;
}

static D4D_COOR D4D_GetTitleBarHeight(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);

  D4D_COOR bar_height = 0;

  if(pMenu->title_text.pText != NULL)
    bar_height = D4D_GetFontHeight(pMenu->title_text.fontId);

  if(pThis->initFlags & D4D_MENU_F_INDEX)
    if(D4D_GetFontHeight(pMenu->indexFontId) > bar_height)
      bar_height = D4D_GetFontHeight(pMenu->indexFontId);

  if(pMenu->pIcon != NULL)
    if(D4D_GetBmpHeight(pMenu->pIcon) > bar_height)
      bar_height = D4D_GetBmpHeight(pMenu->pIcon);

  if(bar_height)
  {
    bar_height += 3;

    #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
      bar_height += pThis->radius / 2;
    #endif
  }

  return bar_height;
}


static D4D_MENU_INDEX D4D_GetItemsCount(D4D_MENU* pMenu)
{
  D4D_MENU_INDEX i = 0;

  while(pMenu->pItems[i].pIcon || pMenu->pItems[i].text.pText)
  {
    i++;
  }

  return i;

}

static void D4D_MenuValue2Coor(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  _calc.position = D4D_GetClientToScreenPoint(pThis, &pThis->position);

  D4D_ComputeGeometry(&(_calc.contentGeom), pThis);

  _calc.titleBar_y = D4D_GetTitleBarHeight(pThis);
  _calc.maxIcon = D4D_GetIconsMaxSize(pMenu);
  _calc.textOff = pMenu->textOff;
  _calc.itemsCnt = D4D_GetItemsCount(pMenu);


  // Get the position count

   if(!pMenu->posCnt)
   {
    if(_calc.maxIcon.cy > (D4D_GetFontHeight(pMenu->itemsFontId)))
      _calc.posCnt = (D4D_MENU_INDEX)((_calc.contentGeom.sz.cy - D4D_GetTitleBarHeight(pThis)) / (_calc.maxIcon.cy + 3));
    else
      _calc.posCnt = (D4D_MENU_INDEX)((_calc.contentGeom.sz.cy - D4D_GetTitleBarHeight(pThis)) / (D4D_GetFontHeight(pMenu->itemsFontId) + 3));
   }
  // if count of items position or text offsets are not declared , then use automatic values
  if(!_calc.textOff)
  {
    if(_calc.posCnt > 1)
      _calc.textOff = (D4D_COOR)( (pThis->size.cy - _calc.titleBar_y) / (_calc.posCnt));
    else
      _calc.textOff = (D4D_COOR)((pThis->size.cy - _calc.titleBar_y) / 2);
  }

  if(pThis->initFlags & D4D_MENU_F_INDEX)
  {

    // Draw index counter
    _calc.index_txt_len = D4D_SprintDecU8((Byte)(pMenu->pData->ix + 1), _calc.index_txt, (D4D_TCHAR) 0);
    _calc.index_txt[_calc.index_txt_len++] = '/';
    _calc.index_txt_len += D4D_SprintDecU8((Byte)_calc.itemsCnt, &_calc.index_txt[_calc.index_txt_len], (D4D_TCHAR) 0);
    _calc.index_txt[_calc.index_txt_len] = 0;

  }


}

/*******************************************************
*
* MENU Drawing routine
*
*******************************************************/

static void D4D_MenuOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_str_prty;
  Byte tmpB;

  tmp_txtbuff.str_properties = &tmp_str_prty;
  tmp_txtbuff.printOff = 0;
  // Get background simple color
  clrT = D4D_ObjectGetForeFillColor(pThis);
  clrB = D4D_ObjectGetBckgFillColor(pThis);

  #ifdef D4D_DEBUG
    // sanity check
    D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
  #endif

  // Compute all tempoarary values
  D4D_MenuValue2Coor(pThis);

  // draw just a color rectangle instead of bitmap
  if(draw & D4D_OBJECT_DRAWFLAGS_COMPLETE)
  {
    D4D_FillRRect(&_calc.position, &pThis->size, clrB, pThis->radius);

    if(pMenu->pIcon != NULL)
    {
      tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + 2);
      tmp_point.y =  (D4D_COOR)(_calc.contentGeom.pnt.y + ((_calc.titleBar_y - D4D_GetBmpHeight(pMenu->pIcon)) / 2));

      D4D_DrawRBmp(&tmp_point, pMenu->pIcon, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
    }

    if(pMenu->title_text.pText)
    {
      tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + 4);

      #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      tmp_point.x += (D4D_COOR)(pThis->radius/2);
      #endif

      if(pMenu->pIcon != NULL)
        tmp_point.x += D4D_GetBmpHeight(pMenu->pIcon);

      tmp_point.y = _calc.contentGeom.pnt.y;

      tmp_size.cx = D4D_GetTextWidth(pMenu->title_text.fontId, pMenu->title_text.pText);
      tmp_size.cy = D4D_GetFontHeight(pMenu->title_text.fontId);

      D4D_DrawTextRect(&tmp_point, &tmp_size, &pMenu->title_text, clrT, clrB);
    }

    if(_calc.titleBar_y)
    {
      D4D_MoveToXY(_calc.contentGeom.pnt.x,\
        (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.titleBar_y));

      D4D_LineToXY((D4D_COOR)(_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cx - 1),\
        (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.titleBar_y), D4D_LINE_THIN, clrT);
    }
  }

  // Draw the frame
  if(draw & (D4D_OBJECT_DRAWFLAGS_COMPLETE | D4D_OBJECT_DRAWFLAGS_STATE))
    D4D_DrawFrame(pThis, clrT, clrB);

  if(pThis->initFlags & D4D_MENU_F_INDEX)
  {
    // Draw index counter
    tmp_txtbuff.printLen = (D4D_INDEX)((_calc.itemsCnt < 10)? 3:5);
    tmp_size.cx = (D4D_COOR)(tmp_txtbuff.printLen * D4D_GetFontWidth(pMenu->indexFontId) + 2);
    tmp_size.cy = (D4D_COOR)(_calc.titleBar_y - 2);
    tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cx - tmp_size.cx - 2);
    tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y);

    #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
      tmp_size.cy -= (D4D_COOR)(pThis->radius / 2);
      tmp_point.x -= (D4D_COOR)(pThis->radius / 2);
      tmp_point.y += (D4D_COOR)(pThis->radius / 2);
    #endif

    tmp_txtbuff.pText = _calc.index_txt;
    tmp_txtbuff.fontId = pMenu->indexFontId;
    tmp_str_prty.font_properties = D4D_MENU_IX_FNT_PRTY_DEFAULT;
    tmp_str_prty.text_properties = D4D_MENU_IX_TXT_PRTY_DEFAULT;
    D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);
  }

  for(tmpB = 0; (tmpB < _calc.posCnt) && (tmpB < _calc.itemsCnt); tmpB++)
  {
    if(((pMenu->pData->ix - pMenu->pData->page_ix) != tmpB) || (!D4D_IsEnabled(pThis)))
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

    // Draw menu item text
    if(pMenu->pItems[tmpB + pMenu->pData->page_ix].text.pText != NULL)
    {
      tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.maxIcon.cx + 4);
      tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.titleBar_y + 1 + (tmpB * _calc.textOff));

      tmp_size.cx = (D4D_COOR)(_calc.contentGeom.sz.cx - (_calc.maxIcon.cx + 6 + ((D4D_GET_MENU_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)? D4D_MENU_SCRLBR_WIDTH:0)));
      #if D4D_ROUND_CORNER_ENABLE != D4D_FALSE
        tmp_point.x += (D4D_COOR)(pThis->radius / 2);
        tmp_size.cx -= (D4D_COOR)(pThis->radius / 1);
      #endif
      tmp_size.cy = D4D_GetFontHeight(pMenu->itemsFontId); // to invoke autosize capability in axis Y

      tmp_txtbuff.pText = pMenu->pItems[tmpB + pMenu->pData->page_ix].text.pText;
      tmp_txtbuff.fontId = pMenu->itemsFontId;
      tmp_txtbuff.buffSize = 0;
      tmp_txtbuff.printLen = D4D_GetTextLength(tmp_txtbuff.pText);
      tmp_str_prty.font_properties = D4D_MENU_ITEM_FNT_PRTY_DEFAULT;
      tmp_str_prty.text_properties = D4D_MENU_ITEM_TXT_PRTY_DEFAULT;


      D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);
    }

    // Draw menu item icon
    if(_calc.maxIcon.cx)
    {
      tmp_point.x = (D4D_COOR)(_calc.contentGeom.pnt.x + 3);

      if(_calc.maxIcon.cy < _calc.textOff)
        tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff) + ((D4D_GetFontHeight(pMenu->itemsFontId) - _calc.maxIcon.cy) / 2));
      else
        tmp_point.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff));

      if(pMenu->pItems[tmpB + pMenu->pData->page_ix].pIcon != NULL)
        D4D_DrawRBmp(&tmp_point, pMenu->pItems[tmpB + pMenu->pData->page_ix].pIcon, (D4D_BOOL)!D4D_IsEnabled(pThis), pThis->radius);
      else
        D4D_FillRRect(&tmp_point, &_calc.maxIcon, D4D_ObjectGetBckgFillColor(pThis), pThis->radius);
    }
  }
}

/*******************************************************
*
* MENU object focus next item routine
*
*******************************************************/
static void D4D_MenuFocusNextItem(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);

  // Compute all tempoarary values
  D4D_MenuValue2Coor(pThis);
  pMenu->pData->ix++;

  if(_calc.itemsCnt >= _calc.posCnt)
  {
    if(pMenu->pData->ix >= _calc.itemsCnt)
    {
      pMenu->pData->ix = 0;
      pMenu->pData->page_ix = 0;
    }else if(pMenu->pData->ix - pMenu->pData->page_ix > (_calc.posCnt - 1))
    {
       pMenu->pData->page_ix++;
    }
    D4D_ScrlBrSetPosition(D4D_GET_MENU_SCROLL_BAR_HOR(pThis), pMenu->pData->page_ix);
  } else
  {
    if(pMenu->pData->ix >= _calc.itemsCnt)
    {
      pMenu->pData->ix = 0;
    }

  }
}

/*******************************************************
*
* MENU object focus previous item routine
*
*******************************************************/
static void D4D_MenuFocusPreviousItem(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);

  // Compute all tempoarary values
  D4D_MenuValue2Coor(pThis);

  pMenu->pData->ix--;

  if(_calc.itemsCnt >= _calc.posCnt)
  {
   if(pMenu->pData->ix < 0)
   {
     pMenu->pData->ix = (D4D_MENU_INDEX)(_calc.itemsCnt - 1);
     pMenu->pData->page_ix = (D4D_MENU_INDEX)(_calc.itemsCnt - _calc.posCnt);

   }else if(pMenu->pData->ix - pMenu->pData->page_ix < 0)
   {
      pMenu->pData->page_ix--;
   }

   D4D_ScrlBrSetPosition(D4D_GET_MENU_SCROLL_BAR_HOR(pThis), pMenu->pData->page_ix);
  } else
  {
    if(pMenu->pData->ix < 0)
    {
      pMenu->pData->ix = (D4D_MENU_INDEX)(_calc.itemsCnt - 1);
    }
  }
}


/*******************************************************
*
* MENU key handling routine
*
*******************************************************/

static void D4D_MenuOnKeyDown(D4D_MESSAGE* pMsg)
{
  if(D4D_GetCapturedObject() == pMsg->pObject)
  {
    if(pMsg->prm.key == D4D_KEY_SCANCODE_DOWN)
    {
      D4D_MenuFocusNextItem(pMsg->pObject);
      D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);

    } else if(pMsg->prm.key == D4D_KEY_SCANCODE_UP)
    {
      D4D_MenuFocusPreviousItem(pMsg->pObject);
      D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
    }
  }
}

/*******************************************************
*
* MENU key handling routine
*
*******************************************************/

static void D4D_MenuOnKeyUp(D4D_MESSAGE* pMsg)
{
    D4D_MENU* pMenu = D4D_GET_MENU(pMsg->pObject);

    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    {
      if(D4D_GetCapturedObject() != pMsg->pObject)
      {
        D4D_CaptureKeys(pMsg->pObject);
      }else
      {
        if(pMenu->OnClicked)
          pMenu->OnClicked(pMsg->pObject, pMenu->pData->ix);

        D4D_InvalidateObject(pMsg->pObject, D4D_FALSE);
      }
    }

    if(pMsg->prm.key == D4D_KEY_SCANCODE_ESC)
    {
      D4D_CaptureKeys(NULL);
    }
}

/*******************************************************
*
* MENU touch screen handling routine
*
*******************************************************/
#if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
static void D4D_MenuOnTouch(D4D_MESSAGE* pMsg, D4D_POINT* pPoint)
{
  D4D_OBJECT* pThis = pMsg->pObject;
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);

  if(pThis->pData->flags & D4D_OBJECT_F_TABSTOP)
  {
    if((D4D_GetFocusedObject(pMsg->pScreen) == pThis) || (pThis->pData->flags & D4D_OBJECT_F_FASTTOUCH))
    {
      D4D_FocusSet(pMsg->pScreen, pThis);

      D4D_MenuValue2Coor(pThis);

      if(pPoint->x < (pThis->position.x + pThis->size.cx - ((D4D_GET_MENU_SCROLL_BAR_HOR(pThis)->pData->flags & D4D_OBJECT_F_VISIBLE)? D4D_MENU_SCRLBR_WIDTH:0)))
      {
      // Select Menu Item
        D4D_COOR tmp_y;
        Byte tmpB;
        D4D_MENU_INDEX tmpItemsCnt = D4D_GetItemsCount(pMenu);

        tmp_y = (D4D_COOR)(pPoint->y - pThis->position.y);

	if(tmp_y <= _calc.titleBar_y)
        	return;

	tmp_y -= _calc.titleBar_y;

        // now is in tmp_y offset of y from title bar

        // Check the list
        for(tmpB = pMenu->pData->page_ix; tmpB < _calc.itemsCnt; tmpB++)
        {
          // Check end if list
          if((tmpB - pMenu->pData->page_ix) >= _calc.posCnt)
            break;

          if(tmp_y < _calc.textOff)
          {
            // founded touched line
            if(pMenu->pData->ix != tmpB)
            {
              // Touched line is not focused
              pMenu->pData->ix = (D4D_MENU_INDEX)tmpB;

            }else
            {
              if(pMenu->OnClicked)
                  pMenu->OnClicked(pThis, pMenu->pData->ix);
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

static D4D_BOOL D4D_MenuScrollBarSetup(D4D_OBJECT* pObject)
{
  // Try to check if the Menu needs scroll Bar
  D4D_MenuValue2Coor(pObject);

  // Switch off possible scroll bar in refresh case
  D4D_GET_MENU_SCROLL_BAR_HOR(pObject)->pData->flags &= ~D4D_OBJECT_F_VISIBLE;

  if((_calc.posCnt < _calc.itemsCnt) || (pObject->initFlags & D4D_MENU_F_SIDEBAR))
  {
    D4D_OBJECT* pScrlBr;
    pScrlBr = D4D_GET_MENU_SCROLL_BAR_HOR(pObject);

    pScrlBr->pData->flags |= D4D_OBJECT_F_VISIBLE;

    // Init child objects coordinations
    pScrlBr->position.x = (D4D_COOR)(_calc.contentGeom.pnt.x + _calc.contentGeom.sz.cx  - D4D_MENU_SCRLBR_WIDTH);
    pScrlBr->position.y = (D4D_COOR)(_calc.contentGeom.pnt.y + _calc.titleBar_y + 1);

    pScrlBr->position = D4D_GetScreenToClientPoint( pObject, &pScrlBr->position);

    pScrlBr->size.cx = D4D_MENU_SCRLBR_WIDTH;
    pScrlBr->size.cy = (D4D_COOR)(_calc.contentGeom.sz.cy - _calc.titleBar_y - 1);

    pScrlBr->radius = D4D_LimitU(pObject->radius, 0, D4D_MENU_SCRLBR_WIDTH / 2);

    if(_calc.itemsCnt < _calc.posCnt)
      D4D_ScrlBrSetRange(pScrlBr, 0, _calc.posCnt);
    else
      D4D_ScrlBrSetRange(pScrlBr, 0, _calc.itemsCnt);

    D4D_ScrlBrSetStep(pScrlBr, _calc.posCnt, D4D_MENU_SCRLBR_STEP);
    D4D_ScrlBrSetPosition(pScrlBr, 0);
    return D4D_TRUE;
  }
  return D4D_FALSE;
}

static void D4D_MenuOnInit(D4D_MESSAGE* pMsg)
{
  // init screen pointer of child objects
  D4D_GET_MENU_SCROLL_BAR_HOR(pMsg->pObject)->pData->pScreen = pMsg->pObject->pData->pScreen;

  (void)D4D_MenuScrollBarSetup(pMsg->pObject);

  // set flag to redraw screen
  D4D_InvalidateObject(pMsg->pObject, D4D_TRUE);
}

void D4D_MenuScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_MENU* pMenu = D4D_GET_MENU(D4D_GetParentObject(pThis));

  D4D_UNUSED(old_position);

  pMenu->pData->page_ix = (D4D_MENU_INDEX)new_position;
  pMenu->pData->ix = (D4D_MENU_INDEX)new_position;

  D4D_InvalidateObject(D4D_GetParentObject(pThis), D4D_FALSE);
}

/******************************************************************************
* Begin of D4D_MENU public functions
*//*! @addtogroup doxd4d_menu_func
* @{
*******************************************************************************/


/**************************************************************************/ /*!
* @brief   Function returns the current selected item index
* @param   pThis - pointer to the menu object
* @return  index on current selct item
* @note    none
*******************************************************************************/
D4D_MENU_INDEX D4D_MenuGetIndex(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);

  if(pThis == NULL)
    return 0;

  return pMenu->pData->ix;

}

/**************************************************************************/ /*!
* @brief   Function select new item by index
* @param   pThis - pointer to the menu object
* @param   ix - index of item that should be selected
* @return  none
* @note    none
*******************************************************************************/
void  D4D_MenuSetIndex(D4D_OBJECT* pThis, D4D_MENU_INDEX ix)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);

  if(pThis == NULL)
    return;

  if(ix > D4D_GetItemsCount(pMenu))
    ix = D4D_GetItemsCount(pMenu);

  pMenu->pData->ix = ix;
  pMenu->pData->page_ix = ix;

  D4D_ScrlBrSetPosition(D4D_GET_MENU_SCROLL_BAR_HOR(pThis), pMenu->pData->page_ix);

}

/**************************************************************************/ /*!
* @brief   Function gets the count of menu items
* @param   pThis - pointer to the menu object
* @return  count of list box items.
* @note    none
*******************************************************************************/
D4D_MENU_INDEX D4D_MenuGetItemCount(D4D_OBJECT_PTR pThis)
{
   D4D_MENU* pMenu = D4D_GET_MENU(pThis);

   return D4D_GetItemsCount(pMenu);
}

/**************************************************************************/ /*!
* @brief   Function finds the index of items with specified user data
* @param   pThis - pointer to the menu object
* @param   pUser - user data that is used to look for the item index (must match)
* @return  index of item if it's found, if not, it returns -1.
* @note    none
*******************************************************************************/
D4D_MENU_INDEX D4D_MenuFindUserDataItem(D4D_OBJECT_PTR pThis, void* pUser)
{
    D4D_MENU* pMenu = D4D_GET_MENU(pThis);
    D4D_MENU_INDEX i_max = D4D_GetItemsCount(pMenu);
    D4D_MENU_INDEX i;

    for(i=0;i< i_max;i++)
    {
      if(pMenu->pItems[i].pUser == pUser)
        return i;
    }

    return -1;
}

/**************************************************************************/ /*!
* @brief   Function gets the selected item user data
* @param   pThis - pointer to the menu object
* @return  user data of selected item.
* @note    none
*******************************************************************************/
void* D4D_MenuGetItemUserData(D4D_OBJECT_PTR pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  return pMenu->pItems[pMenu->pData->ix].pUser;
}

/**************************************************************************/ /*!
* @brief   Function gets the selected item text
* @param   pThis - pointer to the menu object
* @return  pointer to selected  item text
* @note    none
*******************************************************************************/
D4D_STRING* D4D_MenuGetItemText(D4D_OBJECT_PTR pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  return (D4D_STRING*)&(pMenu->pItems[pMenu->pData->ix].text);
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_menu_func                                            */
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
* The MENU message handler
*
*******************************************************/

void D4D_MenuOnMessage(D4D_MESSAGE* pMsg)
{
  #if defined(D4D_LLD_TCH) || defined(D4D_LLD_MOUSE)
  D4D_POINT touchClickPoint;
  #endif
  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_MenuOnDraw(pMsg);
      break;

    case D4D_MSG_KEYDOWN:
      D4D_MenuOnKeyDown(pMsg);
      break;

    case D4D_MSG_KEYUP:
      D4D_MenuOnKeyUp(pMsg);
      break;

#ifdef D4D_LLD_MOUSE
    case D4D_MSG_MOUSE_BTN_LEFT_UP:
        touchClickPoint = D4D_GetMouseCoordinates(pMsg->pObject);
        D4D_MenuOnTouch(pMsg, &touchClickPoint);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_UP:
      D4D_MenuFocusPreviousItem(pMsg->pObject);
      break;

    case D4D_MSG_MOUSE_BTN_WHEEL_DOWN:
      D4D_MenuFocusNextItem(pMsg->pObject);
      break;

#endif

#ifdef D4D_LLD_TCH
    case D4D_MSG_TOUCHED:
    case D4D_MSG_TOUCH_AUTO:
        touchClickPoint = D4D_GetTouchScreenCoordinates(pMsg->pObject);
        D4D_MenuOnTouch(pMsg, &touchClickPoint);
      break;
#endif
    case D4D_MSG_ONINIT:
      pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;

      D4D_MenuOnInit(pMsg);
      break;

    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_MenuGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_MENU(pThis)->title_text);
}
