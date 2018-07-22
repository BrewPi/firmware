/*
 * Copyright 2015 Matthew McGowan.
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

#include "Comms.h"
#include "SystemProfile.h"
#include "Static.h"
#include "Commands.h"
#include <memory>

namespace cbox {


#if CONTROLBOX_STATIC

/**
 * Function prototype expected by the commands implementation to perform
 * a reset.
 * @param exit false on first call, true on second call. The first call (exit==false) is
 * during command processing, so that actions can be taken before the command response is sent.
 * The second call (exit==true) is called to perform the actual reset.
 */
void handleReset(bool exit);

/**
 * Retrieves a reference to the systemRootContainer that hosts objects
 * independently from a system profile.
 */
ObjectContainer& systemRootContainer();

/**
 * Application-provided method to create a new root container for the currently active profile.
 * The application can create default objects in the root container.
 */
ObjectContainer* createRootContainer();

/**
 * Create an application supplied object.
 */
// std::shared_ptr<Object> createApplicationObject2(obj_type_t typeId, DataIn& in, CommandResult& errorCode);

/**
 * This function is called when a connection has been established.
 * Typically the application will output some annotations such as
 * the application version or other data needed to establish the
 * protocol between this device and the
 */
void connectionStarted(DataOut& out);

/**
 * Initialize the controlbox.
 */
void controlbox_setup(size_t loadProfileDelay);

/**
 * Run the background loop for control box processing.
 */
void controlbox_loop();

#endif

} // end namespace cbox
