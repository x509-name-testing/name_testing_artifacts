## example log:
## INFO:mutate.mutator:exec_name: ax , ee_path: /home/xxx/crtStrFuzzer/generated-chain/all-chain4/e92a4b44/0.pem , exit code: 0 , result: ok

import sys
import os
# from util.reader import cert2dict
## for performance
import base64
import asn1tools
import json

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
RFC5280_ASN_PATH = os.path.join(SCRIPT_DIR,
                                'util/rfc5280.asn')

rfc5280_der_rule = asn1tools.compile_files(RFC5280_ASN_PATH, codec='der')
                
out_json_file = 'all.json'

def cert2dict(file):
    """ input: file name(file path)
        output: cert dictionary """
    if not os.path.exists(file):
        raise Exception("FILE_NOT_EXIST_ERROR")

    cert_byte_str = open_file(file)
    return rfc5280_der_rule.decode('Certificate', cert_byte_str)
    
def open_file(file):
    """ input: file name (file path)
        output: certficate byte string (DER format) """ 
    try:
        if file.endswith('pem'):
            pem = open(file, 'r').readlines()
            certBytes = base64.b64decode(''.join(pem[1:-1]))
        elif file.endswith('der'):
            certBytes = open(file, 'rb').read()
        else:
            raise Exception("FILE_SUFFIX_ERROR")
    except Exception as e:
        print(e)
    return certBytes


if len(sys.argv) < 2:
    print("Usage: python logReader.py <log file>")
    exit(1)

log_file = sys.argv[1]
if not os.path.exists(log_file):
    print("Log file does not exist.")
    exit(1)

with open(log_file, 'r') as f:
    lines = f.readlines()

lines = list(set(lines))

ee_paths = []
for line in lines:
    ## get the ee_path
    if "ee_path" in line: 
        ss = line.replace(",", " ").split()
        # import pdb; pdb.set_trace()
        ## get the exec_name
        for i, item in enumerate(ss):
            if "exec_name" in item:
                exec_name = ss[i+1]
                break
        # idx = ss.index("INFO:root:exec_name:")
        # exec_name = ss[idx+1]

        idx = ss.index("ee_path:")
        ee_paths.append(f"{exec_name} {ss[idx+1]}")
        # print("ee_paths:", ee_paths)

## combine the ee_paths if the path is the same, but the exec_name is different
# ee_paths = list(set(ee_paths))

# if os.path.exists(out_json_file):
#     with open(out_json_file,) as fp:
#         file_content = fp.read()
#     dict_ = json.loads(file_content)
# else:
#     dict_ = {}
from collections import OrderedDict
# dict_ = {}
dict_ = OrderedDict()

def resolve_cert(crt_path):
    if not os.path.exists(crt_path.split()[1]):
        print("Path does not exist.")
        pass
    else:
        try:
            tmp_dict = cert2dict(crt_path.split()[1])
            from util.get_str_len import tlv_parser

            # '''for dn'''
            # for t, length, value in tlv_parser(tmp_dict['tbsCertificate']['extensions'][1]['extnValue'][2:]): pass
            # # print(value)
            # if value != b'my@mail.com':
            #     json_val = [{'tag': str(tmp_dict['tbsCertificate']['extensions'][1]['extnID']), 'type': str(t), 'val': str(value), 'path': crt_path.split()[1]}]
            #     if crt_path.split()[0] not in dict_.keys():
            #         dict_.update({crt_path.split()[0]: json_val})
            #     else:
            #         # import pdb; pdb.set_trace()
            #         if json_val not in dict_[crt_path.split()[0]]:
            #             dict_[crt_path.split()[0]] += json_val
            # else:
            #     pass
            
            '''for ccvl'''
            # for t, length, value in tlv_parser(tmp_dict['tbsCertificate']['extensions'][1]['extnValue'][2:]): pass
            # if value == b'my@mail.com': # so it is ccvl, not dn
            #     # import pdb; pdb.set_trace()
            # for item in tmp_dict['tbsCertificate']['issuer'][1][0]:
            for item in tmp_dict['tbsCertificate']['subject'][1][0]:
                if item['type'] == '2.5.4.3': # CN
                    for t, length, value in tlv_parser(item['value']): pass
                    json_val = [{'tag': '2.5.4.3', 'type': str(t), 'val': str(value), 'path': crt_path.split()[1]}]
                    if crt_path.split()[0] not in dict_.keys():
                        dict_.update({crt_path.split()[0]: json_val})
                    else:
                        # import pdb; pdb.set_trace()
                        if json_val not in dict_[crt_path.split()[0]]:
                            dict_[crt_path.split()[0]] += json_val

            
            # print( path.split(), tmp_dict['tbsCertificate']["issuer"][1][0][0] )
            print(crt_path, str(tmp_dict['tbsCertificate']['extensions'][1]))
        except Exception as e: 
            print(crt_path)


for path in ee_paths:
    resolve_cert(path)
# from concurrent.futures import ThreadPoolExecutor, as_completed
# with ThreadPoolExecutor(max_workers=64) as executor:
#     future_to_exec = {executor.submit(resolve_cert, path): path for path in ee_paths}
#     for item in as_completed(future_to_exec):
#         pass

# import pdb; pdb.set_trace();

with open(out_json_file, 'w') as fp:
    json.dump(dict_, fp)