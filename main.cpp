#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <localUtil.h>

void usage(char* callername)
{
    fprintf(stderr, "Usage: %s [-f] [-d] filename [bytes]+\n",
            callername);
    fprintf(stderr, "\t-f only first occurance\n");
    fprintf(stderr, "\t-d out format in decimal\n");
    fprintf(stderr, "\tbytes space separated array of bytes\n");
    exit(EXIT_FAILURE);
}

int bsearch_file(const char* in_file_name, int count, void** vargs)
{
    int result = -1;
    FILE* in_file = 0;
    size_t in_file_sz = 0;
    uint8_t* in_file_raw = 0;

    size_t byte_length = (size_t)vargs[0];
    uint8_t* byte_search = (uint8_t*)vargs[1];
    size_t find_first = (size_t)vargs[2];
    size_t dec_out = (size_t)vargs[3];

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
            printf("%s: found occurance at ", in_file_name);
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
    
    result = 0;
fail:
    SAFE_FREE(in_file_raw);
    return result;
}

int main(int argc, char** argv)
{
    int opt = 0;
    char* in_file_name = 0;

    void* bsearch_args[4] = {0};

    size_t byte_length = 0;
    uint8_t* byte_search = 0;
    size_t find_first = 0;
    size_t dec_out = 0;

    while ((opt = getopt(argc, argv, "df")) != -1)
    {
        switch (opt)
        {
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
        in_file_name = argv[optind];
        optind++;
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

    bsearch_args[0] = (void*)byte_length;
    bsearch_args[1] = byte_search;
    bsearch_args[2] = (void*)find_first;
    bsearch_args[3] = (void*)dec_out;
    recurse_op(bsearch_file, in_file_name, 4, bsearch_args);
    
fail:
    SAFE_FREE(byte_search);
    return 0;
}