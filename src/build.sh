#!/bin/bash

mkdir -p
cd build && cmake .. && make -j $(nprocs)
