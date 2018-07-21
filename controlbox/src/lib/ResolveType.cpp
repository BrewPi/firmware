// the user should create AppTypeRegistry.cpp and define application object types in a similar way as below

#include "ResolveType.h"

namespace cbox {

class InactiveObject;
template<> obj_type_t resolveTypeID<InactiveObject>(){ return  1; }

class CurrentTicksValue;
template<> obj_type_t resolveTypeID<CurrentTicksValue>(){ return  10; }

template<template <typename> class ticksBase>
obj_type_t resolveTypeID(ScaledTicksValue<ticksBase> *){ return  11; }

class EepromValue;
template<> obj_type_t resolveTypeID<EepromValue>(){ return  21; }
class EepromBlock;
template<> obj_type_t resolveTypeID<EepromBlock>(){ return  22; }


} // end namespace cbox

