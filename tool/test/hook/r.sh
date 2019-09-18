#!/bin/bash


export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH

export LD_PRELOAD=`pwd`/libhook.so
#export LD_DEBUG=libs 

./main aa.txt 12345



