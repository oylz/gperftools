#!/bin/bash

rm /tmp/core/* -rf

g++ --std=c++11 -ggdb -O2 -finstrument-functions -finstrument-functions-exclude-file-list=string -o main main.cpp instrument.cpp -ldl -lunwind -lboost_thread -lboost_system






