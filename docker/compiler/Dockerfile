FROM ubuntu:16.04

RUN apt-get update -q && apt-get install -qy git bash curl unzip
RUN apt-get update -q && apt-get install -qy build-essential dfu-util

ENV GCC_ARM_URL="https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q1-update/+download/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2"
ENV GCC_ARM_VERSION="5_3-2016q1"

RUN dpkg --add-architecture i386 && apt-get update -q && apt-get install -qy make isomd5sum bzip2 vim-common libarchive-zip-perl libc6:i386 \
  && curl -o /tmp/gcc-arm-none-eabi.tar.bz2 -sSL ${GCC_ARM_URL} \
  && tar xjvf /tmp/gcc-arm-none-eabi.tar.bz2 -C /usr/local \
  && mv /usr/local/gcc-arm-none-eabi-${GCC_ARM_VERSION}/ /usr/local/gcc-arm-embedded \
  && apt-get remove -qy bzip2 && apt-get clean && apt-get purge \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* /usr/local/gcc-arm-embedded/share

ENV PATH /usr/local/gcc-arm-embedded/bin:$PATH

COPY scripts scripts

ENV BOOST_VERSION=1_65_0
ENV BOOST_HOME=/boost
ENV BOOST_ROOT=$BOOST_HOME/boost_$BOOST_VERSION
RUN mkdir -p $BOOST_HOME && curl -sSL http://downloads.sourceforge.net/project/boost/boost/1.65.0/boost_1_65_0.tar.gz | tar -xz -C $BOOST_HOME 
RUN bash /scripts/build_boost.sh

ENV PROTOC_HOME=/protoc
RUN curl -sSL https://github.com/google/protobuf/releases/download/v3.6.0/protoc-3.6.0-linux-x86_64.zip -o /tmp/protoc.zip \
  && unzip /tmp/protoc.zip -d $PROTOC_HOME
RUN mv $PROTOC_HOME/bin/* /usr/local/bin/
RUN mv $PROTOC_HOME/include/* /usr/local/include/

RUN apt-get update -q && apt-get install -qy python python-pip
RUN pip install protobuf

WORKDIR /firmware/build
CMD bash
