#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the gnutls source tree using the tarball"
confirm

rm -rf $GNUTLS_VER
tar xvf $GNUTLS_VER.tar.xz
