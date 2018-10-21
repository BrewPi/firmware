FROM ubuntu:18.04

COPY target /target/
WORKDIR /target/brewblox-gcc

RUN touch device_key.der server_key.der

# status page
EXPOSE 80
# brewblox protocol
EXPOSE 8332
# mdns
EXPOSE 5353/udp

CMD ./brewblox -i 123456789012345678901234
