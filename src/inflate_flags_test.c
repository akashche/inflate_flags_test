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

int main() {
    read_compressed_data();
    
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_res = inflateInit2(&strm, -MAX_WBITS);
    if (Z_OK != init_res) exit_error("ERROR: inflate init");

    strm.next_in = (unsigned char*) &COMPRESSED;
    strm.avail_in = COMPRESSED_LEN;
    strm.next_out = (unsigned char*) &UNCOMPRESSED;
    strm.avail_out = UNCOMPRESSED_LEN;
    
    int inflate_res = inflate(&strm, Z_FINISH);
    if (Z_STREAM_END != inflate_res) exit_error("ERROR: inflate");
    
    int end_res = inflateEnd(&strm);
    if (Z_OK != end_res) exit_error("ERROR: inflate end");
    
    (void) write_uncompressed_data;
    return 0;
}

