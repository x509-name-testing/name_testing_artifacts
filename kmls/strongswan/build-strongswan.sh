#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

./sighack.sh

pushd $IMPL_NAME

    export WLLVM_CONFIGURE_ONLY=1
    mkdir -p build;
    ./configure --enable-monolithic \
    --enable-static \
    --disable-kernel-netlink  \
    --with-mpz_powm_sec=no \
    --prefix=`pwd`/build

    # need to fix the config.h for some library functions that we don't have
    sed -i 's!^#define HAVE_QSORT_R!#undef HAVE_QSORT_R!g' config.h
    echo "#define NO_CHECK_MEMWIPE" >> config.h

    unset WLLVM_CONFIGURE_ONLY
    make -j$cpuCnt
    make

popd

