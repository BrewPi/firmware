#!/bin/bash
MY_DIR=$(dirname "$(readlink -f "$0")")

function status()
{
if [[ "$1" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
fi
}

pushd "$MY_DIR/../app/brewblox/proto" > /dev/null
echo "Compiling proto files using nanopb for brewblox firmware"
bash generate_proto_cpp.sh;
(( result = $? ))
status $result
(( exit_status = exit_status || result ))

echo "Compiling proto files using google protobuf for unit tests"
bash generate_proto_test_cpp.sh;
(( result = $? ))
status $result
(( exit_status = exit_status || result ))
popd > /dev/null

exit $exit_status