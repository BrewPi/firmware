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


pushd "$MY_DIR/../lib/test_catch" > /dev/null
echo "Building BrewBlox app unit tests"
make -s runner
(( result = $? ))
status $result
(( exit_status = exit_status || result ))

build/runner
(( result = $? ))
status $result
(( exit_status = exit_status || result ))

popd > /dev/null

exit $exit_status
