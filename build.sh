#!/bin/sh

# Makes a Clean Build

mkdir -p bin obj
make clean
make ca5 && ./bin/ca5