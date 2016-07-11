/*
 * Copyright 2016 Matthew McGowan.
 *
 * This file is part of Controlbox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Controlbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <string>

#include "examplebox.h"

int main(int argc, char** argv)
{
	std::string eeprom;
	if (argc>1) {
		eeprom = argv[1];
	}
	ExampleBox box(eeprom);
    box.initialize();
	box.run();
    box.shutdown();
}
