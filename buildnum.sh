#!/bin/bash

echo $1

LD_LIBRARY_PATH=~/commonlib_debug_64/:~/Qt/5.15.2/gcc_64/lib:~/Qt/5.15.2/gcc_64/lib
TEMPLATE=~/buildnumber.h.tmp
OUTPUTFILE=buildnumber.h
PROJECT=Insole02

LD_LIBRARY_PATH=$LD_LIBRARY_PATH ~/buildnum_ -t $TEMPLATE -o $OUTPUTFILE -p $PROJECT
