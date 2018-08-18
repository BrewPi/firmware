// the user should create AppTypeRegistry.cpp and define application object types in a similar way as below

#include "ResolveType.h"

namespace cbox {

SET_SYSTEM_TYPE_ID(InactiveObject, 1);
SET_SYSTEM_TYPE_ID(ContainedObject, 2);
SET_SYSTEM_TYPE_ID(CurrentTicksValue, 10);
// Templated scaled ticks value: 11

} // end namespace cbox

