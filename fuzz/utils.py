from util.reader import cert2dict
from util.writer import dict2cert, modify_signature, write_DER_file, write_PEM_file
from util.sig import get_tbscert, load_key, overwrite_sig, get_sig


def read_cert(cert_path):
    """ input: cert file path
        output: cert dictionary """
    return cert2dict(cert_path)