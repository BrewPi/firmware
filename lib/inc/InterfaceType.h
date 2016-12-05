/*
 * Copyright 2016 BrewPi/Elco Jacobs.
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
#include <stdint.h>

class ActuatorDigitalInterface;
class ActuatorRangeInterface;
class ActuatorThresholdInterface;
class TempSensorInterface;
class SetPointInterface;

// Actuators
inline uint8_t getInterfaceType(const ActuatorDigitalInterface*) { return 10; }
inline uint8_t getInterfaceType(const ActuatorRangeInterface*) { return 11; }
inline uint8_t getInterfaceType(const ActuatorThresholdInterface*) { return 12; }

// Sensors
inline uint8_t getInterfaceType(TempSensorInterface*){ return 50; }

// SetPoints
inline uint8_t getInterfaceType(SetPointInterface*){ return 100; }
