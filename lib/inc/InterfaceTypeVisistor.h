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
#include "AbstractVisitor.h"

class ActuatorDigitalInterface;
class ActuatorRangeInterface;
class ActuatorThresholdInterface;
class TempSensorInterface;
class SetPointInterface;
class ControllerInterface;

class InterfaceTypeVisitor : public AbstractVisitor {
public:
	InterfaceTypeVisitor() : lastType(0) {};
protected:
    virtual ~InterfaceTypeVisitor() = default;
public:
	// Actuators
	void visit(ActuatorDigitalInterface const * const) final { lastType = 10; }
	void visit(ActuatorRangeInterface const * const) final { lastType = 11; }
	void visit(ActuatorThresholdInterface const * const) final { lastType = 12; }

	// Sensors
	void visit(TempSensorInterface const * const) final { lastType = 50; }

	// SetPoints
	void visit(SetPointInterface const * const) final { lastType = 100; }
	void visit(ControllerInterface const * const) final { lastType = 150; }

	uint8_t getType(){
		return lastType;
	}
private:
	uint8_t lastType;
};

inline uint8_t getInterfaceType(Interface& i){
	InterfaceTypeVisitor v;
	i.accept(v);
	return v.getType();
}

