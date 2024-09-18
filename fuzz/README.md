# Simple steps to show how to use
1. Give the base chain (seed)
2. Use `venv` and install dependencies (see [requirements.txt](./requirements.txt))
3. Edit [harness.py](./harness.py). Modify the `exec_dict` and `dn_exec_dict` for NC and HV task, respectively. Please make sure the paths are the path to test harnesses (executables). 
4. Run the script. Example command: `python3 main.py -cp ./baseChain/example-chain/ -o ./res -m mix -f issuer -th all` if you want to fuzz all the test subjects in a run.