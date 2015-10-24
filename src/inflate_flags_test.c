/* 
 * File:   inflate_flags_test.c
 * Author: alex
 *
 * Created on October 24, 2015, 8:53 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"

#define COMPRESSED_LEN 39546 // 0x9a7a
#define UNCOMPRESSED_LEN 103727 // 0x01952f
#define HEADER_LEN 74 // 30 [fixed] + 16 [filename] + 28 [metadata]

unsigned char COMPRESSED[COMPRESSED_LEN];
unsigned char UNCOMPRESSED[UNCOMPRESSED_LEN];

static void exit_error(const char* error) {
    puts(error);
    exit(1);
}

static void read_compressed_data() {
    FILE* fi = fopen("../resources/data/XSDHandler.class.zip", "r");
    if (!fi) exit_error("ERROR: data file open");
    int seek_res = fseek(fi, HEADER_LEN, SEEK_SET);
    if (0 != seek_res) exit_error("ERROR: data seek");
    size_t read_res = fread(&COMPRESSED, COMPRESSED_LEN, 1, fi);
    if (1 != read_res) exit_error("ERROR: data read");
    fclose(fi);
    memset(&UNCOMPRESSED, 0, UNCOMPRESSED_LEN);
}

static void write_uncompressed_data() {
    FILE* fi = fopen("tmp.out", "w");
    if (!fi) exit_error("ERROR: out file open");
    size_t write_res = fwrite(UNCOMPRESSED, UNCOMPRESSED_LEN, 1, fi);
    if (1 != write_res) exit_error("ERROR: data write");
    fclose(fi);
}

static void do_inflate(z_stream* strm_ptr, int flush_mode,
        unsigned int input_buf_len, unsigned int output_buf_len) {
    strm_ptr->next_in = (unsigned char*) &COMPRESSED;
    strm_ptr->avail_in = input_buf_len;
    strm_ptr->next_out = (unsigned char*) &UNCOMPRESSED;
    strm_ptr->avail_out = output_buf_len;
    for (;;) {
        int ret = inflate(strm_ptr, flush_mode);
        switch (ret) {
            case Z_STREAM_END:
                if (UNCOMPRESSED_LEN != strm_ptr->total_out) exit_error("ERROR: stream end");
                return;
            case Z_OK:
            case Z_BUF_ERROR:
                if (0 == strm_ptr->avail_in) {
                    unsigned int left = COMPRESSED_LEN - strm_ptr->total_in;
                    strm_ptr->avail_in = left >= input_buf_len ? input_buf_len : left;
                }
                if (0 == strm_ptr->avail_out) {
                    unsigned int left = UNCOMPRESSED_LEN - strm_ptr->total_out;
                    strm_ptr->avail_out = left >= output_buf_len ? output_buf_len : left;
                }
                break;
            default:
                exit_error("ERROR: inflate");
        }
    }
}

int main(int argc, char* argv[]) {
    printf("%d\n", argc);
    // arguments
    if (argc != 2 && argc != 4) exit_error("ERROR: either one (flush_mode) or"
            " three (flush_mode, input_buf_len, output_buf_len) arguments must be specified");
    int flush_mode = atoi(argv[1]);
    unsigned int input_buf_len = 4 == argc ? atoi(argv[2]) : COMPRESSED_LEN;
    unsigned int output_buf_len = 4 == argc ? atoi(argv[3]) : UNCOMPRESSED_LEN;
    printf("INFO: flush_mode: [%d], input_buf_len: [%u], output_buf_len: [%u]\n", 
            flush_mode, input_buf_len, output_buf_len);
            
    // read data to memory
    read_compressed_data();
    
    // init stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_res = inflateInit2(&strm, -MAX_WBITS);
    if (Z_OK != init_res) exit_error("ERROR: inflate init");
    
    // inflate
    do_inflate(&strm, flush_mode, input_buf_len, output_buf_len);
    
    // don't close stream (finalizer wasn't called)
//    int end_res = inflateEnd(&strm);
//    if (Z_OK != end_res) exit_error("ERROR: inflate end");
    
    // write data to check correctness (disabled)
    (void) write_uncompressed_data;
//    write_uncompressed_data();
    return 0;
}

