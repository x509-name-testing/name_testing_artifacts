#!/bin/bash

export ScriptDir="$(dirname "$(realpath "$0")")"

# set environmental variables
export CC=wllvm
export CFLAGS='-g -O0 -fno-slp-vectorize -fno-vectorize'
CFLAGS+=" -I$KLEE_INCLUDE"; export CFLAGS;
export LDFLAGS="";
LDFLAGS+=" -L$KLEE_LIB -lkleeRuntest"; export LDFLAGS;


rm -rf wpa_supplicant-2.10
tar zxvf wpa_supplicant-2.10.tar.gz

cp nc.c wpa_supplicant-2.10/wpa_supplicant/main.c

./sighack.sh

cd wpa_supplicant-2.10
cat > wpa_supplicant/.config << "EOF"
CONFIG_BACKEND=file
CONFIG_CTRL_IFACE=y
CONFIG_DEBUG_FILE=y
CONFIG_DEBUG_SYSLOG=y
CONFIG_DEBUG_SYSLOG_FACILITY=LOG_DAEMON
CONFIG_DRIVER_NL80211=y
CONFIG_DRIVER_WEXT=y
CONFIG_DRIVER_WIRED=y
CONFIG_EAP_GTC=y
CONFIG_EAP_LEAP=y
CONFIG_EAP_MD5=y
CONFIG_EAP_MSCHAPV2=y
CONFIG_EAP_OTP=y
CONFIG_EAP_PEAP=y
CONFIG_EAP_TLS=y
CONFIG_EAP_TTLS=y
CONFIG_IEEE8021X_EAPOL=y
CONFIG_IPV6=y
CONFIG_LIBNL32=y
CONFIG_PEERKEY=y
CONFIG_PKCS12=y
CONFIG_READLINE=y
CONFIG_SMARTCARD=y
CONFIG_WPS=y
CONFIG_TLS=internal
CONFIG_INTERNAL_LIBTOMMATH=y
CFLAGS += -I/usr/include/libnl3
EOF

cd wpa_supplicant

make -j$(nproc) || exit -1

extract-bc wpa_supplicant && echo "extract-bc done"

cd ../..

# cp wpa_supplicant-2.10/wpa_supplicant/wpa_supplicant ./test_verify
