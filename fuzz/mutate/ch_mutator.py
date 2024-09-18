import numpy as np
import pdb
from collections import OrderedDict

from .char_table import *
from .type_str import *
from util.get_str_len import get_length


def add(string, func):
    """Returns a list of all possible additions to the string."""
    result = []
    for i in range(len(string)):
        space_chr_lst = func()
        for c in space_chr_lst:
            result.append(string[:i] + c + string[i:])
    return result

def delete(string, func=None):
    """Returns a list of all possible deletions of the string."""
    result = []
    for i in range(len(string)):
        result.append(string[:i] + string[i+1:])
    return result if result else ['']

def replace(string, func):
    """Returns a list of all possible replacements of the string."""
    result = []
    for i in range(len(string)):
        # for c in CHAR_TABLE:
        space_chr_lst = func()
        for c in space_chr_lst:
            result.append(string[:i] + c + string[i+1:])
    return result

def random(string, func):
    """Returns a list of all possible random mutations of the string."""
    result = []
    for i in range(len(string)):
        # for c in CHAR_TABLE:
        space_chr_lst = func()
        for c in space_chr_lst:
            result.append(string[:i] + c + string[i+1:])
    return result

def do_nothing(string, func=None):
    # for test
    return [string]


func_dict = OrderedDict([
    ("add",add),
    ("delete",delete),
    ("replace",replace),
    ("random",random),
    # ("do_nothing",do_nothing)
])

def mutate_str_value(bstring, weight=[0.25, 0.25, 0.25, 0.25], logger=None) -> list:
    ''' the wight should be a list of 4 numbers, [a, b, c, d], 
    indicating the weight of add, delete, replace, random, respectively.
    and the a+b+c+d should be 1.'''
    idx = np.random.choice(4, p=weight)
    # idx = 0 # for test
    '''
    :strategy-- 0: add, 1: delete, 2: replace, 3: random
    :get_char_func_name-- the name of the function to get the char list'''
    ### now the get char func is hard coded, should be changed to a parameter
    strategy = list(func_dict.items())[idx][1]
    get_char_func_name = np.random.choice(list(get_char_func.values()))
    print(strategy, get_char_func_name)
    if logger:
        logger.info("Mutate str val, strategy: {}, get_char_func_name: {}".format(strategy, get_char_func_name))
    return strategy(bstring, get_char_func_name)

def char_mutator(str_dict, logger=None):
    val_str = str_dict['value']
    print(val_str)
    ret = mutate_str_value(val_str, logger=logger)
    
    res = []
    for i in ret:
        tmp_dict = str_dict.copy()
        tmp_dict['value'] = i
        
        tmp_dict['length'] = get_length(i)
        res.append(tmp_dict)
        print("length: ", tmp_dict['length'], "value: ", tmp_dict['value'])
    return res



if __name__ == '__main__':
    # print(mutate("hello"))
    print(add("hello"), get_ucd_space())