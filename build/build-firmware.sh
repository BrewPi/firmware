#!/bin/bash
MY_DIR=$(dirname $(readlink -f $0))

function makeit()
{
	echo "building $*"
    make -s all $*
		(( result = $? ))
if [[ "$result" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
fi
	return $result
}

function makeapp()
{
	makeit PLATFORM=gcc $*
	(( result = $? ))
  (( exit_status = exit_status || result ))
	makeit PLATFORM=P1 $*
	(( result = $? ))
  (( exit_status = exit_status || result ))
	makeit PLATFORM=photon $*
	(( result = $? ))
  (( exit_status = exit_status || result ))
	return $exit_status
}

pushd "$MY_DIR" > /dev/null
makeapp APP=brewblox
(( result = $? ))
(( exit_status = exit_status || result ))
popd > /dev/null

exit $exit_status