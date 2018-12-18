/*
 * Copyright 2018 BrewPi B.V.
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
#include "ProcessValueWidgetBase.h"
#include "TempSensor.h"
#include "cbox/CboxPtr.h"

class TempSensorWidget : public ProcessValueWidgetBase {
private:
    cbox::CboxPtr<TempSensor> lookup;

public:
    TempSensorWidget(WidgetWrapper& myWrapper, const cbox::obj_id_t& id);
    virtual ~TempSensorWidget() = default;

    virtual void update() override final;
};