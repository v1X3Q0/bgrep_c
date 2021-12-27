#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "bgrep_e.h"

int bsearch_total(uint8_t* in_file_raw, size_t in_file_sz, uint8_t* byte_search, int byte_length, void** found_addr, int find_first, int dec_out)
{
    int result = -1;

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
                if (found_addr != 0)
                {
                    *found_addr = &in_file_raw[i];
                }
                break;
            }
        }
    }
    
    result = 0;    
fail:
    return result;
}

int bsearch(uint8_t* in_file_raw, size_t in_file_sz, uint8_t* byte_search, int byte_length, void** found_addr)
{
    return bsearch_total(in_file_raw, in_file_sz, byte_search, byte_length, found_addr, 1, 0);
}
