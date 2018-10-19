FROM ubuntu:18.04

COPY target /target/
WORKDIR /target/brewblox-gcc

RUN touch device_key.der server_key.der

EXPOSE 8332
EXPOSE 2345

CMD ./brewblox -i 123456789012345678901234
