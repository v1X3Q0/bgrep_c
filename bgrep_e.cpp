#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <localUtil.h>
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

int search_seq(uint8_t* kern_copy, size_t img_var_sz, uint8_t* byte_search, size_t search_sz,
    size_t offset, size_t step, bool match, void** out_img_off)
{
    int result = -1;
    uint8_t* iterPoint = 0;

    iterPoint = (uint8_t*)((size_t)kern_copy + offset);

    for (int kern_index = offset; kern_index < img_var_sz; kern_index += step, iterPoint += step)
    {
        // principal behind this is we can search for, for instance, a nonzero block using this same
        // routine. so  if 0 == 0) == true, then it will keep going. but if non == 0) ~ false, then
        // all good
        if ((memcmp(byte_search, iterPoint, search_sz) == 0) == match)
        {
            *out_img_off = iterPoint;
            goto found;
        }
    }
    goto fail;
found:
    result = 0;
fail:
    return result;
}

int binary_ss::findPattern_fixed(uint8_t* startAddress, size_t sizeSearch, void** resultAddr)
{
    return search_seq(startAddress, sizeSearch, byte_search, byte_length, offset, step, match, resultAddr);
};

int search_set::findPattern(uint8_t* startAddress, size_t sizeSearch, void** resultAddr)
{
    int result = -1;

    // start at same starting point, but . finish condition is when
    // cur < (start + size)
    if (((signed long)sizeSearch < 0) && (step < 0))
    {
        // sizeSearch = -sizeSearch;
    }
    // start at size from starting address, and step down on each iteration
    // so end at original starting point. finish condition is when
    // cur < (start + size)
    else if (((signed long)sizeSearch > 0) && (step < 0))
    {
        startAddress += sizeSearch;
    }
    // start from an address below wanted start, and positive step towards
    // initial start. finish condition is when cur > (start + size)
    else if (((signed long)sizeSearch < 0) && (step > 0))
    {
        startAddress += sizeSearch;
    }
    // start from start address and step up til size. finish condition is
    // cur > (start + size)
    else if (((signed long)sizeSearch > 0) && (step > 0))
    {

    }

    return findPattern_fixed(startAddress, sizeSearch, resultAddr);
}

// return true if things need to return
bool search_set::evaluateFinish(uint8_t* curAddr, uint8_t* endAddr)
{
    bool result = false;
    
    if (step < 0)
    {
        if (curAddr < endAddr)
        {
            result = true;
        }
    }
    else if (step > 0)
    {
        if (curAddr > endAddr)
        {
            result = true;
        }
    }

    return result;
}

void search_set::internal_reset(size_t offset_a, size_t step_a, bool match_a)
{
    offset = offset_a;
    step = step_a;
    match = match_a;
}
