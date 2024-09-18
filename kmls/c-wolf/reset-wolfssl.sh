#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the wolfssl source tree using the tarball"
confirm

rm -rf $SSL_LIB_NAME
unzip $SSL_LIB_NAME.zip
# tar zxvf $SSL_LIB_NAME.tar.gz
