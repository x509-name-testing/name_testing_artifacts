#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the gmp source tree using the tarball"
confirm

rm -rf $IMPL_NAME
tar xvf $IMPL_NAME.tar.gz