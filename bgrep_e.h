#ifndef BGREP_E_H
#define BGREP_E_H

#include <stdint.h>
#include <stdio.h>

#define SAFE_BAIL(x) \
    if (x) \
    { \
        goto fail; \
    }

#define SAFE_FREE(x) \
    if (x) \
    { \
        free((void*)x); \
        x = 0; \
    }

int bsearch_total(uint8_t* in_file_raw, size_t in_file_sz, uint8_t* byte_search, int byte_length, void** found_addr, int find_first, int dec_out);
int bsearch(uint8_t* in_file_raw, size_t in_file_sz, uint8_t* byte_search, int byte_length, void** found_addr);
int search_seq(uint8_t* kern_copy, size_t img_var_sz, uint8_t* byte_search, size_t search_sz,
    size_t offset, size_t step, bool match, void** out_img_off);

class search_set
{
public:
    virtual int findPattern(uint8_t* startAddress, size_t sizeSearch, void** resultAddr) = 0;
    search_set(size_t offset_a, size_t step_a, bool match_a) : offset(offset_a), step(step_a), match(match_a) {};
protected:
    size_t offset;
    size_t step;
    bool match;
};

class binary_ss : public search_set
{
public:
    uint8_t* byte_search;
    int byte_length;
public:
    binary_ss(uint8_t* byte_search_a, int byte_length_a, size_t offset_a, size_t step_a, bool match_a) : search_set(offset_a, step_a, match_a),
        byte_search(byte_search_a), byte_length(byte_length_a) {};
    int findPattern(uint8_t* startAddress, size_t sizeSearch, void** resultAddr);
};



#endif