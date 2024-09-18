# How to use

1. Run `./cleanup.sh`
2. Run `./build_wpa.sh`.
4. Go to folder `<test_subj_name>/wpa_supplicant`, run KLEE on the generated bitcode file (`<test_subj_name>/wpa_supplicant/wpa_supplicant.bc`).