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
$cmake -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX . && make

