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
* @file      d4d_combo_box.c
*
* @author     Petr Gargulak
*
* @version   0.0.6.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver comboBox object c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

//Internal API
static void D4D_ComboBoxOnMessage(D4D_MESSAGE* pMsg);
static void D4D_ComboBoxStornoSelection(D4D_OBJECT * pThis);

/*******************************************************
*
* Button arrow of
*
*******************************************************/
/* D4D array declaration */
const Byte arrow[];
Word palette_arrow[];
/* End of D4D array declaration - DO NOT MODIFY */

/* D4D bitmap definition */
D4D_DECLARE_BMP(bmp_arrow, arrow, palette_arrow)
/* End of D4D bitmap definition - DO NOT MODIFY */

/* Color palette */
Word palette_arrow[] = {
0xFFFF, 0x0000
};

/* Compression ratio: 50% */
const Byte arrow[] = {

/* Bitmap dimension (W x H) */
0x00, 0x0C, 0x00, 0x06,

/* Color mode */
0x80, 0x07,

/* Data */
0x00, 0x00, 0x0B, 0x01, 0x01, 0x08, 0x00, 0x00, 0x01, 0x01, 0x01, 0x06, 0x00, 0x00, 0x03, 0x01,
0x01, 0x04, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00,
0x03
};

const D4D_OBJECT_SYS_FUNCTION d4d_comboBoxSysFunc =
{
  D4D_DEFSTR("Combo Box"),
  D4D_ComboBoxOnMessage,
  NULL,
  NULL
};

const D4D_MARGIN comboBox_marginDefault =
{
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH,
  D4D_BEVEL_WIDTH
};

// temporary structure for comboBox temporary value calculation
typedef struct
{
    D4D_POINT position;
    D4D_SIZE  size;
    D4D_POINT contentPos;
    D4D_SIZE  contentSize;
    D4D_COOR textOff;
} D4D_COMBO_BOX_TMP_VAL;

#define _calc (*((D4D_COMBO_BOX_TMP_VAL*)d4d_scratchPad))

/*******************************************************
*
* COMBO_BOX Child Object CallBack function from list box
*
*******************************************************/
Byte D4D_ComboBoxListBoxMsgFeedBack(D4D_MESSAGE* pMsg)
{
  D4D_OBJECT *pParent = D4D_GetParentObject(pMsg->pObject);
  if((((pMsg->nMsgId == D4D_MSG_KILLFOCUS) || (pMsg->nMsgId == D4D_MSG_MOUSE_LEAVE)) && !D4D_IsMineFocus(pParent)))
  {
    D4D_ComboBoxStornoSelection(pParent);
  }

  return D4D_MSG_NOSKIP;
}

void D4D_ComboBoxListFeedBack(D4D_OBJECT* pThis, D4D_EVENTID eventId)
{
  D4D_OBJECT* pParent = D4D_GetParentObject(pThis);

  if(eventId == D4D_EVENT_ONCLICK)
  {
    D4D_OBJECT* pListBoxObject = D4D_GET_COMBO_BOX_LIST_BOX(pParent);
    D4D_OBJECT* pEditBoxObject = D4D_GET_COMBO_BOX_EDIT_BOX(pParent);

    D4D_GET_COMBO_BOX_DATA(pParent)->real_ix = D4D_ListBoxGetIndex(pListBoxObject);

    if(D4D_GET_COMBO_BOX(pParent)->OnEvent)
      D4D_GET_COMBO_BOX(pParent)->OnEvent(pParent, D4D_EVENT_ONCLICK);

    D4D_EditBoxClearAll(pEditBoxObject);
    D4D_EditBoxPutString(pEditBoxObject, D4D_ListBoxGetItemText(pListBoxObject));

    D4D_ShowObject(pListBoxObject, D4D_FALSE);
    D4D_InvalidateScreen(pParent->pData->pScreen, D4D_TRUE);
  }

  if(eventId == D4D_EVENT_ONCHANGE)
  {
    //D4D_InvalidateObject(pParent, D4D_FALSE);

    if(D4D_GET_COMBO_BOX(pParent)->OnEvent)
      D4D_GET_COMBO_BOX(pParent)->OnEvent(pParent, D4D_EVENT_ONCHANGE);
  }
}


/*******************************************************
*
* COMBO_BOX Child Object CallBack function from edit box
*
*******************************************************/
void D4D_ComboBoxEditFeedBack(D4D_OBJECT* pThis, D4D_EVENTID eventId)
{
  D4D_OBJECT* pParent = D4D_GetParentObject(pThis);

  D4D_GET_COMBO_BOX(pParent)->pData->real_ix = -1;

  if(eventId == D4D_EVENT_ONCHANGEDONE)
  {
    D4D_COMBO_BOX_INDEX i = D4D_ComboBoxGetItemCount(pParent);
    D4D_COMBO_BOX_INDEX j;

    for(j=0; j<i;j++)
    {
      if(D4D_CompareStringsUniversal(D4D_ComboBoxGetItemTextIx(pParent, j), D4D_EditBoxGetText(D4D_GET_COMBO_BOX_EDIT_BOX(pParent))) == 0)
      {
        //hit - the edit box text is same as in list of items
        D4D_GET_COMBO_BOX(pParent)->pData->real_ix = j;
        break;
      }
    }
  }
}


/*******************************************************
*
* COMBO_BOX Child Object CallBack function from button
*
*******************************************************/
Byte D4D_ComboBoxButtonMsgFeedBack(D4D_MESSAGE* pMsg)
{
  if(pMsg->nMsgId == D4D_MSG_DRAW && bmp_arrow.pParam)
  {
    D4D_COLOR color = D4D_ObjectGetBckgColor(pMsg->pObject, pMsg->prm.draw);

    ((Word*)(bmp_arrow.pParam))[0] = D4D_COLOR_RGB565(D4D_COLOR_GET_R(color), D4D_COLOR_GET_G(color), D4D_COLOR_GET_B(color));

    color = D4D_ObjectGetForeColor(pMsg->pObject, pMsg->prm.draw);

    ((Word*)(bmp_arrow.pParam))[1] = D4D_COLOR_RGB565(D4D_COLOR_GET_R(color), D4D_COLOR_GET_G(color), D4D_COLOR_GET_B(color));
  }

  return D4D_MSG_NOSKIP;
}

// Combo Box Button on click event call back

void D4D_ComboBoxButtonFeedBack(D4D_OBJECT* pThis)
{
  D4D_OBJECT* pParent = D4D_GetParentObject(pThis);
  D4D_OBJECT* pListBoxObject = D4D_GET_COMBO_BOX_LIST_BOX(pParent);

  if(D4D_IsVisible(pListBoxObject))
  {
    D4D_ComboBoxStornoSelection(pParent);
  }else
  {
    D4D_ShowObject(pListBoxObject, D4D_TRUE);
    D4D_CaptureKeys(pListBoxObject);
    D4D_InvalidateObject(pParent, D4D_TRUE);
  }
}

/*******************************************************
*
* The COMBO_BOX Get index function
*
*******************************************************/

D4D_COMBO_BOX_INDEX D4D_ComboBoxGetIndex(D4D_OBJECT* pThis)
{
  D4D_COMBO_BOX* pComboBox = D4D_GET_COMBO_BOX(pThis);

  if(pThis == NULL)
    return 0;

  return pComboBox->pData->real_ix;
}

/*******************************************************
*
* The COMBO_BOX Set index function
*
*******************************************************/

void D4D_ComboBoxSetIndex(D4D_OBJECT* pThis, D4D_COMBO_BOX_INDEX ix)
{
  D4D_COMBO_BOX* pComboBox = D4D_GET_COMBO_BOX(pThis);

  if(pThis == NULL)
    return;

  if(pComboBox->pData->real_ix == ix)
    return;

  if(ix != -1)
  {
    D4D_ListBoxSetIndex( D4D_GET_COMBO_BOX_LIST_BOX(pThis), ix);
    pComboBox->pData->real_ix = D4D_ListBoxGetIndex(D4D_GET_COMBO_BOX_LIST_BOX(pThis));
  }else
  {
    pComboBox->pData->real_ix = -1;

    if(pComboBox->OnEvent)
      pComboBox->OnEvent(pThis, D4D_EVENT_ONCHANGE);
  }


}


/**************************************************************//*!
*
*
*
*     THE INTERNAL API FUNCTIONS
*
*
*
******************************************************************/

static void D4D_ComboBoxStornoSelection(D4D_OBJECT * pThis)
{
  D4D_OBJECT* pListBoxObject = D4D_GET_COMBO_BOX_LIST_BOX(pThis);

  if(D4D_IsVisible(pListBoxObject))
  {
    D4D_ShowObject(pListBoxObject, D4D_FALSE);
    D4D_InvalidateScreen(pThis->pData->pScreen, D4D_TRUE);      //TODO
  }

  D4D_ListBoxSetIndex(pListBoxObject, D4D_GET_COMBO_BOX_DATA(pThis)->real_ix);

}

/*******************************************************
*
* The COMBO_BOX message handler
*
*******************************************************/

static void D4D_ComboBoxOnMessage(D4D_MESSAGE* pMsg)
{
  switch(pMsg->nMsgId)
  {
    case D4D_MSG_ONINIT:
      {
        D4D_OBJECT* pListBoxObject = D4D_GET_COMBO_BOX_LIST_BOX(pMsg->pObject);
        D4D_OBJECT* pEditBoxObject = D4D_GET_COMBO_BOX_EDIT_BOX(pMsg->pObject);

        pMsg->pObject->pData->flags &= ~D4D_OBJECT_F_NOTINIT;

        D4D_GET_COMBO_BOX_DATA(pMsg->pObject)->real_ix = D4D_ListBoxGetIndex(pListBoxObject);
        D4D_EditBoxClearAll(pEditBoxObject);
        D4D_EditBoxPutString(pEditBoxObject, D4D_ListBoxGetItemText(pListBoxObject));
      }

      break;

    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}
