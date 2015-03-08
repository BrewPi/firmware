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

#define NAME_BG_COLOR D4D_COLOR_RGB(64, 64, 64)

#define BEER_BG_COLOR D4D_COLOR_RGB(0xd4, 0xaa, 0x00)
#define BEER_FG_COLOR D4D_COLOR_RGB(0xff, 0xf1, 0xb9)
#define FRIDGE_BG_COLOR D4D_COLOR_RGB(0x36, 0xa2, 0xc4)
#define FRIDGE_FG_COLOR D4D_COLOR_RGB(0xa9, 0xd9, 0xb7)
#define ROOM_BG_COLOR D4D_COLOR_RGB(142, 160, 156)
#define ROOM_FG_COLOR D4D_COLOR_RGB(0xe3, 0xe9, 0xe8)

#define HEATING_COLOR D4D_COLOR_RGB(0xd4, 0x00, 0x00)
#define COOLING_COLOR D4D_COLOR_RGB(0x00, 0x5b, 0xd4)
#define IDLE_COLOR D4D_COLOR_RGB(0x66, 0x66, 0x66)
#define WAITING_TO_HEAT_COLOR D4D_COLOR_RGB(0xd5, 0x80, 0x80)
#define WAITING_TO_COOL_COLOR D4D_COLOR_RGB(0x80, 0xc8, 0xff)
#define REGULAR_TEXT_COLOR D4D_COLOR_RGB(255,255,255)
#define SECONDARY_TEXT_COLOR D4D_COLOR_RGB(192,192,192)
#define LOW_TEXT_COLOR D4D_COLOR_RGB(160,160,160)

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
#define ROW2_CY (45)
#define ROW2_GAP 0

#define ROW3_Y (ROW2_Y+ROW2_CY+ROW2_GAP)
#define ROW3_CY (45)
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

/**
 * Declare a label with the given colors. 
 * The WIDGET_COLOR_SCHEME is availalbe as name_color.
 */
#define D4D_DECLARE_COLOR_LABEL(name, text, x, y, cx, cy, font, type, bg, fg) \
    type WIDGET_COLOR_SCHEME name##_color = { bg, bg, bg, bg, fg, fg, fg, fg }; \
    D4D_DECLARE_LABEL(name, text, x, y, cx, cy, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&name##_color), font, NULL, NULL);


#define LABEL_COLOR LOW_TEXT_COLOR
// Column 1 - labels. 
D4D_DECLARE_COLOR_LABEL(scrController_lbl_name, "Name", COL1_X, ROW1_Y, COL1_CX, ROW1_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_tempunit, "\x7f""C", COL1_X, ROW2_Y, COL1_CX, ROW2_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_setpoint, "Set", COL1_X, ROW3_Y, COL1_CX, ROW3_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_lbl_mode, "Mode", COL1_X, ROW5_Y, COL1_CX, ROW5_CY, SMALL_FONT, D4D_CONST, D4D_COLOR_BLACK, LABEL_COLOR);

// main content
D4D_DECLARE_COLOR_LABEL(scrController_name, "MUSKY_OLD_HORSE_BLANKET", COL2_X, ROW1_Y, COL5_X-COL4_GAP-COL2_X, ROW1_CY, REGULAR_FONT, D4D_CONST, NAME_BG_COLOR, REGULAR_TEXT_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_beertemp, "12.5", COL2_X, ROW2_Y, COL2_CX, ROW2_CY, LARGE_FONT, D4D_CONST, BEER_BG_COLOR, SECONDARY_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_beersv, "15.5", COL2_X, ROW3_Y, COL2_CX, ROW3_CY, LARGE_FONT, D4D_CONST, BEER_BG_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_beer, "Beer", COL2_X, ROW4_Y, COL2_CX, ROW4_CY, SMALL_FONT, D4D_CONST, BEER_BG_COLOR, BEER_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_fridgetemp, "12.5", COL3_X, ROW2_Y, COL3_CX, ROW2_CY, LARGE_FONT, D4D_CONST, FRIDGE_BG_COLOR, SECONDARY_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_fridgesv, "15.5", COL3_X, ROW3_Y, COL3_CX, ROW3_CY, LARGE_FONT, D4D_CONST, FRIDGE_BG_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_fridge, "Fridge", COL3_X, ROW4_Y, COL3_CX, ROW4_CY, SMALL_FONT, D4D_CONST, FRIDGE_BG_COLOR, FRIDGE_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_roomtemp, "12.5", COL4_X, ROW2_Y, COL4_CX, ROW2_CY, LARGE_FONT, D4D_CONST, ROOM_BG_COLOR, SECONDARY_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_roomsv, "", COL4_X, ROW3_Y, COL4_CX, ROW3_CY, LARGE_FONT, D4D_CONST, ROOM_BG_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_room, "Room", COL4_X, ROW4_Y, COL4_CX, ROW4_CY, SMALL_FONT, D4D_CONST, ROOM_BG_COLOR, ROOM_FG_COLOR);

D4D_DECLARE_COLOR_LABEL(scrController_mode, "BEER", COL2_X, ROW5_Y, COL2_CX, ROW5_CY, REGULAR_FONT, D4D_NO_CONST, BEER_BG_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_state, "HEATING", COL3_X, ROW5_Y, COL3_CX+(COL3_GAP>>1), ROW5_CY, REGULAR_FONT, D4D_NO_CONST, HEATING_COLOR, REGULAR_TEXT_COLOR);
D4D_DECLARE_COLOR_LABEL(scrController_time, "2:31:56", COL4_X-(COL3_GAP>>1), ROW5_Y, COL4_CX+(COL3_GAP>>1), ROW5_CY, REGULAR_FONT, D4D_NO_CONST, HEATING_COLOR, REGULAR_TEXT_COLOR);


D4D_DECLARE_STD_SCREEN_BEGIN(screen_controller, ScrController_)
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_name)    
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_tempunit)
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_setpoint)
    D4D_DECLARE_SCREEN_OBJECT(scrController_lbl_mode)
        
    D4D_DECLARE_SCREEN_OBJECT(scrController_name)
        
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
        
        
static void ScrController_OnInit()
{
}

static void ScrController_OnMain()
{
}

static void ScrController_OnActivate()
{
}

static void ScrController_OnDeactivate()
{
}

static Byte ScrController_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    return 0;
}
