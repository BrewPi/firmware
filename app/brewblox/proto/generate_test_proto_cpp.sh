#!/bin/bash

# generates cpp with google's protobuf implementation. Not used in firmware, but used in unit test code

PROTO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

pushd "$PROTO_DIR" # .option files are read from execution directory, so have to cd into this dir 
mkdir -p "test_cpp"

PROTOC_INCLUDE_PATH="-I${PROTO_DIR}"
protoc ${PROTOC_INCLUDE_PATH} \
--cpp_out=test_cpp
${PROTO_DIR}/*.proto \
--proto_path=${PROTO_DIR} \
popd
