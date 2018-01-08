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

#include <memory>
#include "ControllerMixins.h"
#include "ActuatorInterfaces.h"
#include "Sensor.h"
#include "DS2413.h"


/*
 * An actuator or sensor that operates by communicating with a DS2413 device.
 *
 */
class ActuatorOneWire final:
    public ActuatorDigital,
    public ActuatorOneWireMixin,
    public SwitchSensor

{
    public:
        ActuatorOneWire(std::shared_ptr<DS2413> _device,
                        pio_t _pio,
                        bool _invert = true) :
                        device(_device),
                        pio(_pio),
                        invert(_invert){}
        ~ActuatorOneWire() = default;

        void accept(VisitorBase & v) final {
        	v.visit(*this);
        }

        void init(){
            device->update();
        }

        void setActive(bool active, int8_t priority = 127) override final
        {
            // todo: alarm when write fails
            device->writeLatchBit(pio, active ^ invert, true);
        }

        bool isActive() const override final
        {
            return device->latchReadCached(pio, false) ^ invert;
        }

        bool sense()
        {
            device->writeLatchBit(pio, false, false);

            // on device failure, default is high for invert, low for regular.
            return device->readLatchBit(pio, invert, false);
        }

        void write(uint8_t val) {
            setActive(val != 0);
        };

        void update() override final{
            device->update();
        }

        void fastUpdate() override final {} // no actions needed

        /**
         * This function can be used to get a reference to the DS2413, so it can be shared with another actuator.
         * @return shared_ptr<DS2413> to the DS2413 driver class.
         */
        std::shared_ptr<DS2413> getHardwareDevice(){
            return device;
        }


    private:
        std::shared_ptr<DS2413> device;
        pio_t  pio;
        bool   invert;

    friend class ActuatorOneWireMixin;
};
