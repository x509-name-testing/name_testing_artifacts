#!/bin/bash

# sig. hack
./sighack.sh


source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`


cd $SSL_LIB_NAME

    # configure phase
    export WLLVM_CONFIGURE_ONLY=1
    mkdir -p build;
    ./configure --enable-shared=no --enable-opensslextra \
    --enable-static=yes --enable-filesystem=no \
    --enable-debug=yes \
    --prefix=`pwd`/build
    # --enable-inline=no \


    # building phase
    unset WLLVM_CONFIGURE_ONLY
    make -j$cpuCnt
    make install

    # we have to cheat a little to copy internal.h to build/include
    # this is to make writing a test harness easier
    for f in `find -name 'internal.h'`; do
        cp $f build/include/$f
    done

cd -


echo "==> $SSL_LIB_NAME ready"; sleep 3
