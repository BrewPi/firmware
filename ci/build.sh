
pushd platform/spark 
./build-all.sh
ex=$?
popd

exit $ex