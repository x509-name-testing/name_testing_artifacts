#!/bin/bash

function applySigHack
{
    read -r -p "[?] Do you want to apply signature hack? (y/n) " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        patch bearssl-0.6/src/x509/x509_minimal.c < sighack.patch
    fi
}

applySigHack
