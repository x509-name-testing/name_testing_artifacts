#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

# ./reset.sh
./sighack.sh

cd $SSL_LIB_NAME
    make -j$cpuCnt
    # cp ./build/brssl ../
cd -

echo "==> $SSL_LIB_NAME ready"; sleep 3
