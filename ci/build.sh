function status()
{
if [[ "$1" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
  exit 1
fi
}

: ${CC:=gcc}
: ${CXX:=g++}
: ${cmake:=cmake}
$cmake  -D CMAKE_C_COMPILER=$CC -D CMAKE_CXX_COMPILER=$CXX . && make VERBOSE=1

