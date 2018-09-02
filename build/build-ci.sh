#!/bin/sh
bash ../app/brewblox/proto/generate_proto_cpp.sh
bash ../app/brewblox/proto/generate_proto_test_cpp.sh
bash build-tests.sh
bash run-tests.sh
bash build-firmware.sh