cd /firmware/app/brewblox/test/obj
mkdir -p test
mkdir -p coverage

echo "resetting lcov counters"
lcov --zerocounters --directory test

echo "running lcov initial"
lcov -q --capture --initial --directory test --output-file coverage/base.info
./runner

echo "running lcov"
lcov -q --capture --directory test --output-file coverage/test.info

echo "combining tracefiles"
lcov -q --add-tracefile coverage/base.info --add-tracefile coverage/test.info --output-file coverage/total.info

echo "filtering tracefiles"
lcov -q --remove coverage/total.info '/boost/*' '/usr/*' 'build/target/*' -o coverage/filtered.info

echo "generating html"
mkdir -p coverage/html
genhtml -q --prefix /firmware/ coverage/filtered.info --ignore-errors source --output-directory=coverage/html