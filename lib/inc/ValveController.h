/*
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
#include <memory>
#include "DS2408.h"
#include "ActuatorInterfaces.h"
#include "ControllerMixins.h"

/**
 * ValveController controls a single valve on a DS2408.
 *
 * Each DS2408 can control 2 valves, 1 with the upper 4 bits (valve A) and one with the lower 4 bits (valve B). \n
 * Bit 7-6 and 3-2 drive an H-bridge to control the valve motor. Bit 5-4 and 1-0 are used to read out the status of
 * the fully open and fully closed feedback switches. \n
 * bit 7-6: Valve A action: 01 = open, 10 = close, 11 = off, 00 = off but LEDS on \n
 * bit 5-4: Valve A status: 01 = opened, 10 = closed, 11 = in between \n
 * bit 3-2: Valve B action: 01 = open, 10 = close, 11 = off, 00 = off but LEDS on \n
 * bit 1-0: Valve B status: 01 = opened, 10 = closed, 11 = in between \n
 *
 */
class ValveController final : public ActuatorDigital, public ValveControllerMixin {
public:
    /**
     * Constructor that creates a new valve controller, using an already existing DS2408
     * @param device_ reference to the existing DS2408
     * @param output_ the valve is connected to either the upper or the lower bits. 1 for upper (B), 0 for lower (A)
     */
    ValveController(std::shared_ptr<DS2408> device_,
                    uint8_t  output_) :
                    device(device_),
                    desiredAction(VALVE_IDLE_INIT),
                    output(output_)
					{
        device->update();
    }

    /**
     * Destructor does nothing. User is responsible for destructing the DS2408 when it is not used anymore.
     */
    ~ValveController() = default;

    /**
     * The valve itself can be in 3 states: fully closed, fully open or somewhere in between.
     */
    static const uint8_t VALVE_OPENED = 0b01; //  Feedback switch for fully open is connected to GND.
    static const uint8_t VALVE_CLOSED = 0b10; // = 0b10  Feedback switch for fully closed is connected to GND
    static const uint8_t VALVE_HALFWAY= 0b11; // = 0b11  Neither switches are closed, so valve is neither open nor closed
    static const uint8_t VALVE_ERROR = 0b00; // = 0b00  Both switches are closed. This is an error state. Is also used to indicate communication error

    /**
     * The motor can be driven in clockwise, anti-clockwise or idle
     */
    static const uint8_t VALVE_OPENING = 0b01; // H-bridge is driven in direction to open the valve
    static const uint8_t VALVE_CLOSING = 0b10; // H-bridge is driven in direction to close the valve
    static const uint8_t VALVE_IDLE = 0b11;    // H-bridge has both legs at same level, so motor is idle
    // VALVE_IDLE_INIT is set in constructor as output state. This is to be able to distinguish between a desired idle state and uninitialized.
    static const uint8_t VALVE_IDLE_INIT = 0b00;    // H-bridge has both legs at same level, so motor is idle.

    /**
     * Visitor pattern hook
     * @param v
     */
    virtual void accept(VisitorBase & v) override final {
        v.visit(*this);
    }


    /**
     * Gets the state of the single valve (chosen by output nr). \n
     * State is based on reading the I/O pins connected to the feedback switches. \n
     * @returns state of valve (0b01 = opened, 0b10 = closed, 0b11 = in between \n
     */
    uint8_t getPosition() const;

    /**
     * Gets the action currently performed by the motor of the valve, read from the latches. \n
     * @returns action performed by valve (0b01 = opening, 0b10 = closing, 0b11 = idle, 0b00= idle \n
     */
    uint8_t getAction() const;

    /**
     * update reads the status from the valve.
     * When the valve is opening or closing, it reverts back to idle when it detects that the action is completed.
     */
    virtual update_t update(const update_t & t) override final;

    /**
     * setActive will open or close the valve, for compatibility with the actuator interface.
     * @param active true opens the valve, false closes it.
     */

    virtual void setState(const State & state, const update_t & now) override final;

    /**
     * Check if valve is open.
     * @return true if valve is open or halfway or opening, false if closed or closing
     */
    State getState() const override final;

    /**
     * Returns the state of the valve (action and current state) as a single 4 bit value
     * @param doUpdate when true, read new values from the hardware device
     * @return 4-bit value, with upper 2 bits motor state and lower bits the valve state.
     */
    uint8_t read(bool doUpdate = true);

    /**
     * Apply a new motor state to the valve.
     * @param action the new motor state (VALVE_OPENING, VALVE_CLOSING or VALVE_IDLE)
     */
    void write(uint8_t action);

    /**
     * Open the valve
     */
    inline void open(){
        write(VALVE_OPENING);
    }

    /**
     * Close the valve
     */
    inline void close(){
        write(VALVE_CLOSING);
    }

    /**
     * Stop opening or closing the valve. The valves themselves automatically stop driving the motor with an internal switch.
     * This function stops the H-bridge from driving the motor. It could be used to stop the valve halfway.
     */
    inline void idle(){
        write(VALVE_IDLE);
    }

    /**
     * This function can be used to get a reference to the DS2408, so it can be shared with another valve controller.
     * @return shared_ptr<DS2408> to the DS2408 driver class.
     */
    std::shared_ptr<DS2408> getHardwareDevice(){
    	return device;
    }

protected:
    std::shared_ptr<DS2408> device;
    uint8_t desiredAction;
    uint8_t output; // 0=A or 1=B


    friend class ValveControllerMixin;
};

