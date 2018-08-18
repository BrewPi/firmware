/*
 * Block.cpp TODO
 *
 *  Created on: Jul 23, 2018
 *      Author: elco
 */

#include "Block.h"


cbox::CboxError streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize){
    pb_ostream_t stream = { dataOutStreamCallback, &out, maxSize, 0 };
    bool success = pb_encode(&stream, fields, srcStruct);
    out.write(0); // zero terminate every write, so protobuf will stop processing on encountering this zero field tag

    return (success) ? cbox::CboxError::no_error : cbox::CboxError::output_stream_encoding_error;
}

cbox::CboxError streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize){
    pb_istream_t stream = { dataInStreamCallback, &in, maxSize + 1}; // 1 extra byte for the zero termination
    bool success = pb_decode(&stream, fields, destStruct);

    return (success) ? cbox::CboxError::no_error : cbox::CboxError::input_stream_decoding_error;
}
