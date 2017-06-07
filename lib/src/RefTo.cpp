/*
 * Copyright 2017 BrewPi/Elco Jacobs.
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

#include "RefTo.h"

#include "../inc/ProcessValue.h"
#include "ActuatorMocks.h"
#include "TempSensorDisconnected.h"
#include "SetPoint.h"
#include "SensorSetPointPair.h"
#include "VisitorCast.h"

template<>
ActuatorDigital * defaultTarget<ActuatorDigital>(){
    static ActuatorNop s;
    return &s;
}
template<>
ActuatorAnalog * defaultTarget<ActuatorAnalog>(){
    static ActuatorInvalid a;
    return &a;
}

template<>
TempSensor * defaultTarget<TempSensor>(){
    static TempSensorDisconnected t;
    return &t;
}

template<>
SetPoint * defaultTarget<SetPoint>(){
    static SetPointConstant sp(temp_t::invalid());
    return &sp;
}

template<>
ProcessValue * defaultTarget<ProcessValue>(){
    static SensorSetPointPair pv(*defaultTarget<TempSensor>(), *defaultTarget<SetPoint>());
    return &pv;
}

template<>
ActuatorMutexGroup * defaultTarget<ActuatorMutexGroup>(){
    return nullptr; // ActuatorMutexDriver checks for nullptr, so this should be safe
}

// explicit instantiation of template functions
template ActuatorDigital* asInterface<ActuatorDigital>(Interface*);
template ActuatorAnalog* asInterface<ActuatorAnalog>(Interface*);
template TempSensor* asInterface<TempSensor>(Interface*);
template SetPoint* asInterface<SetPoint>(Interface*);
template ActuatorMutexGroup* asInterface<ActuatorMutexGroup>(Interface*);
template ProcessValue* asInterface<ProcessValue>(Interface*);


