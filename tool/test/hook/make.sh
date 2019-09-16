#!/bin/bash

# LD_PRELOAD only effiective for shared libraries
g++ --std=c++11 -fPIC -shared -o libfun.so fun.cpp

g++ --std=c++11 -o main main.cpp -L./ -lfun

g++ --std=c++11 -fPIC -shared -o libhook.so hook.cpp -ldl





