#!/bin/bash

# sig. hack
./sighack.sh

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

NETTLE=`pwd`/$NETTLE_VER/build
GMP=`pwd`/$GMP_VER/build
IDN2=`pwd`/$LIBIDN2_VER/build

pushd $GNUTLS_VER

    CFLAGS+=" -I$NETTLE/include -I$GMP/include -I$IDN2/include"; export CFLAGS;
    LDFLAGS+=" -L$NETTLE/lib64 -L$GMP/lib -L$IDN2/include"; export LDFLAGS;
    # LIBS+="-lkleeRuntest"; export LIBS;
    # export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$KLEE_LIB;
    # PKG_CONFIG_PATH="${NETTLE}/lib64/pkgconfig:${IDN2}/lib64/pkgconfig"; export PKG_CONFIG_PATH;
    PKG_CONFIG_PATH="${NETTLE}/lib64/pkgconfig"; export PKG_CONFIG_PATH;
    echo $PKG_CONFIG_PATH

    mkdir -p build; ./configure --disable-doc \
    --disable-hardware-acceleration \
    --with-included-libtasn1 \
    --with-included-unistring \
    --without-p11-kit \
    --without-zlib \
    --prefix=`pwd`/build \
    --enable-static \
    --disable-shared \
    --disable-doc \
    --disable-threads --without-libpthread-prefix

    make -j$cpuCnt
    make install
    extract-bc build/lib/libgnutls.a  # extract bitcode for the shared lib

popd

echo "==> $GNUTLS_VER ready"; sleep 1
