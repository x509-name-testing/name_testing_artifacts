# How to use

1. Run `./reset-openssl.sh`
2. Run `./build_openssl.sh`.
3. Run `./copy-and-build-harness.sh`
4. Go to folder `<test_subj_name>/kmls`, run KLEE on the generated bitcode file.