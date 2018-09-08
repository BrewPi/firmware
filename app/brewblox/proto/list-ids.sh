#!/bin/bash

# simple script that echo's the lines defining the ID from all proto files using grep

PROTO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$PROTO_DIR" > /dev/null
grep -s -T *.proto -o -e ".msgid =.*"
popd > /dev/null