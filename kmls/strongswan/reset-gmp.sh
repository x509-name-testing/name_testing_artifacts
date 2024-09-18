#!/bin/bash

source ./scripts-commons.sh

echo "[!] I will:"
echo "    1) overwrite the gmp source tree using the tarball"
confirm

rm -rf gmp-6.2.1
tar xvf gmp-6.2.1.tar.xz