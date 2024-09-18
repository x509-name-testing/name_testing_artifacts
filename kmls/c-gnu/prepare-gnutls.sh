#!/bin/bash

# sig. hack
./sighack.sh

# time hack

# hash bucket hack

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

NETTLE=`pwd`/$NETTLE_VER/build
GMP=`pwd`/$GMP_VER/build

pushd $GNUTLS_VER

    sed -i '2985s/^/\/\//' ./lib/priority.c
    sed -i '2999s/^/\/\//' ./lib/priority.c    # temporarily comment out a free() stmt so that example runs

    export WLLVM_CONFIGURE_ONLY=1        # use configure mode of WLLVM

    CFLAGS+=" -I$NETTLE/include -I$GMP/include -I$KLEE_INCLUDE"; export CFLAGS;
    LDFLAGS+=" -L$NETTLE/lib64   -L$GMP/lib -L$KLEE_LIB -lkleeRuntest"; export LDFLAGS;
    # LIBS+="-lkleeRuntest"; export LIBS;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$KLEE_LIB;

    # configure
    PKG_CONFIG_PATH=$NETTLE/lib64/pkgconfig; export PKG_CONFIG_PATH;
    mkdir -p build; ./configure --disable-doc --disable-cxx \
    --disable-hardware-acceleration \
    --with-included-libtasn1 \
    --with-included-unistring \
    --without-p11-kit \
    --without-zlib \
    --prefix=`pwd`/build \
    --enable-static \
    --host=x86_64-linux-gnu \
    --disable-tests

    # now config.h and Makefile are both generated

    # # # let's say we don't have pthread
    # sed -i 's/\/\* \#undef HAVE\_LIBPTHREAD \*\//\#undef HAVE\_LIBPTHREAD/' ./config.h
    # sed -i 's/\#define HAVE\_PTHREAD\_MUTEX\_LOCK 1/\#define HAVE\_PTHREAD\_MUTEX\_LOCK 0/' ./config.h

popd

