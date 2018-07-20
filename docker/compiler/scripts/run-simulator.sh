cd /firmware/build/target/cbox-gcc
touch device_key.der server_key.der
rm -rf coverage
mkdir -p coverage

echo "running lcov initial"
lcov -q --capture --initial --directory ../user --output-file coverage/base.info
./cbox -i 123456789012345678901234

echo "running lcov"
lcov -q --capture --directory ../user --output-file coverage/test.info

echo "combining tracefiles"
lcov -q --add-tracefile coverage/base.info --add-tracefile coverage/test.info --output-file coverage/total.info

echo "filtering tracefiles"
lcov -q --remove coverage/total.info '/boost/*' '/usr/*' 'build/target/*' -o coverage/filtered.info

echo "generating html"
mkdir -p coverage/html
genhtml -q --prefix /firmware/ coverage/filtered.info --ignore-errors source --output-directory=coverage/html