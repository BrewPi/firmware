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

#pragma once

#include "temperatureFormats.h"
#include "TempSensorInterface.h"
#include "defaultDevices.h"
#include "ControllerMixins.h"
#include "RefTo.h"

class TempSensorDelegate final : public TempSensorInterface, public TempSensorDelegateMixin {
public:
    TempSensorDelegate(){}
    TempSensorDelegate(std::function<Interface * ()> lookup) {
        sensor.setLookup(lookup);
    }

    ~TempSensorDelegate() {
    }

    void setLookup(std::function<Interface* ()> lookup) {
        sensor.setLookup(lookup);
    }

    void unsetLookup(){
        setLookup(nullptr);
    }

    std::function<Interface * ()> getLookup() {
        return sensor.getLookup();
    }

    void accept(VisitorBase & v) final {
    	v.visit(*this);
    }

    void update() final {
        sensor().update();
    }

    bool isConnected(void) const final {
        return sensor().isConnected();
    }

    bool init() final {
        return sensor().init();
    }

    temp_t read() const final {
        return sensor().read();
    }

private:
    RefTo<TempSensorInterface> sensor;
    friend class TempSensorDelegateMixin;
};

