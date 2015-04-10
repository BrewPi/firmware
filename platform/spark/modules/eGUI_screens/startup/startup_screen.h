/**
 ******************************************************************************
 * @file    startup_screen.h
 * @authors Matthew McGowan
 ******************************************************************************
  Copyright (c) 2015 Brewpi/Matthew McGowan.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */


#ifndef STARTUP_SCREEN_H
#define	STARTUP_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "d4d.h"

        
void ScrStartup_OnInit();
void ScrStartup_OnMain();
void ScrStartup_OnActivate();
void ScrStartup_OnDeactivate();
Byte ScrStartup_OnObjectMsg(D4D_MESSAGE* pMsg);

extern void calibrateTouchScreen();

D4D_EXTERN_OBJECT(scrStartup_version);
D4D_EXTERN_OBJECT(scrStartup_text);


#ifdef	__cplusplus
}
#endif

#endif	/* STARTUP_SCREEN_H */

