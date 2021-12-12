#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SAFE_BAIL(x) \
    if (x) \
    { \
        goto fail; \
    }

#define SAFE_FREE(x) \
    if (x) \
    { \
        free(x); \
        x = 0; \
    }

void usage(char* callername)
{
    fprintf(stderr, "Usage: %s [-i filename] [-f] [-d] [bytes]+\n",
            callername);
    fprintf(stderr, "\t-i input file\n");
    fprintf(stderr, "\t-f only first occurance\n");
    fprintf(stderr, "\t-d out format in decimal\n");
    fprintf(stderr, "\tbytes space separated array of bytes\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    int opt = 0;
    char* in_file_name = 0;
    FILE* in_file = 0;
    size_t in_file_sz = 0;
    int byte_length = 0;
    uint8_t* byte_search = 0;
    uint8_t* in_file_raw = 0;
    int find_first = 0;
    int dec_out = 0;

    while ((opt = getopt(argc, argv, "dfi:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            in_file_name = optarg;
            break;
        case 'f':
            find_first = 1;
            break;
        case 'd':
            dec_out = 1;
            break;
        default: /* '?' */
            usage(argv[0]);
        }
    }

    if (optind == argc)
    {
        usage(argv[0]);
    }
    else
    {
        int argCopy = optind;
        byte_length = argc - argCopy;
        byte_search = (uint8_t*)calloc(byte_length, 1);

        while (argCopy < argc)
        {
            byte_search[argCopy - optind] = (uint8_t)strtoul(argv[argCopy], NULL, 16);
            argCopy++;
        }
    }

    in_file = fopen(in_file_name, "r");
    SAFE_BAIL(in_file == 0);
    fseek(in_file, 0, SEEK_END);
    in_file_sz = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    in_file_raw = (uint8_t*)calloc(in_file_sz, 1);
    SAFE_BAIL(in_file_raw == 0);
    fread(in_file_raw, 1, in_file_sz, in_file);
    fclose(in_file);

    SAFE_BAIL(in_file_sz < byte_length);
    for (int i = 0; i < (in_file_sz - byte_length); i++)
    {
        if (memcmp(&in_file_raw[i], byte_search, byte_length) == 0)
        {
            printf("found occurance at ");
            if (dec_out)
            {
                printf("%d\n", i);
            }
            else
            {
                printf("0x%x\n", i);
            }

            if (find_first)
            {
                break;
            }
        }
    }
    
fail:
    SAFE_FREE(in_file_raw);
    SAFE_FREE(byte_search);
    return 0;
}