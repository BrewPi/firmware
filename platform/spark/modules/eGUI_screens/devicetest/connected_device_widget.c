/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */ 

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


