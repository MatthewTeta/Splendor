#!/bin/bash

mkdir -p build
cmake -Bbuild -GNinja --fresh && \
cd build && \
ninja
