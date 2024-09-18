# !/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch strongswan-5.9.7/src/libstrongswan/plugins/gmp/gmp_rsa_public_key.c < sighack.patch
    fi
}

applySigHack
