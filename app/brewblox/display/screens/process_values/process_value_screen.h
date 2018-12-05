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

#pragma once

#include "../screen.h"

#ifdef __cplusplus
extern "C" {
#endif

void
SetActuatorButtonState(const D4D_OBJECT* pThis, D4D_BOOL state, uint8_t idx);

D4D_EXTERN_SCREEN(screen_process_values);
D4D_EXTERN_OBJECT(scrProcessValues00);
D4D_EXTERN_OBJECT(scrProcessValues01);
D4D_EXTERN_OBJECT(scrProcessValues02);
D4D_EXTERN_OBJECT(scrProcessValues10);
D4D_EXTERN_OBJECT(scrProcessValues11);
D4D_EXTERN_OBJECT(scrProcessValues12);

D4D_EXTERN_OBJECT(scrProcessValues_wifi_state);
D4D_EXTERN_OBJECT(scrProcessValues_usb_state);

void
ScreenProcessValues_OnInit();
void
ScreenProcessValues_OnMain();
void
ScreenProcessValues_OnActivate();
void
ScreenProcessValues_OnDeactivate();
Byte
ScreenProcessValues_OnObjectMsg(D4D_MESSAGE* pMsg);

#ifdef __cplusplus
}
#endif
