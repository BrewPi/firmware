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

class OneWireBusBlock;
namespace cbox {
	template<> obj_type_t resolveTypeID<OneWireBusBlock>(){ return  256; }
}

class OneWireTempSensorBlock;
namespace cbox {
	template<> obj_type_t resolveTypeID<OneWireTempSensorBlock>(){ return  257; }
}

class PidBlock;
namespace cbox {
	template<> obj_type_t resolveTypeID<PidBlock>(){ return  258; }
}

class SensorSetPointPairBlock;
namespace cbox {
	template<> obj_type_t resolveTypeID<SensorSetPointPairBlock>(){ return  259; }
}

class SetPointSimpleBlock;
namespace cbox {
	template<> obj_type_t resolveTypeID<SetPointSimpleBlock>(){ return  260; }
}

class DeviceIdValue;
namespace cbox {
	template<> obj_type_t resolveTypeID<DeviceIdValue>(){ return  261; }
}
