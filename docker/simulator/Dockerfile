FROM ubuntu:16.04

RUN apt-get update -q && apt-get install -qy bash gdbserver gdb

COPY target /target/
WORKDIR /target/cbox-gcc
 
RUN touch device_key.der server_key.der

EXPOSE 8332
EXPOSE 2345

CMD ./cbox -i 123456789012345678901234
