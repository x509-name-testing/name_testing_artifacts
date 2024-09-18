#!/bin/bash

source ./scripts-commons.sh
cpuCnt=`getCpuThreadCnts`

HARNESSDIR="kmls"

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

# inject test harness
mkdir -p $SSL_LIB_NAME/kmls
cp -R `pwd`/$HARNESSDIR/* $SSL_LIB_NAME/kmls/

pushd $SSL_LIB_NAME/kmls/
	make clean; make	
 	
	extract-bc nc
	extract-bc hv
	extract-bc hv-cn

popd

echo "==> $SSL_LIB_NAME ready"; sleep 1