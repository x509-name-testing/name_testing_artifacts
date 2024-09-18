# !/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch mbedtls-3.3.0/library/pk.c < sighack.patch
    fi
}

applySigHack
