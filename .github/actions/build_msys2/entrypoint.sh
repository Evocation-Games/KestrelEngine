#!/bin/sh -l

cmake -H. -Bbuild
cmake --build build -- -j2
