#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

pushd $GMP_VER
    export WLLVM_CONFIGURE_ONLY=1
    mkdir -p build; ./configure --prefix=`pwd`/build --enable-assembly=no --enable-fft=no --enable-cxx=no
    unset WLLVM_CONFIGURE_ONLY
    make -j$cpuCnt
    make install

    # extract bitcode from the lib file
    extract-bc build/lib/libgmp.a
popd

echo "==> $GMP_VER ready."; sleep 1