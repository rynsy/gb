#!/usr/bin/env bash
set -e

git submodule update --init --recursive

if [ -d emsdk ]; then
    echo "emsdk already exists"
else
    git clone https://github.com/emscripten-core/emsdk.git
fi

cd emsdk
./emsdk install latest
./emsdk activate latest
cd ..

