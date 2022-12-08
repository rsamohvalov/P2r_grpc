#!/bin/bash
if [ ! -d cmake ]
then
    mkdir -p cmake/build
fi
cd cmake/build
cmake -DCMAKE_PREFIX_PATH=/projects/gRPC/install/lib/cmake ../..