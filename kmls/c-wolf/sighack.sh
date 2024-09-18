# !/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        # patch wolfssl-5.1.1/wolfcrypt/src/asn.c < sighack.patch
        patch wolfssl-5.5.4/wolfcrypt/src/asn.c < sighack-5.5.4.patch
    fi
}

applySigHack
