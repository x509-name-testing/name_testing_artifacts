from .len_mutator import length_mutator
from .ch_mutator import char_mutator
from .type_mutator import mutate_str_type
from .oid_mutator import oid_mutator
from util.get_str_len import get_length, tlv_parser, int_to_bytes
from util.writer import writeIssuer2dict, writeSAN2dict
from harness import run_harness, run_harnesses

import os
import binascii
import numpy as np
from collections import OrderedDict
from pprint import pprint


import logging
from datetime import datetime
now = datetime.now()
logging.basicConfig(filename=f'./log/{now.month}_{now.day}_{now.hour}_{now.minute}.log', level=logging.INFO)
logger = logging.getLogger(__name__)

count = [ 1, 1, 1, 1 ] # init [type, len, val, oid], indicate the success count of each mutation rule
# weight = [ 0.3, 0.3, 0.4 ] # [type, len, val], indicate the posibility of each mutation rule
base_vector = OrderedDict() # base vector collect accepted mutations
all_mutated = [] # hashed mutated list
cur_out_path = ""

def get_rand_num():
    # return a 8-digit random number
    return binascii.b2a_hex(os.urandom(4)).decode()

def get_rand_path(out):
    # out should not be None
    while(True):
        tmp = out + "/" + get_rand_num()
        if not os.path.exists(tmp):
            os.mkdir(tmp)
            return tmp

def update_base_vec(base_vector, key, value):
    if key in base_vector:
        base_vector[key] = value
    else:
        base_vector.update({key: value})

def del_base_vec(base_vector, key):
    if key in base_vector:
        base_vector.pop(key)
    else:
        raise("KEY_ERR")

def search_base_vec(base_vector, val):
    if base_vector:
        for key, value in base_vector.items():
            if value == val:
                return key
    return None

def hash_check_dict_duplicate(all_mutated, item):
    if hash(frozenset(item.items())) in all_mutated:
        return True
    else: return False

def update_mutated_list(all_mutated, item):
    all_mutated.append(hash(frozenset(item.items())))

def fuzz_mutator(certdict, mode="mix", field='issuer', path=None, out=None, dn=None, harness_name="open", triplet=None) -> list:
    ### here is the start of the fuzz_mutator function, the input is a dict of the certificate
    ### the output is a list of the mutated certificates (path)
    ''' mode: "mix", "val", "type", "len"'''
    global base_vector, all_mutated, cur_out_path, logger
    tbs = 'tbsCertificate'
    if triplet == None: # triplet is not provided, we need to extract it from certdict
        triplet = {} # init
        if field == 'issuer' or field == 'subject':
            # import pdb; pdb.set_trace()
            if len(certdict[tbs][field][1][0]) == 1:
                # example: {'type': '2.5.4.3', 'value': bytearray(b'\x13\x02aa')}
                oid = certdict[tbs][field][1][0][0]['type']
                hex = certdict[tbs][field][1][0][0]['value']
                for tag, length, value in tlv_parser(hex): pass
                # example: triplet = {'oid': '2.5.4.3', 'tag': 19, 'length': b'\x02', 'value': b'aa'}
                triplet = {'oid': oid, 'tag':tag, 'length':length, 'value':value}
                # if triplet not in base_vector: # check if the triplet is in the base_vector
                #     update_base_vec(base_vector, triplet['oid'], triplet)
                # if search_base_vec(base_vector, triplet) == None:
        elif field == 'SAN':
            for idx, item in enumerate(certdict[tbs]['extensions']):
                if item['extnID'] == '2.5.29.17': # subjectAltName in extensions
                    for t, l, v in tlv_parser(item['extnValue']): pass
                    if v: 
                        for tag, length, value in tlv_parser(v): pass
                    else: 
                        tag, length, value = None, None, None
                    triplet = {'oid': '2.5.29.17','tag':tag, 'length':length, 'value':value}
                    # import pdb; pdb.set_trace()
                    break
        else:
            raise("Note: Multi attribute is not supported yet.")
        if len(base_vector) == 0:
            update_base_vec(base_vector, len(base_vector), triplet)
        # import pdb; pdb.set_trace()
    else:
        pass # triplet is provided

    print("triplet:", triplet)
    mutate_ret, cur_out_path = mutate(certdict, mode=mode, field=field, out=out, triplet=triplet, base_path=path)
    subject_executor(mutate_ret, path, harness_name, cur_out_path, dn, logger)

mutation_rules = {
    0: mutate_str_type,
    1: length_mutator,
    2: char_mutator,
    3: oid_mutator,
}    

def mutate(certdict, mode="mix", field='issuer', out=None, triplet=None, base_path=None):
    global base_vector, all_mutated, cur_out_path, logger
    mutate_ret = []
    rule_idx = -1 # init
    if triplet != []:
        if mode != 'mix':
            if mode == 'type': 
                if field == "issuer": mutate_ret = mutate_str_type(triplet); rule_idx = 0
                else: mutate_ret = None # SAN do not have string type mutation
            elif mode == 'len':mutate_ret = length_mutator(triplet); rule_idx = 1
            elif mode == 'val': mutate_ret = char_mutator(triplet); rule_idx = 2
            elif mode == 'oid': 
                if field == "issuer": mutate_ret = oid_mutator(triplet); rule_idx = 3
                else: mutate_ret = None # SAN do not have string type mutation
            else: raise("MUTATION_RULE_ERR")
        else: # mode == 'mix'
            if field == "issuer": 
                weight = [ w / sum(count) for w in count ]
                # use the weight to decide which mutation rule to use
                rule_idx = np.random.choice([0, 1, 2, 3], p=weight)
            else: # SAN do not have string type mutation
                weight = [ w / sum(count[1:-1]) for w in count[1:-1] ]
                # use the weight to decide which mutation rule to use
                rule_idx = np.random.choice([1, 2,], p=weight)
            print("===============rule:", rule_idx)
            ### without logger
            # mutate_ret = mutation_rules[rule_idx](triplet)
            ### use logger
            mutate_ret = mutation_rules[rule_idx](triplet, logger)
        logger.info("mutation rule: " + str(mutation_rules[rule_idx]))
        logger.info(f"There would be {len(mutate_ret)} mutated certificates in this epoch.")
        mutate_res = [] # results to write back to dict cert
        for item in mutate_ret:
            # tmp_dict = {'type': item['oid'], 'value': int_to_bytes(item['tag']) + item['length'] + str.encode(item['value'])}
            
            # check duplicate
            # if item in all_mutated: pass
            if hash_check_dict_duplicate(all_mutated, item) == True:
                logger.info("Duplicate mutation found.")
                # remove the duplicate
                mutate_ret.remove(item)
                
            else:
                tmp_dict = {'type': item['oid'], 'value': int_to_bytes(item['tag']) + item['length'] + item['value']}
                logger.info("Mutated certificate: " + str(tmp_dict))
                mutate_res.append(tmp_dict)
        if mutate_res:
            cur_out_path = get_rand_path(out)
            # write back to dict
            if field == 'issuer' or field == 'subject':
                writeIssuer2dict(certdict, field, mutate_res, out_path=cur_out_path, path=base_path)
            elif field == 'SAN':
                writeSAN2dict(certdict, field, mutate_res, out_path=cur_out_path, path=base_path)
            else:
                raise("Note: Multi attribute is not supported yet.")
            # update the all_mutated list, record the mutated certificate dict
            for item in mutate_ret:
                update_mutated_list(all_mutated, item)
        
        return mutate_ret, cur_out_path
    else:
        raise("TRIPLET_ERR")

def subject_executor(mutate_ret, path_to_ca, harness_name, cur_out_path, dn=None, logger=None):
    # ### here is the start of connection with the harness, the input is the mutated certificate (path)
    # ### the output is the result 
    # ### TODO: feedback the result to the fuzzer
    global base_vector
    ca = path_to_ca + "/ca.pem"
    ### run harness
    if harness_name != "all": idx_lst, _ = run_harness(harness_name, ca_path=ca, ee_folder=cur_out_path, name=dn, logger=logger) 
    else: 
        idx_dict, _ = run_harnesses(ca_path=ca, ee_path=cur_out_path, name=dn, logger=logger) 

        idx_lst = []
        for i in idx_dict.values():   
            if len(i) < len(mutate_ret):
                idx_lst.extend(i)
        idx_lst = list(set(idx_lst))
    # import pdb; pdb.set_trace()
    for i in idx_lst:
        # print(mutate_ret[i])
        if mutate_ret and (i in mutate_ret):
            if search_base_vec(base_vector, mutate_ret[i]) == None:
                update_base_vec(base_vector, len(base_vector), mutate_ret[i])
    # pdb.set_trace()
    # pprint(base_vector)
    # return base_vector

def runner(certdict, mode="mix", field='issuer', path=None, out=None, harness_name="open", dn=None, triplet=None):
    # run the fuzzer
    global base_vector, all_mutated, logger
    recorded_len = len(base_vector)
    while(True):
        if len(base_vector) == recorded_len:
            fuzz_mutator(certdict, mode=mode, field=field, path=path, out=out, dn=dn, harness_name=harness_name, )
            print("base vec: ", base_vector)
        else: 
            # len(base_vector) > recorded_len:
            for idx in range(len(base_vector) - recorded_len - 1):
                new_vec = base_vector[recorded_len + idx]
                print("="*16, "new_vec:", new_vec, "="*16)
                logger.info("="*16 + "new_vec:" + str(new_vec) + "="*16)
                for i in range(0x1f):
                    logger.info("*"*16 + "mutate with new vec. epoch: %d" % i)
                    logger.info("current vec: " + str(new_vec))
                    
                    fuzz_mutator(certdict, mode=mode, field=field, path=path, out=out, dn=dn, harness_name=harness_name, triplet=new_vec)
                    # if length grows, hang up this loop and start a new one based on the new vector
                    if len(base_vector) > recorded_len:
                        break
        logger.info("base vec: " + str(base_vector))
        recorded_len = len(base_vector)
