import numpy as np
from util.get_str_len import get_length

issuer_oid_lst = [
    "2.5.4.6",  #"Country"
    "2.5.4.10", #"OrganizationalUnit"
    "2.5.4.11", #"Organization"
    "2.5.4.3",   #"CommonName"
    "2.5.4.7",   # "Locality"
    "2.5.4.8",   # "State"
    "2.5.4.12",  # "Title"
    "2.5.4.42",  # "GivenName"
    "2.5.4.43",  # "Initials"
    "2.5.4.4",   # "SurName"
    "0.9.2342.19200300.100.1.25",    # "DomainComponent"
    "1.2.840.113549.1.9.1",  # "EMail"
    "2.5.4.9",   # "StreetAddress"
    "1.2.840.113549.1.9.2",  #"UnstructuredName"
    "1.2.840.113549.1.9.8",  #"UnstructuredAddress"
    "2.5.4.5",   # "DeviceSerialNumber"
]

san_oid_lst = [
    ""
]


def oid_mutator(triplet: list, logger=None):
    oid = triplet['oid']
    tmp_oid_lst = issuer_oid_lst.copy()

    for i in tmp_oid_lst:
        if i == oid:
            tmp_oid_lst.remove(i)
            break
    mutated_oid = np.random.choice(tmp_oid_lst)
    ret = triplet.copy()
    ret['oid'] = mutated_oid
    ret['length'] = get_length(ret['value'])
    if logger:
        logger.info(f"Mutated OID: {mutated_oid}")
    return [ret]