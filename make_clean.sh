#!/bin/bash
if [ -d cmake ]
then
    cd cmake/build
    make clean
    cd ..
    cd ..
    rm -rf cmake
    rm -f libP2r.a
fi