#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the nettle source tree using the tarball"
confirm

rm -rf $
tar zxvf $LIBIDN2_VER.tar.gz