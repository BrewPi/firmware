#!/bin/bash

PROTO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
NANOPB_PATH="${PROTO_DIR}/../../../platform/spark/firmware/nanopb/nanopb"
PROTOC_NANOPB_PLUGIN="${NANOPB_PATH}/generator/protoc-gen-nanopb"
if [[ "${OSTYPE}" == "msys" ]]; then
	PROTOC_NANOPB_PLUGIN="${PROTOC_NANOPB_PLUGIN}.bat" # add .bat when on windows
fi
PROTOC_INCLUDE_PATH="-I${PROTO_DIR} -I${NANOPB_PATH}/generator -I${NANOPB_PATH}/generator/proto"

ls ${PROTOC_NANOPB_PLUGIN}

pushd "$PROTO_DIR" # .option files are read from execution directory, so have to cd into this dir 
mkdir -p "cpp"

PROTOC_INCLUDE_PATH="-I${PROTO_DIR} -I${NANOPB_PATH}/generator -I${NANOPB_PATH}/generator/proto"
protoc ${PROTOC_INCLUDE_PATH} \
--nanopb_out=cpp \
${PROTO_DIR}/*.proto \
--proto_path=${PROTO_DIR} \
--plugin=protoc-gen-nanopb=${PROTOC_NANOPB_PLUGIN}
popd
