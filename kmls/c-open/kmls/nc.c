#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/x509_vfy.h>

#define LEN_SYMBOLIC 1
    // if LEN_SYMBOLIC
    #define UPPER_BOUND  0x02   
    #define LOWER_BOUND  0x01
    // else
    #define CONCRETE_LEN 0x02

// if len_symbolic is set, the concrete length is ignored
#define TEST_CODE   -2
#define PRINT_DEBUG  1 
#define NAME_SYMBOL  1
#define VERIFY       1

#define APPEND_TO_BUF( dest_buf, dest_ind, src_buf, src_len ) { memcpy( dest_buf + dest_ind, src_buf, src_len); dest_ind += src_len; }
#define ADVANCE_BUF_VAL( dest_buf, dest_ind, src_val, src_len ) { memset( dest_buf + dest_ind, src_val, src_len); dest_ind += src_len; }
#define ACCU_SYMVAR( sym_var, cnt_val ) {for ( cnt_val = 0; cnt_val < sym_var; cnt_val++ ){} }

void printBuf( uint8_t *buf, size_t buf_len ) {
    for (size_t i = 0; i < buf_len; i++) {
        printf("%02hhx ", buf[i]);
        if ( (i+1) % 16 == 0)
            printf("\n");
        else if ( (i+1) % 8 == 0 )
            printf(" ");
    }
    printf("\n");
}

void klee_make_symbolic(void *addr, size_t nbytes, const char *name);
// void klee_merge();

void ret_val_ready(void);
int ourEntryFunction(void);

int ret_val = 0;
void ret_val_ready(void) {
    // empty but necessary
}

int u8Array2Int(uint8_t* array) {
    return ((array[0] << 8) | array[1]);
}

void int2U8Array(int value, uint8_t* array) {
    array[0] = value >> 8; array[1] = value & 0xFF;
}


int ourEntryFunction(void){
    BIO *bio_err=NULL;

    if (bio_err == NULL)
        if ((bio_err=BIO_new(BIO_s_file())) != NULL)
            BIO_set_fp(bio_err,stderr,BIO_NOCLOSE|BIO_FP_TEXT);


// ca
uint8_t ca_pri_to_subj[] = {
0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 
0x02, 0x10, 0x20, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 
0x05, 0x00, 0x30, 0x0F, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x04, 0x61, 
0x61, 0x61, 0x61, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x35, 0x30, 0x32, 0x30, 0x37, 0x31, 0x37, 0x32, 
0x34, 0x30, 0x30, 0x5A, 0x17, 0x0D, 0x32, 0x34, 0x30, 0x32, 0x31, 0x34, 0x30, 0x36, 0x32, 0x36, 
0x35, 0x33, 0x5A,};

// uint8_t ca_pri_to_subj[] ={
// 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 
// 0x02, 0x10, 0x20, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 
// 0x05, 0x00, 0x30, 0x0D, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x02, 0xFF, 
// 0xFF, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x35, 0x30, 0x32, 0x30, 0x37, 0x31, 0x37, 0x32, 0x34, 0x30, 
// 0x30, 0x5A, 0x17, 0x0D, 0x32, 0x34, 0x30, 0x32, 0x31, 0x34, 0x30, 0x36, 0x32, 0x36, 0x35, 0x33, 
// 0x5A, };

uint8_t ca_cn_prefix[] = { 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, }; 

uint8_t ca_suffix[] = {0x30, 0x82, 0x01, 0x20, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 
0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0D, 0x00, 0x30, 0x82, 0x01, 0x08, 
0x02, 0x82, 0x01, 0x01, 0x00, 0xCC, 0x76, 0x86, 0xA8, 0x54, 0xC8, 0x6C, 0xA9, 0xF7, 0x77, 0x93, 
0x62, 0x94, 0x29, 0xCA, 0x1D, 0xE4, 0xB1, 0x10, 0x74, 0x09, 0x57, 0x7B, 0x02, 0x50, 0x3B, 0x30, 
0x7D, 0x38, 0xC9, 0xA7, 0x12, 0xB0, 0x16, 0xB0, 0xE7, 0x04, 0xB0, 0xA9, 0x2F, 0x60, 0xB2, 0x03, 
0x35, 0xCC, 0xD1, 0x58, 0x2D, 0x1F, 0xE2, 0xCB, 0x10, 0x87, 0x9F, 0x65, 0x78, 0x1B, 0xDF, 0x91, 
0x6D, 0xCF, 0x6E, 0xBA, 0x44, 0x83, 0x23, 0x1A, 0x4A, 0x9D, 0x8E, 0x2A, 0xEC, 0xD2, 0x0C, 0xA3, 
0xC4, 0xB3, 0xCD, 0x0C, 0xBD, 0x6A, 0xD5, 0x7E, 0xDC, 0xBB, 0x1C, 0xBB, 0x43, 0x61, 0xB7, 0xE6, 
0x6F, 0x5B, 0x53, 0x21, 0x5C, 0xCC, 0x1F, 0x2A, 0x3D, 0x94, 0xBE, 0x30, 0x31, 0x13, 0x2E, 0xA8, 
0x64, 0xD9, 0xDD, 0xFC, 0x16, 0x64, 0xA4, 0x89, 0xFC, 0x1C, 0x03, 0x32, 0xB4, 0xE7, 0xA9, 0x78, 
0x95, 0xFA, 0x54, 0xEF, 0x17, 0x76, 0x83, 0xDB, 0x33, 0xFF, 0xA9, 0x10, 0xFC, 0xBC, 0x9B, 0xB6, 
0xBC, 0x21, 0x1B, 0xFB, 0xA9, 0xF2, 0x89, 0xD0, 0xBA, 0xB5, 0x99, 0x66, 0x33, 0x31, 0x27, 0xFA, 
0x10, 0x55, 0x68, 0x71, 0x14, 0x3E, 0xAB, 0x48, 0xD1, 0x5E, 0x58, 0x3D, 0xEC, 0x69, 0x14, 0x68, 
0xB7, 0xED, 0x0B, 0x21, 0x1F, 0x71, 0x6E, 0xD7, 0xF7, 0xCA, 0x07, 0x15, 0xF1, 0x77, 0xFE, 0x59, 
0x00, 0x71, 0xEB, 0x3E, 0xDB, 0x02, 0x70, 0x46, 0xA9, 0x54, 0xA6, 0x98, 0xD8, 0x40, 0x92, 0x42, 
0x69, 0x98, 0x28, 0x22, 0xC4, 0x36, 0xF8, 0x35, 0x28, 0xF0, 0x9C, 0xBA, 0x6A, 0xB0, 0x82, 0x88, 
0xE3, 0xBE, 0xD1, 0x3C, 0x82, 0x3B, 0x3E, 0x85, 0x80, 0x4B, 0xBB, 0x68, 0x37, 0xA6, 0x92, 0x18, 
0x96, 0x79, 0xDD, 0xA6, 0xE3, 0xC3, 0x7C, 0xE2, 0x4B, 0xD2, 0x44, 0x2E, 0x9F, 0x41, 0x86, 0xF2, 
0x39, 0x13, 0x63, 0x94, 0x03, 0x02, 0x01, 0x03, 0xA3, 0x32, 0x30, 0x30, 0x30, 0x0F, 0x06, 0x03, 
0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF, 0x30, 0x1D, 0x06, 
0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x6B, 0xF9, 0xA4, 0x2D, 0xA5, 0xE9, 0x39, 0x89, 
0xA8, 0x24, 0x58, 0x79, 0x87, 0x11, 0xFC, 0x6F, 0x07, 0x91, 0xEF, 0xA6, 0x30, 0x0D, 0x06, 0x09, 
0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 
0xBB, 0xAD, 0x84, 0xC6, 0x51, 0xD3, 0x2E, 0x48, 0x5C, 0x0E, 0x2E, 0x11, 0xAC, 0xD1, 0xD5, 0x1A, 
0xD6, 0xB5, 0x88, 0x6F, 0xE5, 0xFD, 0x6E, 0xD9, 0x4A, 0x8E, 0xD9, 0x24, 0xE6, 0x69, 0xF0, 0x5B, 
0x95, 0x79, 0x43, 0x86, 0x05, 0x49, 0x64, 0x68, 0x99, 0xF9, 0x07, 0xE2, 0x3E, 0x37, 0xCC, 0x60, 
0xA9, 0xD9, 0x2A, 0xD2, 0xFD, 0xFF, 0x34, 0xDB, 0x73, 0x51, 0x85, 0x08, 0xDE, 0x8C, 0x6C, 0x49, 
0x3F, 0x63, 0xF5, 0x31, 0x4D, 0xA0, 0xB7, 0x11, 0xBD, 0xDB, 0x9F, 0xA6, 0x3D, 0x03, 0x17, 0xD0, 
0x4D, 0x14, 0x35, 0x78, 0x9D, 0xF1, 0x46, 0x1C, 0xE7, 0xD2, 0x34, 0xAB, 0xF9, 0x22, 0x42, 0x64, 
0x2A, 0xBA, 0x1D, 0xF1, 0x16, 0x53, 0x7D, 0x6A, 0x6E, 0xA9, 0x60, 0x3F, 0xAD, 0xC8, 0xAE, 0xC6, 
0x03, 0xFE, 0x00, 0x8E, 0x63, 0x7F, 0x36, 0xC5, 0x53, 0x9B, 0x29, 0x88, 0xE0, 0xA2, 0x71, 0xC8, 
0x51, 0xE1, 0xE5, 0x31, 0x21, 0xC8, 0xBB, 0x4B, 0x70, 0xCC, 0xE2, 0x1A, 0x14, 0xF6, 0x10, 0x2F, 
0xFF, 0x1D, 0xF1, 0x4E, 0xFE, 0xA6, 0xC5, 0xAA, 0x7F, 0x48, 0x98, 0xA3, 0xCE, 0x15, 0x16, 0x68, 
0x3D, 0xE1, 0x3E, 0xC7, 0xC5, 0x01, 0x2D, 0x4F, 0xC5, 0x83, 0x65, 0xB9, 0x0A, 0xE5, 0xA3, 0x7A, 
0xAF, 0x58, 0x3E, 0x4B, 0x80, 0x45, 0x54, 0x83, 0xBA, 0x8B, 0xE8, 0x20, 0xDC, 0x6E, 0x79, 0xF0, 
0x54, 0x5B, 0x3D, 0x79, 0xD1, 0x7A, 0xD6, 0x02, 0x70, 0xB8, 0x55, 0x38, 0xD2, 0x9A, 0xB5, 0x7E, 
0x0C, 0x8F, 0x23, 0x6D, 0x95, 0x0C, 0x0C, 0xB7, 0xC0, 0x65, 0x62, 0x01, 0x1F, 0xAB, 0xAC, 0xC9, 
0x6E, 0xEC, 0xD1, 0xDB, 0x78, 0x8A, 0x47, 0x54, 0x50, 0xE5, 0xA2, 0xD9, 0x36, 0x69, 0x68, 0x48, 
0xEC, 0xA3, 0xD5, 0xDD, 0xB4, 0xBB, 0xC3, 0x6F, 0xCD, 0x60, 0xF3, 0x14, 0xFE, 0x7B, 0xCF, 0x10};


uint8_t sym_ca_len_cn = 0xff; // ca common name length
klee_make_symbolic( &sym_ca_len_cn, sizeof(uint8_t), "sym_ca_len_cn" );

#if LEN_SYMBOLIC
    // klee_assume( sym_ca_len_cn < 0x32 ); // here we assume the cn length is less than 0x32
    klee_assume( sym_ca_len_cn <= UPPER_BOUND ); // here we assume the cn length is less than 0x10
    klee_assume( sym_ca_len_cn >= LOWER_BOUND );
#else
    klee_assume( sym_ca_len_cn == CONCRETE_LEN ); // concrete value
#endif // LEN_SYMBOLIC

int real_len_ca = 0;
ACCU_SYMVAR( sym_ca_len_cn, real_len_ca );
printf("real_len_ca: %d\n", real_len_ca);

// create the set of subj's common name
uint8_t ca_cn[ 0xff ] = {0};
int len_ca_cn = 0;

ADVANCE_BUF_VAL( ca_cn, len_ca_cn, 0x30, 1 );
uint8_t ca_real_set_len = real_len_ca + 6 + 1;
ADVANCE_BUF_VAL( ca_cn, len_ca_cn, ca_real_set_len, 1 );
// }
APPEND_TO_BUF( ca_cn, len_ca_cn, ca_cn_prefix, sizeof(ca_cn_prefix) );
ADVANCE_BUF_VAL( ca_cn, len_ca_cn, real_len_ca, 1 );
ADVANCE_BUF_VAL( ca_cn, len_ca_cn, 0x61, real_len_ca );

#if PRINT_DEBUG
    printf("\nThe len of ca_cn is: %d, ca_cn: \n", len_ca_cn);
    printBuf( ca_cn, len_ca_cn );
#endif // PRINT_DEBUG

// create sq
uint8_t ca_subj[ 0xff*2 ] = {0};
int len_ca_subj = 0;
ADVANCE_BUF_VAL( ca_subj, len_ca_subj, 0x30, 1 );
uint8_t real_subj_len = len_ca_cn + 2;
ADVANCE_BUF_VAL( ca_subj, len_ca_subj, real_subj_len, 1 );
ADVANCE_BUF_VAL( ca_subj, len_ca_subj, 0x31, 1 );
ADVANCE_BUF_VAL( ca_subj, len_ca_subj, len_ca_cn, 1 );
APPEND_TO_BUF( ca_subj, len_ca_subj, ca_cn, len_ca_cn );

#if PRINT_DEBUG
    printf("\nThe len of ca_subj is: %d, ca_subj: \n", len_ca_subj);
    printBuf( ca_subj, len_ca_subj );
#endif // PRINT_DEBUG

// create whole ca cert
int ca_ttl_len = 701 + len_ca_subj;
// int ca_ttl_len = 699 + len_ca_subj;
#if PRINT_DEBUG
    printf("\nThe len of ca_ttl_len is: %d\n", ca_ttl_len);
#endif // PRINT_DEBUG

uint8_t ca_cert[ ca_ttl_len ];

int len_ca_cert = 0;
ADVANCE_BUF_VAL( ca_cert, len_ca_cert, 0x30, 1 );
ADVANCE_BUF_VAL( ca_cert, len_ca_cert, 0x82, 1 );
int caSqLen1 = len_ca_subj + 697;
// int caSqLen1 = len_ca_subj + 695;
uint8_t caTmpSqLen1[2] = {0};
int2U8Array( caSqLen1, caTmpSqLen1 );
APPEND_TO_BUF( ca_cert, len_ca_cert, caTmpSqLen1, 2 );

ADVANCE_BUF_VAL( ca_cert, len_ca_cert, 0x30, 1 );
ADVANCE_BUF_VAL( ca_cert, len_ca_cert, 0x82, 1 );
int caSqLen2 = len_ca_subj + 417;
// int caSqLen2 = len_ca_subj + 415;
uint8_t caTmpSqLen2[2] = {0};
int2U8Array( caSqLen2, caTmpSqLen2 );
APPEND_TO_BUF( ca_cert, len_ca_cert, caTmpSqLen2, 2 );

APPEND_TO_BUF( ca_cert, len_ca_cert, ca_pri_to_subj, sizeof(ca_pri_to_subj) );
APPEND_TO_BUF( ca_cert, len_ca_cert, ca_subj, len_ca_subj );
APPEND_TO_BUF( ca_cert, len_ca_cert, ca_suffix, sizeof(ca_suffix) );


/*****************************************************************************/


// ee 
uint8_t ee_pri_to_iss[] = { // before sq of issuer  
0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 
0x02, 0x10, 0x21, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 
0x05, 0x00,};

uint8_t ee_cn_prefix[] = { // tag: 0x13, len: 0xff(symbol), value: (symbol) // // 0x30, 0x0F, 0x31, 0x0D, 0x30, 0x0B,  OR 0x30, |3| , 0x31, 0x0D, 0x30, |3| 
0x06, 0x03, 0x55, 0x04, 0x03, 0x13,}; 
// 0xFF}; 
// 0x01, 0x61};

uint8_t ee_suffix[] = { // after sq of issuer
0x30, 0x1E, 0x17, 0x0D, 0x31, 0x36, 0x30, 0x32, 0x30, 0x37, 0x31, 0x37, 0x32,  
0x34, 0x30, 0x30, 0x5A, 0x17, 0x0D, 0x32, 0x34, 0x30, 0x32, 0x31, 0x34, 0x30, 0x36, 0x32, 0x36, 
0x35, 0x33, 0x5A, 0x30, 0x0F, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x04, 
0x61, 0x61, 0x61, 0x61, 0x30, 0x82, 0x01, 0x20, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 
0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0D, 0x00, 0x30, 0x82, 0x01, 0x08, 
0x02, 0x82, 0x01, 0x01, 0x00, 0xA8, 0x8A, 0x5E, 0x26, 0x23, 0x1B, 0x31, 0xD3, 0x37, 0x1A, 0x70, 
0xB2, 0xEC, 0x3F, 0x74, 0xD4, 0xB4, 0x44, 0xE3, 0x7A, 0xA5, 0xC0, 0xF5, 0xAA, 0x97, 0x26, 0x9A, 
0x04, 0xFF, 0xDA, 0xBE, 0xE5, 0x09, 0x03, 0x98, 0x3D, 0xB5, 0xBF, 0x01, 0x2C, 0x9A, 0x0A, 0x3A, 
0xFB, 0xBC, 0x3C, 0xE7, 0xBE, 0x83, 0x5C, 0xB3, 0x70, 0xE8, 0x5C, 0xE3, 0xD1, 0x83, 0xC3, 0x94, 
0x08, 0xCD, 0x1A, 0x87, 0xE5, 0xE0, 0x5B, 0x9C, 0x5C, 0x6E, 0xB0, 0x7D, 0xE2, 0x58, 0x6C, 0xC3, 
0xB5, 0xC8, 0x9D, 0x11, 0xF1, 0x5D, 0x96, 0x0D, 0x66, 0x1E, 0x56, 0x7F, 0x8F, 0x59, 0xA7, 0xA5, 
0xE1, 0xC5, 0xE7, 0x81, 0x4C, 0x09, 0x9D, 0x5E, 0x96, 0xF0, 0x9A, 0xC2, 0x8B, 0x70, 0xD5, 0xAB, 
0x79, 0x58, 0x5D, 0xB7, 0x58, 0xAA, 0xFD, 0x75, 0x52, 0xAA, 0x4B, 0xA7, 0x25, 0x68, 0x76, 0x59, 
0x00, 0xEE, 0x78, 0x2B, 0x91, 0xC6, 0x59, 0x91, 0x99, 0x38, 0x3E, 0xA1, 0x76, 0xC3, 0xF5, 0x23, 
0x6B, 0xE6, 0x07, 0xEA, 0x63, 0x1C, 0x97, 0x49, 0xEF, 0xA0, 0xFE, 0xFD, 0x13, 0xC9, 0xA9, 0x9F, 
0xC2, 0x0B, 0xE6, 0x87, 0x92, 0x5B, 0xCC, 0xF5, 0x42, 0x95, 0x4A, 0xA4, 0x6D, 0x64, 0xBA, 0x7D, 
0xCE, 0xCB, 0x04, 0xD0, 0xF8, 0xE7, 0xE3, 0xDA, 0x75, 0x60, 0xD3, 0x8B, 0x6A, 0x64, 0xFC, 0x78, 
0x56, 0x21, 0x69, 0x5A, 0xE8, 0xA7, 0x8F, 0xFB, 0x8F, 0x82, 0xE3, 0xAE, 0x36, 0xA2, 0x93, 0x66, 
0x92, 0xCB, 0x82, 0xA3, 0xBE, 0x84, 0x00, 0x86, 0xDC, 0x7E, 0x6D, 0x53, 0x77, 0x84, 0x17, 0xB9, 
0x55, 0x43, 0x0D, 0xF1, 0x16, 0x1F, 0xD5, 0x43, 0x75, 0x99, 0x66, 0x19, 0x52, 0xD0, 0xAC, 0x5F, 
0x74, 0xAD, 0xB2, 0x90, 0x15, 0x50, 0x04, 0x74, 0x43, 0xDF, 0x6C, 0x35, 0xD0, 0xFD, 0x32, 0x37, 
0xB3, 0x8D, 0xF5, 0xE5, 0x09, 0x02, 0x01, 0x03, 0xA3, 0x61, 0x30, 0x5F, 0x30, 0x0C, 0x06, 0x03, 
0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x02, 0x30, 0x00, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x1D, 
0x11, 0x04, 0x08, 0x30, 0x06, 0x82, 0x04, 0x00, 0x00, 0x00, 0x00, 0x30, 0x1D, 0x06, 0x03, 0x55, 
0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x92, 0x6A, 0x1E, 0x52, 0x3A, 0x1A, 0x57, 0x9F, 0xC9, 0x82, 
0x9A, 0xCE, 0xC8, 0xC0, 0xA9, 0x51, 0x9D, 0x2F, 0xC7, 0x72, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 
0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x6B, 0xF9, 0xA4, 0x2D, 0xA5, 0xE9, 0x39, 0x89, 0xA8, 
0x24, 0x58, 0x79, 0x87, 0x11, 0xFC, 0x6F, 0x07, 0x91, 0xEF, 0xA6, 0x30, 0x0D, 0x06, 0x09, 0x2A, 
0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x38, 
0xF4, 0x9C, 0xF3, 0xE4, 0x21, 0xA0, 0x3C, 0x03, 0x4C, 0x0C, 0x69, 0xF9, 0x79, 0x4E, 0xCF, 0x26, 
0xCC, 0x85, 0x71, 0xBA, 0xC8, 0x9F, 0xE6, 0xC6, 0xE1, 0x03, 0x3C, 0xCA, 0xD7, 0x1F, 0xD5, 0xD8, 
0x49, 0x41, 0xA5, 0x5C, 0x7F, 0x62, 0x3C, 0x51, 0x29, 0x5F, 0xC7, 0x1C, 0x0C, 0x86, 0xB8, 0x13, 
0x6B, 0x2E, 0xA9, 0x1D, 0xC1, 0x5A, 0x81, 0xC7, 0x4A, 0xD8, 0xA8, 0x3D, 0x15, 0x54, 0x44, 0x5A, 
0xD8, 0xAD, 0x0A, 0x10, 0x42, 0xB2, 0x99, 0xE4, 0xFE, 0x9E, 0x27, 0xC4, 0x8C, 0x03, 0xA1, 0x40, 
0xF4, 0xE6, 0x69, 0x24, 0xFA, 0x3B, 0xFD, 0x43, 0x7A, 0xCD, 0x17, 0xC5, 0x78, 0xBB, 0x8D, 0x72, 
0x11, 0x81, 0xD7, 0xD6, 0xFE, 0x4A, 0x79, 0x56, 0x3A, 0x46, 0xEC, 0x50, 0x15, 0x5C, 0x14, 0xA2, 
0xCC, 0x38, 0x2C, 0xEA, 0x58, 0x2E, 0xB7, 0xEC, 0xCF, 0xD9, 0xAB, 0x9F, 0x30, 0x88, 0x36, 0xF6, 
0x95, 0x48, 0x13, 0x03, 0xA1, 0xE8, 0xEB, 0x82, 0x02, 0x89, 0xB2, 0x32, 0xDD, 0xB3, 0x20, 0xF1, 
0xBE, 0x30, 0x5C, 0x55, 0x44, 0xDA, 0xAF, 0x92, 0x42, 0x9E, 0x17, 0x4B, 0x21, 0xAE, 0x16, 0xCD, 
0xA8, 0x64, 0x60, 0x98, 0xAC, 0x0B, 0x5A, 0xCA, 0x7A, 0xFB, 0xC0, 0xCE, 0xCA, 0x59, 0x21, 0x44, 
0x3E, 0x0F, 0xCC, 0x6C, 0x87, 0xA4, 0x0D, 0xB7, 0x3D, 0x32, 0xA2, 0xEC, 0xF7, 0xB6, 0x03, 0x9F, 
0xF9, 0xCB, 0x42, 0xBC, 0x90, 0x7F, 0xDA, 0xD1, 0x2D, 0xB2, 0x15, 0x37, 0x12, 0x3C, 0x93, 0x5B, 
0xE9, 0xC2, 0xD4, 0xF5, 0x77, 0xA6, 0x22, 0x46, 0x99, 0x02, 0xFC, 0x0F, 0x42, 0x0B, 0x87, 0x86, 
0xB0, 0x9D, 0xE5, 0xD8, 0xD1, 0xDC, 0x76, 0x96, 0x65, 0x2D, 0x73, 0x0C, 0x2C, 0xF8, 0x62, 0xD1, 
0x5D, 0xC9, 0x0A, 0xA8, 0x42, 0xC8, 0xF2, 0x9C, 0x50, 0x43, 0x23, 0x19, 0x0B, 0x0D, 0x53};


uint8_t sym_ee_len_cn = 0xff; // ee sq of issuer's CN
klee_make_symbolic( &sym_ee_len_cn, sizeof(uint8_t), "sym_ee_len_cn" );

#if LEN_SYMBOLIC
    // klee_assume( sym_ee_len_cn < 0x32 ); // here we assume the cn length is less than 0x32
    klee_assume( sym_ee_len_cn <= UPPER_BOUND ); // here we assume the cn length is less than 0x10
    klee_assume( sym_ee_len_cn >= LOWER_BOUND ); 
#else
    klee_assume( sym_ee_len_cn == CONCRETE_LEN );
#endif // LEN_SYMBOLIC


int real_len_ee = 0;
ACCU_SYMVAR( sym_ee_len_cn, real_len_ee );
printf("real_len_ee: %d\n", real_len_ee);


// create the set of issuer's common name
uint8_t ee_cn[ 0xff ] = {0};
int len_ee_cn = 0;
// if ( real_len_ee + 7 > 0xff ){
//     uint8_t tmpSetLen[2] = {0};
//     int2U8Array( real_len_ee + 7, tmpSetLen );
//     ADVANCE_BUF_VAL( ee_cn, len_ee_cn, 0x30, 1 );
//     ADVANCE_BUF_VAL( ee_cn, len_ee_cn, 0x82, 1 );
//     APPEND_TO_BUF( ee_cn, len_ee_cn, tmpSetLen, 2 );
// } else {    
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, 0x30, 1 );
uint8_t ee_real_set_len = real_len_ee + 6 + 1;
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, ee_real_set_len, 1 );
// }
APPEND_TO_BUF( ee_cn, len_ee_cn, ee_cn_prefix, sizeof(ee_cn_prefix) );
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, real_len_ee, 1 );
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, 0x61, real_len_ee );


// if ( len_ee_cn > 0xff ) { // sq
//     uint8_t tmpSetLen[2] = {0};
//     int2U8Array( len_ee_cn + 6, tmpSetLen );
//     INSERT_BUF_VAL( ee_cn, len_ee_cn, tmpSetLen, 2, 0 );
// } else {
//     INSERT_BUF_VAL( ee_cn, len_ee_cn, len_ee_cn, 1, 0 );
// }
// INSERT_BUF_VAL( ee_cn, len_ee_cn, 0x30, 1, 0 );

#if PRINT_DEBUG
    printf("========================\n");
    printf("\nThe len of ee_cn is: %d, ee_cn: \n", len_ee_cn);
    printBuf( ee_cn, len_ee_cn );
#endif // PRINT_DEBUG

// create sq
uint8_t ee_iss[ 0xff*2 ] = {0};
int len_ee_iss = 0;
ADVANCE_BUF_VAL( ee_iss, len_ee_iss, 0x30, 1 );
uint8_t real_iss_len = len_ee_cn + 2;
ADVANCE_BUF_VAL( ee_iss, len_ee_iss, real_iss_len, 1 );
ADVANCE_BUF_VAL( ee_iss, len_ee_iss, 0x31, 1 );
ADVANCE_BUF_VAL( ee_iss, len_ee_iss, len_ee_cn, 1 );
APPEND_TO_BUF( ee_iss, len_ee_iss, ee_cn, len_ee_cn );

#if PRINT_DEBUG
    printf("\nThe len of ee_iss is: %d, ee_iss: \n", len_ee_iss);
    printBuf( ee_iss, len_ee_iss );
#endif // PRINT_DEBUG

// create whole cert
int ee_ttl_len = 748 + len_ee_iss;
#if PRINT_DEBUG
    printf("\nThe len of ee_ttl_len is: %d\n", ee_ttl_len);
#endif // PRINT_DEBUG
uint8_t ee_cert[ ee_ttl_len ];
// uint8_t ee_cert[ 0xff*2 ] = {0};
int len_ee_cert = 0;
ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x30, 1 );
ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x82, 1 );
int eeSqLen1 = len_ee_iss + 744;
uint8_t eeTmpSqLen1[2] = {0};
int2U8Array( eeSqLen1, eeTmpSqLen1 );
APPEND_TO_BUF( ee_cert, len_ee_cert, eeTmpSqLen1, 2 );

ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x30, 1 );
ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x82, 1 );
int eeSqLen2 = len_ee_iss + 464;
uint8_t eeTmpSqLen2[2] = {0};
int2U8Array( eeSqLen2, eeTmpSqLen2 );
APPEND_TO_BUF( ee_cert, len_ee_cert, eeTmpSqLen2, 2 );

APPEND_TO_BUF( ee_cert, len_ee_cert, ee_pri_to_iss, sizeof(ee_pri_to_iss) );
APPEND_TO_BUF( ee_cert, len_ee_cert, ee_iss, len_ee_iss );
APPEND_TO_BUF( ee_cert, len_ee_cert, ee_suffix, sizeof(ee_suffix) );

// create name symbol
#if NAME_SYMBOL
    // ca
	// uint8_t sym_ca_iss[2];
    // klee_make_symbolic( sym_ca_iss, sizeof(sym_ca_iss), "sym_ca_iss" );
    // memcpy( &ca_cert[45], sym_ca_iss, sizeof(sym_ca_iss) );

    uint8_t sym_ca_cn[real_len_ca];
    klee_make_symbolic( sym_ca_cn, sizeof(sym_ca_cn), "sym_ca_cn" );
    memcpy( &ca_cert[94], sym_ca_cn, sizeof(sym_ca_cn) );
    //ee
	uint8_t type;
    klee_make_symbolic( &type, sizeof(uint8_t), "type" );
    memset( &ee_cert[43], type, 1 );
	klee_assume( 0 <= type && type <= 30 );

    uint8_t sym_ee_cn[real_len_ee];
    klee_make_symbolic( sym_ee_cn, sizeof(sym_ee_cn), "sym_ee_cn" );
    memcpy( &ee_cert[45], sym_ee_cn, sizeof(sym_ee_cn) );

// // create name constraints
//     if ( real_len_ca == real_len_ee ) {
//         for ( int i = 0; i < real_len_ca; i++ ) {
//             klee_assume( sym_ca_cn[i] != sym_ee_cn[i] );
            
//             // case insensitive
//             if ( 0x61 <= sym_ca_cn[i] && sym_ca_cn[i] <= 0x7a )
//                 klee_assume( sym_ca_cn[i] != sym_ee_cn[i] + 0x20 );
//             if ( 0x61 <= sym_ee_cn[i] && sym_ee_cn[i] <= 0x7a )
//                 klee_assume( sym_ca_cn[i] + 0x20 != sym_ee_cn[i] );
//         }
//     }
// // end of name constraints
#endif // NAME_SYMBOL

#if VERIFY
    int ret;


	// BIO              *certbio = NULL;
	BIO               *outbio = NULL;
	X509          *error_cert = NULL;
	X509                *cert = NULL;
	X509			 *ca = NULL;
	X509_NAME    *certsubject = NULL;
	X509_STORE         *store = NULL;
	X509_STORE_CTX  *vrfy_ctx = NULL;

	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();


	// certbio = BIO_new(BIO_s_file());
	outbio  = BIO_new_fp(stdout, BIO_NOCLOSE);

	if (!(store=X509_STORE_new()))
		BIO_printf(outbio, "Error creating X509_STORE_CTX object\n");


	vrfy_ctx = X509_STORE_CTX_new();

	BIO *bio = BIO_new_mem_buf(ee_cert, sizeof(ee_cert));
	cert = d2i_X509_bio(bio, NULL);
	if (cert == NULL) {
		BIO_printf(outbio, "Error loading cert into memory\n");
		exit(-1);
	}

#if PRINT_DEBUG
    printf("\nThe len of ee_cert is: %d, ee_cert: \n", len_ee_cert);
    printBuf( ee_cert, len_ee_cert );
    printf("========================\n");
#endif // PRINT_DEBUG

	// read ca cert
	BIO *ca_bio = BIO_new_mem_buf(ca_cert, sizeof(ca_cert));
	ca = d2i_X509_bio(ca_bio, NULL);

#if PRINT_DEBUG
    printf("\nThe len of ca_cert is: %d, ca_cert: \n", len_ca_cert);
    printBuf( ca_cert, len_ca_cert );
    printf("========================\n");
#endif // PRINT_DEBUG

	// ret = 34(store, ca, NULL);
	ret = X509_STORE_add_cert(store, ca);
	if (ret != 1){
		BIO_printf(outbio, "Error loading CA cert or chain file\n");
		exit(-1);
	}

	// do not verify the ca's issuer
	X509_STORE_set_flags(store, X509_V_FLAG_PARTIAL_CHAIN);

	X509_STORE_CTX_init(vrfy_ctx, store, cert, NULL);

	ret = X509_verify_cert(vrfy_ctx);
	BIO_printf(outbio, "Verification return code: %d\n", ret);

	if(ret == 0 || ret == 1)
	BIO_printf(outbio, "Verification result text: %s\n",
				X509_verify_cert_error_string(X509_STORE_CTX_get_error(vrfy_ctx)));

	printf("\nRETURN: ");
	if (ret == 0) {
		printf("err\n");
	} else if (ret == 1) {
		printf("ok\n");
	} else {
		printf("unknown\n");
	}

	if(ret == 0) {
		/*  get the offending certificate causing the failure */
		error_cert  = X509_STORE_CTX_get_current_cert(vrfy_ctx);
		// certsubject = X509_NAME_new();
		// certsubject = X509_get_subject_name(error_cert);
		BIO_printf(outbio, "Verification failed cert:\n");
		// X509_NAME_print_ex(outbio, certsubject, 0, XN_FLAG_MULTILINE);
		BIO_printf(outbio, "\n");
	}

	/* ---------------------------------------------------------- *
	* Free up all structures                                     *
	* ---------------------------------------------------------- */
	X509_STORE_CTX_free(vrfy_ctx);
	X509_STORE_free(store);
	X509_free(cert);
	X509_free(ca);
	// BIO_free_all(certbio);
	BIO_free_all(outbio);
	BIO_free_all(bio);
	BIO_free_all(ca_bio);

	// return code
	if ( ret == 1 ) return 0;
	else return 1;
	
#else
    return TEST_CODE;
#endif // VERIFY

}

int main() {

	ret_val = ourEntryFunction();

    ret_val_ready();

    return 0;
}