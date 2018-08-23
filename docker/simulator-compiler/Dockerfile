FROM brewblox/firmware-compiler
CMD rm -rf  /firmware/docker/simulator/target && make APP=cbox PLATFORM=gcc && cp -R /firmware/build/target /firmware/docker/simulator/target
