#pragma once

#include "cbox/ObjectBase.h"
#include "pb.h"
#include <type_traits>

template <uint16_t id>
using Block = cbox::ObjectBase<id>;

// helpers functions to stream protobuf fields
cbox::CboxError
streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize);
cbox::CboxError
streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize);
