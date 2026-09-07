#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>

int main(int argc, char *argv[])
{
    EncodeInfo E1;
    DecodeInfo D1;

    
    //validation of cla 
    int res = check_operation_type(argc,argv);
    if(res == e_encode)
    {
        printf("Encoding is selected\n");
        if(read_and_validate_encode_args(argv, &E1) == e_success)
        {
            printf("Info:: Read and validate encode argument is success\n");
            if(open_files(&E1) == e_success)
            {
                printf("Info:: Input files are opened successfully\n");
                if(do_encoding(&E1) == e_success)
                {
                    printf("Info:: Encoding is success\n");
                    printf("::Secret data encoded in '%s'::\n",E1.stego_image_fname);
                }
                else
                {
                    printf("Failed to encode\n");
                    return 0;
                }
            }
            else
            {
                printf("Open file is failure\n");
                return 0;
            }
        }
        else
        {
            printf("Info:: Read and validate encode argument is failure\n");
            return 0;
        }
    }
    else if(res == e_decode)
    {
        printf("Decoding is selected\n");
        if(read_and_validate_decode_args(argv,&D1) == e_success)
        {
            printf("Info:: Read and validate decode arguments successfully\n");
            if(open_decoding_files(&D1) == e_success)
            {
                printf("Info:: Input files are opened successfully\n");
                if(do_decoding(&D1) == e_success)
                {
                    printf("Info:: Decoding is success\n");
                    printf("::Secret data stored to '%s'::\n",D1.output_txt_fname);
                }
                else
                {
                    printf("Failed to decode\n");
                    return 0;
                }
            }
            else
            {
                printf("Failed to open input files\n");
                return 0;
            }
        }
        else
        {
            printf("Failed to read and validate decode arguments\n");
            return 0;
        }
    }
    else
    {
        printf("Invalid option\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding: ./a.out -d stego.bmp default.txt\n");
    }

    return 0;
}

OperationType check_operation_type(int argc, char *argv[])
{
    if (argc < 2)
        return e_unsupported;

    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc == 4 || argc == 5)
            return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        if (argc == 3 || argc == 4)
            return e_decode;
    }
    return e_unsupported;
}
