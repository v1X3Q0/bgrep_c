#ifndef BGREP_C_UTIL_H
#define BGREP_C_UTIL_H

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

#define SAFE_CLOSEDIR(x) \
    if (x) \
    { \
        closedir(x); \
    }

#ifdef __cplusplus
extern "C"
{
#endif
    int recurse_op(int (*routine_on_file)(const char*, int, void**), const char* path_dir, int count, void** vargs);
#ifdef __cplusplus
}
#endif

#endif