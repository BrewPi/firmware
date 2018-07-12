FROM ubuntu:16.04

WORKDIR /simulator

COPY cbox /simulator/
 
RUN touch /simulator/device_key.der /simulator/server_key.der

EXPOSE 8332

CMD ./cbox -i 123456789012345678901234
