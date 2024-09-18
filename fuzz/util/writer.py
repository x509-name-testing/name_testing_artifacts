import os
import sys
from pprint import pprint
# import pdb
import base64
import asn1tools
import textwrap
import copy

from .sig import get_tbscert, load_key, overwrite_sig, get_sig
from util.file_converter import der_to_pem, pem_to_der
from util.get_str_len import get_length

# use multiprocessing to speed up
from concurrent.futures import ProcessPoolExecutor, as_completed

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
RFC5280_ASN_PATH = os.path.join(SCRIPT_DIR, 'rfc5280.asn')
CHAR_PER_LINE = 64

'''for test'''
from .reader import cert2dict
'''for test'''


def dict2cert(Dict, out_path, key_path=None):
    """ input: original cert dict, mutated cert dict list
        output: mutated cert files (.pem)  
        output: mutated cert files (.der)  **signature update**  """
    rfc5280_der = asn1tools.compile_files(RFC5280_ASN_PATH, codec='der')
    encoded = rfc5280_der.encode('Certificate', Dict)
    # print("encoded:", base64.b64encode(encoded))
    # write_PEM_file(base64.b64encode(encoded), out_path)
    write_DER_file(encoded, out_path + ".der")
    if key_path is not None:
        modify_signature(out_path + ".der", key_path)
        der_to_pem(out_path+".der", out_path + ".pem")
    else:
        write_PEM_file(base64.b64encode(encoded), out_path + ".pem")
    return

def write_DER_file(der_string, path):
    """ input: der format byte string
        output: cert file (.der)  """
    if path.endswith('der'):
        with open(path, 'wb') as f:
            f.write(der_string)
        print("Writing to file %s: Done." % (path, ))
    else:
        raise Exception("OUTPUT_SUFFIX_ERROR")

def write_PEM_file(pem_string, path):
    """ input: pem format byte string
        output: cert file (.pem)  """
    pem_string = pem_string.decode("utf-8") 
    cert_prefix = "-----BEGIN CERTIFICATE-----\n"
    cert_suffix = "\n-----END CERTIFICATE-----"
    pem_string = textwrap.fill(pem_string, CHAR_PER_LINE)
    # print(cert_prefix + pem_string + cert_suffix)
    if path.endswith('pem'):
        with open(path, 'w') as f:
            f.writelines(cert_prefix + pem_string + cert_suffix)
        print("Writing to file %s: Done." % (path, ))
    else:
        raise Exception("OUTPUT_SUFFIX_ERROR")

def modify_signature(cert_path, key_path):
    try:
        tbs_cert = get_tbscert(cert_path)
        skey = load_key(key_path)
        new_sig = get_sig(tbs_cert, skey)
        overwrite_sig(cert_path, new_sig)
        print("Certficate %s signature update: Done." % (cert_path,) )
    except Exception as e:
        print(e)
    return 

def writeIssuer2dict(ori_dict: dict, field: str, res: list ,out_path=None, path=None, logger=None) -> None:
    """ input: original cert dict, mutated cert dict list
        output: mutated cert files (.pem)  """
    tbs = 'tbsCertificate'
    if path is not None: key_path = path + "/key.pem"
    else: key_path = None
    # import pdb
    res_lst = []
    for i, d in enumerate(res):
        tmp_dict = copy.deepcopy(ori_dict) # deep copy to avoid changing original dict
        tmp_dict[tbs][field][1][0][0].update(d)
        res_lst.append(tmp_dict)
        print(tmp_dict[tbs][field][1][0][0])
        if logger is not None:
            logger.info("="*16 +"Mutated cert %s: %s" % (i, str(tmp_dict[tbs][field][1][0][0])))

    # use multiprocessing to speed up
    if len(res) != 1:
        workers = min(16, len(res))
        with ProcessPoolExecutor(max_workers=workers) as executor:
            futures = [executor.submit(dict2cert, res_lst[i], out_path + "/" + str(i), key_path) for i in range(len(res_lst))]
            for future in as_completed(futures):
                print(future.result())
    else:
        dict2cert(res_lst[0], out_path + "/" + str(0), key_path)

def writeSAN2dict(ori_dict: dict, field: str, res: list ,out_path=None, path=None, logger=None) -> None:
    """ input: original cert dict, mutated cert dict list
        output: mutated cert files (.pem)  """
    tbs = 'tbsCertificate'
    if path is not None: key_path = path + "/key.pem"
    else: key_path = None

    res_lst = []
    for i, d in enumerate(res):
        tmp_dict = copy.deepcopy(ori_dict) # deep copy to avoid changing original dict
        for idx, item in enumerate(tmp_dict[tbs]['extensions']):
                if item['extnID'] == '2.5.29.17': # subjectAltName in extensions
                    item['extnValue'] = b'0' + get_length(d['value']) + d['value']
                    print(tmp_dict[tbs]['extensions'][idx])
                    break
        # tmp_dict[tbs][field][1][0][0].update(d)
        res_lst.append(tmp_dict)
        if logger is not None:
            logger.info("="*16 +"Mutated cert %s: %s" % (i, str(tmp_dict[tbs][field][1][0][0])))
    # import pdb; pdb.set_trace()
    # use multiprocessing to speed up
    if len(res) != 1:
        workers = min(16, len(res))
        with ProcessPoolExecutor(max_workers=workers) as executor:
            futures = [executor.submit(dict2cert, res_lst[i], out_path + "/" + str(i), key_path) for i in range(len(res_lst))]
            for future in as_completed(futures):
                print(future.result())
    else:
        dict2cert(res_lst[0], out_path + "/" + str(0), key_path)


if __name__ == '__main__':
    # testDict = cert2dict(sys.argv[1])
    # dict2cert(testDict, sys.argv[2])
    modify_signature(sys.argv[1], sys.argv[2])