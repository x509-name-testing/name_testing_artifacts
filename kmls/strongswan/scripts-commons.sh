#!/bin/bash

function confirm
{
    read -p "[?] OK to continue? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]
    then
        exit 1
    fi
}

function getCpuThreadCnts
{
    # read cpuinfo to figure out how many
    # logical CPUs are available
    # what a hack
    cat /proc/cpuinfo | grep "model name" | wc -l
}

export ScriptDir="$(dirname "$(realpath "$0")")"

# set environmental variables
export USERCOMPILE=''
export GMP_LIBS="`pwd`/gmp-6.2.1/build/lib"
export LIBGMP=$GMP_LIBS/libgmp.a
export CC=wllvm
export CFLAGS=' -g -O0 -fno-lax-vector-conversions -fno-slp-vectorize -fno-vectorize -Wno-tautological-compare -Wno-unused-variable -Wno-uninitialized -Wno-missing-prototypes -fPIC'
CFLAGS+=" -I$KLEE_INCLUDE -I`pwd`/gmp-6.2.1/build/include"; export CFLAGS;
LDFLAGS+=" -L$KLEE_LIB -lkleeRuntest"; export LDFLAGS;


export IMPL_NAME='strongswan-5.9.7'

