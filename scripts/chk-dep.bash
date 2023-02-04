#!/bin/bash

#customize for GTISDK dependevcies
dep="
libx11-dev
libgtk2.0-dev
pkg-config
python-numpy 
libavcodec-dev
libavformat-dev
libswscale-dev
libjpeg-dev
libpng12-dev
libjasper-dev
python-numpy
python-tk
"

for pkg in $dep; do
    dpkg-query -s $pkg &> /dev/null
    if [ $? != 0 ]; then
        echo "$pkg uninstalled"
    fi
done   
