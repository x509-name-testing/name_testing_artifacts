#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the nettle source tree using the tarball"
confirm

rm -rf $NETTLE_VER
tar zxvf $NETTLE_VER.tar.gz