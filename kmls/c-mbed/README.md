# How to use

1. Run `./reset-mbedtls.sh`
2. Run `./build-mbedtls.sh`.
3. Run `./copy-and-build-harness.sh`
4. Go to folder `<test_subj_name>/apps/kmls`, run KLEE on the generated bitcode file.