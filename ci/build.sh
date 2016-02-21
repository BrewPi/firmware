function status()
{
if [[ "$1" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
  exit 1
fi
}

pushd lib/test
make
result=$?
popd
status $result

pushd app/controller/test
make
result=$?
popd
status $result

pushd platform/spark 
./build-all.sh
result=$?
popd

status $result

./boost_test/obj/runner
status $?
