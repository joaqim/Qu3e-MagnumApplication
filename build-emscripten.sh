#!/usr/bin/env sh
mkdir build-emscripten &>/dev/null && cd build-emscripten 

cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../toolchains/generic/Emscripten-wasm.cmake \
    -DCMAKE_PREFIX_PATH=/usr/lib/emscripten/system
    #-DCMAKE_INSTALL_PREFIX=/srv/http/emscripten
    #-DCMAKE_INSTALL_PREFIX=/srv/http/emscripten
cmake --build .
#cmake --build . --target install
