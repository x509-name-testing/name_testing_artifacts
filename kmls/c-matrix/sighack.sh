# !/bin/bash

# Daniel Y.

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch matrixssl-4-3-0-open/crypto/pubkey/pubkey_verify.c < sighack-4-3.patch
        patch matrixssl-4-3-0-open/crypto/keyformat/x509.c < sighack-4-3-2.patch
    fi
}

applySigHack
