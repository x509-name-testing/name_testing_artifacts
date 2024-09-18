#include "bearssl.h"
#include "brssl.h"

typedef VECTOR(br_x509_certificate) cert_list;

static void
free_cert_contents(br_x509_certificate *xc)
{
	xfree(xc->data);
}

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
		printf("Missing parameter\n");
		printf("Usage: ./executable_file <trusted_ca_cert> <leaf_cert>");
		return -1;
    }

    int ret = 0;
    int err = 0;

    const char* caCert     = argv[1];
    const char* verifyCert = argv[2];
    const char* name       = argv[3];

    size_t caLen = 0;
    size_t certLen = 0;

    br_x509_certificate *cert = NULL;

    anchor_list anchors = VEC_INIT;
	cert_list chain = VEC_INIT;
    
    br_x509_minimal_context mc;

    if (read_trust_anchors(&anchors, caCert) == 0)
    {
        goto verify_exit_error;
    }
    cert = read_certificates(verifyCert, &certLen);

    if (cert == NULL)
    {
        goto verify_exit_error;
    }

    VEC_ADDMANY(chain, cert, certLen);
    xfree(cert);

    // printf("ca: %d\n", ca->data_len);
    // printf("cert: %d\n", cert->data_len);

    br_x509_minimal_init(&mc, &br_sha256_vtable, &VEC_ELT(anchors, 0), VEC_LEN(anchors));
	br_x509_minimal_set_hash(&mc, br_sha1_ID, &br_sha1_vtable);
	br_x509_minimal_set_hash(&mc, br_sha224_ID, &br_sha224_vtable);
	br_x509_minimal_set_hash(&mc, br_sha256_ID, &br_sha256_vtable);
	br_x509_minimal_set_hash(&mc, br_sha384_ID, &br_sha384_vtable);
	br_x509_minimal_set_hash(&mc, br_sha512_ID, &br_sha512_vtable);
	br_x509_minimal_set_rsa(&mc, &br_rsa_i31_pkcs1_vrfy);
	br_x509_minimal_set_ecdsa(&mc, &br_ec_prime_i31, &br_ecdsa_i31_vrfy_asn1);

	mc.vtable->start_chain(&mc.vtable, name);
	for (int u = 0; u < VEC_LEN(chain); u ++) {
		br_x509_certificate *xc;

		xc = &VEC_ELT(chain, u);
		mc.vtable->start_cert(&mc.vtable, xc->data_len);
		mc.vtable->append(&mc.vtable, xc->data, xc->data_len);
		mc.vtable->end_cert(&mc.vtable);
	}
	err = mc.vtable->end_chain(&mc.vtable);

    if (err == 0)
    {
        printf("ok\n");
    }
    else
    {
        printf("error: %d", err);

        const char *errname, *errmsg;
        errname = find_error_name(err, &errmsg);
        if (errname != NULL) {
            printf(" (%s): %s\n", errname, errmsg);
        } else {
            printf(" (unknown)\n");
        }

        ret = -1;
    }

verify_exit:
	VEC_CLEAREXT(anchors, &free_ta_contents);
	VEC_CLEAREXT(chain, &free_cert_contents);
	return ret;

verify_exit_error:
	ret = -1;
	goto verify_exit;
}