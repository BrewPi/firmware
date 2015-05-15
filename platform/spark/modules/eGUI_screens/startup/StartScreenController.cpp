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

#include "Brewpi.h"
#include "Platform.h"
#include "fixstl.h"
#include <algorithm>
#include "startup_screen.h"
#include "../BrewPiTouch/BrewPiTouch.h"
#include "UI.h"
#include "UIController.h"

D4D_EXTERN_SCREEN(screen_startup);


class StartupScreenModel {
    
    uint32_t timer;
    bool touched_flag;
    uint8_t fade_color;
public:    
    void start() {
        timer = millis();
        touched_flag = false;
        fade_color = 255;
    }
    
    uint32_t elapsed() {
        return millis()-timer;
    }
    
    bool timeout() {
        return elapsed()>4500;
    }
    
    bool touched() {
        return touched_flag;
    }
    
    void flagTouched() {
        touched_flag = true;
    }
        
    bool fadeColorUpdate(uint8_t newColor) {
        bool changed = newColor!=fade_color;
        fade_color = newColor;
        return changed;
    }
    
    uint8_t fadeColor() { return fade_color; }
};

// really would have liked to have this on the stack, but can't with re-entrant coding model.
StartupScreenModel model;

extern BrewPiTouch touch;
 
void ScrStartup_OnInit()
{    
}

void ScrStartup_OnMain()
{    
    uint32_t elapsed = model.elapsed();
    if (elapsed>2000) {                        
        if (model.fadeColorUpdate(min(255lu, ((elapsed-2000)*255)/(3000)))) {
            uint8_t c = model.fadeColor();
            scrStartup_version.clrScheme->fore = D4D_COLOR_RGB(c,c,c);            
            D4D_InvalidateObject(&scrStartup_version, D4D_FALSE);
        }
    }
    if (model.timeout() || model.touched())
        uiController.notifyStartupComplete();
}

void ScrStartup_OnActivate()
{    
    model.start();
    touch.setStabilityThreshold(16000); // set to high Threshold to disable filter    
}

void ScrStartup_OnDeactivate()
{
    if (model.touched()){
        touch.setStabilityThreshold(5); // require extra stable reading
        calibrateTouchScreen();
    }
    
    touch.setStabilityThreshold(); // reset to default    
}

Byte ScrStartup_OnObjectMsg(D4D_MESSAGE* pMsg)
{    
    if (pMsg->nMsgId==D4D_MSG_TOUCHED) 
        model.flagTouched();
    return 0;
}


