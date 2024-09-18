#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the mbedtls source tree using the tarball"
confirm

rm -rf $SSL_LIB_NAME
tar xvf $SSL_LIB_NAME.tar.gz
