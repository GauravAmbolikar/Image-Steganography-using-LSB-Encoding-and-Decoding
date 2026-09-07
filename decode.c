#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include"common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        decInfo->output_txt_fname = argv[3];
    }
    else
    {
        decInfo->output_txt_fname = "default.txt";
    }

    return e_success;
}

Status open_decoding_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stegoo_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stegoo_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    // output text file
    decInfo->fptr_output_txt = fopen(decInfo->output_txt_fname, "w+");
    // Do Error handling
    if (decInfo->fptr_output_txt == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_txt_fname);

    	return e_failure;
    }

    return e_success;
}

char decode_byte_from_lsb(DecodeInfo *decInfo)
{
    char data = 0;

    for(int i = 0 ; i < 8 ; i++)
    {
        data |= ((decInfo->stego_image_data[i] & 1) << i);
    }

    return data;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stegoo_image,54,SEEK_SET);

    char magic[3];

    for(int i = 0 ; i < strlen(MAGIC_STRING) ; i++)
    {
        fread(decInfo->stego_image_data,8,1,decInfo->fptr_stegoo_image);

        magic[i] = decode_byte_from_lsb(decInfo);
    }
    magic[strlen(MAGIC_STRING)] = '\0';

    if(strcmp(magic,MAGIC_STRING) == 0)
    {
        return e_success;
    }

    return e_failure;
}

Status decode_size_to_lsb(uint *size , DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer,32,1,decInfo->fptr_stegoo_image);

    *size = 0;

    for(int i = 0 ; i < 32 ; i++)
    {
        *size |= ((buffer[i] & 1) << i);
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decode_size_to_lsb(&decInfo->secret_file_extn_size,decInfo);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char str[decInfo->secret_file_extn_size + 1];

    for(int i = 0 ; i < decInfo->secret_file_extn_size ; i++)
    {
        fread(decInfo->stego_image_data,8,1,decInfo->fptr_stegoo_image);
        str[i] = decode_byte_from_lsb(decInfo);
    }
    
    str[decInfo->secret_file_extn_size] = '\0';

    if(strcmp(str,".txt") == 0)
    {
        return e_success;
    }

    return e_failure;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_to_lsb(&decInfo->secret_file_size,decInfo);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data[decInfo->secret_file_size+1];

    for(int i = 0 ; i < decInfo->secret_file_size ; i++)
    {
        fread(decInfo->stego_image_data,8,1,decInfo->fptr_stegoo_image);

        data[i] = decode_byte_from_lsb(decInfo);
    }

    data[decInfo->secret_file_size] = '\0';

    fwrite(data, 1, decInfo->secret_file_size,decInfo->fptr_output_txt);

    return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if(decode_magic_string(decInfo) == e_success)
    {
        printf("Info:: Decoded and verified magic string successfully\n");
        if(decode_secret_file_extn_size(decInfo) == e_success)
        {
            printf("Info:: Decoded secret file extn size successfully\n");
            if(decode_secret_file_extn(decInfo) == e_success)
            {
                printf("Info:: Decoded secret file extn successfully\n");
                if(decode_secret_file_size(decInfo) == e_success)
                {
                    printf("Info:: Decoded secret file size successfully\n");
                    if(decode_secret_file_data(decInfo) == e_success)
                    {
                        printf("Info:: Decoded secret file data successfully\n");
                        printf("\nSecret data:\n");

                        rewind(decInfo->fptr_output_txt);   

                        int ch;
                        while ((ch = fgetc(decInfo->fptr_output_txt)) != EOF)
                        {
                            putchar(ch);
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("Failed to decode secret file data\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to decode secret file size\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to decode secret file extn\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode secret file extn size\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }
    return e_success;
}
