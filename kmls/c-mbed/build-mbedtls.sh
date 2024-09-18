#!/bin/bash

# sig. hack
./sighack.sh

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

# inject test harness
cp -R `pwd`/kmls $SSL_LIB_NAME/kmls
# patch $SSL_LIB_NAME/Makefile < ./Makefile.patch

pushd $SSL_LIB_NAME

    # disable assembly and other intrinsics
    sed -i.bak '/^#define MBEDTLS_HAVE_ASM/{s|^|//|}' include/mbedtls/mbedtls_config.h
    sed -i '/^#define MBEDTLS_AESNI_C/{s|^|//|}' include/mbedtls/mbedtls_config.h
    sed -i '/^#define MBEDTLS_PADLOCK_C/{s|^|//|}' include/mbedtls/mbedtls_config.h

    # make SHARED=1 -j$cpuCnt ### static linking is easier for us
    make -j$cpuCnt

    # extract bitcode from libray files
    pushd library
        for f in `find -name '*.a'`; do
            extract-bc $f
        done
    popd

popd

./copy-and-build-harness.sh
