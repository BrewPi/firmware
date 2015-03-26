/**
 ******************************************************************************
 * @file    ControllerScreenViews.cpp
 * @authors mat
 * @date    10 March 2015
 ******************************************************************************
  Copyright (c) 2015 Spark Labs, Inc.  All rights reserved.

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

#include "ControllerScreenViews.h"
#include "TempControl.h"
#include "controller_screen.h"

bool set_background_color(const D4D_OBJECT* pThis, D4D_COLOR bg)
{
    D4D_COLOR existing = pThis->clrScheme->bckg;    
    pThis->clrScheme->bckg = bg;
    pThis->clrScheme->bckgDis = bg;
    pThis->clrScheme->bckgCapture = bg;
    pThis->clrScheme->bckgFocus = bg;
    if (existing!=bg)
        D4D_InvalidateObject(pThis, D4D_TRUE);
    return existing!=bg;
}

const char* ControllerStatePresenter::state_name[] {
    "IDLE",
    "OFF",
    "DOOR OPEN",
    "HEATING",
    "COOLING",
    "> COOL",
    "> HEAT",
    "([ \\ )",
    "COOLING.",
    "HEATING."
};

D4D_COLOR ControllerStatePresenter::state_color[] = {
	IDLE_COLOR,                     // 0
	STATE_OFF_COLOR,					// 1
	DOOR_OPEN_COLOR,					// 2 used by the Display only
	HEATING_COLOR,					// 3
	COOLING_COLOR,					// 4
	WAITING_TO_COOL_COLOR,			// 5
	WAITING_TO_HEAT_COLOR,			// 6
	WAITING_FOR_PEAK_DETECT_COLOR,	// 7
	COOLING_MIN_TIME_COLOR,			// 8
	HEATING_MIN_TIME_COLOR			// 9	    
};



const D4D_OBJECT* stateDisplay[] = { &scrController_state, &scrController_time };
const D4D_OBJECT* beerTempDisplay[] = { &scrController_beertemp, &scrController_beersv, &scrController_beer };
const D4D_OBJECT* fridgeTempDisplay[] = { &scrController_fridgetemp, &scrController_fridgesv, &scrController_fridge};
const D4D_OBJECT* roomTempDisplay[] = { &scrController_roomtemp, &scrController_roomsv, &scrController_room};

ControllerStateView stateView(stateDisplay);
ControllerStatePresenter statePresenter(stateView);

TemperatureProcessView beerTempView(beerTempDisplay);
TemperatureProcessPresenter beerTempPresenter(beerTempView, BEER_BG_COLOR);

TemperatureProcessView fridgeTempView(fridgeTempDisplay);
TemperatureProcessPresenter fridgeTempPresenter(fridgeTempView, FRIDGE_BG_COLOR);

TemperatureProcessView roomTempView(roomTempDisplay);
TemperatureProcessPresenter roomTempPresenter(roomTempView, ROOM_BG_COLOR);

ControllerModeView modeView(&scrController_mode);
ControllerModePresenter modePresenter(modeView);

void ControllerScreen_Update()
{
    states state = states(tempControl.getState());
    statePresenter.setState(state);
    modePresenter.update(tempControl.getMode());
    
    beerTempPresenter.update(tempControl.getBeerTemp(), tempControl.getBeerSetting());
    fridgeTempPresenter.update(tempControl.getFridgeTemp(), tempControl.getFridgeSetting());
    roomTempPresenter.update(tempControl.getRoomTemp(), INVALID_TEMP, false);

}


void TemperatureProcessPresenter::asString(char* buf, temperature t, unsigned num_decimals, unsigned max_len)
{
    if (t==INVALID_TEMP) {
        strcpy(buf, "--.-");
    }
    else
        tempToString(buf, t, num_decimals, max_len);
}

const char* TemperatureProcessPresenter::ltrim(const char* s) {
    for (;*s==' ';s++);
    return s;
}

void TemperatureProcessPresenter::update(temperature current, temperature setpoint, bool has_setpoint)
{
    char current_str[MAX_TEMP_LEN];
    char setpoint_str[MAX_TEMP_LEN];

    asString(current_str, current, 1, MAX_TEMP_LEN);
    asString(setpoint_str, setpoint, 1, MAX_TEMP_LEN);        
    view_.setBgColor(bg_col);
    view_.update(ltrim(current_str), has_setpoint ? ltrim(setpoint_str) : NULL);
}          
