FROM ubuntu:18.04

COPY target/brewblox-gcc /target/brewblox-gcc
WORKDIR /target/brewblox-gcc

RUN touch device_key.der server_key.der eeprom.bin

# status page
EXPOSE 80
# brewblox protocol
EXPOSE 8332
# mdns
EXPOSE 5353/udp
# websocket display emulation
EXPOSE 7376

CMD ./brewblox -i 123456789012345678901234
