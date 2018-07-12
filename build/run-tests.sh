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

$MY_DIR/../lib/test/obj/runner
status $?

$MY_DIR/../app/cbox/test/obj/runner
status $?