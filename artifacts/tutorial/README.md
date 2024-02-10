## How to see the content of X.509

Here we can provide 2 methods.

1. Use `OpenSSL` command to show. Example: `openssl x509 -in <input.pem> -noout -text`. 
	- Pros: can use `openssl verify` to see more details.
	- Cons: some of the contents are not in hex view; need to clone repo.

2. Use online tools, e.g., [`lapo.it`](https://lapo.it/asn1js/). Copy the X.509 PEM/hex code to the website's input box, decode it, and see the decoded contents on the left side and the corresponding hex code on the right.
	- Pros: see more details of X.509; do not need to clone repo.
	- Cons: cannot use other system built-in tools, e.g., `OpenSSL`.


## String type, character set and encoding rules

We only focus on the string types defined in ASN.1, so you can refer to [this](https://www.oss.com/asn1/resources/asn1-made-simple/asn1-quick-reference.html).

For the character set, we did not see a good reference. So please see the table in the paper. The Wikipedia has the character set, for example, the `teletexString`'s [T.61 charset](https://en.wikipedia.org/wiki/ITU_T.61)

The most used encoding rule should be UTF-8. But UTF-16 and other encodings are also used in the X.509. Please see more in the tables.