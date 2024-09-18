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

cp -Rf $HARNESSDIR $IMPL_NAME/src/libstrongswan

# just in case the source tree is modified, we make it again first
pushd $IMPL_NAME
    make -j$cpuCnt
popd

pushd $IMPL_NAME/src/libstrongswan/$HARNESSDIR
    make -j$cpuCnt

    extract-bc nc
popd

echo "==> test harness ready in $IMPL_NAME/src/libstrongswan/$HARNESSDIR"
