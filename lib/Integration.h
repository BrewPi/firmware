/*
 * Copyright 2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * BrewPi is free software: you can redistribute it and/or modify
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
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INTEGRATION_H
#define	INTEGRATION_H

#include "Comms.h"
#include "SystemProfile.h"
#include "Commands.h"


/**
 * Function prototype expected by the commands implementation to perform
 * a reset.
 * @param exit false on first call, true on second call. The first call (exit==false) is
 * during command processing, so that actions can be taken before the command response is sent.
 * The second call (exit==true) is called to perform the actual reset.
 */
void handleReset(bool exit);


/**
 * Application-provided method to create a new root container for the profile.
 * The application can create default objects in the root container.
 */
extern Container* createRootContainer();

/**
 *
 */
extern Object* createApplicationObject(ObjectDefinition& def, bool dryRun);


extern void printVersion(DataOut& out);

#endif	/* INTEGRATION_H */

