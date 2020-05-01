#!/bin/bash

# Quick function which exits if the last command ran fails. Intended to be used
# after compiling something.
function compileCheckError() {
    if [ ! $? -eq 0 ]; then
        echo "Compilation failed, exiting."

        exit 1
    fi
}

PROJECT_DIR=$(git rev-parse --show-toplevel)
FILES=$PROJECT_DIR/src/main.cpp

mkdir -p build

pushd build
gcc -o gamjam -Wall -Werror -std=c++11 "$FILES" $(sdl2-config --cflags --libs)

compileCheckError

./gamjam
popd
