/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan
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

#include "Brewpi.h"
#include "ActuatorInterfaces.h"
#include "ActuatorBottom.h"
#include "DS2413.h"

/*
 * An actuator or sensor that operates by communicating with a DS2413 device.
 *
 */
class ActuatorOneWire:
    private ActuatorBottom, public ActuatorDigital

#if DS2413_SUPPORT_SENSE
            ,
    SwitchSensor
#endif

{
    public:
        ActuatorOneWire(OneWire *     bus,
                        DeviceAddress address,
                        pio_t         pio,
                        bool          invert = true)
        {
            init(bus, address, pio, invert);
        }

        void init(OneWire *     bus,
                  DeviceAddress address,
                  pio_t         pio,
                  bool          invert = true)
        {
            this -> invert = invert;
            this -> pio    = pio;

            device.init(bus, address);
        }

        void setActive(bool active)
        {
            // todo: alarm when write fails
            device.latchWrite(pio, active ^ invert, true);
        }

        bool isActive()
        {
            return device.latchRead(pio, false, true) ^ invert;
        }

#if DS2413_SUPPORT_SENSE
        bool sense()
        {
            device.latchWrite(pio, 0, false);

            return device.sense(pio, invert);    // on device failure, default is high for invert, low for regular.
        }
#endif
        void write(uint8_t val){
            setActive(val != 0);
        };

        void update(){} // do nothing on periodic update

        void serialize(JSON::Adapter& adapter){
            JSON::Class root(adapter, "ActuatorOneWire");
            bool active = isActive();
            JSON_E(adapter, active);
            JSON_E(adapter, pio);
            JSON_T(adapter, invert);
        }

    private:
        DS2413 device;
        pio_t  pio;
        bool   invert;
};
