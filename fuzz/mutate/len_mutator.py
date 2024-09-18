import numpy as np

from util.get_str_len import int_from_bytes

attr_max_length = {
    'country': 2,
    'state': 128,
    'locality': 128,
    'organization': 64,
    'organizational_unit': 64,
    'common_name': 64,
    'email': 64,
    'serial_number': 64,
    'street_address': 128,
    'postal_code': 16,
    'domain_component': 64,
    'title': 64,
    'surname': 64,
    'given_name': 64,
    'initials': 64,
    'generation_qualifier': 64,
    'dn_qualifier': 64,
    'pseudonym': 64,
    'unstructured_name': 64,
    'unstructured_address': 64,
    'business_category': 64,
    'jurisdiction_country': 2,
    'jurisdiction_state': 128,
    'jurisdiction_locality': 128,
    'jurisdiction_street_address': 128,
    'jurisdiction_postal_code': 16,
    'organization_identifier': 64,
}

from util.get_str_len import get_length

def length_mutator(triplet: list, status=False, logger=None) -> int:
    # original_length = triplet['length']
    # original_value = int_from_bytes(triplet['value'])
    original_value = triplet['value']
    # here we need to mutate the length of value
    # and use the get_length() func to calculate the length of the new value

    ## get random length, we assume the length of the value is less than 255
    random_length = np.random.randint(0, 255)
    ## padding the value with null

    # import pdb; pdb.set_trace()

    new_value = original_value + b'\x00' * (random_length - len(original_value))
    ## calculate the length of the new value
    new_length = get_length(new_value)

    ret = triplet.copy()
    ret['length'] = new_length
    ret['value'] = new_value
    if logger:
        logger.info(f"Mutated length: {triplet['length']} -> {new_length}")
    return [ret]