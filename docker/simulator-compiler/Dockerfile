FROM brewblox/firmware-compiler
WORKDIR /firmware/build
CMD rm -rf  /firmware/docker/simulator/target \
  && make clean APP=brewblox PLATFORM=gcc \
  && make APP=brewblox PLATFORM=gcc \
  && cp -r /firmware/build/target /firmware/docker/simulator/target