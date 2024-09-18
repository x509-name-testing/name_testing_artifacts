#!/bin/bash

source ./scripts-commons.sh

tryInstallPackageIfNotInstalled texinfo 
tryInstallPackageIfNotInstalled libgmp-dev

# build gmp
./build-gmp.sh

# build nettle first
./build-nettle.sh

# # build libidn2
# ./build-libidn2.sh

# finally build gnutls
./build-gnutls.sh

# copy and build test harness
# ./copy-and-build-harness.sh