#!/bin/bash

rm /tmp/core/* -rf

g++ --std=c++11 -g -finstrument-functions -finstrument-functions-exclude-file-list=string -o main main.cpp instrument.cpp -ldl






