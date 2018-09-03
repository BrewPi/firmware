#!/bin/sh
MY_DIR=$(dirname $(readlink -f $0))

function status()
{
if [[ "$1" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
  popd > /dev/null
  exit 1
fi
}

# status $?
echo "Running controlbox unit tests"
pushd $MY_DIR/../controlbox/build/ > /dev/null
./runner
status $?
popd > /dev/null


echo "Running controlbox unit tests"
pushd $MY_DIR/../app/brewblox/test/build > /dev/null
./runner
status $?
popd > /dev/null
