#!/bin/sh

mkdir build

cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j8 --config Release

