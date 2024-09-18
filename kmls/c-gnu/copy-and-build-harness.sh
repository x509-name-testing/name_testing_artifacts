#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

HARNESSDIR=kmls
export GNUDIR=`pwd`/$GNUTLS_VER
export GMPDIR=`pwd`/$GMP_VER
export NETDIR=`pwd`/$NETTLE_VER
export IDN2DIR=`pwd`/$LIBIDN2_VER

# now inject the test harness
mkdir -p $GNUDIR/kmls
cp -R `pwd`/$HARNESSDIR/* ./$GNUTLS_VER/kmls/

pushd ./$GNUTLS_VER/kmls
    make clean; make
    # extract-bc ./nc
    extract-bc ./hv
    extract-bc ./hv-cn

popd
