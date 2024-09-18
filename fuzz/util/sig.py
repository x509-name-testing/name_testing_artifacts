from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.backends import default_backend

def get_tbscert(fname):
    with open(fname, 'rb') as f:
        cert_bytes = f.read()
        cur = 0
        if cert_bytes[cur] != 0x30: # check root's T
            return
        else:
            cur += 1 # move cur to after root's T
            size_bytes = 1 # root's L value is in short-form
            if cert_bytes[cur] & 0x80: # root's L value is in long-form
                size_bytes += cert_bytes[cur] ^ 0x80 # how many bytes of long-form
            cur += size_bytes # move cur to after root's L
            if cert_bytes[cur] != 0x30: # check TBS's T
                return
            else:
                # cur is now at TBS's T
                size_bytes = 1  # TBS's L in short-form
                tbs_size = cert_bytes[cur+1]
                if cert_bytes[cur+1] & 0x80: # TBS's L in long-form
                    size_bytes += cert_bytes[cur+1] ^ 0x80 # how many bytes of long-form
                    tbs_size = 0
                    for i in range(1, size_bytes):
                        tbs_size <<= 8
                        tbs_size += cert_bytes[cur+1+i]
                return cert_bytes[cur:cur+tbs_size+size_bytes+1]  

# def signing_cert(tbs_cert, private_key):
# 	return private_key.sign( tbs_cert, padding.PSS(
# 			mgf=padding.MGF1(hashes.SHA256()),
# 			salt_length=padding.PSS.MAX_LENGTH),
# 		hashes.SHA256()
#     )

def overwrite_sig(fname, forged_sig):
	with open(fname, 'rb+') as outf:
		outf.seek(-len(forged_sig), 2)
		outf.write(forged_sig)

def load_key(filename):
	with open(filename, 'rb') as f:
		privkey = serialization.load_pem_private_key(f.read(), password=None, backend=default_backend())
	return privkey

def get_sig(tbs_cert, skey):
    return skey.sign(tbs_cert, padding.PKCS1v15(), hashes.SHA256())