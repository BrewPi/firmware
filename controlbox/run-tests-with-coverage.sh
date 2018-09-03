#!/bin/bash
pushd build
mkdir -p coverage

echo "resetting lcov counters"
lcov --zerocounters --directory .

echo "running lcov initial"
lcov -q --capture --initial --directory . --output-file coverage/base.info
./runner

echo "running lcov"
lcov -q --capture --directory . --output-file coverage/test.info

echo "combining tracefiles"
lcov -q --add-tracefile coverage/base.info --add-tracefile coverage/test.info --output-file coverage/total.info

echo "filtering tracefiles"
lcov -q --remove coverage/total.info '/usr/*' -o coverage/filtered.info

echo "generating html"
mkdir -p coverage/html
genhtml -q --prefix /firmware/controlbox coverage/filtered.info --ignore-errors source --output-directory=coverage/html
popd