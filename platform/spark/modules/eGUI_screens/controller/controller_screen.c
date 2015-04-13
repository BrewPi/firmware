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
#include "../widget_color_scheme.h"
#include "../fonts.h"
#include "../brewpi-logo.h"
#include "controller_screen.h"


#define COL1_X (0)
#define COL1_CX (30)
#define COL_GAP (4)
#define COL1_GAP COL_GAP

#define UNIT_CX (85)

#define COL2_X (COL1_X+COL1_CX+COL1_GAP)
#define COL2_CX UNIT_CX
#define COL2_GAP COL_GAP

#define COL3_X (COL2_X+COL2_CX+COL2_GAP)
#define COL3_CX UNIT_CX
#define COL3_GAP COL_GAP

#define COL4_X (COL3_X+COL3_CX+COL3_GAP)
#define COL4_CX UNIT_CX
#define COL4_GAP COL_GAP

#define COL5_X (COL4_X+COL4_CX+COL4_GAP)

#define ROW_GAP (4)
#define ROW1_Y (0)
#define ROW1_CY (40)
#define ROW1_GAP ROW_GAP

#define ROW2_Y (ROW1_Y+ROW1_CY+ROW1_GAP)
#define ROW2_CY (20)
#define ROW2_GAP 0

#define ROW3_Y (ROW2_Y+ROW2_CY+ROW2_GAP)
#define ROW3_CY (50)
#define ROW3_GAP 0

#define ROW4_Y (ROW3_Y+ROW3_CY+ROW3_GAP)
#define ROW4_CY (30)
#define ROW4_GAP ROW_GAP

#define ROW5_Y (ROW4_Y+ROW4_CY+ROW4_GAP)
#define ROW5_CY (40)
#define ROW5_GAP ROW_GAP

#define ROW6_Y (ROW5_Y+ROW5_CY+ROW5_GAP)

#define LABEL_COLOR SECONDARY_TEXT_COLOR

#define ROWPV_Y ROW3_Y
#define ROWPV_CY ROW3_CY
#define ROWSV_Y ROW4_Y
#define ROWSV_CY ROW4_CY
#define ROWLAB_Y ROW2_Y
#define ROWLAB_CY ROW2_CY
#define ROWMODE_Y ROW5_Y
#define ROWMODE_CY ROW5_CY




char tempFormat[3];

// Column 1 - labels. 
//D4D_DECLARE_COLOR_LABEL(scrController_lbl_name, "Name", COL1_X, ROW1_Y, COL1_CX, ROW1_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_tempunit, tempFormat, COL1_X, ROWPV_Y, COL1_CX, ROWPV_CY, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_setpoint, "Set", COL1_X, ROWSV_Y, COL1_CX, ROWSV_CY, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_mode, "Mode", COL1_X, ROWMODE_Y, COL1_CX, ROWMODE_CY, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);

// main content
//char controller_name[MAX_NAME_LEN] = "";
char controller_beertemp[MAX_TEMP_LEN]="";
char controller_beerset[MAX_TEMP_LEN]="";
char controller_fridgetemp[MAX_TEMP_LEN]="";
char controller_fridgeset[MAX_TEMP_LEN]="";
char controller_roomtemp[MAX_TEMP_LEN]="";
char controller_roomset[MAX_TEMP_LEN]="";
char controller_mode[MAX_MODE_LEN]="";
char controller_state[MAX_STATE_LEN]="";
char controller_time[MAX_TIME_LEN]="";

//D4D_DECLARE_COLOR_LABEL(scrController_name, controller_name, COL2_X, ROW1_Y, COL5_X-COL4_GAP-COL2_X, ROW1_CY, FONT_REGULAR, D4D_CONST, NAME_BG_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_STD_PICTURE(scrController_logo, 138, 5, 45, 30, &bmp_brewpi_logo_black_45_30);

char beer_text[] = "Beer";
char fridge_text[] = "Fridge";
char room_text[] = "Room";
D4D_DECLARE_COLOR_LABEL(scrController_beertemp, controller_beertemp, COL2_X, ROWPV_Y, COL2_CX, ROWPV_CY, FONT_NUMBER_LARGE, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_beersv, controller_beerset, COL2_X, ROWSV_Y, COL2_CX, ROWSV_CY, FONT_NUMBER_MEDIUM, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_beer, beer_text, COL2_X, ROWLAB_Y, COL2_CX, ROWLAB_CY, FONT_SMALL, D4D_CONST, INITIAL_BLOCK_COLOR, BEER_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_fridgetemp, controller_fridgetemp, COL3_X, ROWPV_Y, COL3_CX, ROWPV_CY, FONT_NUMBER_LARGE, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_fridgesv, controller_fridgeset, COL3_X, ROWSV_Y, COL3_CX, ROWSV_CY, FONT_NUMBER_MEDIUM, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_fridge, fridge_text, COL3_X, ROWLAB_Y, COL3_CX, ROWLAB_CY, FONT_SMALL, D4D_CONST, INITIAL_BLOCK_COLOR, FRIDGE_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_roomtemp, controller_roomtemp, COL4_X, ROWPV_Y, COL4_CX, ROWPV_CY, FONT_NUMBER_LARGE, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_roomsv, controller_roomset, COL4_X, ROWSV_Y, COL4_CX, ROWSV_CY, FONT_NUMBER_MEDIUM, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_room, room_text, COL4_X, ROWLAB_Y, COL4_CX, ROWLAB_CY, FONT_SMALL, D4D_CONST, INITIAL_BLOCK_COLOR, ROOM_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_mode, controller_mode, COL2_X, ROWMODE_Y, COL2_CX, ROWMODE_CY, FONT_REGULAR, D4D_NO_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_state, controller_state, COL3_X, ROWMODE_Y, COL3_CX+(COL3_GAP>>1), ROWMODE_CY, FONT_REGULAR, D4D_NO_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_time, controller_time, COL4_X-(COL3_GAP>>1), ROWMODE_Y, COL4_CX+(COL3_GAP>>1), ROWMODE_CY, FONT_REGULAR, D4D_NO_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);


D4D_DECLARE_STD_SCREEN_BEGIN(screen_controller, ScrController_)
    //D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_name)    
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_tempunit)
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_setpoint)
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_mode)
        
    //D4D_DECLARE_SCREEN_OBJECT(scrController_name)

    D4D_DECLARE_SCREEN_OBJECT(scrController_logo)
        
    D4D_DECLARE_SCREEN_OBJECT(scrController_beertemp)
    D4D_DECLARE_SCREEN_OBJECT(scrController_beersv)
    D4D_DECLARE_SCREEN_OBJECT(scrController_beer)

    D4D_DECLARE_SCREEN_OBJECT(scrController_fridgetemp)
    D4D_DECLARE_SCREEN_OBJECT(scrController_fridgesv)
    D4D_DECLARE_SCREEN_OBJECT(scrController_fridge)

    D4D_DECLARE_SCREEN_OBJECT(scrController_roomtemp)
    D4D_DECLARE_SCREEN_OBJECT(scrController_roomsv)
    D4D_DECLARE_SCREEN_OBJECT(scrController_room)
    
    D4D_DECLARE_SCREEN_OBJECT(scrController_mode)
    D4D_DECLARE_SCREEN_OBJECT(scrController_state)
    D4D_DECLARE_SCREEN_OBJECT(scrController_time)

D4D_DECLARE_SCREEN_END()        
                
