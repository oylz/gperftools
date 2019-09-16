#!/bin/bash


export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH

export LD_PRELOAD=`pwd`/libhook.so

./main aa.txt 12345



