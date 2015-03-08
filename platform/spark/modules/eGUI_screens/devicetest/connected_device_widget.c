
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


