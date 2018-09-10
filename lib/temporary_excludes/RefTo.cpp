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

#include "ProcessValue.h"
//#include "ActuatorMocks.h"
#include "TempSensorDisconnected.h"
#include "Setpoint.h"
#include "SetpointSensorPair.h"
//#include "ActuatorMutexGroupDisabled.h"
#include "VisitorCast.h"

/*
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
*/
template<>
TempSensor * defaultTarget<TempSensor>(){
    static TempSensorDisconnected t;
    return &t;
}

template<>
Setpoint * defaultTarget<Setpoint>(){
    static SetpointConstant sp(temp_t::invalid());
    return &sp;
}

template<>
ProcessValue * defaultTarget<ProcessValue>(){
    static SetpointSensorPair pv(*defaultTarget<TempSensor>(), *defaultTarget<Setpoint>());
    return &pv;
}
/*
template<>
ActuatorMutexGroupInterface * defaultTarget<ActuatorMutexGroupInterface>(){
    static ActuatorMutexGroupDisabled mg;
    return &mg;
}*/

// explicit instantiation of template functions
//template ActuatorDigital* asInterface<ActuatorDigital>(Interface*);
//template ActuatorAnalog* asInterface<ActuatorAnalog>(Interface*);
template TempSensor* asInterface<TempSensor>(Interface*);
template Setpoint* asInterface<Setpoint>(Interface*);
//template ActuatorMutexGroupInterface* asInterface<ActuatorMutexGroupInterface>(Interface*);
template ProcessValue* asInterface<ProcessValue>(Interface*);

// these are needed to search for shared OneWire switch ICs
#if BREWPI_DS2408
template ValveController* asInterface<ValveController>(Interface*);
#endif
#if BREWPI_DS2413
template ActuatorOneWire* asInterface<ActuatorOneWire>(Interface*);
#endif


