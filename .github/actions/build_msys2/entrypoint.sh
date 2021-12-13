#!/bin/sh -l

printenv
which cmake


cmake -H. -Bbuild
cmake --build build -- -j2
