/*
 * Copyright 2015 BrewPi/Elco Jacobs/Matthew McGowan. 
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

#ifndef PILINKHANDLERS_H
#define	PILINKHANDLERS_H

/**
 * Called to invoke a system reset.
 */
void handleReset(bool exit=true);

/**
 * Called to start the platform firmware update process.
 */
void flashFirmware();

#endif	/* PILINKHANDLERS_H */

