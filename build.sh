#!/bin/bash
export CXX=/usr/bin/clang++
export CC=/usr/bin/clang
mkdir -p build
cd build
if [ ! -f Makefile ]; then
  cmake ..
fi

make -j