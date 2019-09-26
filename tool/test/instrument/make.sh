#!/bin/bash

rm /tmp/core/* -rf

IINCLUDE="-I/usr/local/xyz/testing/kk/nn/oo/mm"
LLIB="-L/usr/local/xyz/testing/kk/nn/oo/mm/bb -lmml"
#g++ --std=c++11 -ggdb -O2 -finstrument-functions -finstrument-functions-exclude-file-list=string -o main $IINCLUDE main.cpp instrument.cpp -ldl -lunwind -lboost_thread -lboost_system $LLIB




g++ --std=c++11 -ggdb -O2 -o main $IINCLUDE main.cpp -ldl -lunwind -lboost_thread -lboost_system $LLIB
