// the user should create AppTypeRegistry.cpp and define application object types in a similar way as below

#include "ResolveType.h"

namespace cbox {

class InactiveObject;
template<> obj_type_t resolveTypeID<InactiveObject>(){ return  1; }

class CurrentTicksValue;
template<> obj_type_t resolveTypeID<CurrentTicksValue>(){ return  10; }

// type 11 reserved for ScaledTicksValue. Not done with templating due to nested template making this hard.


} // end namespace cbox

