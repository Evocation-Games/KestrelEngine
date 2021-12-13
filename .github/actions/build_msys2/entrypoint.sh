#!/bin/sh -l

cmake -G"MSYS Makefiles" -H. -Bbuild
cmake -G"MSYS Makefiles" --build build -- -j2
