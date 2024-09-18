#!/bin/bash

sed -i '/^CONF = /s/Unix/UnixClang/' ./bearssl-0.6/mk/SingleUnix.mk
sed -i '/^CC = /s/clang/wllvm/' ./bearssl-0.6/conf/UnixClang.mk
sed -i '/^LD = /s/clang/wllvm/' ./bearssl-0.6/conf/UnixClang.mk
sed -i '/^LDDLL = /s/clang/wllvm/' ./bearssl-0.6/conf/UnixClang.mk

sed -i '/^CFLAGS = /s/-W -Wall -Os -fPIC/-g -O0 -fno-slp-vectorize -fno-vectorize -W -Wall -fPIC/' ./bearssl-0.6/conf/Unix.mk
