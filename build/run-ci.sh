#!/bin/sh

bash build-modules.sh
bash compile-proto.sh
bash build-tests.sh
bash run-tests.sh
bash build-firmware.sh