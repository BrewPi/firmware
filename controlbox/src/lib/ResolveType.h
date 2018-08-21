// Visitor pattern to have a global list of object types
// Values 0-255 are reserved for internal controlbox classes.
// The user application should use values above 255

#pragma once

#include <stdint.h>
#include "Object.h"

namespace cbox {

template<class T>
obj_type_t resolveTypeId();

template<class T>
inline obj_type_t resolveTypeId(const T*) {
	return resolveTypeId<T>();
}

#define SET_TYPE_ID_IMPL(classname, id) \
class classname; \
namespace cbox { \
    template<> obj_type_t resolveTypeId<classname>() { return  id; } \
}

#define SET_TYPE_ID(classname, id) \
static_assert(id >= 256, "User IDs need to be >= 256"); \
SET_TYPE_ID_IMPL(classname, id)

#define SET_SYSTEM_TYPE_ID(classname, id) \
static_assert(id < 256, "System IDs need to be <= 256"); \
class classname; \
template<> obj_type_t resolveTypeId<classname>() { return  id; }

} // end namespace cbox

