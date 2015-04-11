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

#ifndef CONTROLLER_SCREEN_H
#define	CONTROLLER_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "d4d.h"
#include "../brewpi_colors.h"

void ScrController_OnInit();
void ScrController_OnMain();
void ScrController_OnActivate();
void ScrController_OnDeactivate();
Byte ScrController_OnObjectMsg(D4D_MESSAGE* pMsg);

#define TEMP_FORMAT_F_TEXT "\x7f""F"
#define TEMP_FORMAT_C_TEXT "\x7f""C"

D4D_EXTERN_OBJECT(scrController_lbl_name);    
D4D_EXTERN_OBJECT(scrController_lbl_tempunit);
D4D_EXTERN_OBJECT(scrController_lbl_setpoint);
D4D_EXTERN_OBJECT(scrController_lbl_mode);

D4D_EXTERN_OBJECT(scrController_name);

D4D_EXTERN_OBJECT(scrController_beertemp);
D4D_EXTERN_OBJECT(scrController_beersv);
D4D_EXTERN_OBJECT(scrController_beer);

D4D_EXTERN_OBJECT(scrController_fridgetemp);
D4D_EXTERN_OBJECT(scrController_fridgesv);
D4D_EXTERN_OBJECT(scrController_fridge);

D4D_EXTERN_OBJECT(scrController_roomtemp);
D4D_EXTERN_OBJECT(scrController_roomsv);
D4D_EXTERN_OBJECT(scrController_room);

D4D_EXTERN_OBJECT(scrController_mode);
D4D_EXTERN_OBJECT(scrController_state);
D4D_EXTERN_OBJECT(scrController_time);

#define MAX_TEMP_LEN (7)
#define MAX_NAME_LEN (20)
#define MAX_MODE_LEN (10)
#define MAX_STATE_LEN (20)
#define MAX_TIME_LEN (11)       // 4 hour digits : 2 min digits : 2 second digits

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLLER_SCREEN_H */

