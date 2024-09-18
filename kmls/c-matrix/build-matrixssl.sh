#!/bin/bash

# sig. hack
./sighack.sh

# # current time hack
# ./ctimehack.sh

# # DN hash hack
# ./hash-dn-hack.sh

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

cp -R kmls $SSL_LIB_NAME/apps

pushd $SSL_LIB_NAME

    # disable optimization O3
    sed -i 's/OPT:=-O3/OPT:=-O0/g' ./core/makefiles/detect-and-rules.mk || exit -1

    # disable the use of AESNI intrinsics
    sed -i 's/defined(__AES__)/0/g' ./crypto/layer/layer.h  || exit -1

    # disable the CHACHA stuff since the low-lv impl uses intrinsincs
    sed -i 's/#  define USE_CHACHA20_POLY1305_IETF/#undef USE_CHACHA20_POLY1305_IETF/g' ./configs/default/cryptoConfig.h  || exit -1
    sed -i 's/#  define USE_ED25519/#undef USE_ED25519/g' ./configs/default/cryptoConfig.h  || exit -1
    sed -i 's/#  define USE_TLS_CHACHA20_POLY1305_SHA256/#undef USE_TLS_CHACHA20_POLY1305_SHA256/g' ./configs/default/matrixsslConfig.h || exit -1
    
    # somehow the algorithmTest.c hardcoded some CHACHA stuff, so ...
    echo "int main() {}" > crypto/test/algorithmTest.c
    
    # sed -i 's/#  define USE_MATRIX_CHACHA20_POLY1305_IETF/#undef USE_MATRIX_CHACHA20_POLY1305_IETF/g' ./crypto/layer/layer.h
    # sed -i 's/# ifdef USE_CHACHA20_POLY1305_IETF/# if 0/g' ./crypto/cryptoApi.h

    # also disable some other CFLAGS related to SSE and carry-less multiplication
    # they doesn't seem to affect us (since clang doesn't recognize them anyway)
    # doing this is just to stop clang from nagging about unrecognized flags
    sed -i 's/ -mpclmul//g' ./core/makefiles/detect-and-rules.mk || exit -1
    sed -i 's/ -msse4.1//g'  ./core/makefiles/detect-and-rules.mk  || exit -1
    sed -i 's/=-maes/=/g'  ./core/makefiles/detect-and-rules.mk  || exit -1
    sed -i 's/=-msse2/=/g' ./crypto/Makefile  || exit -1
    sed -i 's/=-mssse3/=/g' ./crypto/Makefile  || exit -1
    sed -i 's/=-mavx2/=/g' ./crypto/Makefile  || exit -1

    sed -i 's/ -print-multiarch//g'  ./core/makefiles/detect-and-rules.mk  || exit -1

    # modify the Makefile to accomodate our test harness
    sed -i 's/apps\/dtls/apps\/kmls/g' ./Makefile

    # make
    make -j$cpuCnt

    # # enable crypto trace for easy debugging
    # sed -i 's/\/\* #define USE_CRYPTO_TRACE \*\//#define USE_CRYPTO_TRACE/g' ./crypto/cryptoConfig.h
    # # enable SHA-1 signed certs ...
    # sed -i 's/\/\* #define ENABLE_SHA1_SIGNED_CERTS \*\//#define ENABLE_SHA1_SIGNED_CERTS/g' ./crypto/cryptoConfig.h

    # make -j$cpuCNT

popd

# ./copy-and-build-harness.sh

echo "==> $SSL_LIB_NAME ready"; sleep 1
