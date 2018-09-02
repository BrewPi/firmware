#!/bin/sh
MY_DIR=$(dirname $(readlink -f $0))

function status()
{
if [[ "$1" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
  exit 1
fi
}

# cd $MY_DIR/../lib/test
# make -s
# result=$?
# status $result

pushd $MY_DIR/../app/brewblox/test > /dev/null
make -s
result=$?
status $result
popd > /dev/null

pushd > /dev/null
cd $MY_DIR/../controlbox
make -s
result=$?
status $result
popd > /dev/null