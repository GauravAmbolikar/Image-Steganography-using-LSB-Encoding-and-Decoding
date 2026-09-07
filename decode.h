#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image info */
    char *stego_image_fname;
    FILE *fptr_stegoo_image;
    uint secret_file_extn_size;
    uint secret_file_size;
    char stego_image_data[MAX_IMAGE_BUF_SIZE];

    /* Output file Info */
    char *output_txt_fname;
    FILE *fptr_output_txt;

} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status open_decoding_files(DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
char decode_byte_from_lsb(DecodeInfo *decInfo);
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
Status decode_size_to_lsb(uint *size, DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif