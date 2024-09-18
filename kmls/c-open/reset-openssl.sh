#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the openssl source tree using the tarball"
confirm

rm -rf $OPENSSL_VER
tar xvf $OPENSSL_VER.tar.gz
