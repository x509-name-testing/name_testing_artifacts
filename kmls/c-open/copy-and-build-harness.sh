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

cp -R `pwd`/$HARNESSDIR/* $OPENSSL_VER/kmls/
pushd $OPENSSL_VER
    pushd kmls
        make clean; make
        extract-bc nc
        extract-bc hv
        extract-bc hv-cn
    popd
popd

echo "==> test harness ready in $OPENSSL_VER/kmls"; sleep 1
