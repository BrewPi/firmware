// the user should create AppTypeRegistry.cpp and define application object types in a similar way as below

#include "ResolveType.h"

namespace cbox {

class DynamicContainer;
template<> obj_type_t resolveTypeID<DynamicContainer>(){ return  1; }
class FixedContainer;
template<> obj_type_t resolveTypeID<FixedContainer>(){ return  2; }

class CurrentTicksValue;
template<> obj_type_t resolveTypeID<CurrentTicksValue>(){ return  10; }
class ScaledTicksValue;
template<> obj_type_t resolveTypeID<ScaledTicksValue>(){ return  11; }

class EepromValue;
template<> obj_type_t resolveTypeID<EepromValue>(){ return  21; }
class EepromBlock;
template<> obj_type_t resolveTypeID<EepromBlock>(){ return  22; }


} // end namespace cbox

