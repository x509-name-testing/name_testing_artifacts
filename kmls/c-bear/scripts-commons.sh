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

export SSL_LIB_NAME='bearssl-0.6'

# export CC=gcc
export CC=wllvm
export CFLAGS='-g -O0 -fno-slp-vectorize -fno-vectorize'
CFLAGS+=" -DNO_MULTITHREADING -DPS_NO_ASM -DNO_PS_STAT_CL"
CFLAGS+=" -I$KLEE_INCLUDE";
export CFLAGS;
LDFLAGS+=" -L$KLEE_LIB -lkleeRuntest"; export LDFLAGS;
