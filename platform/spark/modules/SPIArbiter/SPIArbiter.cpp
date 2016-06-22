/*
 * Copyright 2016 BrewPi/Matthew McGowan.
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


#include "SPIArbiter.h"

void SPIArbiter::apply(SPIConfiguration& client){
    if (mode_!=client.getMode()) {
        mode_ = client.getMode();
        spi_.setDataMode(mode_);
    }

    if (bitOrder_!=client.getBitOrder()) {
        bitOrder_ = client.getBitOrder();
        spi_.setBitOrder(bitOrder_);
    }

    if (clockDivider_!=client.getClockDivider()) {
        clockDivider_ = client.getClockDivider();
        spi_.setClockDivider(clockDivider_);
    }
    if (ss_pin_!=client.getSSPin()) {
		if(ss_pin_ != UINT16_MAX){
			digitalWrite(ss_pin_, HIGH); // unselect previous client
		}
		ss_pin_ = client.getSSPin();
		digitalWrite(ss_pin_, LOW); // select new client
	    spi_.begin(ss_pin_);
    }
}

void SPIArbiter::unapply() {
	digitalWrite(ss_pin_, HIGH); // unselect pin
	ss_pin_ = UINT16_MAX;
	spi_.end();
}

SPIArbiter GlobalSPIArbiter(SPI);
