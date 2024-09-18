import os
import argparse
from pprint import pprint
import pdb

from utils import read_cert
# from .mutate.mutator import fuzz_mutator
from mutate.mutator import fuzz_mutator, runner

parser = argparse.ArgumentParser()
parser.add_argument("-cp", "--certchain_path", type=str, help="path to certificate chain folder")
parser.add_argument("-o", "--output", type=str, default=None, help="output repo path")
parser.add_argument("-m", "--mode", type=str, default="mix", help="mutation mode: mix, val, type, len")
parser.add_argument("-n", "--name", type=str, default=None, help="name")
parser.add_argument("-f", "--field", type=str, default="issuer", help="field to mutate: issuer, SAN, ...")
parser.add_argument("-th", "--test_harness", type=str, default="open", help="harness to use: open/mbed/...")
# parser.set_defaults(func=fuzzer)
args = parser.parse_args()

mode = args.mode
field = args.field
dn = args.name
base_path = os.path.abspath(args.certchain_path)
base_ca = os.path.abspath(args.certchain_path) + "/ca.pem"
base_ee = os.path.abspath(args.certchain_path) + "/ee.pem"
base_key = os.path.abspath(args.certchain_path) + "/key.pem"
if args.output is not None:
    output_path = os.path.abspath(args.output)
    print(output_path)
else:
    output_path = None
test_harness = args.test_harness

def fuzzer():
    try:
        certDict = read_cert(base_ee)
        # fuzz_mutator(certDict, mode="oid", out=output_path, path=base_path, harness_name=test_harness)
        ## Issuer-Subject match
        # runner(certDict, field=field, mode=mode, out=output_path, path=base_path, harness_name=test_harness)
        ## subjectAltName match
        runner(certDict, field=field, mode=mode, out=output_path, path=base_path, harness_name=test_harness, dn=dn)
    except Exception as e:
        print(e)
        import traceback; print(''.join(traceback.format_exception(etype=type(e), value=e, tb=e.__traceback__)))
    return


if __name__ == '__main__':
    fuzzer()