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

#pragma once

#include "TempSensorDisconnected.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"
#include "SetPoint.h"
#include "Sensor.h"

#include "ControlLib.h"

CONTROL_LIB_BEGIN

// Objects that the library requires the application to instantiate.

ValueSensor<bool> * defaultSensor();
ActuatorNop * defaultActuator();
ActuatorInvalid * defaultLinearActuator(); // always returns invalid and does nothing
TempSensorBasic * defaultTempSensorBasic();
SetPointConstant * defaultSetPoint();

CONTROL_LIB_END
