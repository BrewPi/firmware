#pragma once

#include "ObjectBase.h"
#include "pb.h"

template <typename T>
using Block = cbox::ObjectBase<T>;

// helpers functions to stream protobuf fields
cbox::CboxError
streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize);
cbox::CboxError
streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize);
