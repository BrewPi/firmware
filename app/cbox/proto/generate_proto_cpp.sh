#!/bin/bash
PROTODIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
NANOPB_ROOT="/home/elco/source/nanopb-0.3.8-linux-x86"

pushd "$PROTODIR" # .option files are read from execution directory, so have to cd into this dir 
mkdir "cpp"

"$NANOPB_ROOT"/generator-bin/protoc "--nanopb_out=-v:cpp" "$PROTODIR"/*.proto --proto_path="$PROTODIR" -I"$NANOPB_ROOT"/generator/proto
popd