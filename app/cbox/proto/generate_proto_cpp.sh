#!/bin/bash
PROTODIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
NANOPB_ROOT="C:/repos/nanopb-0.3.9-windows-x86/"

pushd "$PROTODIR" # .option files are read from execution directory, so have to cd into this dir 
mkdir -p "cpp"

"$NANOPB_ROOT"/generator-bin/protoc "--nanopb_out=-v:cpp" "$PROTODIR"/*.proto --proto_path="$PROTODIR" -I"$NANOPB_ROOT"/generator/proto
popd