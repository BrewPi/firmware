// Visitor pattern to have a global list of object types
// Values 0-255 are reserved for internal controlbox classes.
// The user application should use values above 255

#pragma once

#include <stdint.h>

namespace cbox {

typedef uint16_t obj_type_t;

template<class T>
obj_type_t resolveTypeID();

template<class T>
obj_type_t resolveTypeID(T*){
	return resolveTypeID<T>();
}

} // end namespace cbox

