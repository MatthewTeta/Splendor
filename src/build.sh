#!/bin/bash

mkdir -p build
cmake -Bbuild -GNinja $@ && \
cd build && \
ninja && \
./Splendor
