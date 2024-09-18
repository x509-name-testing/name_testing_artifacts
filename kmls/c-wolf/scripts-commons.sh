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
export CC=wllvm
export CFLAGS='-g -O0 -fno-slp-vectorize -fno-vectorize'
CFLAGS+=" -DTFM_NO_ASM"
export CFLAGS;


export SSL_LIB_NAME='wolfssl-5.5.4'
