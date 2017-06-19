#!/bin/sh

cd ./build
rm -r *
cmake ..
make -j8
mv ./src/viewer-qt/qtlasviewer .
./qtlasviewer
cd ..
