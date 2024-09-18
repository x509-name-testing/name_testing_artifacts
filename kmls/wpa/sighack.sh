# !/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch wpa_supplicant-2.10/src/tls/x509v3.c < sighack.patch
    fi
}

applySigHack
