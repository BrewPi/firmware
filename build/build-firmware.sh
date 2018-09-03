#!/bin/bash
MY_DIR=$(dirname $(readlink -f $0))

function makeit()
{
	echo "building $*"
    make -s all $*
if [[ "$?" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
fi
}

function makeapp()
{
	makeit PLATFORM=gcc $*
	makeit PLATFORM=P1 $*
	makeit PLATFORM=photon $*
}

pushd "$MY_DIR" > /dev/null
makeapp APP=brewblox
popd > /dev/null