import ssl

def der_to_pem(in_path, out_path):
	try:
		with open(in_path, 'rb') as f:
			f_data = f.read()

		pem_cert = ssl.DER_cert_to_PEM_cert(f_data)

		with open(out_path, 'w') as out_f:
			out_f.write(pem_cert)

		print(f"{in_path} to {out_path} done \n")
	except Exception as e:
		print(e)


def pem_to_der(in_path, out_path):
	try:
		with open(in_path, 'r') as f:
			f_data = f.read()

		der_cert = ssl.PEM_cert_to_DER_cert(f_data)

		with open(out_path, 'wb') as out_f:
			out_f.write(der_cert)

		print("OK")
	except Exception as e:
		print(e)