#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

pushd ${LIBIDN2_VER}
    export WLLVM_CONFIGURE_ONLY=1
    mkdir -p build; ./configure --prefix=`pwd`/build \
    --with-included-libunistring \
    --disable-shared \
    --without-libiconv-prefix \
    --without-libintl-prefix \
    --disable-doc
    unset WLLVM_CONFIGURE_ONLY
    make -j$cpuCnt
    make install
    
    # somehow on some systems it has only lib, not lib64
    if [ ! -d "build/lib64" ]; then

        # what do we do?
        # we create a symbolic link so that we can continue
        # what a hack
        ln -rs build/lib build/lib64
    fi
    
    # extract bitcode from the lib files
    for f in `ls ./build/lib64/*.a`; do extract-bc $f; done
    
popd

echo "==> ${LIBIDN2_VER} ready."; sleep 1
