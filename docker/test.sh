#!/usr/bin/env bash

set -e
set -o pipefail

export CMAKEOPTIONS="-DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-4.8"

cd /home/penny/deberg
[ -d build ] && rm -rf build
mkdir -p build
cd build
cmake .. $CMAKEOPTIONS

make -j`nproc`
./tests
