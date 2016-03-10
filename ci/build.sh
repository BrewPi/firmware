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

if [ $result -eq 0 ]; then
   mkdir -p deploy
   TAG=$(git tag -l --contains HEAD)
   cp platform/spark/target/controller-core/controller.bin deploy/brewpi-$TAG-core.bin
   cp platform/spark/target/controller-photon/controller.bin deploy/brewpi-$TAG-photon.bin
fi

status $result

./lib/test/obj/runner
status $?

./app/controller/test/obj/runner
status $?
