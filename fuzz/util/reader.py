import os
import sys
import ssl
from pprint import pprint
# import pdb
import base64
import asn1tools

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
RFC5280_ASN_PATH = os.path.join(SCRIPT_DIR,
                                'rfc5280.asn')

                
def cert2dict(file):
    """ input: file name(file path)
        output: cert dictionary """
    if not os.path.exists(file):
        raise Exception("FILE_NOT_EXIST_ERROR")

    cert_byte_str = open_file(file)
    rfc5280_der_rule = asn1tools.compile_files(RFC5280_ASN_PATH, codec='der')
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


if __name__ == "__main__":
    cert2dict(sys.argv[1])