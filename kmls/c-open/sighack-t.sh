# !/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch openssl-1.1.1t/crypto/evp/pmeth_fn.c < sighack-t.patch
    fi
}

applySigHack
