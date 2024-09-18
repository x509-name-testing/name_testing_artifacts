import sys
import os
import subprocess

import pdb

# BASE_PATH=""
# BASE_PATH="/home/xxx/workspace/ccvl/"
BASE_PATH="/home/xxx/x509-name-testing/ccvl/"
# here is all the test subject's harness 
# DN_BASE_PATH="/home/xxx/workspace/dn/"
DN_BASE_PATH="/home/xxx/x509-name-testing/dn/"

import logging
from datetime import datetime
now = datetime.now()

from concurrent.futures import ThreadPoolExecutor, as_completed

# exec_dict = {
#     # run.sh is the script to run the test subject's harness
#     "ax":BASE_PATH+"c-axtls/test_verify",
#     "bear":BASE_PATH+"c-bearssl/run.sh",
#     "boring":BASE_PATH+"c-boringssl/test_verify",
#     "gnu":BASE_PATH+"c-gnutls/test_verify",
#     "matrix":BASE_PATH+"c-matrix/test_verify",
#     "mbed":BASE_PATH+"c-MbedTls/test_verify",
#     "open":BASE_PATH+"c-openssl/test_verify",
#     "strongswan":BASE_PATH+"c-strongswan/test_verify",
#     "wolf":BASE_PATH+"c-WolfSsl/test_verify",
#     "botan":BASE_PATH+"cpp-botan/test_verify",
#     "otp":BASE_PATH+"erlang-otp-ssl/run.sh",
#     "gocrypto":BASE_PATH+"go-crypto/run.sh",
#     "bc":BASE_PATH+"java-BouncyCastle/run.sh",
#     "sun":BASE_PATH+"java-SUN/run.sh",
#     "seclib":BASE_PATH+"phpseclib/run.sh",
#     "certval":BASE_PATH+"py-certvalidator/run.sh",
#     "webpki":BASE_PATH+"rust-webpki/test_verify",
#     "wpa":BASE_PATH+"wpa/test_verify",
#     "forge":BASE_PATH+"node-forge/run.sh",
#     "open3":BASE_PATH+"c-openssl3/test_verify",
# }

# dn_exec_dict = {
#     "bear":DN_BASE_PATH+"c-bearssl/run.sh",
#     "boring":DN_BASE_PATH+"c-boringssl/run.sh",
#     "gnu":DN_BASE_PATH+"c-gnutls/run.sh",
#     "matrix":DN_BASE_PATH+"c-matrix/run.sh",
#     "mbed":DN_BASE_PATH+"c-MbedTls/run.sh",
#     "open":DN_BASE_PATH+"c-openssl/run.sh",
#     "wolf":DN_BASE_PATH+"c-WolfSsl/run.sh",
#     "botan":DN_BASE_PATH+"cpp-botan/run.sh",
#     "otp":DN_BASE_PATH+"erlang-otp-ssl/run.sh",
#     "gocrypto1":DN_BASE_PATH+"go-crypto-1/run.sh",
#     "gocrypto2":DN_BASE_PATH+"go-crypto-2/run.sh",
#     "bc":DN_BASE_PATH+"java-BouncyCastle/run.sh",
#     "sun":DN_BASE_PATH+"java-SUN/run.sh",
#     "cxf":DN_BASE_PATH+"java-apache-cxf/run.sh",
#     "httpclient":DN_BASE_PATH+"java-apache-httpclient/run.sh",
#     "conscrypt":DN_BASE_PATH+"java-conscrypt/run.sh",
#     "node1":DN_BASE_PATH+"node-js-1/run.sh",
#     "node2":DN_BASE_PATH+"node-js-2/run.sh",
#     "seclib":DN_BASE_PATH+"phpseclib/run.sh",
#     "pyssl":DN_BASE_PATH+"py-ssl/run.sh",
#     "webpki":DN_BASE_PATH+"rust-webpki/run.sh",
#     # "wpa":DN_BASE_PATH+"wpa/run.sh",
#     "open3":DN_BASE_PATH+"c-openssl3/run.sh",
# }
exec_dict = {
    # please give your own path
}
dn_exec_dict = {
    # please give your own path
}

def run_harness(exec_name, ca_path, ee_folder, name=None, logger=None):
    ''' param: exec_name: the path of the test subject's harness
        input: ca_path, ee_path'''
    
    ret_lst = [] # to store the index of the test case that the return msg contains "ok"
    err_lst = [] # for test
    # logging.basicConfig(filename=f'./log/test_harness_{exec_dict[exec_name].split("/")[-2]}_{now.month}_{now.day}_{now.hour}_{now.minute}.log', level=logging.DEBUG)
    # logging.basicConfig(filename=f'./log/{now.month}_{now.day}_{now.hour}_{now.minute}.log', level=logging.DEBUG)
    logger.info("exec_name: %s , ca_path: %s, ee_path: %s" % (exec_name, ca_path, ee_folder))
    for ee in os.listdir(ee_folder):
        try:
            if ee.endswith(".pem"):
                ## if the name is not None, it is used to test subjectAltName matching in test subject(s)
                if name: proc = subprocess.run(["bash", dn_exec_dict[exec_name], ca_path, ee_folder+"/"+ee, name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                else:
                    if exec_dict[exec_name].endswith(".sh"):
                        proc = subprocess.run(["bash", exec_dict[exec_name], ca_path, ee_folder+"/"+ee], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    else:
                        proc = subprocess.run([exec_dict[exec_name], ca_path, ee_folder+"/"+ee], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    # logging.info(f"exec_name: {exec_name}, ca_path: {ca_path}, ee_path: {ee_folder}/{ee}, result: {p.stdout.decode('utf-8')} {p.stderr.decode('utf-8')}")
                    # pdb.set_trace()
                    # proc_res = "ok" if b"ok" in proc.stdout or b"ok" in proc.stderr else "err"
                if proc.returncode == 0 and b"ok" in proc.stdout:
                    proc_res = "ok" 
                elif proc.returncode == 1:
                    proc_res = "vrf_or_pars_err"
                else:
                    proc_res = "unknown"
                logger.info("exec_name: %s , ee_path: %s/%s , exit code: %d , result: %s" % (exec_name, ee_folder, ee, proc.returncode, proc_res))
                if proc_res == "ok":
                    ret_lst.append(int(ee.split(".")[0]))
                else: err_lst.append(int(ee.split(".")[0])) # for test
        except Exception as e:
            logger.error("exec_name: %s , ee_path: %s/%s, result: %s , error: %s" % (exec_name, ee_folder, ee, "unknown", e))
    # import pdb; pdb.set_trace()
    return ret_lst, err_lst
    # return err_lst # for test

def run_harnesses(ca_path, ee_path, name=None, logger=None):
    ''' param: ca_path, ee_path
        return: a dict of the test case index that the return msg contains "ok" '''
    ret_dict = {}
    err_dict = {}
    # for exec_name in exec_dict:
    #     ret_dict[exec_name], err_dict[exec_name] = run_harness(exec_name, ca_path, ee_path, logger=logger)
    with ThreadPoolExecutor(max_workers=len(exec_dict)) as executor:
        if name:
            future_to_exec = {executor.submit(run_harness, exec_name, ca_path, ee_path, name=name, logger=logger): exec_name for exec_name in dn_exec_dict}
        else:
            future_to_exec = {executor.submit(run_harness, exec_name, ca_path, ee_path, logger=logger): exec_name for exec_name in exec_dict}
        for future in as_completed(future_to_exec):
            exec_name = future_to_exec[future]
            try:
                ret_dict[exec_name], err_dict[exec_name] = future.result()
            except Exception as e:
                
                print('%r generated an exception: %s' % (exec_name, e))
            else:
                print('%r harness is done' % exec_name)
    return ret_dict, err_dict


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python harness.py ca_path ee_path")
        exit(1)
    ca_path = sys.argv[1]
    ee_path = sys.argv[2]
    ret_dict, err_dict = run_harnesses(ca_path, ee_path, logger=None)

    from pprint import pprint
    print("==============\nret dict================\n")
    pprint(ret_dict)
    print("==============\nerr dict================\n")
    pprint(err_dict)