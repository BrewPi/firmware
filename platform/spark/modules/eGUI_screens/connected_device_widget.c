
#include "d4d.h"
#include "connected_device_widget.h"

void D4D_CDV_SetValueText(const D4D_OBJECT* pObj, const char* text) 
{
    D4D_SetText(D4D_CDV_VALUE(pObj), (char*)text);
}

void D4D_CDV_SetConnectionText(const D4D_OBJECT* pObj, const char* text)
{
    D4D_SetText(D4D_CDV_CONNECTION(pObj), (char*)text);
}



#if 0
#include "d4d.h"
#include "common_files/d4d_private.h"
#include "connected_device_widget.h"

static void D4D_ConnectedDeviceOnMessage(D4D_MESSAGE* pMsg);
static void D4D_ConnectedDeviceOnDraw(D4D_MESSAGE* pMsg);

const D4D_OBJECT_SYS_FUNCTION d4d_connectedDeviceSysFunc =
{
  D4D_DEFSTR("ConnectedDevice"),
  D4D_ConnectedDeviceOnMessage,
  NULL,
  NULL
};


// temporary structure for comboBox temporary value calculation
typedef struct
{
    D4D_POINT position;
    D4D_SIZE  size;
    D4D_POINT contentPos;
    D4D_SIZE  contentSize;    
} D4D_CONNECTED_DEVICE_TMP_VAL;

#define _calc (*((D4D_CONNECTED_DEVICE_TMP_VAL*)d4d_scratchPad))


static void D4D_ConnectedDeviceOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pThis = pMsg->pObject;
    D4D_CONNECTED_DEVICE* pDevice = D4D_GET_CONNECTED_DEVICE(pThis);
    
}

static void D4D_ConnectedDeviceOnMessage(D4D_MESSAGE* pMsg) {
    switch (pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_ConnectedDeviceOnDraw(pMsg);
        break;

    default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
  }
}


#endif