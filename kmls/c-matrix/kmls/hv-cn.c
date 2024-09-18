#include <stdint.h>
#include <limits.h>
#include <string.h>

#include "matrixssl/matrixsslApi.h"

// #include "klee/klee.h"

#define LEN_SYMBOLIC 1  
#define CONCRETE_LEN 0x04
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
void ret_val_ready(void){
    // empty but necessary
}

int u8Array2Int(uint8_t* array) {
    return ((array[0] << 8) | array[1]);
}

void int2U8Array(int value, uint8_t* array) {
    array[0] = value >> 8; array[1] = value & 0xFF;
}

int ourEntryFunction(void) {

// ca
uint8_t ca[] = {
0x30, 0x82, 0x03, 0x72, 0x30, 0x82, 0x02, 0x5a, 0xa0, 0x03, 0x02, 0x01,
0x02, 0x02, 0x02, 0x10, 0x20, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48,
0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x63, 0x31, 0x0b,
0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31,
0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x18, 0x54, 0x68,
0x65, 0x20, 0x47, 0x6f, 0x20, 0x44, 0x61, 0x64, 0x64, 0x79, 0x20, 0x47,
0x72, 0x6f, 0x75, 0x70, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x31,
0x30, 0x2f, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x28, 0x47, 0x6f, 0x20,
0x44, 0x61, 0x64, 0x64, 0x79, 0x20, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x20,
0x32, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74,
0x79, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x35, 0x30, 0x32, 0x30, 0x37, 0x31,
0x37, 0x32, 0x34, 0x30, 0x30, 0x5a, 0x17, 0x0d, 0x32, 0x34, 0x31, 0x32,
0x31, 0x30, 0x30, 0x35, 0x30, 0x37, 0x31, 0x34, 0x5a, 0x30, 0x63, 0x31,
0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53,
0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x18, 0x54,
0x68, 0x65, 0x20, 0x47, 0x6f, 0x20, 0x44, 0x61, 0x64, 0x64, 0x79, 0x20,
0x47, 0x72, 0x6f, 0x75, 0x70, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31,
0x31, 0x30, 0x2f, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x28, 0x47, 0x6f,
0x20, 0x44, 0x61, 0x64, 0x64, 0x79, 0x20, 0x43, 0x6c, 0x61, 0x73, 0x73,
0x20, 0x32, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61,
0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69,
0x74, 0x79, 0x30, 0x82, 0x01, 0x20, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01,
0x0d, 0x00, 0x30, 0x82, 0x01, 0x08, 0x02, 0x82, 0x01, 0x01, 0x00, 0xd3,
0x46, 0x3f, 0x58, 0xc8, 0x0a, 0x06, 0xcd, 0x66, 0x00, 0x65, 0xb9, 0xaa,
0x05, 0x4d, 0xa3, 0xe3, 0x86, 0xdc, 0xd5, 0x49, 0x26, 0x16, 0xba, 0xa4,
0x09, 0x85, 0x93, 0x69, 0x43, 0xac, 0x8f, 0x10, 0xe3, 0x17, 0x0f, 0xb7,
0x6c, 0x24, 0x49, 0x1b, 0xbe, 0x00, 0xb5, 0x35, 0x6a, 0x6a, 0xb6, 0x6a,
0xfc, 0xee, 0xbf, 0x7d, 0xcc, 0x4c, 0xe2, 0x00, 0x13, 0xa1, 0xb3, 0xc0,
0xe5, 0xb2, 0xaf, 0xdd, 0xda, 0x47, 0x7f, 0x2a, 0x44, 0x85, 0x4b, 0xc3,
0xe8, 0xd7, 0x72, 0x31, 0xfa, 0x4e, 0x42, 0x30, 0xe8, 0x6b, 0xef, 0x55,
0x16, 0x6b, 0xcc, 0x35, 0x1b, 0xe3, 0x17, 0xe7, 0xe5, 0xc1, 0x64, 0xb1,
0x17, 0x3e, 0x6b, 0x05, 0xf6, 0xf4, 0xe6, 0x46, 0xe0, 0x2b, 0xea, 0xdb,
0xbb, 0x5a, 0xf3, 0x65, 0x2f, 0xcc, 0xad, 0x6a, 0xe7, 0xa3, 0x92, 0x95,
0xe8, 0xdc, 0xd4, 0xa4, 0x00, 0xff, 0xb4, 0xec, 0x0c, 0x62, 0xb8, 0x20,
0x36, 0x29, 0xf3, 0x5c, 0xb1, 0x08, 0xc9, 0x69, 0x1f, 0x98, 0x7f, 0xc7,
0x7a, 0x4d, 0x52, 0xaf, 0x3d, 0xba, 0x3a, 0xa2, 0x5e, 0x17, 0x30, 0xc8,
0xe5, 0xde, 0x8c, 0x01, 0x31, 0x64, 0x8b, 0x17, 0x31, 0xa5, 0x40, 0x6d,
0x02, 0x7e, 0x06, 0xb1, 0xc0, 0xcd, 0xc4, 0x5a, 0x40, 0xf4, 0xf8, 0x85,
0xb2, 0x7c, 0x7e, 0x1c, 0x64, 0xb4, 0xcc, 0x7d, 0x84, 0xb0, 0xb7, 0x67,
0xc0, 0x59, 0xff, 0xc8, 0x3d, 0x42, 0xdd, 0x67, 0xe0, 0xe0, 0x7a, 0x5b,
0x17, 0x0a, 0x69, 0x88, 0x9b, 0xf4, 0x05, 0x2f, 0xd5, 0x50, 0x44, 0x7f,
0x78, 0x96, 0x75, 0x04, 0x68, 0xd2, 0xf1, 0xbd, 0x10, 0x6a, 0x91, 0x6e,
0x61, 0xe1, 0x71, 0x62, 0x46, 0x82, 0x5a, 0xed, 0xf9, 0xdc, 0x60, 0x33,
0xc6, 0x81, 0x3f, 0x96, 0xe0, 0x22, 0x38, 0xe5, 0x46, 0x16, 0x84, 0x12,
0xb5, 0xe6, 0xe3, 0x02, 0x01, 0x03, 0xa3, 0x32, 0x30, 0x30, 0x30, 0x0f,
0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03,
0x01, 0x01, 0xff, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16,
0x04, 0x14, 0x97, 0x2c, 0xc2, 0x97, 0x01, 0x97, 0xd5, 0xc3, 0xb0, 0xa0,
0xf9, 0xc4, 0x8e, 0x9e, 0x65, 0xb2, 0x81, 0x5f, 0xca, 0xab, 0x30, 0x0d,
0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05,
0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x10, 0xf5, 0x1f, 0xa9, 0x5d, 0x6c,
0x78, 0xb7, 0x87, 0x8d, 0x2a, 0x26, 0x7d, 0x25, 0x2e, 0x5c, 0xc2, 0x04,
0x4e, 0xc2, 0x1a, 0xca, 0x24, 0xc2, 0x1e, 0x53, 0x1a, 0x88, 0x97, 0x8c,
0x1a, 0x1a, 0x16, 0x7c, 0x8d, 0x24, 0x36, 0xce, 0xb6, 0x4a, 0xa1, 0x2b,
0x46, 0x38, 0xe7, 0xaf, 0x1b, 0xd4, 0x81, 0x68, 0x7d, 0xa3, 0x1b, 0x43,
0x61, 0x09, 0x30, 0x67, 0xa5, 0xeb, 0x6b, 0xd8, 0x1c, 0xe9, 0xd0, 0x15,
0x5f, 0x0c, 0x55, 0x95, 0xd0, 0x5e, 0x07, 0xe8, 0x9c, 0x2c, 0x10, 0x7a,
0xf3, 0x03, 0x31, 0xd9, 0x7d, 0x0e, 0x70, 0x40, 0xb0, 0x9a, 0x8c, 0x92,
0xbc, 0x7d, 0x32, 0x74, 0x8c, 0xf6, 0x9a, 0xae, 0x99, 0xce, 0xca, 0x8e,
0xf1, 0x6b, 0x72, 0x2f, 0x55, 0x4c, 0x83, 0x1d, 0xd0, 0x5c, 0xa5, 0xc8,
0x3d, 0x6d, 0x55, 0xee, 0x00, 0xfe, 0x46, 0x2b, 0xf1, 0x28, 0x22, 0xb6,
0x8d, 0xde, 0xfd, 0x7a, 0xef, 0x9c, 0xb9, 0xd6, 0xbe, 0x20, 0xb0, 0x19,
0x39, 0xc0, 0x98, 0x99, 0xcc, 0xca, 0x38, 0x72, 0x1f, 0x88, 0x0b, 0x30,
0x13, 0x7d, 0x61, 0xce, 0x29, 0xb8, 0xbd, 0xe8, 0x8c, 0xfb, 0x35, 0x47,
0xac, 0xc3, 0xca, 0xc7, 0xb3, 0x8b, 0x8e, 0x0e, 0xd4, 0xa7, 0x41, 0x94,
0xa6, 0x95, 0x37, 0x73, 0x3e, 0xda, 0xa3, 0xcd, 0xf0, 0x39, 0xe9, 0x59,
0xdd, 0x49, 0x4b, 0xde, 0xd7, 0x7b, 0x28, 0x2c, 0xc2, 0x17, 0x76, 0xe2,
0xb1, 0x15, 0x4a, 0x80, 0x7e, 0x24, 0x7a, 0x1a, 0xef, 0x2a, 0x22, 0xac,
0x41, 0xd4, 0xa5, 0x02, 0x88, 0xaf, 0x1a, 0xa0, 0x39, 0xed, 0xa6, 0x20,
0x7e, 0x46, 0xc3, 0x86, 0x45, 0x43, 0x99, 0x3f, 0xf8, 0x15, 0x07, 0xc4,
0x48, 0x8b, 0x07, 0xba, 0x97, 0x3b, 0x99, 0x70, 0xd6, 0xdf, 0xb8, 0x03,
0x67, 0x17, 0x27, 0xe7, 0xc6, 0x98, 0x12, 0x9c, 0x8d, 0x0c };

/*****************************************************************************/

// ee
uint8_t ee_pri_to_subj[] = {
0xa0, 0x03, 0x02, 0x01,
0x02, 0x02, 0x02, 0x10, 0x21, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48,
0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x63, 0x31, 0x0b,
0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31,
0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x18, 0x54, 0x68,
0x65, 0x20, 0x47, 0x6f, 0x20, 0x44, 0x61, 0x64, 0x64, 0x79, 0x20, 0x47,
0x72, 0x6f, 0x75, 0x70, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x31,
0x30, 0x2f, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x28, 0x47, 0x6f, 0x20,
0x44, 0x61, 0x64, 0x64, 0x79, 0x20, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x20,
0x32, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74,
0x79, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x32, 0x30, 0x37, 0x31,
0x37, 0x32, 0x34, 0x30, 0x30, 0x5a, 0x17, 0x0d, 0x32, 0x34, 0x31, 0x32,
0x31, 0x30, 0x30, 0x35, 0x30, 0x37, 0x31, 0x34, 0x5a, };

uint8_t ee_cn_prefix[] = { 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, }; 

uint8_t ee_suffix[] = {  
0x30, 0x82, 0x01, 0x20, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01,
0x0d, 0x00, 0x30, 0x82, 0x01, 0x08, 0x02, 0x82, 0x01, 0x01, 0x00, 0xab,
0x7c, 0x39, 0x14, 0xf1, 0xa4, 0xa1, 0x25, 0xb3, 0x04, 0x3c, 0x42, 0xd1,
0x5d, 0x5c, 0xf7, 0x01, 0xeb, 0x8d, 0x8b, 0x96, 0x56, 0x7f, 0xc1, 0x59,
0x83, 0x1c, 0x3c, 0x6f, 0xc5, 0xeb, 0x9b, 0x9c, 0x86, 0xff, 0x78, 0x13,
0x16, 0xf2, 0x09, 0x78, 0x15, 0x8c, 0x9e, 0x26, 0x85, 0xe8, 0x19, 0x5e,
0xd8, 0x5a, 0x92, 0xa0, 0x06, 0x20, 0xbe, 0x2c, 0xee, 0xeb, 0x09, 0xdb,
0x06, 0x32, 0x1c, 0xe5, 0x60, 0xac, 0xa1, 0xc7, 0xad, 0x4b, 0x81, 0x76,
0x03, 0x33, 0x9e, 0xd3, 0x90, 0x5b, 0x13, 0x9a, 0x39, 0x35, 0x0e, 0x80,
0x50, 0x52, 0xe3, 0x83, 0x5f, 0x45, 0x78, 0x92, 0x07, 0xfc, 0x4d, 0xdd,
0x86, 0xbf, 0xa0, 0xc4, 0x4e, 0xb7, 0xbf, 0xd1, 0xe1, 0x0c, 0xac, 0xd9,
0x02, 0x4c, 0x3a, 0xba, 0x5b, 0xe7, 0x7a, 0x50, 0x78, 0x4c, 0xa2, 0xbf,
0xfe, 0xae, 0xc2, 0xbd, 0xf3, 0xa9, 0x2a, 0x37, 0x10, 0xc5, 0xc1, 0xec,
0x9c, 0xff, 0x60, 0x61, 0x94, 0x5e, 0xb2, 0x5c, 0x5a, 0x65, 0xe4, 0x11,
0x67, 0x5c, 0x4c, 0x04, 0x3b, 0x04, 0x53, 0xc3, 0x46, 0x63, 0xc7, 0xa8,
0xc0, 0x06, 0x73, 0x8b, 0x60, 0x07, 0xf1, 0x66, 0xee, 0x28, 0x1a, 0x24,
0x96, 0x4f, 0x46, 0x5b, 0xd9, 0x9e, 0x6f, 0x6e, 0xba, 0xb4, 0x92, 0xe0,
0xd0, 0xe6, 0xdd, 0xb3, 0x8c, 0x5b, 0xc4, 0x52, 0x3f, 0x69, 0xc6, 0x4a,
0x11, 0xfd, 0x08, 0x16, 0xb0, 0xac, 0x00, 0xea, 0x74, 0x4f, 0x83, 0x62,
0xf7, 0x05, 0xd9, 0xcb, 0x1b, 0xdf, 0x1d, 0xbc, 0x86, 0x6a, 0x17, 0x21,
0x27, 0xb6, 0xc2, 0x96, 0x43, 0x76, 0x7a, 0xc7, 0xc4, 0xcf, 0xff, 0x81,
0x92, 0xc8, 0x67, 0xdb, 0x8e, 0x85, 0x40, 0x99, 0x84, 0x09, 0x43, 0x90,
0xc0, 0xa2, 0x8d, 0x8c, 0x0a, 0x4f, 0x24, 0x70, 0x4a, 0xa3, 0x67, 0x07,
0x39, 0x4f, 0x99, 0x02, 0x01, 0x03, 0xa3, 0x50, 0x30, 0x4e, 0x30, 0x0c,
0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x02, 0x30, 0x00,
0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x24,
0xcd, 0x10, 0xaf, 0x97, 0x0c, 0x95, 0x18, 0x27, 0x8e, 0x72, 0x44, 0xbf,
0xa5, 0xfb, 0x33, 0x5e, 0xae, 0xed, 0x7b, 0x30, 0x1f, 0x06, 0x03, 0x55,
0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x97, 0x2c, 0xc2, 0x97,
0x01, 0x97, 0xd5, 0xc3, 0xb0, 0xa0, 0xf9, 0xc4, 0x8e, 0x9e, 0x65, 0xb2,
0x81, 0x5f, 0xca, 0xab, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00,
0x04, 0x5d, 0xa8, 0x6d, 0x70, 0x48, 0xae, 0xc9, 0x88, 0xd5, 0xaf, 0x1e,
0x25, 0xee, 0x96, 0x12, 0xd7, 0x75, 0xda, 0x5f, 0x2d, 0x87, 0xc2, 0xc4,
0xaa, 0xc6, 0x65, 0xcf, 0xd0, 0x52, 0x07, 0xec, 0x1d, 0x26, 0x35, 0xab,
0xbf, 0x5f, 0x19, 0x3a, 0x13, 0x90, 0x8f, 0x30, 0x54, 0x43, 0xec, 0xc9,
0xd1, 0xdc, 0x8c, 0x83, 0xf9, 0xe5, 0xbd, 0xaa, 0xdf, 0x99, 0x0a, 0x3f,
0x1e, 0x95, 0x58, 0x90, 0x29, 0x93, 0x12, 0x18, 0xbe, 0xa6, 0x07, 0x09,
0xfc, 0x5c, 0x11, 0x7a, 0xb5, 0xd0, 0xbc, 0x7c, 0x7c, 0x7c, 0xc9, 0x82,
0xa5, 0x80, 0x58, 0xf1, 0x0e, 0x8c, 0x22, 0xb2, 0x60, 0xa7, 0xb8, 0x48,
0x49, 0x34, 0xfd, 0x3d, 0x97, 0x1b, 0x7e, 0x2a, 0xd7, 0x46, 0x06, 0x27,
0xad, 0xa4, 0x75, 0x1d, 0x0f, 0x2e, 0xe5, 0x01, 0xda, 0x10, 0xb6, 0x22,
0xbf, 0x39, 0xbc, 0x06, 0x68, 0xe5, 0x2b, 0xa0, 0xbb, 0xfd, 0x51, 0xc3,
0xd4, 0x55, 0xaf, 0x3b, 0x30, 0xdf, 0xc3, 0x64, 0x5d, 0xde, 0x93, 0xa6,
0x38, 0x45, 0x1c, 0x11, 0xd2, 0x07, 0x8c, 0xd6, 0xec, 0x85, 0xa5, 0x40,
0x8b, 0x40, 0x7b, 0x62, 0x2e, 0x3a, 0x8a, 0xd7, 0xf0, 0x38, 0xae, 0xe0,
0xb3, 0xcf, 0xca, 0xd3, 0x11, 0x5c, 0x49, 0x73, 0x6b, 0xe7, 0xb3, 0x6d,
0xf1, 0x26, 0x09, 0x81, 0x4f, 0xa3, 0x2d, 0x29, 0x1d, 0xf1, 0xd0, 0xf4,
0x31, 0xc7, 0xbd, 0x58, 0x28, 0xc6, 0x89, 0x03, 0x27, 0xfc, 0xfa, 0x09,
0x55, 0x26, 0x36, 0x61, 0xd8, 0xa4, 0x35, 0xb5, 0x7f, 0x82, 0xce, 0x91,
0xb4, 0xb1, 0x8f, 0xa6, 0x71, 0x3b, 0x12, 0x66, 0xb5, 0x6a, 0xab, 0xc4,
0xa7, 0xc2, 0x36, 0x1f, 0xc2, 0xf4, 0xc6, 0x59, 0x80, 0xb3, 0x74, 0x09,
0x47, 0xc6, 0x64, 0xaa, 0x71, 0x2a, 0x7d, 0xd1, 0xdc, 0xe6, 0x86, 0xff,
0x37, 0xa6, 0xcb, 0x83};


uint8_t sym_ee_len_cn = 0xff; // ee common name length
klee_make_symbolic( &sym_ee_len_cn, sizeof(uint8_t), "sym_ee_len_cn" );

#if LEN_SYMBOLIC
    // klee_assume( sym_ee_len_cn < 0x32 ); // here we assume the cn length is less than 0x32
    klee_assume( sym_ee_len_cn <= 0x05 ); // here we assume the cn length is less than 0x10
    klee_assume( sym_ee_len_cn >= 0x01 );
#else
    klee_assume( sym_ee_len_cn == 0x04 ); // concrete value
#endif // LEN_SYMBOLIC

int real_len_ee = 0;
ACCU_SYMVAR( sym_ee_len_cn, real_len_ee );
printf("real_len_ee: %d\n", real_len_ee);

// create the set of subj's common name
uint8_t ee_cn[ 0xff ] = {0};
int len_ee_cn = 0;

ADVANCE_BUF_VAL( ee_cn, len_ee_cn, 0x30, 1 );
uint8_t ee_real_set_len = real_len_ee + 6 + 1;
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, ee_real_set_len, 1 );
// }
APPEND_TO_BUF( ee_cn, len_ee_cn, ee_cn_prefix, sizeof(ee_cn_prefix) );
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, real_len_ee, 1 );
ADVANCE_BUF_VAL( ee_cn, len_ee_cn, 0x61, real_len_ee );

#if PRINT_DEBUG
    printf("\nThe len of ee_cn is: %d, ee_cn: \n", len_ee_cn);
    printBuf( ee_cn, len_ee_cn );
#endif // PRINT_DEBUG

// create sq
uint8_t ee_subj[ 0xff*2 ] = {0};
int len_ee_subj = 0;
ADVANCE_BUF_VAL( ee_subj, len_ee_subj, 0x30, 1 );
uint8_t real_subj_len = len_ee_cn + 2;
ADVANCE_BUF_VAL( ee_subj, len_ee_subj, real_subj_len, 1 );
ADVANCE_BUF_VAL( ee_subj, len_ee_subj, 0x31, 1 );
ADVANCE_BUF_VAL( ee_subj, len_ee_subj, len_ee_cn, 1 );
APPEND_TO_BUF( ee_subj, len_ee_subj, ee_cn, len_ee_cn );

// len_ee_subj == len_ee_cn + 4 == real_len_ee + 9 + 4

#if PRINT_DEBUG
    printf("\nThe len of ee_subj is: %d, ee_subj: \n", len_ee_subj);
    printBuf( ee_subj, len_ee_subj );
#endif // PRINT_DEBUG

// create whole ee cert
int ee_ttl_len = 828 + real_len_ee;
#if PRINT_DEBUG
    printf("\nThe len of ee_ttl_len is: %d\n", ee_ttl_len);
#endif // PRINT_DEBUG

uint8_t ee_cert[ ee_ttl_len ];

int len_ee_cert = 0;
ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x30, 1 );
ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x82, 1 );
int eeSqLen1 = len_ee_subj + 811;
uint8_t eeTmpSqLen1[2] = {0};
int2U8Array( eeSqLen1, eeTmpSqLen1 );
APPEND_TO_BUF( ee_cert, len_ee_cert, eeTmpSqLen1, 2 );

ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x30, 1 );
ADVANCE_BUF_VAL( ee_cert, len_ee_cert, 0x82, 1 );
int eeSqLen2 = len_ee_subj + 531;
uint8_t eeTmpSqLen2[2] = {0};
int2U8Array( eeSqLen2, eeTmpSqLen2 );
APPEND_TO_BUF( ee_cert, len_ee_cert, eeTmpSqLen2, 2 );

APPEND_TO_BUF( ee_cert, len_ee_cert, ee_pri_to_subj, sizeof(ee_pri_to_subj) );
APPEND_TO_BUF( ee_cert, len_ee_cert, ee_subj, len_ee_subj );
APPEND_TO_BUF( ee_cert, len_ee_cert, ee_suffix, sizeof(ee_suffix) );

#if PRINT_DEBUG
    printf("\nThe len of ee_cert is: %d, ee_cert: \n", len_ee_cert);
    printBuf( ee_cert, len_ee_cert );
    printf("========================\n");
#endif // PRINT_DEBUG

// ===============================================================
// input hostname

uint8_t sym_host_in_len = 0xff;
klee_make_symbolic( &sym_host_in_len, sizeof(uint8_t), "sym_host_in_len" );

#if LEN_SYMBOLIC
    // klee_assume( sym_host_in_len < 0x32 ); // here we assume the cn length is less than 0x32
    klee_assume( sym_host_in_len <= 0x05 ); // here we assume the cn length is less than 0x10
    klee_assume( sym_host_in_len >= 0x01 );
#else
    klee_assume( sym_host_in_len == CONCRETE_LEN );
#endif // LEN_SYMBOLIC

int real_len_host_in = 0;
ACCU_SYMVAR( sym_host_in_len, real_len_host_in );
#if PRINT_DEBUG
    printf("real_len_host_in: %d\n", real_len_host_in );
#endif // PRINT_DEBUG

// ===============================================================



#if NAME_SYMBOL
// string type symbol
    uint8_t type[1];
    klee_make_symbolic( type, sizeof(uint8_t), "type" );
    memcpy( &ee_cert[176], type, sizeof(uint8_t) );
// create name symbol
    // create hostname symbol in CN
    uint8_t sym_ee_hn[real_len_ee];
    // uint8_t* sym_ee_hn = (uint8_t*) malloc(real_len_ee * sizeof(uint8_t));
    klee_make_symbolic( sym_ee_hn, sizeof(sym_ee_hn), "sym_ee_hn" );
    memcpy( &ee_cert[178], sym_ee_hn, sizeof(sym_ee_hn) );
    // create host name symbol input here

    // uint8_t name[real_len_host_in];
    uint8_t name[real_len_host_in + 1];
    memset( name, 0xff, real_len_host_in );
    klee_make_symbolic( name, sizeof(name), "name" );
    klee_assume( name[real_len_host_in] == 0x00 ); 
    for ( int i = 0; i < real_len_host_in; i++ ) {
        klee_assume( name[i] != 0x00 ); 
    }
#else
    // name is concrete
    char name[] = "aaaa";
#endif // NAME_SYMBOL

#if VERIFY
    int ret;
    psX509Cert_t  *chainToBeChecked = NULL;
    psX509Cert_t  *p = NULL;
    psX509Cert_t  *tempp = NULL;
    psX509Cert_t  *trustedCerts = NULL;
    psX509Cert_t  *ic = NULL;
    psX509Cert_t  *foundIssuerRet = NULL;

    //leaf
    if (ret = psX509ParseCert(NULL, ee_cert, sizeof(ee_cert), &tempp, 0) < 0) {
        printf("Could not parse ee cert\n");
        return -1;
    }
    chainToBeChecked = tempp;
    tempp->next = NULL;
    p = tempp;

    //ca
    if (ret = psX509ParseCert(NULL, ca, sizeof(ca), &trustedCerts ,0) < 0) {
        printf("Could not parse ca cert\n");
        return -1;
    }

    ret = matrixValidateCerts(NULL, chainToBeChecked, trustedCerts, name, &foundIssuerRet, NULL, NULL);
    printf("\nRETURN: %d\n", ret);
    // printf("%d\n", ret);
    if ( ret != 0 ){
        printf("Could not verify chain with err code:%d\n", ret);
        return 1;
    } else {
        printf("ok\n");
        return 0;
    }

#else
    return TEST_CODE;
#endif // VERIFY

}

int main() {
    ret_val = ourEntryFunction();
    ret_val_ready();
    return 0;
}