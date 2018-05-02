/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

namespace cbox {

#include "pgmspace.h"
#include "Values.h"

typedef PCSTR const char*;

class ProgmemStringValue : public Readable<PCSTR>, public AbstractValue
{
	private:
		PCSTR value;

	public:

		PCSTR read() {
			return value;
		}

		void stream(DataOut& out) {
			PCSTR v = value;
			do {
				uint8_t v = pgm_read_byte(v++);
				out.write(v);
			} while (v);
		}

};

}
