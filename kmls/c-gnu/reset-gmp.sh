#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the gmp source tree using the tarball"
confirm

rm -rf $GMP_VER
tar xvf $GMP_VER.tar.lz