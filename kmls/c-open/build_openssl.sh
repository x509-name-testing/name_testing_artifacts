#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

# do sighack
./sighack-t.sh

# do code change
./codechange-t.sh


pushd $OPENSSL_VER
    mkdir -p build
    ./Configure linux-x86_64 -DNO_ATOMIC --prefix=`pwd`/build --openssldir=`pwd`/build no-asm no-threads no-hw -g -O0 -L$KLEE_LIB -lkleeRuntest -I$KLEE_INCLUDE
    
    make -j$cpuCnt

    mkdir -p kmls
    cp ../kmls/* kmls/

popd



echo "==> $OPENSSL_VER ready"; sleep 1
