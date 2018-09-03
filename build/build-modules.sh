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

pushd $MY_DIR/../platform/spark/firmware/modules > /dev/null

echo "Building system modules for P1"
make all PLATFORM=p1
result=$?
status $result

echo "Building system modules for Photon"
make all PLATFORM=photon
result=$?
status $result

popd > /dev/null
