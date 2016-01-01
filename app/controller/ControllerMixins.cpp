/*
 * Copyright 2016 BrewPi/Elco Jacobs.
 * Copyright 2016 Matthew McGowan.
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

#include "ControllerMixins.h"
#include "Pid.h"
#include "TempSensor.h"
#include "TempSensorBasic.h"

void PidMixin::serialize(JSON::Adapter & adapter)
{
    Pid * pid = (Pid *) this;

    JSON::Class root(adapter, "Pid");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON::stream(adapter, "enabled", pid -> enabled, true);
    JSON::stream(adapter, "setPoint", pid -> setPoint, true);
    JSON::stream(adapter, "inputSensor", pid -> inputSensor, true);
    JSON::stream(adapter, "inputError", pid -> inputError, true);
    JSON::stream(adapter, "Kp", pid -> Kp, true);
    JSON::stream(adapter, "Ti", pid -> Ti, true);
    JSON::stream(adapter, "Td", pid -> Td, true);
    JSON::stream(adapter, "p", pid -> p, true);
    JSON::stream(adapter, "i", pid -> i, true);
    JSON::stream(adapter, "d", pid -> d, true);
    JSON::stream(adapter, "actuatorIsNegative", pid -> actuatorIsNegative, true);
    JSON::stream(adapter, "outputActuator", pid -> outputActuator, false);
}

void TempSensorMixin::serialize(JSON::Adapter & adapter)
{
    TempSensor * sensor = (TempSensor *) this;

    JSON::Class root(adapter, "TempSensor");
    std::string name(sensor -> getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON::stream(adapter, "sensor", sensor -> sensor, false);
}
