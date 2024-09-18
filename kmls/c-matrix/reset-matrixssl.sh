#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the matrixssl source tree using the tarball"
confirm

rm -rf $SSL_LIB_NAME
unzip $SSL_LIB_NAME.zip
