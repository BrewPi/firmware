/*
 * Copyright 2015 Matthew McGowan
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

#include "defaultDevices.h"
#include <memory>

ValueSensor<bool> * defaultSensor(){
    static ValueSensor<bool> * s = new ValueSensor<bool>(false);
    return s;
}

std::shared_ptr<ActuatorDigital> defaultActuator(){
    static std::shared_ptr<ActuatorDigital> a = std::make_shared<ActuatorNop>();
    return a;
}

std::shared_ptr<ActuatorRange> defaultLinearActuator(){ // always returns invalid and does nothing
    static std::shared_ptr<ActuatorRange> a = std::make_shared<ActuatorInvalid>();
    return a;
}


TempSensorDisconnected * defaultTempSensorBasic(){
    static TempSensorDisconnected * t = new TempSensorDisconnected;
    return t;
}

SetPointConstant * defaultSetPoint(){
    static SetPointConstant * sp = new SetPointConstant(temp_t::invalid());
    return sp;
}
