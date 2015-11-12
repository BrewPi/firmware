/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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
#include "DS2408.h"
#include "ActuatorInterfaces.h"
#include "ActuatorBottom.h"

class ValveController :
    private ActuatorBottom, public ActuatorDigital{
public:
    ValveController(OneWire *     bus,
                    DeviceAddress address,
                    pio_t         pio_) :
                    switchState(0xff), // Set outputs and inputs to OFF state
                    sense(0b11), // Set sense to OFF state (in between)
                    act(0b11),   // set output to OFF (not open/closed, no action)
                    pio(pio_){  //
        device.init(bus, address);
    }
    virtual ~ValveController(){};

    enum class ValveActions : uint8_t {
        OFF_LOW = 0b00,
        OPEN = 0b01,
        CLOSE = 0b10,
        OFF = 0b11,
        OPENING = 4,
        CLOSING = 5
    };
    
    void update();
    void setActive(bool active){
        if(active){
            open();
        }
        else{
            close();
        }
    }

    bool isActive(){
        // return active when not closed, so a half open valve also returns active
        return sense != uint8_t(ValveActions::CLOSE);
    }

    uint8_t read(bool doUpdate = true);

    void write(ValveActions action);

    inline void open(){
        write(ValveActions::OPEN);
    }
    inline void close(){
        write(ValveActions::CLOSE);
    }
    inline void stop(){ // can be used to stop the valve half way
        write(ValveActions::OFF);
    }

    void serialize(JSON::Adapter& adapter){
        JSON::Class root(adapter, "Valve");
        JSON_E(adapter, pio);
        JSON_T(adapter, sense);
    }

protected:
    uint8_t switchState; // state bits of the entire switch
    uint8_t sense; // sensed value (feedback)
    uint8_t act; // written value (actuator)
    pio_t pio; // 0=A or 1=B
    DS2408 device;
};

