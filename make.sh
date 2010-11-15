#!/bin/bash

mkdir -p bin
gcc -O2 -std=c99 -o bin/image-nuggets-c image-nuggets.c image-nuggets-util.c

