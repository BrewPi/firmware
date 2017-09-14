#!/bin/bash
PROTODIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
NANOPB_ROOT="/home/elco/source/nanopb-0.3.8-linux-x86"
mkdir -p "$PROTODIR/cpp"
"$NANOPB_ROOT"/generator-bin/protoc "$PROTODIR"/*.proto --proto_path="$PROTODIR" -I"$NANOPB_ROOT"/generator/proto --nanopb_out="$PROTODIR"/cpp
