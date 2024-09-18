#!/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch gnutls-3.7.4/lib/pubkey.c < sighack.patch
    fi
}

applySigHack
