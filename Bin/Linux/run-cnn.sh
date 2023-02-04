#!/bin/bash

if [ -z $1 ]; then
    if [ -z ${GTISDKPATH} ]; then
        cat <<EOF
This is the command to test GTISDK and GTI hardward functions.

Usage: $0 GTISDKPATH

  GTISDKPATH : You need to setup GTISDKPATH to piont out the path of data/Model file.

EOF
        exit 1
    fi
else
    export GTISDKPATH=$1
fi


#setup default
GTISDKROOT=$PWD/../../
export CPU_ARCH=x86_64
export OS_ARCH=Linux

#setup library path
export LD_LIBRARY_PATH=$GTISDKROOT/Lib/${OS_ARCH}/${CPU_ARCH}:$GTISDKROOT/Lib/${OS_ARCH}/OpenCV/${CPU_ARCH}:$LD_LIBRARY_PATH
./${CPU_ARCH}/cnnSample
