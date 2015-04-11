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

#ifndef CONTROLLERSCREENVIEWS_H
#define	CONTROLLERSCREENVIEWS_H

extern "C" {
#include "d4d.h"
}

#include "controller_screen.h"
#include "TempControl.h"
#include "TemperatureFormats.h"

bool set_background_color(const D4D_OBJECT* pThis, D4D_COLOR bg);



class ControllerStateView
{        
    const D4D_OBJECT** backgrounds;
    
public:
    
    static const unsigned NUM_OBJECTS = 2;

    /**
     * Defines the widgets that show the background state
     * @param backgrounds
     */    
    ControllerStateView(const D4D_OBJECT* backgrounds[NUM_OBJECTS]) {
        this->backgrounds = backgrounds;
    }
    
    void update(D4D_COLOR bg, const char* text)
    {
        D4D_SetText(backgrounds[0], text);
        for (unsigned i=0; i<NUM_OBJECTS; i++) {
            set_background_color(backgrounds[i], bg);
        }
    }
};

class ControllerStatePresenter
{
    static D4D_COLOR state_color[];
    static const char* state_name[];
    ControllerStateView& view_;
    
    D4D_COLOR colorForState(states state)
    {
        return state_color[state];
    }
    
    const char* nameForState(states state) 
    {
        return state_name[state];
    }

public:    
    
    ControllerStatePresenter(ControllerStateView& view)
    : view_(view)
    {        
    }
    
    void setState(states state)
    {
        view_.update(colorForState(state), nameForState(state));
    }
    
};

class TemperatureProcessView
{
    const D4D_OBJECT**    objects;
    
public:

    /**
     * The first object is the current temperature.
     * The second object is the set point.
     * @param objects
     */
    TemperatureProcessView(const D4D_OBJECT* objects[]) {
        this->objects = objects;
    }
    
    void setBgColor(D4D_COLOR col)
    {
        set_background_color(objects[0], col);
        set_background_color(objects[1], col);
        set_background_color(objects[2], col);
    }
    
    void update(const char* currentTemp, const char* setpoint)
    {
        D4D_SetText(objects[0], currentTemp);
        if (setpoint) {
            D4D_SetText(objects[1], setpoint ? setpoint : "");            
        }        
    }
};

/**
 * Presents both a set point and the current temperature.
 */
class TemperatureProcessPresenter
{
    TemperatureProcessView& view_;    
    D4D_COLOR bg_col;
public:

    TemperatureProcessPresenter(TemperatureProcessView& view, D4D_COLOR col) :
        view_(view), bg_col(col)
    {}
    
    static void asString(char* buf, temperature t, unsigned num_decimals, unsigned max_len);
    
    static const char* ltrim(const char* s);
        
    void update(temperature current, temperature setpoint, bool has_setpoint=true);
};

class ControllerModeView
{
    const D4D_OBJECT* obj;
    
public:

    ControllerModeView(const D4D_OBJECT* obj)
    {
        this->obj = obj;
    }
    
    void update(const char* mode, D4D_COLOR color)
    {
        D4D_SetText(obj, mode);
        set_background_color(obj, color);
    }
};


class ControllerModePresenter
{
    ControllerModeView& view_;
    
    static const char modes[5];
    static const char* names[5];
    static D4D_COLOR colors[5];
    
    unsigned modeToIndex(control_mode_t mode) 
    {
        for (unsigned int i=0; i<arraySize(modes); i++) 
        {
            if (modes[i]==mode)
                return i;
        }        
        return 3;   // OFF
    }
    
    const char* nameForMode(control_mode_t mode) {
        return names[modeToIndex(mode)];
    }
    
    D4D_COLOR colorForMode(control_mode_t mode) {
        return colors[modeToIndex(mode)];
    }
    
public:

    ControllerModePresenter(ControllerModeView& view)
            : view_(view) {}
            
    void update(control_mode_t mode)
    {
        view_.update(nameForMode(mode), colorForMode(mode));
    }
};


class ControllerTimeView
{
    const D4D_OBJECT* obj;
public:

    ControllerTimeView(const D4D_OBJECT* obj)
    {
        this->obj = obj;
    }
    
    void update(const char* time) 
    {
        D4D_SetText(obj, time);
    }
};

uint16_t fetch_time(states state);

class ControllerTimePresenter
{
    ControllerTimeView& view_;
    
public:

    ControllerTimePresenter(ControllerTimeView& view)
        : view_(view) {}
            
    void update() {
        char time_str[MAX_TIME_LEN];
        int time = fetch_time(tempControl.getState());
        if (time<0)
            time_str[0] = 0;
        else
            sprintf(time_str, "%d:%02d:%02d", uint16_t(time/3600), uint16_t((time/60)%60), uint16_t(time%60));
        view_.update(time_str);
    }
};

class TextView
{    
    const D4D_OBJECT* obj;
    
public:

    TextView(const D4D_OBJECT* obj)
    {
        this->obj = obj;
    }
    
    void update(const char* text)
    {
        D4D_SetText(obj, text);        
    }    
};

class ControllerTemperatureFormatPresenter
{
    TextView& view_;
    
    const char* formatText();
    
public:
  
    ControllerTemperatureFormatPresenter(TextView& view) : view_(view)
    {        
    }
            
    void update()
    {
        view_.update(formatText());
    }
};


const char ControllerModePresenter::modes[5] = {
    MODE_FRIDGE_CONSTANT,
    MODE_BEER_CONSTANT,
    MODE_BEER_PROFILE,
    MODE_OFF,
    MODE_TEST    
};

const char* ControllerModePresenter::names[5] = {
    "FRIDGE",
    "BEER",
    "PROFILE",
    "OFF",
    "TEST"
};

D4D_COLOR ControllerModePresenter::colors[5] = {
    MODE_FRIDGE_COLOR,
    MODE_BEER_COLOR,
    MODE_PROFILE_COLOR,
    MODE_OFF_COLOR,
    MODE_TEST_COLOR
};

#endif	/* CONTROLLERSCREENVIEWS_H */

