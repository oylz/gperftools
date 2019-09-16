#!/bin/bash

# LD_LIBRARY_PATH only effictive  for shared libraries 
gcc -fPIC -shared -o libfun.so fun.c

gcc -o main main.c -L./ -lfun

gcc -fPIC -shared -o libhook.so hook.c -ldl



