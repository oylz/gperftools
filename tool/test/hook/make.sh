#!/bin/bash

rm /tmp/core/* -rf

rm libfun.so main libhook.so -rf

# LD_PRELOAD only effiective for shared libraries
g++ --std=c++11 -g -fPIC -shared -o libfun.so fun.cpp

g++ --std=c++11 -g -o main main.cpp -L./ -lfun

g++ --std=c++11 -g -fPIC -shared -o libhook.so hook.cpp -ldl





