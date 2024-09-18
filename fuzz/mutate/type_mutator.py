import numpy as np
import pdb

from .type_str import *
from .char_table import *
from util.get_str_len import get_length

# the oid is in the oid_dict of type_str
def mutate_str_type(triplet: list, logger=None, weight=[1/(len(oid_dict)-1)]*(len(oid_dict)-1)) -> list:
    ret = triplet.copy()
    origin_type = triplet['tag']
    tmp_dict = oid_dict.copy()
    
    # remove the origin type
    for key in tmp_dict.keys():
        if tmp_dict[key] == origin_type:
            del tmp_dict[key]
            break
    
    # import pdb; pdb.set_trace()
    # get the mutated type
    mutated_type = np.random.choice(list(tmp_dict.keys()), p=weight)
    ret['tag'] = oid_dict[mutated_type]

    # write the length attribut to bytes in triplet 
    ret['length'] = get_length(ret['value'])
    if logger:
        logger.info(f"Mutated tag: {oid_dict[mutated_type]}")
    # pdb.set_trace()
    return [ret]