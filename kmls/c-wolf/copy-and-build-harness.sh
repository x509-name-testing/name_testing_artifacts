#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

HARNESSDIR='kmls'

### parse cl-args
while getopts ":d:" opt; do
    case ${opt} in
        d )
            HARNESSDIR=$OPTARG
        ;;
        \? )
            echo "Invalid argument -$OPTARG"
            exit 1
        ;;
        : )
            echo "Option -$OPTARG requires an argument"
            exit 1
        ;;
    esac
done

# build the main library source again, just in case
pushd $SSL_LIB_NAME
    make -j$cpuCnt
    make install
popd

# inject and build test harness
cp $HARNESSDIR/* $SSL_LIB_NAME/kmls
echo $HARNESSDIR > $SSL_LIB_NAME/kmls/CURRENT_HARNESS
pushd $SSL_LIB_NAME/kmls
    make clean; make
    extract-bc ./hv
    extract-bc ./hv-cn
popd

echo "==> $SSL_LIB_NAME ready in $SSL_LIB_NAME/kmls"
