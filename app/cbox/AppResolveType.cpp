/*
 * Copyright 2018 Elco Jacobs / BrewPi B.V.
 *
 * This file is part of BrewBlox.
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
 * along with BrewBlox. If not, see <http://www.gnu.org/licenses/>.
 */


// Values 0-255 are reserved for internal controlbox classes.
// The user application should can use values above 255

#include "ResolveType.h"

SET_TYPE_ID(OneWireBusBlock, 256);
SET_TYPE_ID(OneWireTempSensorBlock, 257);
SET_TYPE_ID(PidBlock, 258);
SET_TYPE_ID(SensorSetPointPairBlock, 259);
SET_TYPE_ID(SetPointSimpleBlock, 260);
SET_TYPE_ID(DeviceIdObject,261);
// TicksBlock: hardcoded at 262 <- TODO: make resolveTypeId work for template class
SET_TYPE_ID(ProfilesBlock,263);
SET_TYPE_ID(SysInfoBlock, 264);
