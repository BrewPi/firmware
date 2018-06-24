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
#include "DS2413.h"


/*
 * An actuator or sensor that operates by communicating with a DS2413 device.
 *
 */
class ActuatorOneWire final:
    public ActuatorDigital,
	public ActuatorOneWireMixin
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

        void setState(State state, int8_t priority = 127) override final
        {
            bool bitVal = (state == State::Active) ^ invert;
        	device->writeLatchBit(pio, bitVal, true);
        }

        State getState() const override final
        {
            bool result;
        	if(device->latchReadCached(pio, result)){
        		return (result ^ invert) ? State::Active : State::Inactive;
        	}
        	return State::Unknown;
        }

        State sense()
        {
            if(device->writeLatchBit(pio, false, false)){
            	bool result;
            	if(device->readLatchBit(pio, result, false)){
            		return (result ^ invert) ? State::Active : State::Inactive;
            	}
            }
            return State::Unknown;

        }

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
