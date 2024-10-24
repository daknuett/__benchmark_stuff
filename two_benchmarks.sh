#!/usr/bin/env bash

CXX=g++ meson setup build_gcc
CXX=clang++ meson setup build_clang

meson compile -C build_gcc
meson compile -C build_clang

echo "RUNNING USING GCC"
./build_gcc/add_bench

echo "RUNNING USING CLANG"
./build_clang/add_bench
