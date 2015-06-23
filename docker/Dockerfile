FROM ubuntu:12.04

RUN apt-get update
RUN apt-get -y dist-upgrade
RUN apt-get -q install -y software-properties-common
RUN apt-get -q install -y python-software-properties
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN add-apt-repository -y ppa:boost-latest/ppa
RUN apt-get update >/dev/null
RUN apt-get -q install -y g++-4.8 cmake
RUN apt-get -q install -y libboost1.54-all-dev
RUN apt-get -q install -y libglm-dev
RUN apt-get -q install -y build-essential

RUN useradd -ms /bin/bash penny
USER penny
ENV HOME /home/penny
WORKDIR /home/penny
