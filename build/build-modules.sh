#!/bin/bash
MY_DIR=$(dirname $(readlink -f $0))

function status()
{
if [[ "$1" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
fi
}

pushd "$MY_DIR/../platform/spark/firmware/modules" > /dev/null

echo "Building system modules for P1"
make -s all PLATFORM=p1;
(( result = $? ))
status $result
(( exit_status = exit_status || result ))

echo "Building system modules for Photon"
make -s all PLATFORM=photon
(( result = $? ))
status $result
(( exit_status = exit_status || result ))

popd > /dev/null

exit $exit_status