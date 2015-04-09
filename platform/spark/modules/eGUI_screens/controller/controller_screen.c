/**
 ******************************************************************************
 * @file    controller_screen.c
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
#define ROW2_CY (60)
#define ROW2_GAP 0

#define ROW3_Y (ROW2_Y+ROW2_CY+ROW2_GAP)
#define ROW3_CY (20)
#define ROW3_GAP 0

#define ROW4_Y (ROW3_Y+ROW3_CY+ROW3_GAP)
#define ROW4_CY (20)
#define ROW4_GAP ROW_GAP

#define ROW5_Y (ROW4_Y+ROW4_CY+ROW4_GAP)
#define ROW5_CY (40)
#define ROW5_GAP ROW_GAP

#define ROW6_Y (ROW5_Y+ROW5_CY+ROW5_GAP)

#define SMALL_FONT FONT_PROGGY_SQUARE
#define REGULAR_FONT FONT_SEGOEUI11
#define LARGE_FONT FONT_PROGGY_SQUARE_BIG
#define LARGE_NUMBER_FONT FONT_ALTE_DIN_22
#define MEDIUM_NUMBER_FONT FONT_ALTE_DIN_16
#define MEDIUM_FONT FONT_ARIAL7_BIG

#define LABEL_COLOR SECONDARY_TEXT_COLOR

// Column 1 - labels. 
//D4D_DECLARE_COLOR_LABEL(scrController_lbl_name, "Name", COL1_X, ROW1_Y, COL1_CX, ROW1_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_tempunit, "\x7f""C", COL1_X, ROW2_Y, COL1_CX, ROW2_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_setpoint, "Set", COL1_X, ROW3_Y, COL1_CX, ROW3_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_mode, "Mode", COL1_X, ROW5_Y, COL1_CX, ROW5_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);

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

//D4D_DECLARE_COLOR_LABEL(scrController_name, controller_name, COL2_X, ROW1_Y, COL5_X-COL4_GAP-COL2_X, ROW1_CY, REGULAR_FONT, D4D_CONST, NAME_BG_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_STD_PICTURE(scrController_logo, 138, 5, 45, 30, &bmp_brewpi_logo_black_45_30);

D4D_DECLARE_COLOR_LABEL(scrController_beertemp, controller_beertemp, COL2_X, ROW2_Y, COL2_CX, ROW2_CY, LARGE_NUMBER_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_beersv, controller_beerset, COL2_X, ROW3_Y, COL2_CX, ROW3_CY, MEDIUM_NUMBER_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);

char beer_text[] = "Beer";
char fridge_text[] = "Fridge";
char room_text[] = "Room";
D4D_DECLARE_COLOR_LABEL(scrController_beer, beer_text, COL2_X, ROW4_Y, COL2_CX, ROW4_CY, SMALL_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, BEER_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_fridgetemp, controller_fridgetemp, COL3_X, ROW2_Y, COL3_CX, ROW2_CY, LARGE_NUMBER_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_fridgesv, controller_fridgeset, COL3_X, ROW3_Y, COL3_CX, ROW3_CY, MEDIUM_NUMBER_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_fridge, fridge_text, COL3_X, ROW4_Y, COL3_CX, ROW4_CY, SMALL_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, FRIDGE_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_roomtemp, controller_roomtemp, COL4_X, ROW2_Y, COL4_CX, ROW2_CY, LARGE_NUMBER_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_roomsv, controller_roomset, COL4_X, ROW3_Y, COL4_CX, ROW3_CY, MEDIUM_NUMBER_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_room, room_text, COL4_X, ROW4_Y, COL4_CX, ROW4_CY, SMALL_FONT, D4D_CONST, INITIAL_BLOCK_COLOR, ROOM_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_mode, controller_mode, COL2_X, ROW5_Y, COL2_CX, ROW5_CY, REGULAR_FONT, D4D_NO_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_state, controller_state, COL3_X, ROW5_Y, COL3_CX+(COL3_GAP>>1), ROW5_CY, REGULAR_FONT, D4D_NO_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_time, controller_time, COL4_X-(COL3_GAP>>1), ROW5_Y, COL4_CX+(COL3_GAP>>1), ROW5_CY, REGULAR_FONT, D4D_NO_CONST, INITIAL_BLOCK_COLOR, REGULAR_TEXT_COLOR);


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
        
#define clear_array(x) memset(x, 0, sizeof(x));
        
static void ScrController_OnInit()
{
}

static void ScrController_OnMain()
{
    ControllerScreen_Update();
}


static void ScrController_OnActivate()
{    
    *room_text = 0;
   *beer_text = 0;
   *fridge_text = 0;
}

static void ScrController_OnDeactivate()
{
}

static Byte ScrController_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    return 0;
}
