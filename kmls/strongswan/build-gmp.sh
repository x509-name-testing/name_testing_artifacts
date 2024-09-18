#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

pushd gmp-6.2.1

    export WLLVM_CONFIGURE_ONLY=1
    mkdir -p build; ./configure --prefix=`pwd`/build --enable-assembly=no --enable-fft=no --enable-cxx=no
    unset WLLVM_CONFIGURE_ONLY
    make -j$cpuCnt
    make install

    # extract bitcode from the lib file
    extract-bc build/lib/libgmp.so

popd

echo "==> gmp-6.2.1 ready."; sleep 2
