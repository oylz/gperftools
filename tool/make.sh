#!/bin/bash

rm bin -rf
mkdir bin

g++ --std=c++11 -o bin/cpu_profile_parser cpu_profile_parser.cpp



