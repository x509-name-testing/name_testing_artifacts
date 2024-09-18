#!/bin/bash

function packageInstalled
{
    dpkg-query -W -f='${Status}' $1 2>/dev/null | grep -c 'ok installed'
}

function confirm
{
    read -p "[?] OK to continue? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]
    then
        exit 1
    fi
}

function tryInstallPackageIfNotInstalled
{
    if [[ $(packageInstalled "$1") -eq 0 ]]; then        
        echo "[!] I will now do this:"
        echo "    sudo apt-get install $1"
        confirm
        sudo apt-get install $1
    else
        echo "$1 is installed; we are good."
    fi
}

function getCpuThreadCnts
{
    # read cpuinfo to figure out how many
    # logical CPUs are available
    # what a hack
    cat /proc/cpuinfo | grep "model name" | wc -l
}

export CC=wllvm
export CFLAGS='-g -O0 -fno-slp-vectorize -fno-vectorize'
CFLAGS+=" -I$KLEE_INCLUDE"; export CFLAGS;
export LDFLAGS="";
LDFLAGS+=" -L$KLEE_LIB -lkleeRuntest"; export LDFLAGS;

export NETTLE_VER=nettle-3.6
# export NETTLE_VER=nettle-3.8
export GMP_VER=gmp-6.2.1
export GNUTLS_VER=gnutls-3.7.4
# export GNUTLS_VER=gnutls-3.7.9
# export GNUTLS_VER=gnutls-3.8.0
export LIBIDN2_VER=libidn2-2.3.4