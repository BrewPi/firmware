
#ifndef CONTROLLER_SCREEN_H
#define	CONTROLLER_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "d4d.h"

void ControllerScreen_Update();    

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


#define HEATING_COLOR D4D_COLOR_RGB(0xd4, 0x00, 0x00)
#define COOLING_COLOR D4D_COLOR_RGB(0x00, 0x5b, 0xd4)
#define IDLE_COLOR D4D_COLOR_RGB(0x66, 0x66, 0x66)
#define WAITING_TO_HEAT_COLOR D4D_COLOR_RGB(0xd5, 0x80, 0x80)
#define WAITING_TO_COOL_COLOR D4D_COLOR_RGB(0x80, 0xc8, 0xff)
#define STATE_OFF_COLOR D4D_COLOR_RGB(0,0,0)
#define WAITING_FOR_PEAK_DETECT_COLOR IDLE_COLOR
#define DOOR_OPEN_COLOR D4D_COLOR_RGB(128,128,0)
#define COOLING_MIN_TIME_COLOR COOLING_COLOR
#define HEATING_MIN_TIME_COLOR HEATING_COLOR 

#define NAME_BG_COLOR D4D_COLOR_RGB(64, 64, 64)
#define BEER_BG_COLOR D4D_COLOR_RGB(0xd4, 0xaa, 0x00)
#define BEER_FG_COLOR D4D_COLOR_RGB(0xff, 0xf1, 0xb9)
#define FRIDGE_BG_COLOR D4D_COLOR_RGB(0x36, 0xa2, 0xc4)
#define FRIDGE_FG_COLOR D4D_COLOR_RGB(0xa9, 0xd9, 0xb7)
#define ROOM_BG_COLOR D4D_COLOR_RGB(142, 160, 156)
#define ROOM_FG_COLOR D4D_COLOR_RGB(0xe3, 0xe9, 0xe8)

#define REGULAR_TEXT_COLOR D4D_COLOR_RGB(255,255,255)
#define SECONDARY_TEXT_COLOR D4D_COLOR_RGB(192,192,192)
#define LOW_TEXT_COLOR D4D_COLOR_RGB(160,160,160)


#define MODE_OFF_COLOR D4D_COLOR_RGB(0,0,0)
#define MODE_TEST_COLOR D4D_COLOR_RGB(0,0,64)
#define MODE_BEER_COLOR BEER_BG_COLOR
#define MODE_FRIDGE_COLOR FRIDGE_BG_COLOR
#define MODE_PROFILE_COLOR BEER_BG_COLOR

#define MAX_TEMP_LEN (7)
#define MAX_NAME_LEN (20)
#define MAX_MODE_LEN (10)
#define MAX_STATE_LEN (20)
#define MAX_TIME_LEN (11)       // 4 hour digits : 2 min digits : 2 second digits

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLLER_SCREEN_H */

